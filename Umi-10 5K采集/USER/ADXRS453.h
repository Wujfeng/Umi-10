#ifndef __ADXRS453_H
#define __ADXRS453_H

#include "SPI1.h"
#include "SPI2.h"
#include "SPI3.h"
  
#define ADXRS453_X_CS_LOW       SPI1_CS1_LOW()   
#define ADXRS453_X_CS_HIGH      SPI1_CS1_HIGH()  
#define ADXRS453_Y_CS_LOW       SPI2_CS1_LOW()  
#define ADXRS453_Y_CS_HIGH      SPI2_CS1_HIGH()  
#define ADXRS453_Z_CS_LOW       SPI3_CS1_LOW()  
#define ADXRS453_Z_CS_HIGH      SPI3_CS1_HIGH()  
 
#define ADXRS453_X_SEND(X)       SPI1_SendHalfWord(X) 
#define ADXRS453_Y_SEND(X)       SPI2_SendHalfWord(X)  
#define ADXRS453_Z_SEND(X)       SPI3_SendHalfWord(X)  
  
#define READ_Command              0x80000000              /*Command [31:30]  10 : read; 01 : write*/
#define WRITE_Command             0x40000000 
#define PARITY_BIT                0x00000001              /* parity bit */

#define RATE_Address              0x00000000              /*register_Address [25:17]  RATE_Address : 0X00*/
#define TEMP_Address              (0x02<<17)              /* TEMP_Address : 0x02  */
#define PID_Address               (0x0c<<17)              /* PID_Address :  0x0C  */

#define GYRO_scale                0.0125f

typedef struct 
{
  int64_t sum_X;
  int64_t sum_Y;
  int64_t sum_Z;
  int64_t sum_TEMP;
  
  unsigned int X_count;
  unsigned int Y_count;
  unsigned int Z_count;
  unsigned int TEMP_count;
  
}ADXRS453_data;

typedef struct 
{
  float X_f;
  float Y_f;
  float Z_f;
  float TEMP_f;
  
}ADXRS453_final_data;

typedef enum 
{
  gyro_x,
  gyro_y,
  gyro_z,
  gyro_temp,
  
}GYRO_data_source;

extern ADXRS453_data ADXRS453_collect_data;
extern ADXRS453_final_data MCU_Out_Gyro;

void ADXRS453_Init(void);
uint8_t ADXRS453_ID_Check(void);

void ADXRS453_X_read(void);
void ADXRS453_Y_read(void); 
void ADXRS453_Z_read(void); 

uint8_t ADXRS453_data_check(unsigned int data);
void ADXRS453_data_save(int16_t data,ADXRS453_data* ADXRS453_collect ,GYRO_data_source GYRO);
unsigned char ADXRS453_data_calculate(ADXRS453_data* ADXRS453_collect,ADXRS453_final_data* AD_OUT_DATA);

#endif
