/****************************************************************
 * Copyright (C) 2016, Yagro, all right reserved.
 * File name:    SCC2000.h
 * Date:         2016.03.22
 * Description:  SCC2000 Driver, SPI Communication Interface
*****************************************************************/

#ifndef __SCC2230_SCR2100_H
#define __SCC2230_SCR2100_H


#include "SPI3.h"

/************ SCC2000 CS Signal **********/

#define SCC2000_GYRO_Z_CS_LOW()    SPI3_CS1_LOW()
#define SCC2000_GYRO_Z_CS_HIGH()   SPI3_CS1_HIGH()

#define SCC2000_GYRO_Z_SendHalfWord(HalfWord)   SPI3_SendHalfWord(HalfWord)

/******************************************/

/******* SCC2000 Data Conversion **********/

#define _GYRO_K_SCC2000   (0.02f)
#define _ACC_K_SCC2000    (0.0001699f)  /* (1.0f / 3.0f / 1962.0f) */
#define _TEMP_K_SCC2000   (14.7f)

/******************************************/

/********************* SCC2000 Register **********************/

#define REQ_READ_RATE               ((uint32_t)0x040000F7)    /* GYRO */

#define REQ_READ_ACC_X              ((uint32_t)0x100000E9)    /* ACC */
#define REQ_READ_ACC_Y              ((uint32_t)0x140000EF)
#define REQ_READ_ACC_Z              ((uint32_t)0x180000E5)

#define REQ_READ_TEMP               ((uint32_t)0x1C0000E3)    /* TEMP */

#define REQ_READ_RATE_STAT1         ((uint32_t)0x240000C7)    /* GYRO Status 1 */
#define REQ_READ_RATE_STAT2         ((uint32_t)0x280000CD)    /* GYRO Status 2 */

#define REQ_READ_ACC_STAT           ((uint32_t)0x3C0000D3)    /* ACC Status */

#define REQ_HARD_RESET              ((uint32_t)0xD8000431)    /*  Reset Control */

#define REQ_READ_COM_STAT1          ((uint32_t)0x6C0000AB)    /* Common Status */

#define REQ_READ_STAT_SUM           ((uint32_t)0x7C0000B3)    /* Status Summary */

#define REQ_WRITE_FLT_60            ((uint32_t)0xFC200006)    /* Set 60Hz Filter Active */
#define REQ_WRITE_FLT_10            ((uint32_t)0xFC1000C7)    /* Set 10Hz Filter Active */

/*************************************************************/

#pragma pack(push)
#pragma pack(1)

typedef struct
{
  int16_t ACC_X;
  int16_t ACC_Y;
  int16_t ACC_Z;

  int16_t GYRO_Z;

}SCC2000;

typedef struct
{
  float fGyroZ;

  float fAccX;
  float fAccY;
  float fAccZ;

}fData_SCC2000;

#pragma pack(pop)


extern SCC2000 scc2000;

void ADS1258_Init(void);
void ADS1258_Read(void);
   
 
void SCC2000_Reset(void);
void SCC2000_Init(void);
uint8_t SCC2000_Status(void);
int16_t SCC2230_ReadReg(uint32_t ADDR);
void SCC2000_Read(SCC2000* scc2000);

uint8_t CalculateCRC(uint32_t Data);
uint8_t CRc8(uint8_t BitValue, uint8_t CRc);

#endif 	/* __SCC2230_SCR2100_H */
