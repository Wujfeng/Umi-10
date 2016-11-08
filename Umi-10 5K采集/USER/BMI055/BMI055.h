/****************************************************************
 * Copyright (C) 2016, Yagro, all right reserved.
 * File name:    BMI055.h
 * Date:         2016.04.13
 * Description:  BMI055 Driver, SPI Communication Interface
*****************************************************************/

#ifndef __BMI055_H
#define __BMI055_H

#include "SPI1.h"
#include "SPI2.h"

/************* BMI055 CS Signal ************/

#define BMI055_GYRO_X_CS_LOW()    SPI1_CS1_LOW()
#define BMI055_GYRO_X_CS_HIGH()   SPI1_CS1_HIGH()

#define BMI055_GYRO_Y_CS_LOW()    SPI2_CS1_LOW()
#define BMI055_GYRO_Y_CS_HIGH()   SPI2_CS1_HIGH()


#define BMI055_GYRO_X_SendHalfWord(HalfWord)   SPI1_SendHalfWord(HalfWord)

#define BMI055_GYRO_Y_SendHalfWord(HalfWord)   SPI2_SendHalfWord(HalfWord)

/*******************************************/

/****** ACC GYRO TEMP Data Conversion ******/

#define ACC_LSB_TO_G          (1.0f/512.0f)
#define GYRO_LSB_TO_DPS       (500.0f/32768.0f)
#define TEMP_LSB_TO_C         (0.5f)

#define GYRO_LSB_TO_DPS_125   (125.0f/32768.0f)
#define GYRO_LSB_TO_DPS_250   (250.0f/32768.0f)
#define GYRO_LSB_TO_DPS_500   (500.0f/32768.0f)
#define GYRO_LSB_TO_DPS_1000  (1000.0f/32768.0f)
#define GYRO_LSB_TO_DPS_2000  (2000.0f/32768.0f)

/*******************************************/

/************** ACC Register ***************/

#define ACCD_ID             ((uint8_t)0x00)

#define ACCD_X_LSB          ((uint8_t)0x02)
#define ACCD_X_MSB          ((uint8_t)0x03)
#define ACCD_Y_LSB          ((uint8_t)0x04)
#define ACCD_Y_MSB          ((uint8_t)0x05)
#define ACCD_Z_LSB          ((uint8_t)0x06)
#define ACCD_Z_MSB          ((uint8_t)0x07)

#define ACCD_TEMP           ((uint8_t)0x08)

#define ACCD_INT_STATUS_0   ((uint8_t)0x09)
#define ACCD_INT_STATUS_1   ((uint8_t)0x0A)
#define ACCD_INT_STATUS_2   ((uint8_t)0x0B)
#define ACCD_INT_STATUS_3   ((uint8_t)0x0C)

#define ACCD_FIFO_STATUS    ((uint8_t)0x0E)

#define ACCD_RANGE          ((uint8_t)0x0F)
#define ACCD_BW             ((uint8_t)0x10)
#define ACCD_LPW            ((uint8_t)0x11)
#define ACCD_LOW_POWER      ((uint8_t)0x12)

#define ACCD_HBW            ((uint8_t)0x13)

#define ACCD_BGW_SOFTRESET  ((uint8_t)0x14)

#define ACCD_INT_EN_0       ((uint8_t)0x16)
#define ACCD_INT_EN_1       ((uint8_t)0x17)
#define ACCD_INT_EN_2       ((uint8_t)0x18)

#define ACCD_INT_MAP_0      ((uint8_t)0x19)
#define ACCD_INT_MAP_1      ((uint8_t)0x1A)
#define ACCD_INT_MAP_2      ((uint8_t)0x1B)

#define ACCD_INT_SRC        ((uint8_t)0x1E)
#define ACCD_INT_OUT_CTRL   ((uint8_t)0x20)
#define ACCD_INT_RST_LATCH  ((uint8_t)0x21)

#define ACCD_INT_0          ((uint8_t)0x22)
#define ACCD_INT_1          ((uint8_t)0x23)
#define ACCD_INT_2          ((uint8_t)0x24)
#define ACCD_INT_3          ((uint8_t)0x25)
#define ACCD_INT_4          ((uint8_t)0x26)
#define ACCD_INT_5          ((uint8_t)0x27)
#define ACCD_INT_6          ((uint8_t)0x28)
#define ACCD_INT_7          ((uint8_t)0x29)
#define ACCD_INT_8          ((uint8_t)0x2A)
#define ACCD_INT_9          ((uint8_t)0x2B)
#define ACCD_INT_A          ((uint8_t)0x2C)
#define ACCD_INT_B          ((uint8_t)0x2D)
#define ACCD_INT_C          ((uint8_t)0x2E)
#define ACCD_INT_D          ((uint8_t)0x2F)

#define ACCD_FIFO_CONFIG_0  ((uint8_t)0x30)

#define ACCD_PMU_SELF_TEST  ((uint8_t)0x32)

#define ACCD_TRIM_NVM_CTRL  ((uint8_t)0x33)

#define ACCD_BGW_SPI3_WDT   ((uint8_t)0x34)

#define ACCD_OFC_CTRL       ((uint8_t)0x36)
#define ACCD_OFC_SETTING    ((uint8_t)0x37)
#define ACCD_OFC_OFFSET_X   ((uint8_t)0x38)
#define ACCD_OFC_OFFSET_Y   ((uint8_t)0x39)
#define ACCD_OFC_OFFSET_Z   ((uint8_t)0x3A)

#define ACCD_TRIM_GP0       ((uint8_t)0x3B)
#define ACCD_TRIM_GP1       ((uint8_t)0x3C)

#define ACCD_FIFO_CONFIG_1  ((uint8_t)0x3E)
#define ACCD_FIFO_DATA      ((uint8_t)0x3F)

/********************************************/

/************** GYRO Register ***************/

#define GYRO_ID             ((uint8_t)0x00)

#define GYRO_X_LSB          ((uint8_t)0x02)
#define GYRO_X_MSB          ((uint8_t)0x03)
#define GYRO_Y_LSB          ((uint8_t)0x04)
#define GYRO_Y_MSB          ((uint8_t)0x05)
#define GYRO_Z_LSB          ((uint8_t)0x06)
#define GYRO_Z_MSB          ((uint8_t)0x07)

#define GYRO_INT_STATUS_0   ((uint8_t)0x09)
#define GYRO_INT_STATUS_1   ((uint8_t)0x0A)
#define GYRO_INT_STATUS_2   ((uint8_t)0x0B)
#define GYRO_INT_STATUS_3   ((uint8_t)0x0C)

#define GYRO_FIFO_STATUS    ((uint8_t)0x0E)

#define GYRO_RANGE          ((uint8_t)0x0F)
#define GYRO_BW             ((uint8_t)0x10)
#define GYRO_LPM1           ((uint8_t)0x11)
#define GYRO_LPM2           ((uint8_t)0x12)

#define GYRO_HBW            ((uint8_t)0x13)

#define GYRO_BGW_SOFTRESET  ((uint8_t)0x14)

#define GYRO_INT_EN_0       ((uint8_t)0x15)
#define GYRO_INT_EN_1       ((uint8_t)0x16)

#define GYRO_INT_MAP_0      ((uint8_t)0x17)
#define GYRO_INT_MAP_1      ((uint8_t)0x18)
#define GYRO_INT_MAP_2      ((uint8_t)0x19)

#define GYRO_Register0      ((uint8_t)0x1A)
#define GYRO_Register1      ((uint8_t)0x1B)
#define GYRO_Register2      ((uint8_t)0x1C)
#define GYRO_Register3      ((uint8_t)0x1E)

#define GYRO_INT_RST_LATCH  ((uint8_t)0x21)

#define GYRO_High_Th_x      ((uint8_t)0x22)
#define GYRO_High_Dur_x     ((uint8_t)0x23)
#define GYRO_High_Th_y      ((uint8_t)0x24)
#define GYRO_High_Dur_y     ((uint8_t)0x25)
#define GYRO_High_Th_z      ((uint8_t)0x26)
#define GYRO_High_Dur_z     ((uint8_t)0x27)

#define GYRO_SOC            ((uint8_t)0x31)

#define GYRO_A_FOC          ((uint8_t)0x32)

#define GYRO_TRIM_NVM_CTRL  ((uint8_t)0x33)

#define GYRO_BGW_SPI3_WDT   ((uint8_t)0x34)

#define GYRO_OFC1           ((uint8_t)0x36)
#define GYRO_OFC2           ((uint8_t)0x37)
#define GYRO_OFC3           ((uint8_t)0x38)
#define GYRO_OFC4           ((uint8_t)0x39)

#define GYRO_TRIM_GP0       ((uint8_t)0x3A)
#define GYRO_TRIM_GP1       ((uint8_t)0x3B)

#define GYRO_BIST           ((uint8_t)0x3C)

#define GYRO_FIFO_CONFIG_0  ((uint8_t)0x3D)
#define GYRO_FIFO_CONFIG_1  ((uint8_t)0x3E)

#define GYRO_FIFO_DATA      ((uint8_t)0x3F)

/**********************************************/

/************** ACC Range *********************/

#define ACC_2g              ((uint8_t)0x03)
#define ACC_4g              ((uint8_t)0x05)
#define ACC_8g              ((uint8_t)0x08)
#define ACC_16g             ((uint8_t)0x0C)

/**********************************************/

/********** GYRO Filter Bandwidth *************/

#define ACC_BW_7_81Hz       ((uint8_t)0x08)
#define ACC_BW_15_63Hz      ((uint8_t)0x09)
#define ACC_BW_31_25Hz      ((uint8_t)0x0A)
#define ACC_BW_62_5Hz       ((uint8_t)0x0B)
#define ACC_BW_125Hz        ((uint8_t)0x0C)
#define ACC_BW_250Hz        ((uint8_t)0x0D)
#define ACC_BW_500Hz        ((uint8_t)0x0E)
#define ACC_BW_1000Hz       ((uint8_t)0x0F)

/**********************************************/

/*************** GYRO Range *******************/

#define GYRO_125dps         ((uint8_t)0x04)
#define GYRO_250dps         ((uint8_t)0x03)
#define GYRO_500dps         ((uint8_t)0x02)
#define GYRO_1000dps        ((uint8_t)0x01)
#define GYRO_2000dps        ((uint8_t)0x00)

/**********************************************/

/********** GYRO Filter Bandwidth *************/

/*GYRO Rate 100Hz*/
#define GYRO_BW_12Hz        ((uint8_t)0x05)
#define GYRO_BW_32Hz        ((uint8_t)0x07)

/*GYRO Rate 200Hz*/
#define GYRO_BW_23Hz        ((uint8_t)0x04)
#define GYRO_BW_64Hz        ((uint8_t)0x06)

/*GYRO Rate 400Hz*/
#define GYRO_BW_47Hz        ((uint8_t)0x03)

/*GYRO Rate 1000Hz*/
#define GYRO_BW_116Hz       ((uint8_t)0x02)

/*GYRO Rate 2000Hz*/
#define GYRO_BW_230Hz       ((uint8_t)0x01)
#define GYRO_BW_523Hz       ((uint8_t)0x00)

/**********************************************/

#define BMI055_X_INT3		GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_2)
#define BMI055_Y_INT3		GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_6)

#pragma pack(push)
#pragma pack(1)

typedef struct
{
	int16_t GYRO_X;
	int16_t GYRO_Y;

}BMI055;

typedef struct
{
  float fGyroX;
  float fGyroY;

}fData_BMI055;

#pragma pack(pop)


extern BMI055 bmi055;


uint8_t BMI055_X_ReadData(uint8_t ADDR);
void BMI055_X_WriteData(uint8_t ADDR, uint8_t Data);
uint8_t BMI055_Y_ReadData(uint8_t ADDR);
void BMI055_Y_WriteData(uint8_t ADDR, uint8_t Data);
void BMI055_X_INT3_Init(void);
void BMI055_Y_INT3_Init(void);
void BMI055_X_Init(uint8_t Range, uint8_t Filter_BW);
void BMI055_Y_Init(uint8_t Range, uint8_t Filter_BW);
uint8_t BMI055_Init(uint8_t Range, uint8_t WB);
void BMI055_Reset(void);
void BMI055_Read(BMI055* bmi055);
void BMI055_DataConversion(BMI055* bmi055, fData_BMI055* fData_bmi055);

#endif 	/* __BMI055_H */
