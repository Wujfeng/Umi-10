/****************************************************************
 * Copyright (C) 2016, Yagro, all right reserved.
 * File name:    SPI2.h
 * Date:         2016.03.22
 * Description:  SPI2 Driver
*****************************************************************/

#ifndef __SPI2_H
#define __SPI2_H

#include "stm32f4xx.h"


#define SPI2_BufferSize_Tx  ((uint16_t)2)
#define SPI2_BufferSize_Rx  ((uint16_t)2)


extern uint16_t SPI2_Buffer_Tx[SPI2_BufferSize_Tx];
extern uint16_t SPI2_Buffer_Rx[SPI2_BufferSize_Rx];


/***************** SPI2 Configuration *********************/

#define RCC_AHB1Periph_SPI2_SCK      RCC_AHB1Periph_GPIOB
#define RCC_AHB1Periph_SPI2_MISO     RCC_AHB1Periph_GPIOB
#define RCC_AHB1Periph_SPI2_MOSI     RCC_AHB1Periph_GPIOB
#define RCC_AHB1Periph_SPI2_CS1      RCC_AHB1Periph_GPIOB
#define RCC_AHB1Periph_SPI2_CS2      RCC_AHB1Periph_GPIOB

#define GPIO_SPI2_SCK                GPIOB
#define GPIO_SPI2_MISO               GPIOB
#define GPIO_SPI2_MOSI               GPIOB
#define GPIO_SPI2_CS1                GPIOB
#define GPIO_SPI2_CS2                GPIOB

#define GPIO_Pin_SPI2_SCK            GPIO_Pin_13
#define GPIO_Pin_SPI2_MISO           GPIO_Pin_14
#define GPIO_Pin_SPI2_MOSI           GPIO_Pin_15
#define GPIO_Pin_SPI2_CS1            GPIO_Pin_12
#define GPIO_Pin_SPI2_CS2            GPIO_Pin_1

#define GPIO_PinSource_SPI2_SCK      GPIO_PinSource13
#define GPIO_PinSource_SPI2_MISO     GPIO_PinSource14
#define GPIO_PinSource_SPI2_MOSI     GPIO_PinSource15

#define SPI2_CS1_LOW()               GPIO_ResetBits(GPIOB, GPIO_Pin_12)
#define SPI2_CS1_HIGH()              GPIO_SetBits(GPIOB, GPIO_Pin_12)

#define SPI2_CS2_LOW()               GPIO_ResetBits(GPIOB, GPIO_Pin_1)
#define SPI2_CS2_HIGH()              GPIO_SetBits(GPIOB, GPIO_Pin_1)

/**********************************************************/


void SPI2_Init(void);
void SPI2_DMA_TX_Init(uint32_t DMA_Memory0BaseAddr, uint16_t DMA_BufferSize);
void SPI2_DMA_RX_Init(uint32_t DMA_Memory0BaseAddr, uint16_t DMA_BufferSize);
void SPI2_DMA_TX(uint16_t DMA_BufferSize);
void SPI2_DMA_RX(uint16_t DMA_BufferSize);
uint16_t SPI2_SendHalfWord(uint16_t HalfWord);

#endif	/* __SPI2_H */
