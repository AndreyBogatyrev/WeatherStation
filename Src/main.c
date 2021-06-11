#include "stm32f1xx.h"
#include "bme280.h"
#include "ds3231.h"
#include "i2c.h"
#include "tim.h"
#include "lcd.h"
#include "main.h"

struct bme280_calib_data calib_data = {0};//для хранения калибровочных данных
struct DS3231_calendar setting_time = {0};//для хранения данных в режиме настройки
uint8_t state = 1;//состояние контроллера
uint8_t toggle = 0;//переменная для настройки интервалов переключения

void SetSysClock(void){
  //настройка тактирования системы
    RCC->CR |= RCC_CR_HSEON;
    while ((RCC->CR & RCC_CR_HSERDY) == 0){}
    RCC->CFGR |= RCC_CFGR_SW_HSE;
    while ((RCC->CFGR & RCC_CFGR_SWS_HSE) == 0){}
    SystemCoreClockUpdate();
}

void SysTickEnable(uint32_t load){
  //запуск системного таймера
    if (load>0xFFFFFF)
	load=0xFFFFFF;
    SysTick->LOAD =  load;
    SysTick->CTRL |= (SysTick_CTRL_ENABLE_Msk);
}

void SysTickDisable(void){
  //остановка системного таймера
    SysTick->CTRL &= ~(SysTick_CTRL_ENABLE_Msk | SysTick_CTRL_TICKINT_Msk);
    SysTick->VAL &= ~SysTick_VAL_CURRENT_Msk;
}

void Wait(uint32_t delay_mks){
  //функция задержки
    delay_mks *= (SystemCoreClock/8000000UL);
    SysTickEnable(delay_mks);
    while(!(SysTick->CTRL & SysTick_CTRL_COUNTFLAG_Msk)){};
    SysTickDisable();
}

uint8_t Division(uint32_t data){
  //Вспомогательная функция для преобразования данных с датчика в нужных вид для вывода на дисплей
    uint8_t out = data%10;
    return out<<4;
}

void Convert_BME280_for_LCD(struct LCD_BME280* out, struct bme280_data meas){
  //Заполнение структуры, которая будет использоваться при отображении информации
    out->sign = (meas.temp<0?0xDD:0xBD);
    out->percent = 0x5D;
    out->dot = 0xED;
    out->degree[0] = 0xDD;
    out->degree[1] = 0xFD;
    out->C[0] = 0x4D;
    out->C[1] = 0x3D;
    out->m[0] = 0x6D;
    out->m[1] = 0xDD;
    out->H[0] = 0x4D;
    out->H[1] = 0x8D;
    out->g[0] = 0x6D;
    out->g[1] = 0x7D;

    uint32_t temp =((meas.temp<0?-meas.temp:meas.temp)+5)/10;

    for (int idx=2; idx>-1; idx--){
	out->temp[idx]=Division(temp) | 0xD;
	temp/=10;
	out->press[idx]=Division(meas.press) | 0xD;
	meas.press/=10;
	out->hum[idx]=Division(meas.hum) | 0xD;
	meas.hum/=10;
    }

    out->temp_fd = (out->temp[0]==0?0x2D:0x3D);
    out->hum_fd = (out->hum[1]==0?0x2D:0x3D);
}

void Convert_DS3231_for_LCD(struct LCD_DS3231* out, struct DS3231_calendar cal){
  //Заполнение структуры, которая будет использоваться при отображении информации
    uint8_t day_of_week[6][7] = {{0x4D,0x5D,0x5D,0x5D,0x4D,0x5D,0x5D},
				 {0xDD,0x4D,0x7D,0x4D,0x6D,0x3D,0x3D},
				 {0x6D,0x7D,0x6D,0x6D,0x7D,0x6D,0x7D},
				 {0xFD,0x5D,0x5D,0x8D,0x2D,0x1D,0x5D},
				 {0x6D,0x6D,0x6D,0x7D,0x6D,0x7D,0x6D},
				 {0xED,0x5D,0x4D,0x5D,0x9D,0x4D,0xED}};
    out->colon=0xAD;
    out->slash[0]=0x2D;
    out->slash[1]=0xFD;

    out->hour[0]=(cal.hour & 0x30) | 0xD;
    out->hour[1]=(cal.hour<<4) | 0xD;
    out->min[0]=(cal.minute & 0x70) | 0xD;
    out->min[1]=(cal.minute<<4) | 0xD;

    for (uint8_t i=0; i<6; i++){
	out->dayW[i]=day_of_week[i][cal.dayWeek-1];
    }

    out->day[0]=(cal.dayMonth & 0x30) | 0xD;
    out->day[1]=(cal.dayMonth<<4) | 0xD;
    out->month[0]=(cal.month & 0x10) | 0xD;
    out->month[1]=(cal.month<<4) | 0xD;
    out->year[0]=(cal.year & 0xF0) | 0xD;
    out->year[1]=(cal.year<<4) | 0xD;
}

void Update_Data(void){
  //Функция обновления информации на дисплее
    toggle++;
    toggle%=10;
    if (toggle>5){
	struct bme280_data meas = {0};
	struct LCD_BME280 print = {0};
	BME280_GetMeasurement(&meas, &calib_data);
	Convert_BME280_for_LCD(&print,meas);
	Print_BME280(&print);
    }else{
	struct DS3231_calendar cal = {0};
	struct LCD_DS3231 print = {0};
	DS3231_GetData(&cal);
	Convert_DS3231_for_LCD(&print,cal);
	Print_DS3231(&print);
    }
}

void EXTI0_IRQHandler(void){
  //Прерывание от RTC
    Update_Data();
    EXTI->PR = EXTI_PR_PR0;
}

void Toggle_LCD_LED(void){
  //переключение состояния подсветки
    LCD_Write((0x4 | state<<3),37U);
    LCD_Write((0x84 | state<<3 | state<<6),37U);
}

void Toggle_Cursor_Blink(void){
  //Переключение состояния мигания курсока
    uint8_t B=0;
    if (state==1){
	B=0x10;
    }

    LCD_Write(0xC,37U);
    LCD_Write(0xCC | B,37U);
}

void Change_Cursor_Pos(void){
  //Смена положения курсора в режиме настройки
    uint8_t pos=0x80;
    switch(state){
      case 3:
	pos = 0x85;
	break;
      case 4:
	pos = 0x8B;
	break;
      case 5:
	pos = 0xC4;
	break;
      case 6:
	pos = 0xC7;
	break;
      case 7:
	pos = 0xCA;
	break;
      default:
	pos=0x82;
    }
    LCD_Write((pos & 0xF0) | 0xC, 37U);
    LCD_Write(pos<<4 | 0xC, 37U);
}

uint8_t DECtoBCD(uint8_t data){
  //Преобразование десятичного числа в BCD формат
    data = ((data/10)<<4)+(data%10);
    return data;
}

uint8_t BCDtoDEC(uint8_t data){
  //Преобразование числа в BCD формате в десятичное число
    data = (data>>4)*10+(data & 0xF);
    return data;
}

void Change_Value(void){
  //Изменение значений даты и времени в режиме настройки
    struct LCD_DS3231 print = {0};
    uint8_t data, year, month, divider;
    uint8_t day[12] = {31,28,31,30,31,30,31,31,30,31,30,31};

    switch(state){
      case 3:
	data=BCDtoDEC(setting_time.minute)+1;
	data %=60;
	setting_time.minute=DECtoBCD(data);
	break;
      case 4:
	setting_time.dayWeek %= 7;
	setting_time.dayWeek++;
	break;
      case 5:
	month = BCDtoDEC(setting_time.month)-1;
	year = BCDtoDEC(setting_time.year);
	divider = day[month];

	if ((year%4==0) & (month==1)){
	    divider++;
	}

	data=BCDtoDEC(setting_time.dayMonth)%divider;
	setting_time.dayMonth=DECtoBCD(++data);
	break;
      case 6:
	data=BCDtoDEC(setting_time.month)%12;
	setting_time.month=DECtoBCD(++data);
	break;
      case 7:
	data=BCDtoDEC(setting_time.year)+1;
	data%=100;
	setting_time.year=DECtoBCD(data);
	break;
      default:
	data=BCDtoDEC(setting_time.hour)+1;
	data %=24;
	setting_time.hour=DECtoBCD(data);
    }

    Convert_DS3231_for_LCD(&print,setting_time);
    Print_DS3231(&print);
}

void TIM1_TRG_COM_IRQHandler(void){
  //Прерывание для обработки нажатия кнопки
    NVIC_DisableIRQ(EXTI0_IRQn);
    if ((TIM1->SR & TIM_DIER_TIE)!=0){
	TIM1->SR &= ~TIM_DIER_TIE;
	uint16_t width = TIM1->CNT;
	TIM1->CNT=0;

	if (width>1){
	    if (width<500){
		if (state<2){
		    state = 1-state;
		    Toggle_LCD_LED();
		}else{
		    Change_Value();
		    Change_Cursor_Pos();
		}
	    }
	    else{
		if (state==1){
		    struct LCD_DS3231 print = {0};
		    DS3231_GetData(&setting_time);
		    setting_time.second=0x0;
		    Convert_DS3231_for_LCD(&print,setting_time);
		    Print_DS3231(&print);
		    Toggle_Cursor_Blink();
		}else if(state==7){
		    Toggle_Cursor_Blink();
		    DS3231_SetData(&setting_time);
		}

		if (state>0){
		    state %= 7;
		    state++;
		    Change_Cursor_Pos();
		}
	    }
	}
    }
    if (state==1){
	NVIC_EnableIRQ(EXTI0_IRQn);
    }
}

int main(void){
//Настройка тактирования
    SetSysClock();

    AFIO->MAPR |= AFIO_MAPR_SWJ_CFG_JTAGDISABLE_Msk;

//Тактирование GPIOA
    RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;

//Конфигурирование PA0
    GPIOA->CRL &= ~GPIO_CRL_CNF0;
    GPIOA->CRL |= GPIO_CRL_CNF0_1;
    GPIOA->ODR |= GPIO_ODR_ODR0;

//Инициализация I2C модуля
    I2C_Init();
//Инициализация DS3231
    DS3231_Init();
//Инициализация LCD
    LCD_Init();
//Инициализация BME280
    BME280_Init();
//Получение калибровочных данных
    BME280_GetCalibData(&calib_data);
//Инициализация таймера
    TIM_Init();
//Настройка внешнего прерывания
    EXTI->IMR=EXTI_IMR_MR0;
    EXTI->RTSR=EXTI_RTSR_TR0;
//Разрешить внешние прерывания
    NVIC_EnableIRQ(EXTI0_IRQn);
//Разрешить прерывания таймера
    NVIC_EnableIRQ(TIM1_TRG_COM_IRQn);
    while (1){}
}
