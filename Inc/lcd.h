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

#ifndef LCD_H_
#define LCD_H_

typedef struct LCD_DS3231
{
    uint8_t hour[2];
    uint8_t min[2];
    uint8_t sec[2];

    uint8_t dayW[6];
    uint8_t day[2];
    uint8_t month[2];
    uint8_t year[2];

    uint8_t colon;
    uint8_t slash[2];
};

struct LCD_BME280
{
    uint8_t sign;
    uint8_t percent;
    uint8_t dot;
    uint8_t degree[2];
    uint8_t C[2];
    uint8_t m[2];
    uint8_t H[2];
    uint8_t g[2];

    uint8_t temp[3];
    uint8_t press[3];
    uint8_t hum[3];
    uint8_t temp_fd;
    uint8_t hum_fd;
};

void Print_DS3231(struct LCD_DS3231*);
void Print_BME280(struct LCD_BME280*);
void LCD_Init(void);
void LCD_Write(uint8_t, uint32_t);

#endif /* LCD_H_ */
