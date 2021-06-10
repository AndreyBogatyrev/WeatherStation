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
