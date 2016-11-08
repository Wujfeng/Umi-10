/****************************************************************
 * Copyright (C) 2016, Yagro, all right reserved.
 * File name:    main.c
 * Date:         2016.03.22
 * Description:  GYRO ACC Sampling collection Upload
*****************************************************************/

#include "stm32f4xx.h"
#include "Command.h"
#include "ADS1258.h"
#include "ADXRS453.h"
#include "BMP280.h"

#define  EXTI_LINE1_ON

void TIM_Configuration(void);
void NVIC_Configuration(void);
void EXTI_Configuration(void);
void System_Init(void);

/* Serial reception flag */
uint8_t BMP280_Normal_Flag = 0;
uint8_t ADXRS453_Normal_Flag = 0;
uint8_t ADS1258_Normal_Flag = 0;

uint8_t BMP280_calculate_finish_Flag = 0;
uint8_t Bmp_Conversion_Finish_Flag = 0;

int main(void)
{
  System_Init();  /* Initialization System. */
  
  for(;;)
  {
    if(Bmp_Conversion_Finish_Flag == 1)
    {
      Bmp_Conversion_Finish_Flag = 0;
      
      BMP280 p = bmp280;  /* Data protection */
      
      BMP280_UT2TEMP(&bmp280_Calib, &p, &fData_bmp280);  // First calculate the temperature, barometric pressure after count. 
      BMP280_UP2PRESS(&bmp280_Calib, &p, &fData_bmp280);
      
      BMP280_calculate_finish_Flag = 1;
      
    }
  }
}

/****************************************************
 * Function:    TIM_Configuration
 * Description: TIM3 Configuration.
 * Input:
 * Output:
 * Return:
*****************************************************/
void TIM_Configuration(void)
{
  TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;

  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);  /* Enable TIM3 clock */

  TIM_TimeBaseInitStructure.TIM_Prescaler = 1;                    /* Specifies the prescaler value used to divide the TIM clock. */
  TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up; /* Specifies the counter mode */
  TIM_TimeBaseInitStructure.TIM_Period = 41999;                   /* Reload value	approximate 2.3KHz 18260 1k->41999*/
  TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
  TIM_TimeBaseInit(TIM3, &TIM_TimeBaseInitStructure);

  TIM_ClearFlag(TIM3, TIM_FLAG_Update);

  TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE);

  TIM_Cmd(TIM3, ENABLE);
}

/****************************************************
 * Function:    NVIC_Configuration
 * Description: Interrupt Vector Table Configuration.
 * Input:
 * Output:
 * Return:
*****************************************************/
void NVIC_Configuration(void)
{
  NVIC_InitTypeDef NVIC_InitStructure;

  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
  
  NVIC_InitStructure.NVIC_IRQChannel = EXTI9_5_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x00;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x01;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);

  NVIC_InitStructure.NVIC_IRQChannel = DMA2_Stream0_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x00;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x02;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);

  NVIC_InitStructure.NVIC_IRQChannel = DMA1_Stream3_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x00;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x03;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);

  NVIC_InitStructure.NVIC_IRQChannel = DMA1_Stream0_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x00;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x04;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);

  NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x01;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x00;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);

#ifdef EXTI_LINE0_ON
  NVIC_InitStructure.NVIC_IRQChannel = EXTI0_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x00;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x00;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
#endif

#ifdef EXTI_LINE1_ON
  NVIC_InitStructure.NVIC_IRQChannel = EXTI1_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x00;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x00;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
#endif
}

/****************************************************
 * Function:    EXTI_Configuration
 * Description: External Interrupt Configuration.
 * Input:
 * Output:
 * Return:
*****************************************************/
void EXTI_Configuration(void)
{
  EXTI_InitTypeDef EXTI_InitStructure;
  GPIO_InitTypeDef GPIO_InitStructure;

#ifdef EXTI_LINE0_ON
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);   /* Enable GPIOA clock */
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);  /* Enable SYSCFG clock */

  /* Configure PA0 pin as input floating */
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
  GPIO_Init(GPIOA, &GPIO_InitStructure);

  /* Connect EXTI Line0 to PA0 pin */
  SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOA, EXTI_PinSource0);

  EXTI_InitStructure.EXTI_Line = EXTI_Line0;
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising_Falling;
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  EXTI_Init(&EXTI_InitStructure);
#endif

#ifdef EXTI_LINE1_ON
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);   /* Enable GPIOA clock */
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);  /* Enable SYSCFG clock */

  /* Configure PA1 pin as input floating */
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
  GPIO_Init(GPIOA, &GPIO_InitStructure);

  /* Connect EXTI Line1 to PA1 pin */
  SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOA, EXTI_PinSource1);

  EXTI_InitStructure.EXTI_Line = EXTI_Line1;
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising_Falling;
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  EXTI_Init(&EXTI_InitStructure);
#endif
}



/****************************************************
 * Function:    System_Init
 * Description: Initialization System.
 * Input:
 * Output:
 * Return:
*****************************************************/
void System_Init(void)
{

//  if(Flash_Read(&system_parameter) != FLASH_WRITE_FLAG)
//  {
//    Flash_Init(&system_parameter);
//    Flash_Write(&system_parameter);
//  }

  Delay_Init();
  
  USART1_Init(921600, USART_Mode_Tx|USART_Mode_Rx, USART_IT_IDLE, USART_DMAReq_Tx|USART_DMAReq_Rx);
  USART1_DMA_TX_Init((uint32_t)USART1_Buffer_Tx, USART1_BufferSize_Tx);
  USART1_DMA_RX_Init((uint32_t)USART1_Buffer_Rx, USART1_BufferSize_Rx);

  SPI1_Init();
  SPI2_Init();
  SPI3_Init();
  
  Delay_ms(50);
  
  ADS1258_Normal_Flag = ADS1258_ID_check();
  
  ADS1258_Init();

  ADXRS453_Normal_Flag = ADXRS453_ID_Check();
  ADXRS453_Init();
  
  BMP280_Normal_Flag = BMP280_Init(&bmp280_Calib, BMP280_INDOOR_NAVIGATION_MODE);

  SPI1_DMA_TX_Init((uint32_t)SPI1_Buffer_Tx, SPI1_BufferSize_Tx);
  SPI2_DMA_TX_Init((uint32_t)SPI2_Buffer_Tx, SPI2_BufferSize_Tx);
  SPI3_DMA_TX_Init((uint32_t)SPI3_Buffer_Tx, SPI3_BufferSize_Tx);

  SPI1_DMA_RX_Init((uint32_t)SPI1_Buffer_Rx, SPI1_BufferSize_Rx);
  SPI2_DMA_RX_Init((uint32_t)SPI2_Buffer_Rx, SPI2_BufferSize_Rx);
  SPI3_DMA_RX_Init((uint32_t)SPI3_Buffer_Rx, SPI3_BufferSize_Rx);

  EXTI_Configuration(); 

  NVIC_Configuration();
}
