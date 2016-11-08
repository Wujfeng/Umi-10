/****************************************************************
 * Copyright (C) 2016, Yagro, all right reserved.
 * File name:    SPI1.h
 * Date:         2016.03.22
 * Description:  SPI1 Driver
*****************************************************************/

#ifndef __SPI1_H
#define __SPI1_H

#include "stm32f4xx.h"


#define SPI1_BufferSize_Tx  ((uint16_t)2)
#define SPI1_BufferSize_Rx  ((uint16_t)2)


extern uint16_t SPI1_Buffer_Tx[SPI1_BufferSize_Tx];
extern uint16_t SPI1_Buffer_Rx[SPI1_BufferSize_Rx];


/***************** SPI1 Configuration *********************/

#define RCC_AHB1Periph_SPI1_SCK      RCC_AHB1Periph_GPIOA
#define RCC_AHB1Periph_SPI1_MISO     RCC_AHB1Periph_GPIOA
#define RCC_AHB1Periph_SPI1_MOSI     RCC_AHB1Periph_GPIOA
#define RCC_AHB1Periph_SPI1_CS1      RCC_AHB1Periph_GPIOA
#define RCC_AHB1Periph_SPI1_CS2      RCC_AHB1Periph_GPIOB

#define GPIO_SPI1_SCK                GPIOA
#define GPIO_SPI1_MISO               GPIOA
#define GPIO_SPI1_MOSI               GPIOA
#define GPIO_SPI1_CS1                GPIOA
#define GPIO_SPI1_CS2                GPIOB

#define GPIO_Pin_SPI1_SCK            GPIO_Pin_5
#define GPIO_Pin_SPI1_MISO           GPIO_Pin_6
#define GPIO_Pin_SPI1_MOSI           GPIO_Pin_7
#define GPIO_Pin_SPI1_CS1            GPIO_Pin_4
#define GPIO_Pin_SPI1_CS2            GPIO_Pin_0

#define GPIO_PinSource_SPI1_SCK      GPIO_PinSource5
#define GPIO_PinSource_SPI1_MISO     GPIO_PinSource6
#define GPIO_PinSource_SPI1_MOSI     GPIO_PinSource7

#define SPI1_CS1_LOW()               GPIO_ResetBits(GPIOA, GPIO_Pin_4)
#define SPI1_CS1_HIGH()              GPIO_SetBits(GPIOA, GPIO_Pin_4)

#define SPI1_CS2_LOW()               GPIO_ResetBits(GPIOB, GPIO_Pin_0)
#define SPI1_CS2_HIGH()              GPIO_SetBits(GPIOB, GPIO_Pin_0)

/**********************************************************/


void SPI1_Init(void);
void SPI1_DMA_TX_Init(uint32_t DMA_Memory0BaseAddr, uint16_t DMA_BufferSize);
void SPI1_DMA_RX_Init(uint32_t DMA_Memory0BaseAddr, uint16_t DMA_BufferSize);
void SPI1_DMA_TX(uint16_t DMA_BufferSize);
void SPI1_DMA_RX(uint16_t DMA_BufferSize);
uint16_t SPI1_SendHalfWord(uint16_t HalfWord);

#endif	/* __SPI1_H */
