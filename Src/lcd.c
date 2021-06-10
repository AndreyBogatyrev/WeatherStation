#include "stm32f1xx.h"
#include "main.h"
#include "i2c.h"
#include "lcd.h"

void Print_DS3231(struct LCD_DS3231* out){
    uint8_t dub=0x3D;
    uint8_t print[42]={0x8C, 0x2C, dub, out->hour[0], dub, out->hour[1], dub, out->colon,
			dub, out->min[0], dub, out->min[1], 0x2D, 0xD, 0x2D, 0xD, 0x8C, 0xBC,
			out->dayW[0], out->dayW[1], out->dayW[2], out->dayW[3], out->dayW[4], out->dayW[5],
			0xCC, 0x4C, dub, out->day[0], dub, out->day[1], out->slash[0], out->slash[1],
			dub, out->month[0], dub, out->month[1], out->slash[0], out->slash[1],
			dub, out->year[0], dub, out->year[1]};

    for (uint8_t i=0; i<42;i++){
	LCD_Write(print[i],37U);
    }
}

void Print_BME280(struct LCD_BME280* out){
    uint8_t dub=0x3D;
    uint8_t sub=0x2D;
    uint8_t print[42]={0x8C, 0x2C, sub, out->sign, out->temp_fd, out->temp[0], dub, out->temp[1], sub, out->dot, dub, out->temp[2], out->degree[0], out->degree[1],
		      out->C[0], out->C[1], 0x8C, 0xBC, out->hum_fd, out->hum[1], dub, out->hum[2], sub, out->percent, 0xCC, 0x4C, dub, out->press[0], dub,
		      out->press[1], dub, out->press[2], sub, 0xD, out->m[0], out->m[1], out->m[0], out->m[1], out->H[0], out->H[1], out->g[0], out->g[1]};

    for (uint8_t i=0; i<42;i++){
	LCD_Write(print[i],37U);
    }
}

void LCD_Write(uint8_t data, uint32_t delay){
    I2C_Write(LCD_I2C_ADDR_WRITE, &data,1);
    data &= ~0x4;
    I2C_Write(LCD_I2C_ADDR_WRITE, &data,1);
    Wait(delay);
}

void LCD_Init(void){
    Wait(40000UL);
    uint8_t data[14] = {0x3C,0x3C,0x3C,0x2C,0x2C,0x8C,0x0C,0x8C,0x0C,0x1C,0x0C,0x6C,0x0C,0xCC};

    uint32_t delay[14] = {[0 ... 13] = 37UL};
    delay[0] = 4100UL;
    delay[1] = 100UL;
    delay[9] = 1520UL;

    for (uint8_t i=0; i<14;i++){
	LCD_Write(data[i],delay[i]);
    }
}
