#include "stm32f1xx.h"
#include "ds3231.h"
#include "i2c.h"

void DS3231_Init()
{
  //инициализация часов реального времени
    uint8_t init[3] = {DS3231_CONTROL_ADDR,0x0,0x0};
    I2C_Write(DS3231_I2C_ADDR_WRITE, init, 3);
}

void DS3231_GetData(struct DS3231_calendar* cal)
{
  //Получение даты и времени из RTC
    uint8_t date[7] = {0};
    uint8_t addr = 0;
    I2C_Write(DS3231_I2C_ADDR_WRITE, &addr, 1);
    I2C_Read(DS3231_I2C_ADDR_READ, date, 7);

    cal->second=date[0];
    cal->minute=date[1];
    cal->hour=date[2];
    cal->dayWeek=date[3];
    cal->dayMonth=date[4];
    cal->month=date[5];
    cal->year=date[6];
}

void DS3231_SetData(struct DS3231_calendar* cal)
{
  //Записать дату и время в RTC
    uint8_t new_data[8] = {DS3231_SEC_ADDR,
		      cal->second,
		      cal->minute,
		      cal->hour,
		      cal->dayWeek,
		      cal->dayMonth,
		      cal->month,
		      cal->year};

    I2C_Write(DS3231_I2C_ADDR_WRITE, new_data, 8);
}
