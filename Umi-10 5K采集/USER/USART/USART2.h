/****************************************************************
 * Copyright (C) 2016, Yagro, all right reserved.
 * File name:    USART2.h
 * Date:         2016.03.22
 * Description:  USART2 Driver
*****************************************************************/

#ifndef __USART2_H
#define __USART2_H

#include "stm32f4xx.h"
#include <string.h>


#define USART2_BufferSize_Tx  ((uint16_t)512)
#define USART2_BufferSize_Rx  ((uint16_t)512)


extern uint8_t USART2_Buffer_Tx[USART2_BufferSize_Tx];
extern uint8_t USART2_Buffer_Rx[USART2_BufferSize_Rx];


/***************** USART2 Configuration *********************/

#define RCC_AHB1Periph_USART2_Tx     RCC_AHB1Periph_GPIOA
#define RCC_AHB1Periph_USART2_Rx     RCC_AHB1Periph_GPIOA

#define GPIO_USART2_Tx               GPIOA
#define GPIO_USART2_Rx               GPIOA

#define GPIO_Pin_USART2_Tx           GPIO_Pin_2
#define GPIO_Pin_USART2_Rx           GPIO_Pin_3

#define GPIO_PinSource_USART2_Tx     GPIO_PinSource2
#define GPIO_PinSource_USART2_Rx     GPIO_PinSource3

/************************************************************/


void USART2_Init(uint32_t BaudRate, uint16_t USART_Mode, uint16_t USART_IT, uint16_t USART_DMAReq);
void USART2_DMA_TX_Init(uint32_t DMA_Memory0BaseAddr, uint16_t DMA_BufferSize);
void USART2_DMA_RX_Init(uint32_t DMA_Memory0BaseAddr, uint16_t DMA_BufferSize);
void USART2_DMA_TX(uint16_t DMA_BufferSize);
void USART2_DMA_RX(uint16_t DMA_BufferSize);
void USART2_OUT(uint8_t* Buffer, uint16_t BufferSize);
void USART2_IN(void);

#endif 	/* __USART2_H */
