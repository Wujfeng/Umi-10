/****************************************************************
 * Copyright (C) 2016, Yagro, all right reserved.
 * File name:    USART1.h
 * Date:         2016.03.22
 * Description:  USART1 Driver
*****************************************************************/

#ifndef __USART1_H
#define __USART1_H

#include "stm32f4xx.h"
#include <string.h>


#define USART1_BufferSize_Tx  ((uint16_t)100)
#define USART1_BufferSize_Rx  ((uint16_t)2048)


extern uint8_t USART1_Buffer_Tx[USART1_BufferSize_Tx];
extern uint8_t USART1_Buffer_Rx[USART1_BufferSize_Rx];


/***************** USART1 Configuration *********************/

#define RCC_AHB1Periph_USART1_Tx     RCC_AHB1Periph_GPIOA
#define RCC_AHB1Periph_USART1_Rx     RCC_AHB1Periph_GPIOA

#define GPIO_USART1_Tx               GPIOA
#define GPIO_USART1_Rx               GPIOA

#define GPIO_Pin_USART1_Tx           GPIO_Pin_9
#define GPIO_Pin_USART1_Rx           GPIO_Pin_10

#define GPIO_PinSource_USART1_Tx     GPIO_PinSource9
#define GPIO_PinSource_USART1_Rx     GPIO_PinSource10

/************************************************************/


void USART1_Init(uint32_t BaudRate, uint16_t USART_Mode, uint16_t USART_IT, uint16_t USART_DMAReq);
void USART1_DMA_TX_Init(uint32_t DMA_Memory0BaseAddr, uint16_t DMA_BufferSize);
void USART1_DMA_RX_Init(uint32_t DMA_Memory0BaseAddr, uint16_t DMA_BufferSize);
void USART1_DMA_TX(uint16_t DMA_BufferSize);
void USART1_DMA_RX(uint16_t DMA_BufferSize);
void USART1_OUT(uint8_t* Buffer, uint16_t BufferSize);
void USART1_IN(void);

#endif 	/* __USART1_H */
