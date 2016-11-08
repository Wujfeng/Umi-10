/****************************************************************
 * Copyright (C) 2016, Yagro, all right reserved.
 * File name:    Flash.h
 * Date:         2016.03.22
 * Description:  System Parameter Initialization Read Write
*****************************************************************/

#ifndef __FLASH_H
#define __FLASH_H

#include "stm32f4xx.h"
#include "Demarcate.h"
#include "BMI055.h"


#define FLASH_WRITE_FLAG  (0xAA55AA55)


typedef struct
{
  IMU_Dem imu_dem;            /* IMU Calibration parameters */

  uint32_t MODE;              /* Mode Selection(0:navigation  1:Calibration) */
  
  uint32_t BMI055_Range;      /* BMI055 GYRO Range */
  
  uint32_t BMI055_BW;         /* BMI055 GYRO Filter Bandwidth */

  uint32_t Data_Upload;       /* Data Upload(0:ON  1:OFF) */

  uint32_t ID;                /* Product ID */

}System_Parameter;


extern System_Parameter system_parameter;


void Flash_Init(System_Parameter* system_parameter);
void Flash_Write(System_Parameter* system_parameter);
uint32_t Flash_Read(System_Parameter* system_parameter);

#endif 	/* __FLASH_H */

