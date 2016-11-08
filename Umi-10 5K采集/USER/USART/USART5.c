/****************************************************************
 * Copyright (C) 2016, Yagro, all right reserved.
 * File name:    USART5.c
 * Date:         2016.03.22
 * Description:  USART5 Driver
*****************************************************************/


#include "USART5.h"


uint8_t USART5_Buffer_Tx[USART5_BufferSize_Tx];
uint8_t USART5_Buffer_Rx[USART5_BufferSize_Rx];


/****************************************************
 * Function:    USART5_Init
 * Description: USART5 Configuration.
 * Input:       BaudRate
 *              USART_Mode
 *              USART_IT
 *              USART_DMAReq
 * Output:
 * Return:
*****************************************************/
void USART5_Init(uint32_t BaudRate, uint16_t USART_Mode, uint16_t USART_IT, uint16_t USART_DMAReq)
{
  GPIO_InitTypeDef GPIO_InitStructure;
  USART_InitTypeDef USART_InitStructure;
  
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_USART5_Tx, ENABLE);  /* Enable USART5_Tx GPIO clock */
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_USART5_Rx, ENABLE);  /* Enable USART5_Rx GPIO clock */
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART5, ENABLE);      /* Enable USART5 clock */

  GPIO_PinAFConfig(GPIO_USART5_Tx, GPIO_PinSource_USART5_Tx, GPIO_AF_UART5);  /* Connect GPIOxn to USART5_Tx */
  GPIO_PinAFConfig(GPIO_USART5_Rx, GPIO_PinSource_USART5_Rx, GPIO_AF_UART5);  /* Connect GPIOxn to USART5_Rx */

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_USART5_Tx;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
  GPIO_Init(GPIO_USART5_Tx, &GPIO_InitStructure);
    
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_USART5_Rx;
  GPIO_Init(GPIO_USART5_Rx, &GPIO_InitStructure);

  USART_InitStructure.USART_BaudRate = BaudRate;
  USART_InitStructure.USART_WordLength = USART_WordLength_8b;
  USART_InitStructure.USART_StopBits = USART_StopBits_1;
  USART_InitStructure.USART_Parity = USART_Parity_No;
  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
  USART_InitStructure.USART_Mode = USART_Mode;
  
  USART_Init(UART5, &USART_InitStructure);

  if(USART_DMAReq == USART_DMAReq_Tx)
    USART_DMACmd(UART5, USART_DMAReq_Tx, ENABLE); /* Enabling the DMA request specified USART5 */
  if(USART_DMAReq == USART_DMAReq_Rx)
    USART_DMACmd(UART5, USART_DMAReq_Rx, ENABLE); /* Enabling the DMA request specified USART5 */
  if(USART_DMAReq == (USART_DMAReq_Tx|USART_DMAReq_Rx))
  {
    USART_DMACmd(UART5, USART_DMAReq_Tx, ENABLE); /* Enabling the DMA request specified USART5 */
    USART_DMACmd(UART5, USART_DMAReq_Rx, ENABLE); /* Enabling the DMA request specified USART5 */
  }

  if(USART_IT)
    USART_ITConfig(UART5, USART_IT, ENABLE);  /* Enabling interrupts specified USART5 */

  USART_Cmd(UART5, ENABLE);
}

/****************************************************
 * Function:    USART5_DMA_TX_Init
 * Description: USART5 DMA TX Configuration.
 * Input:       DMA_Memory0BaseAddr
 *              DMA_BufferSize
 * Output:
 * Return:
*****************************************************/
void USART5_DMA_TX_Init(uint32_t DMA_Memory0BaseAddr, uint16_t DMA_BufferSize)
{
  DMA_InitTypeDef DMA_InitStructure;

  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA1, ENABLE);  /* Enable DMA1 clock */
  
  DMA_DeInit(DMA1_Stream7);
  while(DMA_GetCmdStatus(DMA1_Stream7) != DISABLE);

  DMA_InitStructure.DMA_Channel = DMA_Channel_4;                          /* Channel selection */
  DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&UART5->DR;        /* Set the DMA source: peripheral data register address */
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
  
  DMA_Init(DMA1_Stream7, &DMA_InitStructure);
  DMA_Cmd(DMA1_Stream7, DISABLE);
  
  /* Starting a byte of data to make a DMA transmission completion flag */
  USART5_DMA_TX(1);
}

/****************************************************
 * Function:    USART5_DMA_RX_Init
 * Description: USART5 DMA RX Configuration.
 * Input:       DMA_Memory0BaseAddr
 *              DMA_BufferSize
 * Output:
 * Return:
*****************************************************/
void USART5_DMA_RX_Init(uint32_t DMA_Memory0BaseAddr, uint16_t DMA_BufferSize)
{
  DMA_InitTypeDef DMA_InitStructure;

  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA1, ENABLE);  /* Enable DMA1 clock */
  
  DMA_DeInit(DMA1_Stream0);
  while(DMA_GetCmdStatus(DMA1_Stream0) != DISABLE);

  DMA_InitStructure.DMA_Channel = DMA_Channel_4;                          /* Channel selection */
  DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&UART5->DR;        /* Set the DMA source: peripheral data register address */
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
  
  DMA_Init(DMA1_Stream0, &DMA_InitStructure);
  DMA_Cmd(DMA1_Stream0, ENABLE);
}

/****************************************************
 * Function:    USART5_DMA_TX
 * Description: Start USART5 DMA Send.
 * Input:       DMA_BufferSize
 * Output:
 * Return:
*****************************************************/
void USART5_DMA_TX(uint16_t DMA_BufferSize)
{
  DMA_ClearFlag(DMA1_Stream7, DMA_FLAG_TCIF7);  /* Clear Pending DMA channel x flag (DMA1 DMA1_Stream7 transfer completion flag) */
  DMA_Cmd(DMA1_Stream7, DISABLE);
  while(DMA_GetCmdStatus(DMA1_Stream7) != DISABLE);
  DMA_SetCurrDataCounter(DMA1_Stream7, DMA_BufferSize);
  DMA_Cmd(DMA1_Stream7, ENABLE);
}

/****************************************************
 * Function:    USART5_DMA_RX
 * Description: Start USART5 DMA Receive.
 * Input:       DMA_BufferSize
 * Output:
 * Return:
*****************************************************/
void USART5_DMA_RX(uint16_t DMA_BufferSize)
{
  DMA_ClearFlag(DMA1_Stream0, DMA_FLAG_TCIF0);  /* Clear Pending DMA channel x flag (DMA1 DMA1_Stream0 transfer completion flag) */
  DMA_Cmd(DMA1_Stream0, DISABLE);
  while(DMA_GetCmdStatus(DMA1_Stream0) != DISABLE);
  DMA_SetCurrDataCounter(DMA1_Stream0, DMA_BufferSize);
  DMA_Cmd(DMA1_Stream0, ENABLE);
}

/****************************************************
 * Function:    USART5_OUT
 * Description: USART5 Send.
 * Input:       Buffer
                BufferSize
 * Output:
 * Return:
*****************************************************/
void USART5_OUT(uint8_t* Buffer, uint16_t BufferSize)
{
  while(DMA_GetFlagStatus(DMA1_Stream7, DMA_FLAG_TCIF7)==0);

  memset(USART5_Buffer_Tx, 0, USART5_BufferSize_Tx);  /* Empty memory */
  memcpy(USART5_Buffer_Tx, Buffer, BufferSize);       /* Copy memory */
  
  USART5_DMA_TX(BufferSize);
}

/****************************************************
 * Function:    USART5_IN
 * Description: USART5 Receive.
 * Input:
 * Output:
 * Return:
*****************************************************/
void USART5_IN(void)
{
  memset(USART5_Buffer_Rx, 0, USART5_BufferSize_Rx);  /* Empty memory */
  
  USART5_DMA_RX(USART5_BufferSize_Rx);
}
