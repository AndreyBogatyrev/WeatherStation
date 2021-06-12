/*******************************************************************
WeatherStation
Copyright (C) 2021  Andrey Bogatyrev

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <https://www.gnu.org/licenses/>.
*******************************************************************/

#include "stm32f1xx.h"
#include "bme280.h"
#include "i2c.h"

void BME280_GetMeasurement(struct bme280_data* meas, struct bme280_calib_data* calib_data){
uint8_t data[8]={0};

uint8_t addr = BME280_DATA_ADDR;
I2C_Write(BME280_I2C_ADDR_WRITE, &addr, 1U);
I2C_Read(BME280_I2C_ADDR_READ, data, 8U);

struct bme280_uncomp_data uncomp_data = {(data[0]<<12 | data[1]<<4 | data[2]>>4),(data[3]<<12 | data[4]<<4 | data[5]>>4),(data[6]<<8 | data[7])};

//Последующие расчеты предоставлены производителем датчика, можно найти в datasheet
int32_t var1;
int32_t var2;
int32_t var3;
int32_t var4;
int32_t var5;

uint32_t humidity;
uint32_t pressure;
int32_t temperature;
int32_t temperature_min = -4000;
int32_t temperature_max = 8500;

//Для температуры

var1 = (int32_t)((uncomp_data.temperature / 8) - ((int32_t)calib_data->dig_t1 * 2));
var1 = (var1 * ((int32_t)calib_data->dig_t2)) / 2048;
var2 = (int32_t)((uncomp_data.temperature / 16) - ((int32_t)calib_data->dig_t1));
var2 = (((var2 * var2) / 4096) * ((int32_t)calib_data->dig_t3)) / 16384;
calib_data->t_fine = var1 + var2;
temperature = (calib_data->t_fine * 5 + 128) / 256;

if (temperature < temperature_min)
{
    temperature = temperature_min;
}
else if (temperature > temperature_max)
{
    temperature = temperature_max;
}

//Для давления

var1 = (((int32_t)calib_data->t_fine)>>1)-(int32_t)64000;
var2 = (((var1>>2) * (var1>>2)) >> 11 ) * ((int32_t)calib_data->dig_p6);
var2 = var2 + ((var1*((int32_t)calib_data->dig_p5))<<1);
var2 = (var2>>2)+(((int32_t)calib_data->dig_p4)<<16);
var1 = (((calib_data->dig_p3 * (((var1>>2) * (var1>>2)) >> 13 )) >> 3) + ((((int32_t)calib_data->dig_p2) *
var1)>>1))>>18;
var1 =((((32768+var1))*((int32_t)calib_data->dig_p1))>>15);

if (var1 != 0)
{
    pressure = (((uint32_t)(((int32_t)1048576)-uncomp_data.pressure)-(var2>>12)))*3125;
    if (pressure < 0x80000000)
    {
	pressure = (pressure << 1) / ((uint32_t)var1);
    }
    else
    {
	pressure = (pressure / (uint32_t)var1) * 2;
    }
    var1 = (((int32_t)calib_data->dig_p9) * ((int32_t)(((pressure>>3) * (pressure>>3))>>13)))>>12;
    var2 = (((int32_t)(pressure>>2)) * ((int32_t)calib_data->dig_p8))>>13;
    pressure = (uint32_t)((int32_t)pressure + ((var1 + var2 + calib_data->dig_p7) >> 4));
}

//---------------------------------------------hum--------------------------------------------------

var1 = calib_data->t_fine - ((int32_t)76800);
var2 = (int32_t)(uncomp_data.humidity * 16384);
var3 = (int32_t)(((int32_t)calib_data->dig_h4) * 1048576);
var4 = ((int32_t)calib_data->dig_h5) * var1;
var5 = (((var2 - var3) - var4) + (int32_t)16384) / 32768;
var2 = (var1 * ((int32_t)calib_data->dig_h6)) / 1024;
var3 = (var1 * ((int32_t)calib_data->dig_h3)) / 2048;
var4 = ((var2 * (var3 + (int32_t)32768)) / 1024) + (int32_t)2097152;
var2 = ((var4 * ((int32_t)calib_data->dig_h2)) + 8192) / 16384;
var3 = var5 * var2;
var4 = ((var3 / 32768) * (var3 / 32768)) / 128;
var5 = var3 - ((var4 * ((int32_t)calib_data->dig_h1)) / 16);
var5 = (var5 < 0 ? 0 : var5);
var5 = (var5 > 419430400 ? 419430400 : var5);
humidity = ((uint32_t)(var5 / 4096))>>10;

meas->temp=temperature;
meas->press=(pressure*7600/101325+5)/10;
meas->hum=(humidity>99?99:humidity);
}

uint8_t BME280_getID(void)
{
    //Получение ID датчика, нужно для определения его типа
    uint8_t id=0;
    uint8_t addr = BME280_CHIP_ID_ADDR;

    I2C_Write(BME280_I2C_ADDR_WRITE, &addr, 1U);
    I2C_Read(BME280_I2C_ADDR_READ, &id, 1U);
    return id;
}

void BME280_GetStatus(void)
{
    uint8_t data[4]={0};
    uint8_t addr = BME280_CTRL_HUM_ADDR;

    I2C_Write(BME280_I2C_ADDR_WRITE, &addr, 1U);
    I2C_Read(BME280_I2C_ADDR_READ, data, 4U);
}

void BME280_GetCalibData(struct bme280_calib_data* calib_data)
{
    //Получение информации и расчет компенсационных данных
    uint8_t calib_1[26]={0};
    uint8_t calib_2[7]={0};
    uint8_t addr = BME280_TPH_CALIB_ADDR;

    I2C_Write(BME280_I2C_ADDR_WRITE, &addr, 1U);
    I2C_Read(BME280_I2C_ADDR_READ, calib_1, 26U);

    addr = BME280_H_CALIB_ADDR;
    I2C_Write(BME280_I2C_ADDR_WRITE, &addr, 1U);
    I2C_Read(BME280_I2C_ADDR_READ, calib_2, 7U);

    calib_data->dig_t1 = calib_1[1]<<8 | calib_1[0];
    calib_data->dig_t2 = calib_1[3]<<8 | calib_1[2];
    calib_data->dig_t3 = calib_1[5]<<8 | calib_1[4];
    calib_data->dig_p1 = calib_1[7]<<8 | calib_1[6];
    calib_data->dig_p2 = calib_1[9]<<8 | calib_1[8];
    calib_data->dig_p3 = calib_1[11]<<8 | calib_1[10];
    calib_data->dig_p4 = calib_1[13]<<8 | calib_1[12];
    calib_data->dig_p5 = calib_1[15]<<8 | calib_1[14];
    calib_data->dig_p6 = calib_1[17]<<8 | calib_1[16];
    calib_data->dig_p7 = calib_1[19]<<8 | calib_1[18];
    calib_data->dig_p8 = calib_1[21]<<8 | calib_1[20];
    calib_data->dig_p9 = calib_1[23]<<8 | calib_1[22];
    calib_data->dig_h1 = calib_1[25];
    calib_data->dig_h2 = calib_2[1]<<8 | calib_2[0];
    calib_data->dig_h3 = calib_2[2];
    calib_data->dig_h4 = calib_2[3]<<4 | (calib_2[4] & 0xF);
    calib_data->dig_h5 = calib_2[5]<<4 | (calib_2[4]>>4);
    calib_data->dig_h6 = calib_2[6];
}

void BME280_Init(void){
  //инициализация датчика
uint8_t data[6] = {BME280_CONFIG_ADDR, BME280_T_STANDBY_1000 | BME280_FILTER_16,
		BME280_CTRL_HUM_ADDR, BME280_H_OVERSAMPLING_1X, BME280_CTRL_MEAS_ADDR,
		BME280_P_OVERSAMPLING_16X | BME280_T_OVERSAMPLING_16X | BME280_NORMAL_MODE};

I2C_Write(BME280_I2C_ADDR_WRITE, data, 6U);
}
