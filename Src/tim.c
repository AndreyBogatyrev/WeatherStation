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
#include "tim.h"

void TIM_Init(){
  RCC->APB2ENR |= RCC_APB2ENR_IOPAEN | RCC_APB2ENR_AFIOEN | RCC_APB2ENR_TIM1EN;
  GPIOA->CRH &= ~GPIO_CRH_CNF8;
  GPIOA->CRH |= GPIO_CRH_CNF8_1;

  TIM1->PSC = 79999U;
  TIM1->CCMR1 = TIM_CCMR1_CC2S_1 | TIM_CCMR1_IC1F_3;
  TIM1->CCER = TIM_CCER_CC2P;
  TIM1->SMCR = TIM_SMCR_TS_2 | TIM_SMCR_TS_0;
  TIM1->SMCR |= TIM_SMCR_SMS_2 | TIM_SMCR_SMS_0;
  TIM1->CR1 = TIM_CR1_CEN;
  TIM1->DIER = TIM_DIER_TIE;
}
