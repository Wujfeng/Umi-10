/****************************************************************
 * Copyright (C) 2016, Yagro, all right reserved.
 * File name:    BMP280.c
 * Date:         2016.07.07
 * Description:  BMP280 Driver, SPI Communication Interface
*****************************************************************/


#include "BMP280.h"


BMP280_Calib bmp280_Calib;
BMP280 bmp280;
fData_BMP280 fData_bmp280;
BMP280_Final_data MCU_Out_Press;

/****************************************************
 * Function:    BMP280_ReadData
 * Description: Read data from BMP280 register.
 * Input:       ADDR: BMP280 Register Address
 * Output:
 * Return:      Data register
*****************************************************/
uint8_t BMP280_ReadData(uint8_t ADDR)
{
  uint16_t data, i;

  ADDR = ADDR|0x80;
  
  for(i=0; i<0x05; i++);

  BMP280_CS_LOW();

  data = BMP280_SendHalfWord(((uint16_t)ADDR<<8)|0xFF);

  BMP280_CS_HIGH();
  
  for(i=0; i<0x05; i++);

  return(data);
}

/****************************************************
 * Function:    BMP280_WriteData
 * Description: Write data from BMP280 register.
 * Input:       ADDR: BMP280 Register Address
 *              Data: Data
 * Output:
 * Return:
*****************************************************/
void BMP280_WriteData(uint8_t ADDR, uint8_t Data)
{
  uint8_t i;
  
  for(i=0; i<0x05; i++);
  
  BMP280_CS_LOW();

  BMP280_SendHalfWord(((uint16_t)ADDR<<8)|Data);

  BMP280_CS_HIGH();
  
  for(i=0; i<0x05; i++);
}

/****************************************************
 * Function:    BMP280_Reset
 * Description: Reset BMP280.
 * Input:
 * Output:
 * Return:
*****************************************************/
void BMP280_Reset(void)
{
  BMP280_WriteData(BMP280_RESET_REG, 0xB6);
}

/****************************************************
 * Function:    BMP280_Init
 * Description: Initialization BMP280.
 * Input:       &bmp280_Calib
 *              Mode
 * Output:      1: Normal, 0: Unusual
 * Return:
*****************************************************/
uint8_t BMP280_Init(BMP280_Calib* bmp280_Calib, uint8_t Mode)
{
  if(BMP280_ReadData(BMP280_CHIPID_REG) != 0x58)
  {
    return 0;
  }
  
  if( (BMP280_ReadData(BMP280_STATUS_REG)&0x01) == 0x01 )
  {
    return 0;
  }
  
  bmp280_Calib->dig_T1 = BMP280_ReadData(BMP280_DIG_T1_MSB_REG)<<8;
  bmp280_Calib->dig_T1 |= BMP280_ReadData(BMP280_DIG_T1_LSB_REG);
  
  bmp280_Calib->dig_T2 = BMP280_ReadData(BMP280_DIG_T2_MSB_REG)<<8;
  bmp280_Calib->dig_T2 |= BMP280_ReadData(BMP280_DIG_T2_LSB_REG);
  
  bmp280_Calib->dig_T3 = BMP280_ReadData(BMP280_DIG_T3_MSB_REG)<<8;
  bmp280_Calib->dig_T3 |= BMP280_ReadData(BMP280_DIG_T3_LSB_REG);
  
  bmp280_Calib->dig_P1 = BMP280_ReadData(BMP280_DIG_P1_MSB_REG)<<8;
  bmp280_Calib->dig_P1 |= BMP280_ReadData(BMP280_DIG_P1_LSB_REG);
  
  bmp280_Calib->dig_P2 = BMP280_ReadData(BMP280_DIG_P2_MSB_REG)<<8;
  bmp280_Calib->dig_P2 |= BMP280_ReadData(BMP280_DIG_P2_LSB_REG);
  
  bmp280_Calib->dig_P3 = BMP280_ReadData(BMP280_DIG_P3_MSB_REG)<<8;
  bmp280_Calib->dig_P3 |= BMP280_ReadData(BMP280_DIG_P3_LSB_REG);
  
  bmp280_Calib->dig_P4 = BMP280_ReadData(BMP280_DIG_P4_MSB_REG)<<8;
  bmp280_Calib->dig_P4 |= BMP280_ReadData(BMP280_DIG_P4_LSB_REG);
  
  bmp280_Calib->dig_P5 = BMP280_ReadData(BMP280_DIG_P5_MSB_REG)<<8;
  bmp280_Calib->dig_P5 |= BMP280_ReadData(BMP280_DIG_P5_LSB_REG);
  
  bmp280_Calib->dig_P6 = BMP280_ReadData(BMP280_DIG_P6_MSB_REG)<<8;
  bmp280_Calib->dig_P6 |= BMP280_ReadData(BMP280_DIG_P6_LSB_REG);
  
  bmp280_Calib->dig_P7 = BMP280_ReadData(BMP280_DIG_P7_MSB_REG)<<8;
  bmp280_Calib->dig_P7 |= BMP280_ReadData(BMP280_DIG_P7_LSB_REG);
  
  bmp280_Calib->dig_P8 = BMP280_ReadData(BMP280_DIG_P8_MSB_REG)<<8;
  bmp280_Calib->dig_P8 |= BMP280_ReadData(BMP280_DIG_P8_LSB_REG);
  
  bmp280_Calib->dig_P9 = BMP280_ReadData(BMP280_DIG_P9_MSB_REG)<<8;
  bmp280_Calib->dig_P9 |= BMP280_ReadData(BMP280_DIG_P9_LSB_REG);
  
  BMP280_Mode(Mode);
  
  return 1;
}

/****************************************************
 * Function:    BMP280_Mode
 * Description: Set BMP280 Mode.
 * Input:       Mode
 * Output:
 * Return:
*****************************************************/
void BMP280_Mode(uint8_t Mode)
{
  if(Mode == BMP280_INDOOR_NAVIGATION_MODE)
  {
    BMP280_WriteData(BMP280_CTRL_MEAS_REG, (BMP280_ULTRA_HIGH_RESOLUTION_OSRS_T<<5)|(BMP280_ULTRA_HIGH_RESOLUTION_OSRS_P<<2)|BMP280_SLEEP_MODE);
    BMP280_WriteData(BMP280_CONFIG_REG, (BMP280_STANDBY_TIME_0_5_MS<<5)|(BMP280_FILTER_COEFF_16<<2));
    BMP280_WriteData(BMP280_CTRL_MEAS_REG, (BMP280_ULTRA_HIGH_RESOLUTION_OSRS_T<<5)|(BMP280_ULTRA_HIGH_RESOLUTION_OSRS_P<<2)|BMP280_NORMAL_MODE);
  }
  else if(Mode == BMP280_DROP_DETECTION_MODE)
  {
    BMP280_WriteData(BMP280_CTRL_MEAS_REG, (BMP280_LOW_POWER_OSRS_T<<5)|(BMP280_LOW_POWER_OSRS_P<<2)|BMP280_SLEEP_MODE);
    BMP280_WriteData(BMP280_CONFIG_REG, (BMP280_STANDBY_TIME_0_5_MS<<5)|(BMP280_FILTER_COEFF_OFF<<2));
    BMP280_WriteData(BMP280_CTRL_MEAS_REG, (BMP280_LOW_POWER_OSRS_T<<5)|(BMP280_LOW_POWER_OSRS_P<<2)|BMP280_NORMAL_MODE);
  }
  else if(Mode == BMP280_HANDHELD_DEVICE_DYNAMIC_MODE)
  {
    BMP280_WriteData(BMP280_CTRL_MEAS_REG, (BMP280_STANDARD_RESOLUTION_OSRS_T<<5)|(BMP280_STANDARD_RESOLUTION_OSRS_P<<2)|BMP280_SLEEP_MODE);
    BMP280_WriteData(BMP280_CONFIG_REG, (BMP280_STANDBY_TIME_0_5_MS<<5)|(BMP280_FILTER_COEFF_16<<2));
    BMP280_WriteData(BMP280_CTRL_MEAS_REG, (BMP280_STANDARD_RESOLUTION_OSRS_T<<5)|(BMP280_STANDARD_RESOLUTION_OSRS_P<<2)|BMP280_NORMAL_MODE);
  }
}

/****************************************************
 * Function:    BMP280_Read
 * Description: Read BMP280 PRESS Data.
 * Input:       &bmp280
 * Output:
 * Return:
*****************************************************/
void BMP280_Read(BMP280* bmp280)
{
  uint32_t Data;
  
  Data = BMP280_ReadData(BMP280_PRESS_MSB_REG)<<12;
  Data |= BMP280_ReadData(BMP280_PRESS_LSB_REG)<<4;
  Data |= BMP280_ReadData(BMP280_PRESS_XLSB_REG)>>4;

  bmp280->PRESS = Data;
  
  Data = BMP280_ReadData(BMP280_TEMP_MSB_REG)<<12;
  Data |= BMP280_ReadData(BMP280_TEMP_LSB_REG)<<4;
  Data |= BMP280_ReadData(BMP280_TEMP_XLSB_REG)>>4;

  bmp280->TEMP = Data;
}

/****************************************************
 * Function:    BMP280_UT2TEMP
 * Description: Data Conversion.
 * Input:       &bmp280_Calib
 *              &bmp280
 *              &fData_bmp280
 * Output:
 * Return:
*****************************************************/
void BMP280_UT2TEMP(BMP280_Calib* bmp280_Calib, BMP280* bmp280, fData_BMP280* fData_bmp280)
{
  int32_t var1, var2, T;
  
  var1 = (((((bmp280->TEMP)>>3) - ((int32_t)(bmp280_Calib->dig_T1)<<1))) * ((int32_t)(bmp280_Calib->dig_T2))) >> 11;
  var2 = ((((((bmp280->TEMP)>>4) - ((int32_t)(bmp280_Calib->dig_T1))) * (((bmp280->TEMP)>>4) - ((int32_t)(bmp280_Calib->dig_T1)))) >> 12) * ((int32_t)(bmp280_Calib->dig_T3))) >> 14;
  bmp280_Calib->t_fine = var1 + var2;
  T = ((bmp280_Calib->t_fine) * 5 + 128) >> 8;
  
  fData_bmp280->fTemp = T * 0.01f;
}

/****************************************************
 * Function:    BMP280_UP2PRESS
 * Description: Data Conversion.
 * Input:       &bmp280_Calib
 *              &bmp280
 *              &fData_bmp280
 * Output:
 * Return:
*****************************************************/
void BMP280_UP2PRESS(BMP280_Calib* bmp280_Calib, BMP280* bmp280, fData_BMP280* fData_bmp280)
{
  int64_t var1, var2, P;
  
  var1 = ((int64_t)(bmp280_Calib->t_fine)) - 128000;
  var2 = var1 * var1 * (int64_t)(bmp280_Calib->dig_P6);
  var2 = var2 + ((var1 * (int64_t)(bmp280_Calib->dig_P5))<<17);
  var2 = var2 + (((int64_t)(bmp280_Calib->dig_P4))<<35);
  var1 = ((var1 * var1 * (int64_t)(bmp280_Calib->dig_P3))>>8) + ((var1 * (int64_t)(bmp280_Calib->dig_P2))<<12);
  var1 = (((((int64_t)1)<<47) + var1)) * ((int64_t)(bmp280_Calib->dig_P1))>>33;
  
  if(var1 == 0)
  {
    return; /* avoid exception caused by division by zero */
  }
  
  P = 1048576 - bmp280->PRESS;
  P = (((P<<31) - var2) * 3125) / var1;
  var1 = (((int64_t)(bmp280_Calib->dig_P9)) * (P>>13) * (P>>13)) >> 25;
  var2 = (((int64_t)(bmp280_Calib->dig_P8)) * P) >> 19;
  P = ((P + var1 + var2)>>8) + (((int64_t)(bmp280_Calib->dig_P7))<<4);
  
  fData_bmp280->dPress = (double)P / (double)256.0;
  
  fData_bmp280->fAltitude = (float)(44330.0 * (1.0 - pow((fData_bmp280->dPress) * BMP280_CONVERSION1, BMP280_CONVERSION2)));
}

void BMP280_Data_Sum(fData_BMP280* fData_bmp280,BMP280_Final_data* BMP280_SUM)
{
  BMP280_SUM->sum_dPress += fData_bmp280->dPress;
  BMP280_SUM->sum_fAltitude += fData_bmp280->fAltitude;
  BMP280_SUM->sum_fTemp += fData_bmp280->fTemp;
  
  BMP280_SUM->count ++;
}

unsigned char BMP280_calculate_AVE(BMP280_Final_data* BMP280_SUM)
{
  unsigned char data_OK_flag = 1;
  
  if(BMP280_SUM->count > 0)
  {
    BMP280_SUM->ave_dPress = BMP280_SUM->sum_dPress / BMP280_SUM->count;
    BMP280_SUM->ave_fAltitude = BMP280_SUM->sum_fAltitude / BMP280_SUM->count;
    BMP280_SUM->ave_fTemp = BMP280_SUM->sum_fTemp / BMP280_SUM->count;
    
    BMP280_SUM->count = 0;
    BMP280_SUM->sum_dPress = 0;
    BMP280_SUM->sum_fAltitude = 0;
    BMP280_SUM->sum_fTemp = 0;
  }
  else data_OK_flag = 0;
  
  return data_OK_flag;
}
