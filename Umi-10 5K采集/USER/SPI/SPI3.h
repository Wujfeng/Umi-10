/****************************************************************
 * Copyright (C) 2016, Yagro, all right reserved.
 * File name:    SPI3.h
 * Date:         2016.03.22
 * Description:  SPI3 Driver
*****************************************************************/

#ifndef __SPI3_H
#define __SPI3_H

#include "stm32f4xx.h"


#define SPI3_BufferSize_Tx  ((uint16_t)2)
#define SPI3_BufferSize_Rx  ((uint16_t)2)


extern uint16_t SPI3_Buffer_Tx[SPI3_BufferSize_Tx];
extern uint16_t SPI3_Buffer_Rx[SPI3_BufferSize_Rx];


/***************** SPI3 Configuration *********************/

#define RCC_AHB1Periph_SPI3_SCK      RCC_AHB1Periph_GPIOB
#define RCC_AHB1Periph_SPI3_MISO     RCC_AHB1Periph_GPIOB
#define RCC_AHB1Periph_SPI3_MOSI     RCC_AHB1Periph_GPIOB
#define RCC_AHB1Periph_SPI3_CS1      RCC_AHB1Periph_GPIOA

#define GPIO_SPI3_SCK                GPIOB
#define GPIO_SPI3_MISO               GPIOB
#define GPIO_SPI3_MOSI               GPIOB
#define GPIO_SPI3_CS1                GPIOA

#define GPIO_Pin_SPI3_SCK            GPIO_Pin_3
#define GPIO_Pin_SPI3_MISO           GPIO_Pin_4
#define GPIO_Pin_SPI3_MOSI           GPIO_Pin_5
#define GPIO_Pin_SPI3_CS1            GPIO_Pin_15

#define GPIO_PinSource_SPI3_SCK      GPIO_PinSource3
#define GPIO_PinSource_SPI3_MISO     GPIO_PinSource4
#define GPIO_PinSource_SPI3_MOSI     GPIO_PinSource5

#define SPI3_CS1_LOW()               GPIO_ResetBits(GPIOA, GPIO_Pin_15)
#define SPI3_CS1_HIGH()              GPIO_SetBits(GPIOA, GPIO_Pin_15)

/**********************************************************/


void SPI3_Init(void);
void SPI3_DMA_TX_Init(uint32_t DMA_Memory0BaseAddr, uint16_t DMA_BufferSize);
void SPI3_DMA_RX_Init(uint32_t DMA_Memory0BaseAddr, uint16_t DMA_BufferSize);
void SPI3_DMA_TX(uint16_t DMA_BufferSize);
void SPI3_DMA_RX(uint16_t DMA_BufferSize);
uint16_t SPI3_SendHalfWord(uint16_t HalfWord);

#endif	/* __SPI2_H */
