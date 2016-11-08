/****************************************************************
 * Copyright (C) 2016, Yagro, all right reserved.
 * File name:    BMP280.h
 * Date:         2016.07.07
 * Description:  BMP280 Driver, SPI Communication Interface
*****************************************************************/

#ifndef __BMP280_H
#define __BMP280_H

#include "SPI1.h"
#include "math.h"

/************ BMP280 CS Signal *****************/

#define BMP280_CS_LOW()    SPI1_CS2_LOW()
                         
#define BMP280_CS_HIGH()   SPI1_CS2_HIGH()


#define BMP280_SendHalfWord(HalfWord)   SPI1_SendHalfWord(HalfWord)

/************************************************/

/************* Data Conversion ******************/

#define BMP280_CONVERSION1      (0.000009869232667)  /* (1.0f / 101325.0f) */
#define BMP280_CONVERSION2      (0.190294957183635)  /* (1.0f / 5.255f) */

/************************************************/

/**************** BMP280 Register ***************/

#define BMP280_DIG_T1_LSB_REG   ((uint8_t)0x08)
#define BMP280_DIG_T1_MSB_REG   ((uint8_t)0x09)
#define BMP280_DIG_T2_LSB_REG   ((uint8_t)0x0A)
#define BMP280_DIG_T2_MSB_REG   ((uint8_t)0x0B)
#define BMP280_DIG_T3_LSB_REG   ((uint8_t)0x0C)
#define BMP280_DIG_T3_MSB_REG   ((uint8_t)0x0D)
#define BMP280_DIG_P1_LSB_REG   ((uint8_t)0x0E)
#define BMP280_DIG_P1_MSB_REG   ((uint8_t)0x0F)
#define BMP280_DIG_P2_LSB_REG   ((uint8_t)0x10)
#define BMP280_DIG_P2_MSB_REG   ((uint8_t)0x11)
#define BMP280_DIG_P3_LSB_REG   ((uint8_t)0x12)
#define BMP280_DIG_P3_MSB_REG   ((uint8_t)0x13)
#define BMP280_DIG_P4_LSB_REG   ((uint8_t)0x14)
#define BMP280_DIG_P4_MSB_REG   ((uint8_t)0x15)
#define BMP280_DIG_P5_LSB_REG   ((uint8_t)0x16)
#define BMP280_DIG_P5_MSB_REG   ((uint8_t)0x17)
#define BMP280_DIG_P6_LSB_REG   ((uint8_t)0x18)
#define BMP280_DIG_P6_MSB_REG   ((uint8_t)0x19)
#define BMP280_DIG_P7_LSB_REG   ((uint8_t)0x1A)
#define BMP280_DIG_P7_MSB_REG   ((uint8_t)0x1B)
#define BMP280_DIG_P8_LSB_REG   ((uint8_t)0x1C)
#define BMP280_DIG_P8_MSB_REG   ((uint8_t)0x1D)
#define BMP280_DIG_P9_LSB_REG   ((uint8_t)0x1E)
#define BMP280_DIG_P9_MSB_REG   ((uint8_t)0x1F)

#define BMP280_CHIPID_REG       ((uint8_t)0x50)  /* Chip ID Register */
#define BMP280_RESET_REG        ((uint8_t)0x60)  /* Softreset Register */
#define BMP280_STATUS_REG       ((uint8_t)0x73)  /* Status Register */
#define BMP280_CTRL_MEAS_REG    ((uint8_t)0x74)  /* Ctrl Measure Register */
#define BMP280_CONFIG_REG       ((uint8_t)0x75)  /* Configuration Register */
#define BMP280_PRESS_MSB_REG    ((uint8_t)0x77)  /* Pressure MSB Register */
#define BMP280_PRESS_LSB_REG    ((uint8_t)0x78)  /* Pressure LSB Register */
#define BMP280_PRESS_XLSB_REG   ((uint8_t)0x79)  /* Pressure XLSB Register */
#define BMP280_TEMP_MSB_REG     ((uint8_t)0x7A)  /* Temperature MSB Reg */
#define BMP280_TEMP_LSB_REG     ((uint8_t)0x7B)  /* Temperature LSB Reg */
#define BMP280_TEMP_XLSB_REG    ((uint8_t)0x7C)  /* Temperature XLSB Reg */

/************************************************/

/************* BMP280 Power Modes ***************/

#define BMP280_SLEEP_MODE       ((uint8_t)0x00)
#define BMP280_FORCED_MODE      ((uint8_t)0x01)
#define BMP280_NORMAL_MODE      ((uint8_t)0x03)

/************************************************/

/************* BMP280 Oversampling **************/

#define BMP280_OVERSAMPLING_SKIPPED ((uint8_t)0x00)
#define BMP280_OVERSAMPLING_X1      ((uint8_t)0x01)
#define BMP280_OVERSAMPLING_X2      ((uint8_t)0x02)
#define BMP280_OVERSAMPLING_X4      ((uint8_t)0x03)
#define BMP280_OVERSAMPLING_X8      ((uint8_t)0x04)
#define BMP280_OVERSAMPLING_X16     ((uint8_t)0x05)

/************************************************/

/************ BMP280 Measurement Time ***********/

#define BMP280_ULTRA_LOW_POWER_OSRS_P         BMP280_OVERSAMPLING_X1
#define BMP280_ULTRA_LOW_POWER_OSRS_T         BMP280_OVERSAMPLING_X1

#define BMP280_LOW_POWER_OSRS_P	              BMP280_OVERSAMPLING_X2
#define BMP280_LOW_POWER_OSRS_T	              BMP280_OVERSAMPLING_X1

#define BMP280_STANDARD_RESOLUTION_OSRS_P     BMP280_OVERSAMPLING_X4
#define BMP280_STANDARD_RESOLUTION_OSRS_T     BMP280_OVERSAMPLING_X1

#define BMP280_HIGH_RESOLUTION_OSRS_P         BMP280_OVERSAMPLING_X8
#define BMP280_HIGH_RESOLUTION_OSRS_T         BMP280_OVERSAMPLING_X1

#define BMP280_ULTRA_HIGH_RESOLUTION_OSRS_P   BMP280_OVERSAMPLING_X16
#define BMP280_ULTRA_HIGH_RESOLUTION_OSRS_T   BMP280_OVERSAMPLING_X2

/************************************************/

/************ BMP280 Standby Time ***************/

#define BMP280_STANDBY_TIME_0_5_MS    ((uint8_t)0x00)
#define BMP280_STANDBY_TIME_62_5_MS   ((uint8_t)0x01)
#define BMP280_STANDBY_TIME_125_MS    ((uint8_t)0x02)
#define BMP280_STANDBY_TIME_250_MS    ((uint8_t)0x03)
#define BMP280_STANDBY_TIME_500_MS    ((uint8_t)0x04)
#define BMP280_STANDBY_TIME_1000_MS   ((uint8_t)0x05)
#define BMP280_STANDBY_TIME_2000_MS   ((uint8_t)0x06)
#define BMP280_STANDBY_TIME_4000_MS   ((uint8_t)0x07)

/************************************************/

/************** BMP280 IIR Filter ***************/

#define BMP280_FILTER_COEFF_OFF   ((uint8_t)0x00)
#define BMP280_FILTER_COEFF_2     ((uint8_t)0x01)
#define BMP280_FILTER_COEFF_4     ((uint8_t)0x02)
#define BMP280_FILTER_COEFF_8     ((uint8_t)0x03)
#define BMP280_FILTER_COEFF_16    ((uint8_t)0x04)

/************************************************/

/**************** BMP280 Modes ******************/

#define BMP280_INDOOR_NAVIGATION_MODE         ((uint8_t)0x00) /* Indoor Navigation Mode.(osrs_p:x16, osrs_t:x2, Filter:16, Timing:0.5ms)(ODR[Hz]:26.3, BW[Hz]:0.55) */
#define BMP280_DROP_DETECTION_MODE            ((uint8_t)0x01) /* Drop Detection Mode.(osrs_p:x2, osrs_t:x1, Filter:off, Timing:0.5ms)(ODR[Hz]:125, BW[Hz]:full) */
#define BMP280_HANDHELD_DEVICE_DYNAMIC_MODE   ((uint8_t)0x02) /* Handheld Device Dynamic Mode.(osrs_p:x4, osrs_t:x1, Filter:16, Timing:0.5ms)(ODR[Hz]:83.3, BW[Hz]:1.75) */

/************************************************/


#pragma pack(push)
#pragma pack(1)

typedef struct
{
	uint16_t dig_T1;
	int16_t dig_T2;
	int16_t dig_T3;
	uint16_t dig_P1;
	int16_t dig_P2;
	int16_t dig_P3;
	int16_t dig_P4;
	int16_t dig_P5;
	int16_t dig_P6;
	int16_t dig_P7;
	int16_t dig_P8;
	int16_t dig_P9;

	int32_t t_fine;
  
}BMP280_Calib;

typedef struct
{
  int32_t PRESS;

  int32_t TEMP;

}BMP280;

typedef struct
{
  double dPress;
  float fAltitude;

  float fTemp;

}fData_BMP280;

typedef struct
{
  long double sum_dPress;
  double  sum_fAltitude;

  double  sum_fTemp;
  
  int    count;

  double ave_dPress;
  float  ave_fAltitude;
  float  ave_fTemp;
  
}BMP280_Final_data;


#pragma pack(pop)


extern BMP280_Calib bmp280_Calib;
extern BMP280 bmp280;
extern fData_BMP280 fData_bmp280;
extern BMP280_Final_data MCU_Out_Press;

uint8_t BMP280_ReadData(uint8_t ADDR);
void BMP280_WriteData(uint8_t ADDR, uint8_t Data);
void BMP280_Reset(void);
uint8_t BMP280_Init(BMP280_Calib* bmp280_Calib, uint8_t Mode);
void BMP280_Mode(uint8_t Mode);
void BMP280_Read(BMP280* bmp280);
void BMP280_UT2TEMP(BMP280_Calib* bmp280_Calib, BMP280* bmp280, fData_BMP280* fData_bmp280);
void BMP280_UP2PRESS(BMP280_Calib* bmp280_Calib, BMP280* bmp280, fData_BMP280* fData_bmp280);

void BMP280_Data_Sum(fData_BMP280* fData_bmp280,BMP280_Final_data* BMP280_SUM);
unsigned char BMP280_calculate_AVE(BMP280_Final_data* BMP280_SUM);

#endif 	/* __BMP280_H */
