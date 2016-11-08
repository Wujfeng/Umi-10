/****************************************************************
 * Copyright (C) 2016, Yagro, all right reserved.
 * File name:    SPI1.c
 * Date:         2016.03.22
 * Description:  SPI1 Driver
*****************************************************************/


#include "SPI1.h"


uint16_t SPI1_Buffer_Tx[SPI1_BufferSize_Tx];
uint16_t SPI1_Buffer_Rx[SPI1_BufferSize_Rx];


/****************************************************
 * Function:    SPI1_Init
 * Description: SPI1 Configuration.
 * Input:
 * Output:
 * Return:
*****************************************************/
void SPI1_Init(void)
{
  SPI_InitTypeDef  SPI_InitStructure;
  GPIO_InitTypeDef GPIO_InitStructure;

  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_SPI1_SCK, ENABLE);   /* Enable SPI1_SCK GPIO clock */
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_SPI1_MISO, ENABLE);  /* Enable SPI1_MISO GPIO clock */
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_SPI1_MOSI, ENABLE);	 /* Enable SPI1_MOSI GPIO clock */
  
#ifdef RCC_AHB1Periph_SPI1_CS1
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_SPI1_CS1, ENABLE);   /* Enable SPI1_CS1 GPIO clock */
#endif
  
#ifdef RCC_AHB1Periph_SPI1_CS2
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_SPI1_CS2, ENABLE);   /* Enable SPI1_CS2 GPIO clock */
#endif

  RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE);       /* Enable SPI1 clock */

  GPIO_PinAFConfig(GPIO_SPI1_SCK, GPIO_PinSource_SPI1_SCK, GPIO_AF_SPI1);   /* Connect GPIOxn to SPI1_SCK */
  GPIO_PinAFConfig(GPIO_SPI1_MISO, GPIO_PinSource_SPI1_MISO, GPIO_AF_SPI1); /* Connect GPIOxn to SPI1_MISO */
  GPIO_PinAFConfig(GPIO_SPI1_MOSI, GPIO_PinSource_SPI1_MOSI, GPIO_AF_SPI1); /* Connect GPIOxn to SPI1_MOSI */

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_SPI1_SCK;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
  GPIO_Init(GPIO_SPI1_SCK, &GPIO_InitStructure);
  
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_SPI1_MISO;
  GPIO_Init(GPIO_SPI1_MISO, &GPIO_InitStructure);
  
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_SPI1_MOSI;
  GPIO_Init(GPIO_SPI1_MOSI, &GPIO_InitStructure);

#ifdef RCC_AHB1Periph_SPI1_CS1
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_SPI1_CS1;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_Init(GPIO_SPI1_CS1, &GPIO_InitStructure);
#endif

#ifdef RCC_AHB1Periph_SPI1_CS2
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_SPI1_CS2;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_Init(GPIO_SPI1_CS2, &GPIO_InitStructure);
#endif

#ifdef RCC_AHB1Periph_SPI1_CS1
  SPI1_CS1_HIGH();
#endif

#ifdef RCC_AHB1Periph_SPI1_CS2
  SPI1_CS2_HIGH();
#endif

  SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;  /* Two-line bidirectional full-duplex */
  SPI_InitStructure.SPI_Mode = SPI_Mode_Master;                       /* Master SPI Mode */
  SPI_InitStructure.SPI_DataSize = SPI_DataSize_16b;                  /* 16-bit data frame */
  SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;
  SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;
  SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
  SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_16; /* Baud rate prescaler is 16 (5.25MHz) */
  SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;                  /* Data transfer takes place from the MSB */
  SPI_InitStructure.SPI_CRCPolynomial = 7;
  SPI_Init(SPI1, &SPI_InitStructure);

  SPI_DMACmd(SPI1, SPI_DMAReq_Tx, ENABLE);
  SPI_DMACmd(SPI1, SPI_DMAReq_Rx, ENABLE);

  SPI_Cmd(SPI1, ENABLE);
}

/****************************************************
 * Function:    SPI1_DMA_TX_Init
 * Description: SPI1 DMA TX Configuration.
 * Input:       DMA_Memory0BaseAddr
 *              DMA_BufferSize
 * Output:
 * Return:
*****************************************************/
void SPI1_DMA_TX_Init(uint32_t DMA_Memory0BaseAddr, uint16_t DMA_BufferSize)
{
  DMA_InitTypeDef DMA_InitStructure;

  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2, ENABLE);  /* Enable DMA2 clock */

  DMA_DeInit(DMA2_Stream3);
  while(DMA_GetCmdStatus(DMA2_Stream3) != DISABLE);

  DMA_InitStructure.DMA_Channel = DMA_Channel_3;                              /* Channel selection */
  DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&SPI1->DR;             /* Set the DMA source: peripheral data register address */
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

  DMA_Init(DMA2_Stream3, &DMA_InitStructure);

  DMA_Cmd(DMA2_Stream3, DISABLE);
}

/****************************************************
 * Function:    SPI1_DMA_RX_Init
 * Description: SPI1 DMA RX Configuration.
 * Input:       DMA_Memory0BaseAddr
 *              DMA_BufferSize
 * Output:
 * Return:
*****************************************************/
void SPI1_DMA_RX_Init(uint32_t DMA_Memory0BaseAddr, uint16_t DMA_BufferSize)
{
  DMA_InitTypeDef DMA_InitStructure;

  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2, ENABLE);  /* Enable DMA2 clock */

  DMA_DeInit(DMA2_Stream0);
  while(DMA_GetCmdStatus(DMA2_Stream0) != DISABLE);

  DMA_InitStructure.DMA_Channel = DMA_Channel_3;                              /* Channel selection */
  DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&SPI1->DR;             /* Set the DMA source: peripheral data register address */
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

  DMA_Init(DMA2_Stream0, &DMA_InitStructure);

  DMA_ITConfig(DMA2_Stream0, DMA_IT_TC, ENABLE);

  DMA_Cmd(DMA2_Stream0, ENABLE);
}

/****************************************************
 * Function:    SPI1_DMA_TX
 * Description: Start SPI1 DMA Send.
 * Input:       DMA_BufferSize
 * Output:
 * Return:
*****************************************************/
void SPI1_DMA_TX(uint16_t DMA_BufferSize)
{
  DMA_ClearFlag(DMA2_Stream3, DMA_FLAG_TCIF3);  /* Clear Pending DMA channel x flag (DMA2 Stream3 transfer completion flag) */
  DMA_Cmd(DMA2_Stream3, DISABLE);
  while(DMA_GetCmdStatus(DMA2_Stream3) != DISABLE);
  DMA_SetCurrDataCounter(DMA2_Stream3, DMA_BufferSize);
  DMA_Cmd(DMA2_Stream3, ENABLE);
}

/****************************************************
 * Function:    SPI1_DMA_RX
 * Description: Start SPI1 DMA Receive.
 * Input:       DMA_BufferSize
 * Output:
 * Return:
*****************************************************/
void SPI1_DMA_RX(uint16_t DMA_BufferSize)
{
  DMA_ClearFlag(DMA2_Stream0, DMA_FLAG_TCIF0);  /* Clear Pending DMA channel x flag (DMA2 Stream0 transfer completion flag) */
  DMA_Cmd(DMA2_Stream0, DISABLE);
  while(DMA_GetCmdStatus(DMA2_Stream0) != DISABLE);
  DMA_SetCurrDataCounter(DMA2_Stream0, DMA_BufferSize);
  DMA_Cmd(DMA2_Stream0, ENABLE);
}

/****************************************************
 * Function:    SPI1_SendHalfWord
 * Description: Read Write two bytes.
 * Input:       HalfWord
 * Output:
 * Return:      Read Data
*****************************************************/
uint16_t SPI1_SendHalfWord(uint16_t HalfWord)
{
  /* Loop while DR register in not emplty */
  while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET);

  /* Send Half Word through the SPI1 peripheral */
  SPI_I2S_SendData(SPI1, HalfWord);

  /* Wait to receive a Half Word */
  while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == RESET);

  /* Return the Half Word read from the SPI bus */
  return SPI_I2S_ReceiveData(SPI1);
}
