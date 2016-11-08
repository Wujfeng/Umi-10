/****************************************************************
 * Copyright (C) 2016, Yagro, all right reserved.
 * File name:    Demarcate.h
 * Date:         2016.06.14
 * Description:  Demarcate
*****************************************************************/

#ifndef __DEMARCATE_H
#define __DEMARCATE_H


typedef struct
{
  float fGyroX;
  float fGyroY;
  float fGyroZ;

  float fAccX;
  float fAccY;
  float fAccZ;

}fData_IMU;

typedef struct
{
  float ACC_Zero[3];          /* ACC Zero Compensation */

  float ACC_Scale[3];         /* ACC Scale Compensation */

  float ACC_Coupling[3][3];   /* ACC Cross-coupling Compensation */


  float GYRO_Zero[3];         /* GYRO Zero Compensation */

  float GYRO_Scale[3];        /* GYRO Scale Compensation */

  float GYRO_Coupling[3][3];  /* GYRO Cross-coupling Compensation */

}IMU_Dem;


extern fData_IMU fData_imu;


void IMU_Error_Com(fData_IMU* fData_imu, IMU_Dem* imu_dem);

#endif 	/* __DEMARCATE_H */
