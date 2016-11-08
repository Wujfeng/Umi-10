#ifndef __ADS1258_H
#define __ADS1258_H

#include "SPI2.h"

#define ADS1258_CS_LOW               SPI2_CS2_LOW()   
#define ADS1258_CS_HIGH              SPI2_CS2_HIGH() 
#define ADS1258_SendWord(X)          SPI2_SendHalfWord(X)
#define ADS1258_START_LOW()          GPIO_ResetBits(GPIOB, GPIO_Pin_6)
#define ADS1258_START_HIGH()         GPIO_SetBits(GPIOB, GPIO_Pin_6)

typedef struct ADS1258_data
{
  int X_32;                   /*true 32 data*/
  int Y_32;   
  int Z_32;
  int TEMP_32;
  
  int64_t sum_X;              /*data sum*/
  int64_t sum_Y;
  int64_t sum_Z;
  int64_t sum_TEMP;
  
  unsigned int X_count;       /*data count*/
  unsigned int Y_count;
  unsigned int Z_count;
  unsigned int TEMP_count;
  
}ADS1258_data;

typedef struct 
{
  float ACC_X_f;
  float ACC_Y_f;
  float ACC_Z_f;
  float TEMP;
  
}ADS1258_Final_Data;

extern ADS1258_data ad_collect_data;
extern ADS1258_Final_Data MCU_Out_Acc;

#define Channel_ACC_X             0x0800
#define Channel_ACC_Y             0x0A00
#define Channel_ACC_Z             0x0C00
#define Channel_TEMP              0x0E00
#define Channel_VREF              0x1D00

#define Register_Write_Command    0x60
#define Register_Read_Command     0x40
#define Register_Reset_Command    0xc0

#define CONFIG0_ADDRESS           0x00
#define CONFIG1_ADDRESS           0x01
#define MUXSCH_ADDRESS            0x02
#define MUXDIF_ADDRESS            0x03
#define MUXSG0_ADDRESS            0x04
#define MUXSG1_ADDRESS            0x05
#define SYSRED_ADDRESS            0x06
#define GPIOC_ADDRESS             0x07
#define GPIOD_ADDRESS             0x08
#define ID_ADDRESS                0x09

#define ID_Value                  0x8b

#define AD_scale                 (1/7864320.0f)           /*PDF P25 code*VREF/0x780000*/
#define Vref                      2.5f

uint8_t ADS1258_ID_check(void);
void ADS1258_Init(void);
void ADS1258_Read(void);
unsigned char ADS1258_data_calculate(ADS1258_data* AD_collect,ADS1258_Final_Data* MCU_PUTOUT_Data);
void ADS1258_data_check_accumulation(unsigned int H_byte,unsigned int L_byte);

#endif
