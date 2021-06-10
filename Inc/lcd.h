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
