#include "ADXRS453.h"
#include "Delay.h"
#include "SPI1.h"
#include "SPI2.h"
#include "SPI3.h"

ADXRS453_data ADXRS453_collect_data;
ADXRS453_final_data MCU_Out_Gyro;

/****************************************************************************
 * Function:    ADXRS453_Init
 * Description: According to user guide Page 20,init chip
 * Input:       
 * Output:      
 * Return:      
****************************************************************************/
void ADXRS453_Init()
{
  unsigned char i=0;

  Delay_ms(100);

/*********************SPI1********************/  
  ADXRS453_X_CS_HIGH;
  Delay_us(1);
  ADXRS453_X_CS_LOW; 
  
  ADXRS453_X_SEND(0x2000);
  ADXRS453_X_SEND(0x0003);
  
  Delay_us(10);
  ADXRS453_X_CS_HIGH;  
/*********************SPI2********************/  
  ADXRS453_Y_CS_HIGH;
  Delay_us(1);
  ADXRS453_Y_CS_LOW; 
  
  ADXRS453_Y_SEND(0x2000);
  ADXRS453_Y_SEND(0x0003);
  
  Delay_us(10);
  ADXRS453_Y_CS_HIGH;
/*********************SPI3********************/    
  ADXRS453_Z_CS_HIGH;
  Delay_us(1);
  ADXRS453_Z_CS_LOW; 
  
  ADXRS453_Z_SEND(0x2000);
  ADXRS453_Z_SEND(0x0003);
  
  Delay_us(10);
  ADXRS453_Z_CS_HIGH;
  
  for(i=0;i<3;i++)
  { 
    Delay_ms(50);
/*********************SPI1********************/     
    ADXRS453_X_CS_LOW; 
    
    ADXRS453_X_SEND(0x2000);
    ADXRS453_X_SEND(0x0000);
    
    Delay_us(10);
    ADXRS453_X_CS_HIGH;  
/*********************SPI2********************/      
    ADXRS453_Y_CS_LOW; 
    
    ADXRS453_Y_SEND(0x2000);
    ADXRS453_Y_SEND(0x0000);
    
    Delay_us(10);
    ADXRS453_Y_CS_HIGH;
/*********************SPI3********************/    
    ADXRS453_Z_CS_LOW; 
    
    ADXRS453_Z_SEND(0x2000);
    ADXRS453_Z_SEND(0x0000);
    
    Delay_us(10);
    ADXRS453_Z_CS_HIGH;        
  }   
}

/****************************************************************************
 * Function:    ADXRS453_ID_Check
 * Description: READ ADS1258 ID and check.
 * Input:       
 * Output:      
 * Return:      if ID is right return 1,else return 0
****************************************************************************/
uint8_t ADXRS453_ID_Check()
{
  int H_ID = 0,L_ID = 0,ID = 0;
  ADXRS453_X_CS_HIGH;
  ADXRS453_Y_CS_HIGH;
  ADXRS453_Z_CS_HIGH;
  Delay_us(1);
  ADXRS453_X_CS_LOW; 
  ADXRS453_Y_CS_LOW; 
  ADXRS453_Z_CS_LOW; 
  
  ADXRS453_X_SEND( (READ_Command | PID_Address)>>16);
  ADXRS453_X_SEND( (u16)(READ_Command | PID_Address));
  
  ADXRS453_Y_SEND( (READ_Command | PID_Address)>>16);
  ADXRS453_Y_SEND( (u16)(READ_Command | PID_Address));
  
  ADXRS453_Z_SEND( (READ_Command | PID_Address)>>16);
  ADXRS453_Z_SEND( (u16)(READ_Command | PID_Address));
  
  Delay_us(10);
  ADXRS453_X_CS_HIGH;
  ADXRS453_Y_CS_HIGH;
  ADXRS453_Z_CS_HIGH;
  Delay_us(1);
  ADXRS453_X_CS_LOW; 
  ADXRS453_Y_CS_LOW; 
  ADXRS453_Z_CS_LOW; 
  
  H_ID = ADXRS453_X_SEND( (READ_Command | PID_Address)>>16);
  L_ID = ADXRS453_X_SEND( (u16)(READ_Command | PID_Address));
  
  ID = (((H_ID<<16) | L_ID)>>5)&0x0000ffff;
  
  if( (ID&0xfff0) != 0x5200)                                        /*ID may be 0x5201,0x5202,0x5203 and so on*/
  {
    return 0;
  }
  
  H_ID = ADXRS453_Y_SEND( (READ_Command | PID_Address)>>16);
  L_ID = ADXRS453_Y_SEND( (u16)(READ_Command | PID_Address));
  
  ID = (((H_ID<<16) | L_ID)>>5)&0x0000ffff;
  
  if( (ID&0xfff0) != 0x5200)                                        /*ID may be 0x5201,0x5202,0x5203 and so on*/
  {
    return 0;
  }
  
  H_ID = ADXRS453_Z_SEND( (READ_Command | PID_Address)>>16);
  L_ID = ADXRS453_Z_SEND( (u16)(READ_Command | PID_Address));
  
  ID = (((H_ID<<16) | L_ID)>>5)&0x0000ffff;
   
  Delay_us(10);
  ADXRS453_X_CS_HIGH;  
  ADXRS453_Y_CS_HIGH; 
  ADXRS453_Z_CS_HIGH; 
  
  if( (ID&0xfff0) != 0x5200)                                        /*ID may be 0x5201,0x5202,0x5203 and so on*/
  {
    return 0;
  }
  else return 1;                                                    /*ADXRS453 is good*/   
}

/****************************************************************************
 * Function:    ADXRS453 DMA read start
 * Description: put read command into DMA send buffer and start communication
 * Input:       
 * Output:      
 * Return:     
****************************************************************************/
void ADXRS453_X_read()  
{
   ADXRS453_X_CS_LOW;
    
   SPI1_Buffer_Tx[0] = 0x2000;             /*Command [31:29]    register_address [25:17]*/
   SPI1_Buffer_Tx[1] = 0x0000;             /*DATA    [16:1]     */

   SPI1_DMA_TX(SPI1_BufferSize_Tx);    
}

void ADXRS453_Y_read()  
{
   ADXRS453_Y_CS_LOW;
    
   SPI2_Buffer_Tx[0] = 0x2000;
   SPI2_Buffer_Tx[1] = 0x0000;

   SPI2_DMA_TX(SPI2_BufferSize_Tx);   
}

void ADXRS453_Z_read()  
{ 
   ADXRS453_Z_CS_LOW;
  
   SPI3_Buffer_Tx[0] = 0x2000;
   SPI3_Buffer_Tx[1] = 0x0000;

   SPI3_DMA_TX(SPI3_BufferSize_Tx);  
}

/****************************************************************************
 * Function:    ADXRS453_data_check
 * Description: judge the ST[27:26] whether 01 and Odd parity check P0[31:16],P1[31:0]
 * Input:       sensor collect data
 * Output:      
 * Return:      if data is right return 1,else return 0
****************************************************************************/
uint8_t ADXRS453_data_check(unsigned int data)
{
  uint8_t P0 = 0,P1 = 0,i; 
  uint8_t odd_P0 = 0,odd_P1 =0;
  unsigned int data_P0 = 0,data_P1 = 0;
  uint8_t data_flag;

  if( ( (data>>26)&(0x03) ) == 0x01)                      /*ST [27:26] -> 01 ,data is right*/ 
   {
      P0 = data&0x10000000;
      P1 = data&0x00000001;
      data &=~(1<<28);                                    /*clear p0,p1*/
      data &=~(1<<0);       

      for(i=16;i<32;i++)                            
      {
        data_P0= data>>i;
        
        if(data_P0&0x01)
          odd_P0++;    
      }     
     
      for(i=0;i<16;i++)
      {
         data_P1= data>>i;
        
        if(data_P1&0x01)
          odd_P1++;    
      }
      odd_P1 += odd_P0;
      
      if ( (P0 == (odd_P0&0x01) && (P1 == (odd_P1&0x01) ) ) )
      {
        data_flag = 1;                                       /*data is right*/
      }
    }
   else  data_flag = 0;                                       /*data error*/
    
   return   data_flag;  
}

/****************************************************************************
 * Function:    ADXRS453_data_save
 * Description: accumulate  sensor collect data
 * Input:       sensor collect data
 * Output:      ADXRS453_collect_data
 * Return:      void
****************************************************************************/
void ADXRS453_data_save(int16_t data,ADXRS453_data* ADXRS453_collect ,GYRO_data_source GYRO)
{
   switch (GYRO)
   {
     case gyro_x : ADXRS453_collect->sum_X += data;
                   ADXRS453_collect->X_count ++;
                   break;
     case gyro_y : ADXRS453_collect->sum_Y += data;
                   ADXRS453_collect->Y_count ++;
                   break;
     case gyro_z : ADXRS453_collect->sum_Z += data;
                   ADXRS453_collect->Z_count ++;
                   break;
     case gyro_temp : ADXRS453_collect->sum_TEMP += data;
                      ADXRS453_collect->TEMP_count ++;
                      break;
     default :  break;
   }
}

/****************************************************************************
 * Function:    ADXRS453_data_calculate
 * Description: calculate  sensor collect data
 * Input:       ADXRS453_collect_data and MCU_Out_Gyro
 * Output:      MCU_Out_Gyro
 * Return:      void
****************************************************************************/
unsigned char ADXRS453_data_calculate(ADXRS453_data* ADXRS453_collect,ADXRS453_final_data* AD_OUT_DATA)
{
  unsigned char data_OK_flag = 1;
  
  if(ADXRS453_collect->X_count > 0)
  {
    AD_OUT_DATA->X_f = ( ADXRS453_collect->sum_X / ADXRS453_collect->X_count ) * GYRO_scale;
    
    ADXRS453_collect->X_count = 0; 
    ADXRS453_collect->sum_X = 0;
  }
  else data_OK_flag = 0;
  
  if(ADXRS453_collect->Y_count > 0)
  {
    AD_OUT_DATA->Y_f = ( ADXRS453_collect->sum_Y / ADXRS453_collect->Y_count ) * GYRO_scale;
    
    ADXRS453_collect->Y_count = 0;
    ADXRS453_collect->sum_Y = 0;
  }
  else data_OK_flag = 0;
  
  if(ADXRS453_collect->Z_count > 0)
  {
    AD_OUT_DATA->Z_f = ( ADXRS453_collect->sum_Z / ADXRS453_collect->Z_count ) * GYRO_scale;
    
    ADXRS453_collect->Z_count = 0;
    ADXRS453_collect->sum_Z = 0;
  }
  else data_OK_flag = 0;
  
  if(ADXRS453_collect->TEMP_count > 0)
  {
    AD_OUT_DATA->TEMP_f = ( ADXRS453_collect->sum_TEMP / ADXRS453_collect->TEMP_count ) * 0.2f +45.0f;   /*temp = data*0.2 + 45 degrees */
    
    ADXRS453_collect->TEMP_count = 0;
    ADXRS453_collect->sum_TEMP = 0;
  }
  else data_OK_flag = 0;
  
  return data_OK_flag;
}






