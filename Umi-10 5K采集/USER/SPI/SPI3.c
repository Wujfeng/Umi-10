/****************************************************************
 * Copyright (C) 2016, Yagro, all right reserved.
 * File name:    SPI3.c
 * Date:         2016.03.22
 * Description:  SPI3 Driver
*****************************************************************/


#include "SPI3.h"


uint16_t SPI3_Buffer_Tx[SPI3_BufferSize_Tx];
uint16_t SPI3_Buffer_Rx[SPI3_BufferSize_Rx];


/****************************************************
 * Function:    SPI3_Init
 * Description: SPI3 Configuration.
 * Input:
 * Output:
 * Return:
*****************************************************/
void SPI3_Init(void)
{
  SPI_InitTypeDef  SPI_InitStructure;
  GPIO_InitTypeDef GPIO_InitStructure;

  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_SPI3_SCK, ENABLE);   /* Enable SPI3_SCK GPIO clock */
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_SPI3_MISO, ENABLE);  /* Enable SPI3_MISO GPIO clock */
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_SPI3_MOSI, ENABLE);	 /* Enable SPI3_MOSI GPIO clock */
  
#ifdef RCC_AHB1Periph_SPI3_CS1
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_SPI3_CS1, ENABLE);   /* Enable SPI3_CS1 GPIO clock */
#endif

  RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI3, ENABLE);       /* Enable SPI3 clock */

  GPIO_PinAFConfig(GPIO_SPI3_SCK, GPIO_PinSource_SPI3_SCK, GPIO_AF_SPI3);   /* Connect GPIOxn to SPI3_SCK */
  GPIO_PinAFConfig(GPIO_SPI3_MISO, GPIO_PinSource_SPI3_MISO, GPIO_AF_SPI3); /* Connect GPIOxn to SPI3_MISO */
  GPIO_PinAFConfig(GPIO_SPI3_MOSI, GPIO_PinSource_SPI3_MOSI, GPIO_AF_SPI3); /* Connect GPIOxn to SPI3_MOSI */

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_SPI3_SCK;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
  GPIO_Init(GPIO_SPI3_SCK, &GPIO_InitStructure);
  
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_SPI3_MISO;
  GPIO_Init(GPIO_SPI3_MISO, &GPIO_InitStructure);
  
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_SPI3_MOSI;
  GPIO_Init(GPIO_SPI3_MOSI, &GPIO_InitStructure);

#ifdef RCC_AHB1Periph_SPI3_CS1
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_SPI3_CS1;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_Init(GPIO_SPI3_CS1, &GPIO_InitStructure);
#endif

#ifdef RCC_AHB1Periph_SPI3_CS1
  SPI3_CS1_HIGH();
#endif

  SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;  /* Two-line bidirectional full-duplex */
  SPI_InitStructure.SPI_Mode = SPI_Mode_Master;                       /* Master SPI Mode */
  SPI_InitStructure.SPI_DataSize = SPI_DataSize_16b;                  /* 16-bit data frame */
  SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;               
  SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;
  SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
  SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_8;  /* Baud rate prescaler is 8	(5.25MHz) */
  SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;                  /* Data transfer takes place from the MSB */
  SPI_InitStructure.SPI_CRCPolynomial = 7;
  SPI_Init(SPI3, &SPI_InitStructure);

  SPI_DMACmd(SPI3, SPI_DMAReq_Tx, ENABLE);
  SPI_DMACmd(SPI3, SPI_DMAReq_Rx, ENABLE);

  SPI_Cmd(SPI3, ENABLE);
}

/****************************************************
 * Function:    SPI3_DMA_TX_Init
 * Description: SPI3 DMA TX Configuration.
 * Input:       DMA_Memory0BaseAddr
 *              DMA_BufferSize
 * Output:
 * Return:
*****************************************************/
void SPI3_DMA_TX_Init(uint32_t DMA_Memory0BaseAddr, uint16_t DMA_BufferSize)
{
  DMA_InitTypeDef DMA_InitStructure;

  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA1, ENABLE);  /* Enable DMA1 clock */

  DMA_DeInit(DMA1_Stream5);
  while(DMA_GetCmdStatus(DMA1_Stream5) != DISABLE);

  DMA_InitStructure.DMA_Channel = DMA_Channel_0;                              /* Channel selection */
  DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&SPI3->DR;             /* Set the DMA source: peripheral data register address */
  DMA_InitStructure.DMA_Memory0BaseAddr = DMA_Memory0BaseAddr;                /* Memory address */
  DMA_InitStructure.DMA_DIR = DMA_DIR_MemoryToPeripheral;                     /* Data Memory to Peripheral */
  DMA_InitStructure.DMA_BufferSize = DMA_BufferSize;                          /* Specifies the DMA channel DMA buffer size */
  DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;            /* Peripheral address does not increase */
  DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;                     /* Memory address increment */
  DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord; /* Peripheral data width of 16 bit */
  DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;         /* Memory data width of 16 bit */
  DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;                               /* Use Normal mode */
  DMA_InitStructure.DMA_Priority = DMA_Priority_High;                         /* High priority */
  DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;
  DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_Full;
  DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;                 /* A single transmission burst memory */
  DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;         /* Peripherals single transmission burst */

  DMA_Init(DMA1_Stream5, &DMA_InitStructure);

  DMA_Cmd(DMA1_Stream5, DISABLE);
}

/****************************************************
 * Function:    SPI3_DMA_RX_Init
 * Description: SPI3 DMA RX Configuration.
 * Input:       DMA_Memory0BaseAddr
 *              DMA_BufferSize
 * Output:
 * Return:
*****************************************************/
void SPI3_DMA_RX_Init(uint32_t DMA_Memory0BaseAddr, uint16_t DMA_BufferSize)
{
  DMA_InitTypeDef DMA_InitStructure;

  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA1, ENABLE);  /* Enable DMA1 clock */

  DMA_DeInit(DMA1_Stream0);
  while(DMA_GetCmdStatus(DMA1_Stream0) != DISABLE);

  DMA_InitStructure.DMA_Channel = DMA_Channel_0;                              /* Channel selection */
  DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&SPI3->DR;             /* Set the DMA source: peripheral data register address */
  DMA_InitStructure.DMA_Memory0BaseAddr = DMA_Memory0BaseAddr;                /* Memory address */
  DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralToMemory;                     /* Data Peripheral to Memory */
  DMA_InitStructure.DMA_BufferSize = DMA_BufferSize;                          /* Specifies the DMA channel DMA buffer size */
  DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;            /* Peripheral address does not increase */
  DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;                     /* Memory address increment */
  DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord; /* Peripheral data width of 16 bit */
  DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;         /* Memory data width of 16 bit */
  DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;                             /* Use Circular mode */
  DMA_InitStructure.DMA_Priority = DMA_Priority_High;                         /* High priority */
  DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;
  DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_Full;
  DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;                 /* A single transmission burst memory */
  DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;         /* Peripherals single transmission burst */

  DMA_Init(DMA1_Stream0, &DMA_InitStructure);

  DMA_ITConfig(DMA1_Stream0, DMA_IT_TC, ENABLE);

  DMA_Cmd(DMA1_Stream0, ENABLE);
}

/****************************************************
 * Function:    SPI3_DMA_TX
 * Description: Start SPI3 DMA Send.
 * Input:       DMA_BufferSize
 * Output:
 * Return:
*****************************************************/
void SPI3_DMA_TX(uint16_t DMA_BufferSize)
{
  DMA_ClearFlag(DMA1_Stream5, DMA_FLAG_TCIF5);  /* Clear Pending DMA channel x flag (DMA1 Stream5 transfer completion flag) */
  DMA_Cmd(DMA1_Stream5, DISABLE);
  while(DMA_GetCmdStatus(DMA1_Stream5) != DISABLE);
  DMA_SetCurrDataCounter(DMA1_Stream5, DMA_BufferSize);
  DMA_Cmd(DMA1_Stream5, ENABLE);
}

/****************************************************
 * Function:    SPI3_DMA_RX
 * Description: Start SPI3 DMA Receive.
 * Input:       DMA_BufferSize
 * Output:
 * Return:
*****************************************************/
void SPI3_DMA_RX(uint16_t DMA_BufferSize)
{
  DMA_ClearFlag(DMA1_Stream0, DMA_FLAG_TCIF0);  /* Clear Pending DMA channel x flag (DMA1 Stream0 transfer completion flag) */
  DMA_Cmd(DMA1_Stream0, DISABLE);
  while(DMA_GetCmdStatus(DMA1_Stream0) != DISABLE);
  DMA_SetCurrDataCounter(DMA1_Stream0, DMA_BufferSize);
  DMA_Cmd(DMA1_Stream0, ENABLE);
}

/****************************************************
 * Function:    SPI3_SendHalfWord
 * Description: Read Write two bytes.
 * Input:       HalfWord
 * Output:
 * Return:      Read Data
*****************************************************/
uint16_t SPI3_SendHalfWord(uint16_t HalfWord)
{
  /* Loop while DR register in not emplty */
  while (SPI_I2S_GetFlagStatus(SPI3, SPI_I2S_FLAG_TXE) == RESET);

  /* Send Half Word through the SPI3 peripheral */
  SPI_I2S_SendData(SPI3, HalfWord);

  /* Wait to receive a Half Word */
  while (SPI_I2S_GetFlagStatus(SPI3, SPI_I2S_FLAG_RXNE) == RESET);

  /* Return the Half Word read from the SPI bus */
  return SPI_I2S_ReceiveData(SPI3);
}


