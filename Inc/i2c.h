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

#ifndef I2C_H_
#define I2C_H_

#define LCD_I2C_ADDR_WRITE                        (uint8_t)0x40
#define LCD_I2C_ADDR_READ                         (uint8_t)0x41
#define BME280_I2C_ADDR_WRITE                     (uint8_t)0xEC
#define BME280_I2C_ADDR_READ                      (uint8_t)0xED
#define DS3231_I2C_ADDR_WRITE                     (uint8_t)0xD0
#define DS3231_I2C_ADDR_READ                      (uint8_t)0xD1

void I2C_Init(void);
void I2C_Write(uint8_t,  uint8_t*, uint32_t);
void I2C_Read(uint8_t,  uint8_t*, uint32_t);

#endif /* I2C_H_ */
