#ifndef BME280_H_
#define BME280_H_

struct bme280_calib_data
{
    uint16_t dig_t1;
    int16_t dig_t2;
    int16_t dig_t3;
    uint16_t dig_p1;
    int16_t dig_p2;
    int16_t dig_p3;
    int16_t dig_p4;
    int16_t dig_p5;
    int16_t dig_p6;
    int16_t dig_p7;
    int16_t dig_p8;
    int16_t dig_p9;
    uint8_t dig_h1;
    int16_t dig_h2;
    uint8_t dig_h3;
    int16_t dig_h4;
    int16_t dig_h5;
    int8_t dig_h6;
    int32_t t_fine;
};

struct bme280_data
{
    uint32_t press;
    int32_t temp;
    uint32_t hum;
};

struct bme280_uncomp_data
{
    uint32_t pressure;
    uint32_t temperature;
    uint32_t humidity;
};

#define BME280_TPH_CALIB_ADDR                     (uint8_t)0x88
#define BME280_CHIP_ID_ADDR                       (uint8_t)0xD0
#define BME280_RESET_ADDR                         (uint8_t)0xE0
#define BME280_H_CALIB_ADDR                       (uint8_t)0xE1
#define BME280_CTRL_HUM_ADDR                      (uint8_t)0xF2
#define BME280_STATUS_ADDR                        (uint8_t)0xF3
#define BME280_CTRL_MEAS_ADDR                     (uint8_t)0xF4
#define BME280_CONFIG_ADDR                        (uint8_t)0xF5
#define BME280_DATA_ADDR                          (uint8_t)0xF7

#define BME280_MODE_MSK                           (uint8_t)0x03
#define BME280_MODE_POS                           0U
#define BME280_SLEEP_MODE                         ((uint8_t)0x00 << BME280_MODE_POS)
#define BME280_FORCED_MODE                        ((uint8_t)0x01 << BME280_MODE_POS)
#define BME280_NORMAL_MODE                        ((uint8_t)0x03 << BME280_MODE_POS)

#define BME280_CTRL_HUM_MSK                       (uint8_t)0x07
#define BME280_CTRL_PRESS_MSK                     (uint8_t)0x1C
#define BME280_CTRL_TEMP_MSK                      (uint8_t)0xE0
#define BME280_CTRL_HUM_POS                       0U
#define BME280_CTRL_PRESS_POS                     2U
#define BME280_CTRL_TEMP_POS                      5U
#define BME280_NO_OVERSAMPLING                    (uint8_t)0x00
#define BME280_T_OVERSAMPLING_1X                  ((uint8_t)0x01 << BME280_CTRL_TEMP_POS)
#define BME280_T_OVERSAMPLING_2X                  ((uint8_t)0x02 << BME280_CTRL_TEMP_POS)
#define BME280_T_OVERSAMPLING_4X                  ((uint8_t)0x03 << BME280_CTRL_TEMP_POS)
#define BME280_T_OVERSAMPLING_8X                  ((uint8_t)0x04 << BME280_CTRL_TEMP_POS)
#define BME280_T_OVERSAMPLING_16X                 ((uint8_t)0x05 << BME280_CTRL_TEMP_POS)

#define BME280_P_OVERSAMPLING_1X                  ((uint8_t)0x01 << BME280_CTRL_PRESS_POS)
#define BME280_P_OVERSAMPLING_2X                  ((uint8_t)0x02 << BME280_CTRL_PRESS_POS)
#define BME280_P_OVERSAMPLING_4X                  ((uint8_t)0x03 << BME280_CTRL_PRESS_POS)
#define BME280_P_OVERSAMPLING_8X                  ((uint8_t)0x04 << BME280_CTRL_PRESS_POS)
#define BME280_P_OVERSAMPLING_16X                 ((uint8_t)0x05 << BME280_CTRL_PRESS_POS)

#define BME280_H_OVERSAMPLING_1X                  ((uint8_t)0x01 << BME280_CTRL_HUM_POS)
#define BME280_H_OVERSAMPLING_2X                  ((uint8_t)0x02 << BME280_CTRL_HUM_POS)
#define BME280_H_OVERSAMPLING_4X                  ((uint8_t)0x03 << BME280_CTRL_HUM_POS)
#define BME280_H_OVERSAMPLING_8X                  ((uint8_t)0x04 << BME280_CTRL_HUM_POS)
#define BME280_H_OVERSAMPLING_16X                 ((uint8_t)0x05 << BME280_CTRL_HUM_POS)

#define BME280_T_STANDBY_MSK                      (uint8_t)0xE0
#define BME280_T_STANDBY_POS                      5U
#define BME280_T_STANDBY_0D5                      ((uint8_t)0x00 << BME280_T_STANDBY_POS)
#define BME280_T_STANDBY_62D5                     ((uint8_t)0x01 << BME280_T_STANDBY_POS)
#define BME280_T_STANDBY_125                      ((uint8_t)0x02 << BME280_T_STANDBY_POS)
#define BME280_T_STANDBY_250                      ((uint8_t)0x03 << BME280_T_STANDBY_POS)
#define BME280_T_STANDBY_500                      ((uint8_t)0x04 << BME280_T_STANDBY_POS)
#define BME280_T_STANDBY_1000                     ((uint8_t)0x05 << BME280_T_STANDBY_POS)
#define BME280_T_STANDBY_10                       ((uint8_t)0x06 << BME280_T_STANDBY_POS)
#define BME280_T_STANDBY_20                       ((uint8_t)0x07 << BME280_T_STANDBY_POS)

#define BME280_FILTER_MSK                         (uint8_t)0x1C
#define BME280_FILTER_POS                         2U
#define BME280_FILTER_OFF                         ((uint8_t)0x00 << BME280_FILTER_POS)
#define BME280_FILTER_2                           ((uint8_t)0x01 << BME280_FILTER_POS)
#define BME280_FILTER_4                           ((uint8_t)0x02 << BME280_FILTER_POS)
#define BME280_FILTER_8                           ((uint8_t)0x03 << BME280_FILTER_POS)
#define BME280_FILTER_16                          ((uint8_t)0x04 << BME280_FILTER_POS)

void BME280_Init(void);
void BME280_GetMeasurement(struct bme280_data*, struct bme280_calib_data*);
void BME280_GetStatus(void);
void BME280_GetCalibData(struct bme280_calib_data*);
uint8_t BME280_getID(void);

#endif /* BME280_H_ */
