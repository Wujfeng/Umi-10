/****************************************************************
 * Copyright (C) 2016, Yagro, all right reserved.
 * File name:    Demarcate.c
 * Date:         2016.06.14
 * Description:  Demarcate
*****************************************************************/


#include "Demarcate.h"


fData_IMU fData_imu;

/****************************************************
 * Function:    IMU_Error_Com
 * Description: Error Compensation.
 * Input:       &fData_imu
 *              &imu_dem
 * Output:
 * Return:
*****************************************************/
void IMU_Error_Com(fData_IMU* fData_imu, IMU_Dem* imu_dem)
{
  float ACC_Tep[3], GYRO_Tep[3];

  /* ACC Zero Scale Compensation */
  ACC_Tep[0] = fData_imu->fAccX/imu_dem->ACC_Scale[0] - imu_dem->ACC_Zero[0];
  ACC_Tep[1] = fData_imu->fAccY/imu_dem->ACC_Scale[1] - imu_dem->ACC_Zero[1];
  ACC_Tep[2] = fData_imu->fAccZ/imu_dem->ACC_Scale[2] - imu_dem->ACC_Zero[2];

  /* ACC Cross-coupling Compensation */
  fData_imu->fAccX = imu_dem->ACC_Coupling[0][0]*ACC_Tep[0] + imu_dem->ACC_Coupling[0][1]*ACC_Tep[1] + imu_dem->ACC_Coupling[0][2]*ACC_Tep[2];
  fData_imu->fAccY = imu_dem->ACC_Coupling[1][0]*ACC_Tep[0] + imu_dem->ACC_Coupling[1][1]*ACC_Tep[1] + imu_dem->ACC_Coupling[1][2]*ACC_Tep[2];
  fData_imu->fAccZ = imu_dem->ACC_Coupling[2][0]*ACC_Tep[0] + imu_dem->ACC_Coupling[2][1]*ACC_Tep[1] + imu_dem->ACC_Coupling[2][2]*ACC_Tep[2];


  /* GYRO Zero Scale Compensation */
  GYRO_Tep[0] = fData_imu->fGyroX/imu_dem->GYRO_Scale[0] - imu_dem->GYRO_Zero[0];
  GYRO_Tep[1] = fData_imu->fGyroY/imu_dem->GYRO_Scale[1] - imu_dem->GYRO_Zero[1];
  GYRO_Tep[2] = fData_imu->fGyroZ/imu_dem->GYRO_Scale[2] - imu_dem->GYRO_Zero[2];

  /* GYRO Cross-coupling Compensation */
  fData_imu->fGyroX = imu_dem->GYRO_Coupling[0][0]*GYRO_Tep[0] + imu_dem->GYRO_Coupling[0][1]*GYRO_Tep[1] + imu_dem->GYRO_Coupling[0][2]*GYRO_Tep[2];
  fData_imu->fGyroY = imu_dem->GYRO_Coupling[1][0]*GYRO_Tep[0] + imu_dem->GYRO_Coupling[1][1]*GYRO_Tep[1] + imu_dem->GYRO_Coupling[1][2]*GYRO_Tep[2];
  fData_imu->fGyroZ = imu_dem->GYRO_Coupling[2][0]*GYRO_Tep[0] + imu_dem->GYRO_Coupling[2][1]*GYRO_Tep[1] + imu_dem->GYRO_Coupling[2][2]*GYRO_Tep[2];
}
