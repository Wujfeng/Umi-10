/****************************************************************
 * Copyright (C) 2016, Yagro, all right reserved.
 * File name:    Flash.c
 * Date:         2016.03.22
 * Description:  System Parameter Initialization Read Write
*****************************************************************/


#include "Flash.h"


/* System Parameter */
System_Parameter system_parameter;


/****************************************************
 * Function:    Flash_Init
 * Description: Flash initialization parameters in.
 * Input:       &system_parameter
 * Output:
 * Return:
*****************************************************/
void Flash_Init(System_Parameter* system_parameter)
{
  system_parameter->imu_dem.ACC_Zero[0] = 0.0f;
  system_parameter->imu_dem.ACC_Zero[1] = 0.0f;
  system_parameter->imu_dem.ACC_Zero[2] = 0.0f;

  system_parameter->imu_dem.ACC_Scale[0] = 1.0f;
  system_parameter->imu_dem.ACC_Scale[1] = 1.0f;
  system_parameter->imu_dem.ACC_Scale[2] = 1.0f;

  system_parameter->imu_dem.ACC_Coupling[0][0] = 1.0f;
  system_parameter->imu_dem.ACC_Coupling[0][1] = 0.0f;
  system_parameter->imu_dem.ACC_Coupling[0][2] = 0.0f;

  system_parameter->imu_dem.ACC_Coupling[1][0] = 0.0f;
  system_parameter->imu_dem.ACC_Coupling[1][1] = 1.0f;
  system_parameter->imu_dem.ACC_Coupling[1][2] = 0.0f;

  system_parameter->imu_dem.ACC_Coupling[2][0] = 0.0f;
  system_parameter->imu_dem.ACC_Coupling[2][1] = 0.0f;
  system_parameter->imu_dem.ACC_Coupling[2][2] = 1.0f;


  system_parameter->imu_dem.GYRO_Zero[0] = 0.0f;
  system_parameter->imu_dem.GYRO_Zero[1] = 0.0f;
  system_parameter->imu_dem.GYRO_Zero[2] = 0.0f;

  system_parameter->imu_dem.GYRO_Scale[0] = 1.0f;
  system_parameter->imu_dem.GYRO_Scale[1] = 1.0f;
  system_parameter->imu_dem.GYRO_Scale[2] = 1.0f;

  system_parameter->imu_dem.GYRO_Coupling[0][0] = 1.0f;
  system_parameter->imu_dem.GYRO_Coupling[0][1] = 0.0f;
  system_parameter->imu_dem.GYRO_Coupling[0][2] = 0.0f;

  system_parameter->imu_dem.GYRO_Coupling[1][0] = 0.0f;
  system_parameter->imu_dem.GYRO_Coupling[1][1] = 1.0f;
  system_parameter->imu_dem.GYRO_Coupling[1][2] = 0.0f;

  system_parameter->imu_dem.GYRO_Coupling[2][0] = 0.0f;
  system_parameter->imu_dem.GYRO_Coupling[2][1] = 0.0f;
  system_parameter->imu_dem.GYRO_Coupling[2][2] = 1.0f;


  system_parameter->MODE = 0x00000000;


  system_parameter->BMI055_Range = GYRO_125dps;
  system_parameter->BMI055_BW = GYRO_BW_47Hz;


  system_parameter->ID = 0x00000000;
}

/****************************************************
 * Function:    Flash_Write
 * Description: Write Flash Parameter.
 * Input:       &system_parameter
 * Output:
 * Return:
*****************************************************/
void Flash_Write(System_Parameter* system_parameter)
{
  union
  {
    float fData;
    uint32_t uData;

  }Flash_Data;

  /* Starting Address */
  uint32_t Address = 0x08020000;

  FLASH_Unlock();
  FLASH_DataCacheCmd(DISABLE);  /* During FLASH erase data cache must be disabled */
  FLASH_EraseSector(FLASH_Sector_5, VoltageRange_3);  /* VCC = 2.7 ~ 3.6V !!! erase sector (last sector) */


  /* Write Flash programmer flag */
  FLASH_ProgramWord(Address, FLASH_WRITE_FLAG);
  Address += 4;


  Flash_Data.fData = system_parameter->imu_dem.ACC_Zero[0];
  FLASH_ProgramWord(Address, Flash_Data.uData);
  Address += 4;
  Flash_Data.fData = system_parameter->imu_dem.ACC_Zero[1];
  FLASH_ProgramWord(Address, Flash_Data.uData);
  Address += 4;
  Flash_Data.fData = system_parameter->imu_dem.ACC_Zero[2];
  FLASH_ProgramWord(Address, Flash_Data.uData);
  Address += 4;

  Flash_Data.fData = system_parameter->imu_dem.ACC_Scale[0];
  FLASH_ProgramWord(Address, Flash_Data.uData);
  Address += 4;
  Flash_Data.fData = system_parameter->imu_dem.ACC_Scale[1];
  FLASH_ProgramWord(Address, Flash_Data.uData);
  Address += 4;
  Flash_Data.fData = system_parameter->imu_dem.ACC_Scale[2];
  FLASH_ProgramWord(Address, Flash_Data.uData);
  Address += 4;

  Flash_Data.fData = system_parameter->imu_dem.ACC_Coupling[0][0];
  FLASH_ProgramWord(Address, Flash_Data.uData);
  Address += 4;
  Flash_Data.fData = system_parameter->imu_dem.ACC_Coupling[0][1];
  FLASH_ProgramWord(Address, Flash_Data.uData);
  Address += 4;
  Flash_Data.fData = system_parameter->imu_dem.ACC_Coupling[0][2];
  FLASH_ProgramWord(Address, Flash_Data.uData);
  Address += 4;

  Flash_Data.fData = system_parameter->imu_dem.ACC_Coupling[1][0];
  FLASH_ProgramWord(Address, Flash_Data.uData);
  Address += 4;
  Flash_Data.fData = system_parameter->imu_dem.ACC_Coupling[1][1];
  FLASH_ProgramWord(Address, Flash_Data.uData);
  Address += 4;
  Flash_Data.fData = system_parameter->imu_dem.ACC_Coupling[1][2];
  FLASH_ProgramWord(Address, Flash_Data.uData);
  Address += 4;

  Flash_Data.fData = system_parameter->imu_dem.ACC_Coupling[2][0];
  FLASH_ProgramWord(Address, Flash_Data.uData);
  Address += 4;
  Flash_Data.fData = system_parameter->imu_dem.ACC_Coupling[2][1];
  FLASH_ProgramWord(Address, Flash_Data.uData);
  Address += 4;
  Flash_Data.fData = system_parameter->imu_dem.ACC_Coupling[2][2];
  FLASH_ProgramWord(Address, Flash_Data.uData);
  Address += 4;

  Flash_Data.fData = system_parameter->imu_dem.GYRO_Zero[0];
  FLASH_ProgramWord(Address, Flash_Data.uData);
  Address += 4;
  Flash_Data.fData = system_parameter->imu_dem.GYRO_Zero[1];
  FLASH_ProgramWord(Address, Flash_Data.uData);
  Address += 4;
  Flash_Data.fData = system_parameter->imu_dem.GYRO_Zero[2];
  FLASH_ProgramWord(Address, Flash_Data.uData);
  Address += 4;

  Flash_Data.fData = system_parameter->imu_dem.GYRO_Scale[0];
  FLASH_ProgramWord(Address, Flash_Data.uData);
  Address += 4;
  Flash_Data.fData = system_parameter->imu_dem.GYRO_Scale[1];
  FLASH_ProgramWord(Address, Flash_Data.uData);
  Address += 4;
  Flash_Data.fData = system_parameter->imu_dem.GYRO_Scale[2];
  FLASH_ProgramWord(Address, Flash_Data.uData);
  Address += 4;

  Flash_Data.fData = system_parameter->imu_dem.GYRO_Coupling[0][0];
  FLASH_ProgramWord(Address, Flash_Data.uData);
  Address += 4;
  Flash_Data.fData = system_parameter->imu_dem.GYRO_Coupling[0][1];
  FLASH_ProgramWord(Address, Flash_Data.uData);
  Address += 4;
  Flash_Data.fData = system_parameter->imu_dem.GYRO_Coupling[0][2];
  FLASH_ProgramWord(Address, Flash_Data.uData);
  Address += 4;

  Flash_Data.fData = system_parameter->imu_dem.GYRO_Coupling[1][0];
  FLASH_ProgramWord(Address, Flash_Data.uData);
  Address += 4;
  Flash_Data.fData = system_parameter->imu_dem.GYRO_Coupling[1][1];
  FLASH_ProgramWord(Address, Flash_Data.uData);
  Address += 4;
  Flash_Data.fData = system_parameter->imu_dem.GYRO_Coupling[1][2];
  FLASH_ProgramWord(Address, Flash_Data.uData);
  Address += 4;

  Flash_Data.fData = system_parameter->imu_dem.GYRO_Coupling[2][0];
  FLASH_ProgramWord(Address, Flash_Data.uData);
  Address += 4;
  Flash_Data.fData = system_parameter->imu_dem.GYRO_Coupling[2][1];
  FLASH_ProgramWord(Address, Flash_Data.uData);
  Address += 4;
  Flash_Data.fData = system_parameter->imu_dem.GYRO_Coupling[2][2];
  FLASH_ProgramWord(Address, Flash_Data.uData);
  Address += 4;


  FLASH_ProgramWord(Address, system_parameter->MODE);
  Address += 4;


  FLASH_ProgramWord(Address, system_parameter->BMI055_Range);
  Address += 4;
  FLASH_ProgramWord(Address, system_parameter->BMI055_BW);
  Address += 4;


  FLASH_ProgramWord(Address, system_parameter->ID);


  FLASH_DataCacheCmd(ENABLE); /* FLASH writing is completed, open the data cache */
  FLASH_Lock();
}

/****************************************************
 * Function:    Flash_Read
 * Description: Read Flash Parameter.
 * Input:       &system_parameter
 * Output:
 * Return:      Flash programmer flag
*****************************************************/
uint32_t Flash_Read(System_Parameter* system_parameter)
{
  union
  {
    float fData;
    uint32_t uData;

  }Flash_Data;

  uint32_t Address = 0x08020000;  /* Starting Address */
  uint32_t Flash_Write_Flag;      /* Flash programmer flag */


  Flash_Write_Flag = (*(vu32*)Address);
  Address += 4;



  Flash_Data.uData = (*(vu32*)Address);
  system_parameter->imu_dem.ACC_Zero[0] = Flash_Data.fData;
  Address += 4;
  Flash_Data.uData = (*(vu32*)Address);
  system_parameter->imu_dem.ACC_Zero[1] = Flash_Data.fData;
  Address += 4;
  Flash_Data.uData = (*(vu32*)Address);
  system_parameter->imu_dem.ACC_Zero[2] = Flash_Data.fData;
  Address += 4;

  Flash_Data.uData = (*(vu32*)Address);
  system_parameter->imu_dem.ACC_Scale[0] = Flash_Data.fData;
  Address += 4;
  Flash_Data.uData = (*(vu32*)Address);
  system_parameter->imu_dem.ACC_Scale[1] = Flash_Data.fData;
  Address += 4;
  Flash_Data.uData = (*(vu32*)Address);
  system_parameter->imu_dem.ACC_Scale[2] = Flash_Data.fData;
  Address += 4;

  Flash_Data.uData = (*(vu32*)Address);
  system_parameter->imu_dem.ACC_Coupling[0][0] = Flash_Data.fData;
  Address += 4;
  Flash_Data.uData = (*(vu32*)Address);
  system_parameter->imu_dem.ACC_Coupling[0][1] = Flash_Data.fData;
  Address += 4;
  Flash_Data.uData = (*(vu32*)Address);
  system_parameter->imu_dem.ACC_Coupling[0][2] = Flash_Data.fData;
  Address += 4;

  Flash_Data.uData = (*(vu32*)Address);
  system_parameter->imu_dem.ACC_Coupling[1][0] = Flash_Data.fData;
  Address += 4;
  Flash_Data.uData = (*(vu32*)Address);
  system_parameter->imu_dem.ACC_Coupling[1][1] = Flash_Data.fData;
  Address += 4;
  Flash_Data.uData = (*(vu32*)Address);
  system_parameter->imu_dem.ACC_Coupling[1][2] = Flash_Data.fData;
  Address += 4;

  Flash_Data.uData = (*(vu32*)Address);
  system_parameter->imu_dem.ACC_Coupling[2][0] = Flash_Data.fData;
  Address += 4;
  Flash_Data.uData = (*(vu32*)Address);
  system_parameter->imu_dem.ACC_Coupling[2][1] = Flash_Data.fData;
  Address += 4;
  Flash_Data.uData = (*(vu32*)Address);
  system_parameter->imu_dem.ACC_Coupling[2][2] = Flash_Data.fData;
  Address += 4;

  Flash_Data.uData = (*(vu32*)Address);
  system_parameter->imu_dem.GYRO_Zero[0] = Flash_Data.fData;
  Address += 4;
  Flash_Data.uData = (*(vu32*)Address);
  system_parameter->imu_dem.GYRO_Zero[1] = Flash_Data.fData;
  Address += 4;
  Flash_Data.uData = (*(vu32*)Address);
  system_parameter->imu_dem.GYRO_Zero[2] = Flash_Data.fData;
  Address += 4;

  Flash_Data.uData = (*(vu32*)Address);
  system_parameter->imu_dem.GYRO_Scale[0] = Flash_Data.fData;
  Address += 4;
  Flash_Data.uData = (*(vu32*)Address);
  system_parameter->imu_dem.GYRO_Scale[1] = Flash_Data.fData;
  Address += 4;
  Flash_Data.uData = (*(vu32*)Address);
  system_parameter->imu_dem.GYRO_Scale[2] = Flash_Data.fData;
  Address += 4;

  Flash_Data.uData = (*(vu32*)Address);
  system_parameter->imu_dem.GYRO_Coupling[0][0] = Flash_Data.fData;
  Address += 4;
  Flash_Data.uData = (*(vu32*)Address);
  system_parameter->imu_dem.GYRO_Coupling[0][1] = Flash_Data.fData;
  Address += 4;
  Flash_Data.uData = (*(vu32*)Address);
  system_parameter->imu_dem.GYRO_Coupling[0][2] = Flash_Data.fData;
  Address += 4;

  Flash_Data.uData = (*(vu32*)Address);
  system_parameter->imu_dem.GYRO_Coupling[1][0] = Flash_Data.fData;
  Address += 4;
  Flash_Data.uData = (*(vu32*)Address);
  system_parameter->imu_dem.GYRO_Coupling[1][1] = Flash_Data.fData;
  Address += 4;
  Flash_Data.uData = (*(vu32*)Address);
  system_parameter->imu_dem.GYRO_Coupling[1][2] = Flash_Data.fData;
  Address += 4;

  Flash_Data.uData = (*(vu32*)Address);
  system_parameter->imu_dem.GYRO_Coupling[2][0] = Flash_Data.fData;
  Address += 4;
  Flash_Data.uData = (*(vu32*)Address);
  system_parameter->imu_dem.GYRO_Coupling[2][1] = Flash_Data.fData;
  Address += 4;
  Flash_Data.uData = (*(vu32*)Address);
  system_parameter->imu_dem.GYRO_Coupling[2][2] = Flash_Data.fData;
  Address += 4;


  system_parameter->MODE = (*(vu32*)Address);
  Address += 4;


  system_parameter->BMI055_Range = (*(vu32*)Address);
  Address += 4;
  system_parameter->BMI055_BW = (*(vu32*)Address);
  Address += 4;


  system_parameter->ID = (*(vu32*)Address);

  return Flash_Write_Flag;
}

