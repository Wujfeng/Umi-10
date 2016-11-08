/****************************************************************
 * Copyright (C) 2016, Yagro, all right reserved.
 * File name:    Command.h
 * Date:         2016.03.22
 * Description:  Command Protocol
*****************************************************************/

#ifndef __COMMAND_H
#define __COMMAND_H

#include "Delay.h"
#include "Flash.h"
#include "USART1.h"
#include <stdlib.h>
#include <stdio.h>


void Data_Decoding(uint8_t* data);
void Command_Class(uint8_t* data);
void Get_Class(uint8_t* data);
void Set_Class(uint8_t* data);
void Upload(uint8_t* data);
void Save_Config(void);
void Factory_Reset(void);
void Out_Command_Bad(void);
void Out_Config_OK(void);
void packsum(uint8_t* Buffer ,uint16_t lenght);
void Get_GyroScale(IMU_Dem* imu_dem);
void Get_GyroZero(IMU_Dem* imu_dem);
void Get_GyroGyron(IMU_Dem* imu_dem, uint8_t Index);
void Get_AccScale(IMU_Dem* imu_dem);
void Get_AccZero(IMU_Dem* imu_dem);
void Get_AccAccn(IMU_Dem* imu_dem, uint8_t Index);
void Get_Mode(uint32_t mode);
void Get_BMI055_Range(uint32_t Range);
void Get_BMI055_BW(uint32_t BW);
void Get_ID(uint32_t id);
void Setup_GyroScale(uint8_t* data, IMU_Dem* imu_dem);
void Setup_GyroZero(uint8_t* data, IMU_Dem* imu_dem);
void Setup_GyroGyron(uint8_t* data, IMU_Dem* imu_dem, uint8_t Index);
void Setup_AccScale(uint8_t* data, IMU_Dem* imu_dem);
void Setup_AccZero(uint8_t* data, IMU_Dem* imu_dem);
void Setup_AccAccn(uint8_t* data, IMU_Dem* imu_dem, uint8_t Index);
void Setup_Mode(uint8_t* data, uint32_t* mode);
void Setup_BMI055_Range(uint8_t* data, uint32_t* range);
void Setup_BMI055_BW(uint8_t* data, uint32_t* bw);
void Setup_ID(uint8_t* data, uint32_t* id);

#endif 	/* __COMMAND_H */
