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

#ifndef INC_MAIN_H_
#define INC_MAIN_H_

void TIM2_IRQHandler(void);
void EXTI0_IRQHandler(void);
void SysTick_Handler(void);
void SysTickEnable(uint32_t);
void SysTickDisable(void);
void Wait(uint32_t);
void SetSysClock(void);
void Update_Data(void);
int main(void);
void Toggle_LCD_LED(void);
void Convert_DS3231_for_LCD(struct LCD_DS3231*, struct DS3231_calendar);
void Convert_BME280_for_LCD(struct LCD_BME280*, struct bme280_data);
void Toggle_Cursor_Blink(void);
void Change_Cursor_Pos(void);
uint8_t Division(uint32_t);
uint8_t DECtoBCD(uint8_t);
uint8_t BCDtoDEC(uint8_t);

#endif /* INC_MAIN_H_ */
