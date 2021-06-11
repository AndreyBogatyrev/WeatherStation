#include "stm32f1xx.h"
#include "i2c.h"

void I2C_Init(void)
{
  //Инициализация I2C модуля к контроллере
    RCC->APB2ENR |= RCC_APB2ENR_IOPBEN | RCC_APB2ENR_AFIOEN;
    GPIOB->CRL |= GPIO_CRL_CNF6 | GPIO_CRL_CNF7 | GPIO_CRL_MODE6_0 | GPIO_CRL_MODE7_0;
    RCC->APB1ENR |= RCC_APB1ENR_I2C1EN;

    I2C1->CR2 |= 0x8UL;
    I2C1->CCR |= 0x32UL;
    I2C1->TRISE &= (~I2C_TRISE_TRISE | 0x9UL);
    I2C1->CR1 |= I2C_CR1_PE;
}

void I2C_Write(uint8_t addr, uint8_t* data, uint32_t num)
{
  //Передача информации от контроллера
    I2C1->CR1 |= I2C_CR1_ACK;
    I2C1->CR1 |= I2C_CR1_START;

    while ((I2C1->SR1 & I2C_SR1_SB)==0){};

    (void) I2C1->SR1;
    I2C1->DR = addr;

    while ((I2C1->SR1 & I2C_SR1_ADDR)==0){};

    (void) I2C1->SR1;
    (void) I2C1->SR2;
    I2C1->DR = *data;

    for (uint32_t i=1; i<num; i++)
    {
	I2C1->DR = *(data+i);
	while ((I2C1->SR1 & I2C_SR1_TXE)==0){};
    }

    while ((I2C1->SR1 & I2C_SR1_BTF)==0){};

    I2C1->CR1 |= I2C_CR1_STOP;
}

void I2C_Read(uint8_t addr, uint8_t* data, uint32_t num)
{
  //Передача информации в контроллер
    I2C1->CR1 |= I2C_CR1_ACK;
    I2C1->CR1 |= I2C_CR1_START;

    while ((I2C1->SR1 & I2C_SR1_SB)==0){};

    (void) I2C1->SR1;
    I2C1->DR = addr;

    while ((I2C1->SR1 & I2C_SR1_ADDR)==0){};

    (void) I2C1->SR1;
    (void) I2C1->SR2;

    for (uint32_t i=0; i<num; i++)
    {
	if((num-i)==1)
	{
	    I2C1->CR1 &= ~I2C_CR1_ACK;
	    I2C1->CR1 |= I2C_CR1_STOP;
	}

	while ((I2C1->SR1 & I2C_SR1_RXNE)==0){};

	*(data+i) = I2C1->DR;
    }
}
