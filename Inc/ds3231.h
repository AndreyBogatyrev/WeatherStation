#ifndef DS3231_H_
#define DS3231_H_

struct DS3231_calendar
{
    uint8_t second;
    uint8_t minute;
    uint8_t hour;
    uint8_t dayWeek;
    uint8_t dayMonth;
    uint8_t month;
    uint8_t year;
};

#define DS3231_SEC_ADDR                           (uint8_t)0x0
#define DS3231_MIN_ADDR                           (uint8_t)0x1
#define DS3231_HOUR_ADDR                          (uint8_t)0x2
#define DS3231_DAY_WEEK_ADDR                      (uint8_t)0x3
#define DS3231_DAY_MONTH_ADDR                     (uint8_t)0x4
#define DS3231_MONTH_ADDR                         (uint8_t)0x5
#define DS3231_YEAR_ADDR                          (uint8_t)0x6
#define DS3231_A1M1_ADDR                          (uint8_t)0x7
#define DS3231_A1M2_ADDR                          (uint8_t)0x8
#define DS3231_A1M3_ADDR                          (uint8_t)0x9
#define DS3231_A1M4_ADDR                          (uint8_t)0xA
#define DS3231_CONTROL_ADDR                       (uint8_t)0xE
#define DS3231_CON_STAT_ADDR                      (uint8_t)0xF

void DS3231_Init();
void DS3231_GetData(struct DS3231_calendar*);
void DS3231_SetData(struct DS3231_calendar*);

#endif /* DS3231_H_ */
