/****************************************************************
 * Copyright (C) 2016, Yagro, all right reserved.
 * File name:    BMI055.c
 * Date:         2016.04.13
 * Description:  BMI055 Driver, SPI Communication Interface
*****************************************************************/


#include "BMI055.h"


BMI055 bmi055;

/****************************************************
 * Function:    BMI055_X_ReadData
 * Description: Read data from BMI055 GYRO register.
 * Input:       ADDR: BMI055 GYRO Register Address
 * Output:
 * Return:      Data register
*****************************************************/
uint8_t BMI055_X_ReadData(uint8_t ADDR)
{
  uint16_t data, i;

  ADDR = ADDR|0x80;

  for(i=0; i<0x05; i++);

  BMI055_GYRO_X_CS_LOW();
  data = BMI055_GYRO_X_SendHalfWord(((uint16_t)ADDR<<8)|0xFF);
  BMI055_GYRO_X_CS_HIGH();

  for(i=0; i<0x05; i++);

  return(data);
}

/****************************************************
 * Function:    BMI055_X_WriteData
 * Description: Write data from BMI055 GYRO register.
 * Input:       ADDR: BMI055 GYRO Register Address
 *              Data: Data
 * Output:
 * Return:
*****************************************************/
void BMI055_X_WriteData(uint8_t ADDR, uint8_t Data)
{
  uint8_t i;

  for(i=0; i<0x05; i++);

  BMI055_GYRO_X_CS_LOW();
  BMI055_GYRO_X_SendHalfWord(((uint16_t)ADDR<<8)|Data);
  BMI055_GYRO_X_CS_HIGH();

  for(i=0; i<0x05; i++);
}

/****************************************************
 * Function:    BMI055_Y_ReadData
 * Description: Read data from BMI055 GYRO register.
 * Input:       ADDR: BMI055 GYRO Register Address
 * Output:
 * Return:      Data register
*****************************************************/
uint8_t BMI055_Y_ReadData(uint8_t ADDR)
{
  uint16_t data, i;

  ADDR = ADDR|0x80;

  for(i=0; i<0x05; i++);

  BMI055_GYRO_Y_CS_LOW();
  data = BMI055_GYRO_Y_SendHalfWord(((uint16_t)ADDR<<8)|0xFF);
  BMI055_GYRO_Y_CS_HIGH();

  for(i=0; i<0x05; i++);

  return(data);
}

/****************************************************
 * Function:    BMI055_Y_WriteData
 * Description: Write data from BMI055 GYRO register.
 * Input:       ADDR: BMI055 GYRO Register Address
 *              Data: Data
 * Output:
 * Return:
*****************************************************/
void BMI055_Y_WriteData(uint8_t ADDR, uint8_t Data)
{
  uint8_t i;

  for(i=0; i<0x05; i++);

  BMI055_GYRO_Y_CS_LOW();
  BMI055_GYRO_Y_SendHalfWord(((uint16_t)ADDR<<8)|Data);
  BMI055_GYRO_Y_CS_HIGH();

  for(i=0; i<0x05; i++);
}

/****************************************************
 * Function:    BMI055_X_INT3_Init
 * Description: Initialization BMI055 INT3 pin.
 * Input:
 * Output:
 * Return:
*****************************************************/
void BMI055_X_INT3_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;
  EXTI_InitTypeDef EXTI_InitStructure;

  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA , ENABLE);
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;

  GPIO_Init(GPIOA, &GPIO_InitStructure);
  
  SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOA, EXTI_PinSource2);

  EXTI_InitStructure.EXTI_Line = EXTI_Line2;
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  EXTI_Init(&EXTI_InitStructure);
}

/****************************************************
 * Function:    BMI055_Y_INT3_Init
 * Description: Initialization BMI055 INT3 pin.
 * Input:
 * Output:
 * Return:
*****************************************************/
void BMI055_Y_INT3_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;
  EXTI_InitTypeDef EXTI_InitStructure;

  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB , ENABLE);
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;

  GPIO_Init(GPIOB, &GPIO_InitStructure);
  
  SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOB, EXTI_PinSource6);

  EXTI_InitStructure.EXTI_Line = EXTI_Line6;
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  EXTI_Init(&EXTI_InitStructure);
}

/****************************************************
 * Function:    BMI055_X_Init
 * Description: Initialization BMI055 GYRO.
 * Input:       Range: GYRO Range
 *              Filter_BW: ACC Filter Bandwidth
 * Output:
 * Return:
*****************************************************/
void BMI055_X_Init(uint8_t Range, uint8_t Filter_BW)
{
  BMI055_X_WriteData(GYRO_RANGE, Range);
  BMI055_X_WriteData(GYRO_BW, Filter_BW);
}

/****************************************************
 * Function:    BMI055_Y_Init
 * Description: Initialization BMI055 GYRO.
 * Input:       Range: GYRO Range
 *              Filter_BW: GYRO Filter Bandwidth
 * Output:
 * Return:
*****************************************************/
void BMI055_Y_Init(uint8_t Range, uint8_t Filter_BW)
{
  BMI055_Y_WriteData(GYRO_RANGE, Range);
  BMI055_Y_WriteData(GYRO_BW, Filter_BW);
}

/****************************************************
 * Function:    BMI055_Init
 * Description: Initialization BMI055.
 * Input:       Range
                WB
 * Output:
 * Return:      1: Normal, 0: Unusual
*****************************************************/
uint8_t BMI055_Init(uint8_t Range, uint8_t WB)
{
  if( BMI055_X_ReadData(GYRO_ID) != 0x0F )
  {
    return 0;
  }
  
  if( BMI055_Y_ReadData(GYRO_ID) != 0x0F )
  {
    return 0;
  }
  
  BMI055_X_INT3_Init();
  BMI055_Y_INT3_Init();
  BMI055_X_WriteData(GYRO_INT_EN_0, 0x80);
  BMI055_Y_WriteData(GYRO_INT_EN_0, 0x80);
  BMI055_X_WriteData(GYRO_INT_EN_1, 0x01);
  BMI055_Y_WriteData(GYRO_INT_EN_1, 0x01);
  BMI055_X_WriteData(GYRO_INT_MAP_1, 0x01);
  BMI055_Y_WriteData(GYRO_INT_MAP_1, 0x01);
  BMI055_X_Init(Range, WB);
  BMI055_Y_Init(Range, WB);
  
  return 1;
}

/****************************************************
 * Function:    BMI055_Reset
 * Description: Reset BMI055.
 * Input:
 * Output:
 * Return:
*****************************************************/
void BMI055_Reset(void)
{
  BMI055_X_WriteData(GYRO_BGW_SOFTRESET, 0xB6);
  BMI055_Y_WriteData(GYRO_BGW_SOFTRESET, 0xB6);
}

/****************************************************
 * Function:    BMI055_Read
 * Description: Read BMI055 ACC GYRO Data.
 * Input:       &bmi055
 * Output:
 * Return:
*****************************************************/
void BMI055_Read(BMI055* bmi055)
{
  bmi055->GYRO_X = BMI055_X_ReadData(GYRO_Z_LSB);
  bmi055->GYRO_X |= BMI055_X_ReadData(GYRO_Z_MSB)<<8;

  bmi055->GYRO_Y = BMI055_Y_ReadData(GYRO_Z_LSB);
  bmi055->GYRO_Y |= BMI055_Y_ReadData(GYRO_Z_MSB)<<8;
}

/****************************************************
 * Function:    BMI055_DataConversion
 * Description: Data Conversion.
 * Input:       &bmi055
 *              &fData_bmi055
 * Output:
 * Return:
*****************************************************/
void BMI055_DataConversion(BMI055* bmi055, fData_BMI055* fData_bmi055)
{
  fData_bmi055->fGyroX = bmi055->GYRO_X * GYRO_LSB_TO_DPS;
  fData_bmi055->fGyroY = bmi055->GYRO_Y * GYRO_LSB_TO_DPS;
}
