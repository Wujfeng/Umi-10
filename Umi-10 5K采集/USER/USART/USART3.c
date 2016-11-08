/****************************************************************
 * Copyright (C) 2016, Yagro, all right reserved.
 * File name:    USART3.c
 * Date:         2016.03.22
 * Description:  USART3 Driver
*****************************************************************/


#include "USART3.h"


uint8_t USART3_Buffer_Tx[USART3_BufferSize_Tx];
uint8_t USART3_Buffer_Rx[USART3_BufferSize_Rx];


/****************************************************
 * Function:    USART3_Init
 * Description: USART3 Configuration.
 * Input:       BaudRate
 *              USART_Mode
 *              USART_IT
 *              USART_DMAReq
 * Output:
 * Return:
*****************************************************/
void USART3_Init(uint32_t BaudRate, uint16_t USART_Mode, uint16_t USART_IT, uint16_t USART_DMAReq)
{
  GPIO_InitTypeDef GPIO_InitStructure;
  USART_InitTypeDef USART_InitStructure;

  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_USART3_Tx, ENABLE);  /* Enable USART3_Tx GPIO clock */
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_USART3_Rx, ENABLE);  /* Enable USART3_Rx GPIO clock */
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);     /* Enable USART3 clock */

  GPIO_PinAFConfig(GPIO_USART3_Tx, GPIO_PinSource_USART3_Tx, GPIO_AF_USART3); /* Connect GPIOxn to USART3_Tx */
  GPIO_PinAFConfig(GPIO_USART3_Rx, GPIO_PinSource_USART3_Rx, GPIO_AF_USART3); /* Connect GPIOxn to USART3_Rx */

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_USART3_Tx;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
  GPIO_Init(GPIO_USART3_Tx, &GPIO_InitStructure);
    
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_USART3_Rx;
  GPIO_Init(GPIO_USART3_Rx, &GPIO_InitStructure);

  USART_InitStructure.USART_BaudRate = BaudRate;
  USART_InitStructure.USART_WordLength = USART_WordLength_8b;
  USART_InitStructure.USART_StopBits = USART_StopBits_1;
  USART_InitStructure.USART_Parity = USART_Parity_No;
  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
  USART_InitStructure.USART_Mode = USART_Mode;

  USART_Init(USART3, &USART_InitStructure);

  if(USART_DMAReq == USART_DMAReq_Tx)
    USART_DMACmd(USART3, USART_DMAReq_Tx, ENABLE);  /* Enabling the DMA request specified USART3 */
  if(USART_DMAReq == USART_DMAReq_Rx)
    USART_DMACmd(USART3, USART_DMAReq_Rx, ENABLE);  /* Enabling the DMA request specified USART3 */
  if(USART_DMAReq == (USART_DMAReq_Tx|USART_DMAReq_Rx))
  {
    USART_DMACmd(USART3, USART_DMAReq_Tx, ENABLE);  /* Enabling the DMA request specified USART3 */
    USART_DMACmd(USART3, USART_DMAReq_Rx, ENABLE);  /* Enabling the DMA request specified USART3 */
  }

  if(USART_IT)
    USART_ITConfig(USART3, USART_IT, ENABLE); /* Enabling interrupts specified USART3 */

  USART_Cmd(USART3, ENABLE);
}

/****************************************************
 * Function:    USART3_DMA_TX_Init
 * Description: USART3 DMA TX Configuration.
 * Input:       DMA_Memory0BaseAddr
 *              DMA_BufferSize
 * Output:
 * Return:
*****************************************************/
void USART3_DMA_TX_Init(uint32_t DMA_Memory0BaseAddr, uint16_t DMA_BufferSize)
{
  DMA_InitTypeDef DMA_InitStructure;

  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA1, ENABLE);  /* Enable DMA1 clock */

  DMA_DeInit(DMA1_Stream3);
  while(DMA_GetCmdStatus(DMA1_Stream3) != DISABLE);

  DMA_InitStructure.DMA_Channel = DMA_Channel_4;                          /* Channel selection */
  DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&USART3->DR;       /* Set the DMA source: peripheral data register address */
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

  DMA_Init(DMA1_Stream3, &DMA_InitStructure);
  DMA_Cmd(DMA1_Stream3, DISABLE);
  
  /* Starting a byte of data to make a DMA transmission completion flag */
  USART3_DMA_TX(1);
}

/****************************************************
 * Function:    USART3_DMA_RX_Init
 * Description: USART3 DMA RX Configuration.
 * Input:       DMA_Memory0BaseAddr
 *              DMA_BufferSize
 * Output:
 * Return:
*****************************************************/
void USART3_DMA_RX_Init(uint32_t DMA_Memory0BaseAddr, uint16_t DMA_BufferSize)
{
  DMA_InitTypeDef DMA_InitStructure;

  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA1, ENABLE);  /* Enable DMA1 clock */

  DMA_DeInit(DMA1_Stream1);
  while(DMA_GetCmdStatus(DMA1_Stream1) != DISABLE);

  DMA_InitStructure.DMA_Channel = DMA_Channel_4;                          /* Channel selection */
  DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&USART3->DR;       /* Set the DMA source: peripheral data register address */
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

  DMA_Init(DMA1_Stream1, &DMA_InitStructure);
  DMA_Cmd(DMA1_Stream1, ENABLE);
}

/****************************************************
 * Function:    USART3_DMA_TX
 * Description: Start USART3 DMA Send.
 * Input:       DMA_BufferSize
 * Output:
 * Return:
*****************************************************/
void USART3_DMA_TX(uint16_t DMA_BufferSize)
{
  DMA_ClearFlag(DMA1_Stream3, DMA_FLAG_TCIF3);  /* Clear Pending DMA channel x flag (DMA1 DMA1_Stream3 transfer completion flag) */
  DMA_Cmd(DMA1_Stream3, DISABLE);
  while(DMA_GetCmdStatus(DMA1_Stream3) != DISABLE);
  DMA_SetCurrDataCounter(DMA1_Stream3, DMA_BufferSize);
  DMA_Cmd(DMA1_Stream3, ENABLE);
}

/****************************************************
 * Function:    USART3_DMA_RX
 * Description: Start USART3 DMA Receive.
 * Input:       DMA_BufferSize
 * Output:
 * Return:
*****************************************************/
void USART3_DMA_RX(uint16_t DMA_BufferSize)
{
  DMA_ClearFlag(DMA1_Stream1, DMA_FLAG_TCIF1);  /* Clear Pending DMA channel x flag (DMA1 DMA1_Stream1 transfer completion flag) */
  DMA_Cmd(DMA1_Stream1, DISABLE);
  while(DMA_GetCmdStatus(DMA1_Stream1) != DISABLE);
  DMA_SetCurrDataCounter(DMA1_Stream1, DMA_BufferSize);
  DMA_Cmd(DMA1_Stream1, ENABLE);
}

/****************************************************
 * Function:    USART3_OUT
 * Description: USART3 Send.
 * Input:       Buffer
                BufferSize
 * Output:
 * Return:
*****************************************************/
void USART3_OUT(uint8_t* Buffer, uint16_t BufferSize)
{
  while(DMA_GetFlagStatus(DMA1_Stream3, DMA_FLAG_TCIF3)==0);

  memset(USART3_Buffer_Tx, 0, USART3_BufferSize_Tx);  /* Empty memory */
  memcpy(USART3_Buffer_Tx, Buffer, BufferSize);       /* Copy memory */
  
  USART3_DMA_TX(BufferSize);
}

/****************************************************
 * Function:    USART3_IN
 * Description: USART3 Receive.
 * Input:
 * Output:
 * Return:
*****************************************************/
void USART3_IN(void)
{
  memset(USART3_Buffer_Rx, 0, USART3_BufferSize_Rx);  /* Empty memory */
  
  USART3_DMA_RX(USART3_BufferSize_Rx);
}
