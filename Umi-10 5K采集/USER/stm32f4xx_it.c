/**
  ******************************************************************************
  * @file    GPIO/GPIO_IOToggle/stm32f4xx_it.c 
  * @author  MCD Application Team
  * @version V1.6.0
  * @date    04-September-2015
  * @brief   Main Interrupt Service Routines.
  *          This file provides template for all exceptions handler and 
  *          peripherals interrupt service routine.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2015 STMicroelectronics</center></h2>
  *
  * Licensed under MCD-ST Liberty SW License Agreement V2, (the "License");
  * You may not use this file except in compliance with the License.
  * You may obtain a copy of the License at:
  *
  *        http://www.st.com/software_license_agreement_liberty_v2
  *
  * Unless required by applicable law or agreed to in writing, software 
  * distributed under the License is distributed on an "AS IS" BASIS, 
  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  * See the License for the specific language governing permissions and
  * limitations under the License.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_it.h"
#include "ADXRS453.h"
#include "BMP280.h"
#include "ADS1258.h"
#include "Command.h"
#include "Demarcate.h"
#include  <math.h> 

uint8_t Bmp_Update_Flag = 0;      /* Prevent Repeat Read Register */
uint8_t Bmp_Update_Flag_Old = 0;  /* Prevent Repeat Read Register */

extern uint8_t BMP280_Normal_Flag ;
extern uint8_t ADXRS453_Normal_Flag ;
extern uint8_t ADS1258_Normal_Flag ;
extern uint8_t BMP280_calculate_finish_Flag;
extern uint8_t Bmp_Conversion_Finish_Flag;

/** @addtogroup STM32F4xx_StdPeriph_Examples
  * @{
  */

/** @addtogroup GPIO_IOToggle
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/******************************************************************************/
/*            Cortex-M4 Processor Exceptions Handlers                         */
/******************************************************************************/

/**
  * @brief   This function handles NMI exception.
  * @param  None
  * @retval None
  */
void NMI_Handler(void)
{
}

/**
  * @brief  This function handles Hard Fault exception.
  * @param  None
  * @retval None
  */
void HardFault_Handler(void)
{
  /* Go to infinite loop when Hard Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Memory Manage exception.
  * @param  None
  * @retval None
  */
void MemManage_Handler(void)
{
  /* Go to infinite loop when Memory Manage exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Bus Fault exception.
  * @param  None
  * @retval None
  */
void BusFault_Handler(void)
{
  /* Go to infinite loop when Bus Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Usage Fault exception.
  * @param  None
  * @retval None
  */
void UsageFault_Handler(void)
{
  /* Go to infinite loop when Usage Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles SVCall exception.
  * @param  None
  * @retval None
  */
void SVC_Handler(void)
{
}

/**
  * @brief  This function handles Debug Monitor exception.
  * @param  None
  * @retval None
  */
void DebugMon_Handler(void)
{
}

/**
  * @brief  This function handles PendSVC exception.
  * @param  None
  * @retval None
  */
void PendSV_Handler(void)
{
}

/**
  * @brief  This function handles SysTick Handler.
  * @param  None
  * @retval None
  */
void SysTick_Handler(void)
{
}

/****************************************************
 * Function:    EXTI1_IRQHandler
 * Description: EXTI1 interrupt.
 * Input:
 * Output:
 * Return:
*****************************************************/
void EXTI1_IRQHandler(void)
{
  unsigned char Gyro_data_OK_flag = 0,ACC_data_OK_flag = 0,Press_data_OK_flag = 0; 
  static unsigned int time_10ms = 0;
  int16_t ACC_TEMP = 0,GYRO_TEMP = 0,Press_temp = 0;
  if(EXTI_GetITStatus(EXTI_Line1) != RESET)
  {
    time_10ms += 10;
    
    if(time_10ms == 20000)
      time_10ms = 0;
    
    ACC_data_OK_flag = ADS1258_data_calculate(&ad_collect_data, &MCU_Out_Acc);
    Gyro_data_OK_flag = ADXRS453_data_calculate(&ADXRS453_collect_data, &MCU_Out_Gyro);
    Press_data_OK_flag = BMP280_calculate_AVE(&MCU_Out_Press);
    
    MCU_Out_Data.Head[0]=0xAB;
    MCU_Out_Data.Head[1]=0x12;
    
    MCU_Out_Data.Flag |= (1<<0);                                                                                               /*mode is navigation mode*/

    if( (fabs(MCU_Out_Gyro.X_f) < 400.0) && (fabs(MCU_Out_Gyro.Y_f) < 400.0) && (fabs(MCU_Out_Gyro.Z_f) < 400.0) && (Gyro_data_OK_flag == 1) )
      MCU_Out_Data.Flag |= (1<<1);                                                                                             /*GYRO data is right*/
    else 
      MCU_Out_Data.Flag &=~ (1<<1);
    
    if( (fabs(MCU_Out_Acc.ACC_X_f) < 1.7f) && (fabs(MCU_Out_Acc.ACC_Y_f) < 1.7f)  && (fabs(MCU_Out_Acc.ACC_Z_f) < 1.7f) && (ACC_data_OK_flag == 1) )
      MCU_Out_Data.Flag |= (1<<2);                                                                                             /*ACC data is right*/
    else 
      MCU_Out_Data.Flag &=~ (1<<2);
    
    if( (MCU_Out_Press.ave_fAltitude < 9000.0f) && (MCU_Out_Press.ave_fAltitude > -500.0f) && (Press_data_OK_flag == 1) )
      MCU_Out_Data.Flag |= (1<<3);                                                                                             /*Pressure data is right*/
    else 
      MCU_Out_Data.Flag &=~ (1<<3);
    
    if( ADXRS453_Normal_Flag == 1)                                                                                          /*GYRO chip is good*/
      MCU_Out_Data.Flag |= (1<<4);
    else
      MCU_Out_Data.Flag &=~ (1<<4);
    
    if( ADS1258_Normal_Flag == 1)                                                                                           /*AD->ACC chip is good*/
      MCU_Out_Data.Flag |= (1<<5);
    else
      MCU_Out_Data.Flag &=~ (1<<5);
    
    if( BMP280_Normal_Flag == 1)                                                                                            /*Pressure chip is good*/
      MCU_Out_Data.Flag |= (1<<6);
    else
      MCU_Out_Data.Flag &=~ (1<<6);
    
    MCU_Out_Data.Time = time_10ms;
    
    MCU_Out_Data.Gyro_X = MCU_Out_Gyro.X_f;
    MCU_Out_Data.Gyro_Y = MCU_Out_Gyro.Y_f;
    MCU_Out_Data.Gyro_Z = MCU_Out_Gyro.Z_f;
    GYRO_TEMP = MCU_Out_Gyro.TEMP_f * 16;
    MCU_Out_Data.Gyro_Temp = GYRO_TEMP;
    
    MCU_Out_Data.Acc_X = MCU_Out_Acc.ACC_X_f;
    MCU_Out_Data.Acc_Y = MCU_Out_Acc.ACC_Y_f;
    MCU_Out_Data.Acc_Z = MCU_Out_Acc.ACC_Z_f;
    ACC_TEMP = MCU_Out_Acc.TEMP * 16;
    MCU_Out_Data.Acc_Temp = ACC_TEMP;
    
    MCU_Out_Data.Altitude = MCU_Out_Press.ave_fAltitude;
    Press_temp = MCU_Out_Press.ave_fTemp * 16;
    MCU_Out_Data.Bmp_Temp = Press_temp;
    
    MCU_Out_Data.Check = check_sum((uint8_t*)&MCU_Out_Data,sizeof(MCU_Out_Data) - 1);
    
    USART1_OUT((uint8_t*)&MCU_Out_Data, sizeof(MCU_Out_Data));

    EXTI_ClearITPendingBit(EXTI_Line1);
  }
}

/****************************************************
 * Function:    EXTI2_IRQHandler
 * Description: EXTI2 interrupt.
 * Input:
 * Output:
 * Return:
*****************************************************/
void EXTI2_IRQHandler(void)
{
  if(EXTI_GetITStatus(EXTI_Line2) != RESET)
  {


    EXTI_ClearITPendingBit(EXTI_Line2);
  }
}

/****************************************************
 * Function:    EXTI9_5_IRQHandler
 * Description: EXTI9_5 interrupt.
 * Input:
 * Output:
 * Return:
*****************************************************/
void EXTI9_5_IRQHandler(void)
{
  if(EXTI_GetITStatus(EXTI_Line7) != RESET)
  {
    if(BMP280_calculate_finish_Flag == 1)
    {
      BMP280_Data_Sum(&fData_bmp280, &MCU_Out_Press);
      
      BMP280_calculate_finish_Flag = 0;
    }
    
    ADS1258_CS_LOW;
    
    SPI2_Buffer_Tx[0] = 0X0000;
    SPI2_Buffer_Tx[1] = 0X0000;

    SPI2_DMA_TX(SPI2_BufferSize_Tx);
    
    EXTI_ClearITPendingBit(EXTI_Line7);
  }
}


/****************************************************
 * Function:    USART1_IRQHandler
 * Description: USART1 interrupt.
 * Input:
 * Output:
 * Return:
*****************************************************/
void USART1_IRQHandler(void)
{
  if(USART_GetITStatus(USART1, USART_IT_IDLE) != RESET)
  {
    DMA_Cmd(DMA2_Stream2,DISABLE);

    USART1->SR;
    USART1->DR;


    USART_ClearITPendingBit(USART1, USART_IT_IDLE);
  }
}

/****************************************************
 * Function:    TIM3_IRQHandler
 * Description: TIM3 interrupt.
 * Input:
 * Output:
 * Return:
*****************************************************/
void TIM3_IRQHandler(void)
{
   
    TIM_ClearITPendingBit(TIM3, TIM_IT_Update);
}

/****************************************************
 * Function:    DMA2_Stream0_IRQHandler
 * Description: SPI1 DMA Reception completion interrupt.
 * Input:
 * Output:
 * Return:
*****************************************************/

void Delay_500ns(void)
{
  __nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();
  __nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();
  __nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();
  __nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();
  __nop();__nop();
}

void DMA2_Stream0_IRQHandler(void)
{
  static unsigned char Number_SPI1 = 0;
  int16_t gyro_X_16data;
  unsigned int H_data, L_data;
  unsigned int data;
  
  if(DMA_GetITStatus(DMA2_Stream0, DMA_IT_TCIF0) != RESET)
  {
    if(Number_SPI1 == 0)
    {
      ADXRS453_X_CS_HIGH;
      Delay_500ns();
      ADXRS453_X_CS_LOW;

      SPI1_Buffer_Tx[0] = 0x2000;
      SPI1_Buffer_Tx[1] = 0x0000;
      SPI1_DMA_TX(SPI1_BufferSize_Tx);

      Number_SPI1 = 1;
    }
    else if(Number_SPI1 == 1)
    {
      ADXRS453_X_CS_HIGH;
      Delay_500ns();
      BMP280_CS_LOW();
      
      H_data = SPI1_Buffer_Rx[0]<<16;
      L_data = SPI1_Buffer_Rx[1];
      
      if( ADXRS453_data_check( H_data | L_data ) )
      {
        gyro_X_16data = ((H_data+L_data)>>10)&0x0000ffff;                  /*LOAD DATA    [20:5]   16 bits  */
        ADXRS453_data_save(gyro_X_16data,&ADXRS453_collect_data,gyro_x);
      }
      
      SPI1_Buffer_Tx[0] = ((uint16_t)(BMP280_STATUS_REG|0x80)<<8)|0xff;
      SPI1_Buffer_Tx[1] = 0xffff;
      
      SPI1_DMA_TX(SPI1_BufferSize_Tx);
      
      Number_SPI1 = 2;
    }
    else if(Number_SPI1 == 2)
    {
      BMP280_CS_HIGH();
      
      data = SPI1_Buffer_Rx[0];
      Bmp_Update_Flag = data & 0x00000008;
      
      if( Bmp_Update_Flag_Old==0x08 && Bmp_Update_Flag==0x00 )
      {
        Delay_500ns();
        BMP280_CS_LOW();
        
        SPI1_Buffer_Tx[0] = ((uint16_t)(BMP280_TEMP_MSB_REG|0x80)<<8)|0xff;
        SPI1_Buffer_Tx[1] = 0xffff;
        
        SPI1_DMA_TX(SPI1_BufferSize_Tx);
        
        Number_SPI1 = 3;
      }
      else
      {
        Number_SPI1 = 0;
      }
      
      Bmp_Update_Flag_Old = Bmp_Update_Flag;
    }
    else if(Number_SPI1 == 3)
    {
      BMP280_CS_HIGH();
      Delay_500ns();
      BMP280_CS_LOW();
      
      data = SPI1_Buffer_Rx[0];
      data <<= 16;
      data |= SPI1_Buffer_Rx[1];
      data = (data&0x00fffff0)>>4;
      
      bmp280.TEMP = data;
      
      SPI1_Buffer_Tx[0] = ((uint16_t)(BMP280_PRESS_MSB_REG|0x80)<<8)|0xff;
      SPI1_Buffer_Tx[1] = 0xffff;
      
      SPI1_DMA_TX(SPI1_BufferSize_Tx);
      
      Number_SPI1 = 4;
    }
    else
    {
      BMP280_CS_HIGH();
      
      data = SPI1_Buffer_Rx[0];
      data <<= 16;
      data |= SPI1_Buffer_Rx[1];
      data = (data&0x00fffff0)>>4;
      
      bmp280.PRESS = data;
      
      Bmp_Conversion_Finish_Flag = 1;
      
      Number_SPI1 = 0;
    }
    
    DMA_ClearITPendingBit(DMA2_Stream0, DMA_IT_TCIF0);
  
  }      
}


/****************************************************
 * Function:    DMA1_Stream3_IRQHandler
 * Description: SPI2 DMA Reception completion interrupt.
 * Input:
 * Output:
 * Return:
*****************************************************/
void DMA1_Stream3_IRQHandler(void)                                         /*AD GYRO_X_Y_Z collect start GYRO_Y*/
{
   static unsigned char Number_SPI2 = 0;
   int16_t gyro_Y_16data;
   unsigned int H_data, L_data;
  
  if(DMA_GetITStatus(DMA1_Stream3, DMA_IT_TCIF3) != RESET)
  {
    if(Number_SPI2 == 0)
    {
      ADS1258_CS_HIGH;
      
      H_data = SPI2_Buffer_Rx[0];
      L_data = SPI2_Buffer_Rx[1];
      
      ADS1258_data_check_accumulation(H_data,L_data);
      
      Delay_500ns();
      
      ADXRS453_X_read();
      ADXRS453_Y_read(); 
      ADXRS453_Z_read();
      
      Number_SPI2 = 1;
    }
    else if(Number_SPI2 == 1 )
    {     
      ADXRS453_Y_CS_HIGH;      
      Delay_500ns();
      ADXRS453_Y_CS_LOW; 
    
      SPI2_Buffer_Tx[0] = 0x2000;
      SPI2_Buffer_Tx[1] = 0x0000;
      SPI2_DMA_TX(SPI2_BufferSize_Tx); 
      
      Number_SPI2 = 2;     
    }
    else if(Number_SPI2 == 2)
    {
      ADXRS453_Y_CS_HIGH;
      
      H_data = SPI2_Buffer_Rx[0]<<16;
      L_data = SPI2_Buffer_Rx[1];
      
      if( ADXRS453_data_check( H_data | L_data ) )
      {
        gyro_Y_16data = ((H_data+L_data)>>10)&0x0000ffff;                  /*LOAD DATA    [20:5]   16 bits  */
        
        ADXRS453_data_save(gyro_Y_16data,&ADXRS453_collect_data,gyro_y);
      }
      
      Number_SPI2 = 0;
    }
    DMA_ClearITPendingBit(DMA1_Stream3, DMA_IT_TCIF3);
  }
}

/****************************************************
 * Function:    DMA1_Stream0_IRQHandler
 * Description: SPI3 DMA Reception completion interrupt.
 * Input:
 * Output:
 * Return:
*****************************************************/
void DMA1_Stream0_IRQHandler(void)
{
  static unsigned char Number_SPI3 = 0;
  int16_t gyro_Z_16data, temp_10_data;
  unsigned int H_data,L_data;
  
  if(DMA_GetITStatus(DMA1_Stream0, DMA_IT_TCIF0) != RESET)
  {
    if(Number_SPI3 == 0)
    {
      ADXRS453_Z_CS_HIGH;
      Delay_500ns();
      ADXRS453_Z_CS_LOW; 
      
      SPI3_Buffer_Tx[0] = 0x2000;
      SPI3_Buffer_Tx[1] = 0x0000;
      SPI3_DMA_TX(SPI3_BufferSize_Tx);
      
      Number_SPI3 = 1;     
    }
    else if(Number_SPI3 == 1)
    {
      ADXRS453_Z_CS_HIGH;
      Delay_500ns();
      ADXRS453_Z_CS_LOW;
      
      H_data = SPI3_Buffer_Rx[0]<<16;
      L_data = SPI3_Buffer_Rx[1];
      
      if( ADXRS453_data_check( H_data | L_data ) )
      {
        gyro_Z_16data = ((H_data+L_data)>>10)&0x0000ffff;                  /*LOAD DATA    [20:5]   16 bits  */
        ADXRS453_data_save(gyro_Z_16data,&ADXRS453_collect_data,gyro_z);
      }
      
      SPI3_Buffer_Tx[0] = (READ_Command|TEMP_Address|PARITY_BIT)>>16;
      SPI3_Buffer_Tx[1] = (u16)( READ_Command|TEMP_Address|PARITY_BIT );
      SPI3_DMA_TX(SPI3_BufferSize_Tx); 
    
      Number_SPI3 = 2;
    }
    else if(Number_SPI3==2)
    {
      ADXRS453_Z_CS_HIGH;
      Delay_500ns();
      ADXRS453_Z_CS_LOW;
      
      SPI3_Buffer_Tx[0] = (READ_Command|TEMP_Address|PARITY_BIT)>>16;
      SPI3_Buffer_Tx[1] = (u16)(READ_Command|TEMP_Address|PARITY_BIT);
      SPI3_DMA_TX(SPI3_BufferSize_Tx); 
  
      Number_SPI3 = 3;
    }
    else if(Number_SPI3==3)
    {
      ADXRS453_Z_CS_HIGH;
      
      H_data = SPI3_Buffer_Rx[0]<<16;                                     /*read temp*/
      L_data = SPI3_Buffer_Rx[1];

/**************************calculate temperature*********************************/   

        
      gyro_Z_16data = ((H_data+L_data)>>5)&0x0000ffff;                    /*LOAD DATA    [20:5]   16 bits  */
      temp_10_data = gyro_Z_16data>>6;                                    /*temp useful bit is [15:6] ,[5:0] is not useful*/
        
      if( ( temp_10_data & 0x200 )>0 )                                    /*change signed 10bits number into signed 16bits*/
      {
        temp_10_data |= 0xfc00;
      }
      
      ADXRS453_data_save(temp_10_data,&ADXRS453_collect_data,gyro_temp);
      
      Number_SPI3=0;
    }
    
    DMA_ClearITPendingBit(DMA1_Stream0, DMA_IT_TCIF0);
  }
}

/****************************************************
 * Function:    check_sum
 * Description: Parity ASCII data.
 * Input:       &Buffer: ASCII data
 *              lenght: Data lenght
 * Output:
 * Return:      Checksum
*****************************************************/
uint8_t check_sum(uint8_t* Buffer, int length)
{
  uint16_t sum = 0;
  uint8_t i;
  uint8_t sum_low;

  for(i=2; i<length; i++)
  {
    sum += Buffer[i];
  }

  sum_low = sum & 0x00ff;
  return sum_low;
}



/******************************************************************************/
/*                 STM32F4xx Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32f40xx.s/startup_stm32f427x.s/startup_stm32f429x.s).    */
/******************************************************************************/

/**
  * @brief  This function handles PPP interrupt request.
  * @param  None
  * @retval None
  */
/*void PPP_IRQHandler(void)
{
}*/

/**
  * @}
  */ 

/**
  * @}
  */ 


/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

