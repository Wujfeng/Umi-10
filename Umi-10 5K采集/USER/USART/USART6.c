/****************************************************************
 * Copyright (C) 2016, Yagro, all right reserved.
 * File name:    USART6.c
 * Date:         2016.03.22
 * Description:  USART6 Driver
*****************************************************************/


#include "USART6.h"


uint8_t USART6_Buffer_Tx[USART6_BufferSize_Tx];
uint8_t USART6_Buffer_Rx[USART6_BufferSize_Rx];


/****************************************************
 * Function:    USART6_Init
 * Description: USART6 Configuration.
 * Input:       BaudRate
 *              USART_Mode
 *              USART_IT
 *              USART_DMAReq
 * Output:
 * Return:
*****************************************************/
void USART6_Init(uint32_t BaudRate, uint16_t USART_Mode, uint16_t USART_IT, uint16_t USART_DMAReq)
{
  GPIO_InitTypeDef GPIO_InitStructure;
  USART_InitTypeDef USART_InitStructure;
  
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_USART6_Tx, ENABLE);  /* Enable USART6_Tx GPIO clock */
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_USART6_Rx, ENABLE);  /* Enable USART6_Rx GPIO clock */
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART6, ENABLE);     /* Enable USART6 clock */

  GPIO_PinAFConfig(GPIO_USART6_Tx, GPIO_PinSource_USART6_Tx, GPIO_AF_USART6);  /* Connect GPIOxn to USART6_Tx */
  GPIO_PinAFConfig(GPIO_USART6_Rx, GPIO_PinSource_USART6_Rx, GPIO_AF_USART6);  /* Connect GPIOxn to USART6_Rx */

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_USART6_Tx;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
  GPIO_Init(GPIO_USART6_Tx, &GPIO_InitStructure);
    
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_USART6_Rx;
  GPIO_Init(GPIO_USART6_Rx, &GPIO_InitStructure);

  USART_InitStructure.USART_BaudRate = BaudRate;
  USART_InitStructure.USART_WordLength = USART_WordLength_8b;
  USART_InitStructure.USART_StopBits = USART_StopBits_1;
  USART_InitStructure.USART_Parity = USART_Parity_No;
  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
  USART_InitStructure.USART_Mode = USART_Mode;
  
  USART_Init(USART6, &USART_InitStructure);

  if(USART_DMAReq == USART_DMAReq_Tx)
    USART_DMACmd(USART6, USART_DMAReq_Tx, ENABLE);  /* Enabling the DMA request specified USART6 */
  if(USART_DMAReq == USART_DMAReq_Rx)
    USART_DMACmd(USART6, USART_DMAReq_Rx, ENABLE);  /* Enabling the DMA request specified USART6 */
  if(USART_DMAReq == (USART_DMAReq_Tx|USART_DMAReq_Rx))
  {
    USART_DMACmd(USART6, USART_DMAReq_Tx, ENABLE);  /* Enabling the DMA request specified USART6 */
    USART_DMACmd(USART6, USART_DMAReq_Rx, ENABLE);  /* Enabling the DMA request specified USART6 */
  }

  if(USART_IT)
    USART_ITConfig(USART6, USART_IT, ENABLE); /* Enabling interrupts specified USART6 */

  USART_Cmd(USART6, ENABLE);
}

/****************************************************
 * Function:    USART6_DMA_TX_Init
 * Description: USART6 DMA TX Configuration.
 * Input:       DMA_Memory0BaseAddr
 *              DMA_BufferSize
 * Output:
 * Return:
*****************************************************/
void USART6_DMA_TX_Init(uint32_t DMA_Memory0BaseAddr, uint16_t DMA_BufferSize)
{
  DMA_InitTypeDef DMA_InitStructure;

  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2, ENABLE);  /* Enable DMA2 clock */

  DMA_DeInit(DMA2_Stream6);
  while(DMA_GetCmdStatus(DMA2_Stream6) != DISABLE);

  DMA_InitStructure.DMA_Channel = DMA_Channel_5;                          /* Channel selection */
  DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&USART6->DR;       /* Set the DMA source: peripheral data register address */
  DMA_InitStructure.DMA_Memory0BaseAddr = DMA_Memory0BaseAddr;            /* Memory address */
  DMA_InitStructure.DMA_DIR = DMA_DIR_MemoryToPeripheral;                 /* Data Memory to Peripheral */
  DMA_InitStructure.DMA_BufferSize = DMA_BufferSize;                      /* Specifies the DMA channel DMA buffer size */
  DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;        /* Peripheral address does not increase */
  DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;                 /* Memory address increment */
  DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte; /* Peripheral data width of 8 bit */
  DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;         /* Memory data width of 8 bit */
  DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;                           /* Use Normal mode */
  DMA_InitStructure.DMA_Priority = DMA_Priority_Medium;                   /* Medium priority */
  DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;
  DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_Full;
  DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;             /* A single transmission burst memory */
  DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;     /* Peripherals single transmission burst */
  
  DMA_Init(DMA2_Stream6, &DMA_InitStructure);
  DMA_Cmd(DMA2_Stream6, DISABLE);
  
  /* Starting a byte of data to make a DMA transmission completion flag */
  USART6_DMA_TX(1);
}

/****************************************************
 * Function:    USART6_DMA_RX_Init
 * Description: USART6 DMA RX Configuration.
 * Input:       DMA_Memory0BaseAddr
 *              DMA_BufferSize
 * Output:
 * Return:
*****************************************************/
void USART6_DMA_RX_Init(uint32_t DMA_Memory0BaseAddr, uint16_t DMA_BufferSize)
{
  DMA_InitTypeDef DMA_InitStructure;

  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2, ENABLE);  /* Enable DMA2 clock */
  
  DMA_DeInit(DMA2_Stream1);
  while(DMA_GetCmdStatus(DMA2_Stream1) != DISABLE);

  DMA_InitStructure.DMA_Channel = DMA_Channel_5;                          /* Channel selection */
  DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&USART6->DR;       /* Set the DMA source: peripheral data register address */
  DMA_InitStructure.DMA_Memory0BaseAddr = DMA_Memory0BaseAddr;            /* Memory address */
  DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralToMemory;                 /* Data Peripheral to Memory */
  DMA_InitStructure.DMA_BufferSize = DMA_BufferSize;                      /* Specifies the DMA channel DMA buffer size */
  DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;        /* Peripheral address does not increase */
  DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;                 /* Memory address increment */
  DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte; /* Peripheral data width of 8 bit */
  DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;         /* Memory data width of 8 bit */
  DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;                         /* Use Circular mode */
  DMA_InitStructure.DMA_Priority = DMA_Priority_Medium;                   /* Medium priority */
  DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;
  DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_Full;
  DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;             /* A single transmission burst memory */
  DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;     /* Peripherals single transmission burst */
  
  DMA_Init(DMA2_Stream1, &DMA_InitStructure);
  DMA_Cmd(DMA2_Stream1, ENABLE);
}

/****************************************************
 * Function:    USART6_DMA_TX
 * Description: Start USART6 DMA Send.
 * Input:       DMA_BufferSize
 * Output:
 * Return:
*****************************************************/
void USART6_DMA_TX(uint16_t DMA_BufferSize)
{
  DMA_ClearFlag(DMA2_Stream6, DMA_FLAG_TCIF6);  /* Clear Pending DMA channel x flag (DMA2 DMA2_Stream6 transfer completion flag) */
  DMA_Cmd(DMA2_Stream6, DISABLE);
  while(DMA_GetCmdStatus(DMA2_Stream6) != DISABLE);
  DMA_SetCurrDataCounter(DMA2_Stream6, DMA_BufferSize);
  DMA_Cmd(DMA2_Stream6, ENABLE);
}

/****************************************************
 * Function:    USART6_DMA_RX
 * Description: Start USART6 DMA Receive.
 * Input:       DMA_BufferSize
 * Output:
 * Return:
*****************************************************/
void USART6_DMA_RX(uint16_t DMA_BufferSize)
{
  DMA_ClearFlag(DMA2_Stream1, DMA_FLAG_TCIF1);  /* Clear Pending DMA channel x flag (DMA2 DMA2_Stream1 transfer completion flag) */
  DMA_Cmd(DMA2_Stream1, DISABLE);
  while(DMA_GetCmdStatus(DMA2_Stream1) != DISABLE);
  DMA_SetCurrDataCounter(DMA2_Stream1, DMA_BufferSize);
  DMA_Cmd(DMA2_Stream1, ENABLE);
}

/****************************************************
 * Function:    USART6_OUT
 * Description: USART6 Send.
 * Input:       Buffer
                BufferSize
 * Output:
 * Return:
*****************************************************/
void USART6_OUT(uint8_t* Buffer, uint16_t BufferSize)
{
  while(DMA_GetFlagStatus(DMA2_Stream6, DMA_FLAG_TCIF6)==0);

  memset(USART6_Buffer_Tx, 0, USART6_BufferSize_Tx);  /* Empty memory */
  memcpy(USART6_Buffer_Tx, Buffer, BufferSize);       /* Copy memory */
  
  USART6_DMA_TX(BufferSize);
}

/****************************************************
 * Function:    USART6_IN
 * Description: USART6 Receive.
 * Input:
 * Output:
 * Return:
*****************************************************/
void USART6_IN(void)
{
  memset(USART6_Buffer_Rx, 0, USART6_BufferSize_Rx);  /* Empty memory */
  
  USART6_DMA_RX(USART6_BufferSize_Rx);
}
