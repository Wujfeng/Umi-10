/****************************************************************
 * Copyright (C) 2016, Yagro, all right reserved.
 * File name:    USART3.h
 * Date:         2016.03.22
 * Description:  USART3 Driver
*****************************************************************/

#ifndef __USART3_H
#define __USART3_H

#include "stm32f4xx.h"
#include <string.h>


#define USART3_BufferSize_Tx  ((uint16_t)512)
#define USART3_BufferSize_Rx  ((uint16_t)512)


extern uint8_t USART3_Buffer_Tx[USART3_BufferSize_Tx];
extern uint8_t USART3_Buffer_Rx[USART3_BufferSize_Rx];


/***************** USART3 Configuration *********************/

#define RCC_AHB1Periph_USART3_Tx     RCC_AHB1Periph_GPIOB
#define RCC_AHB1Periph_USART3_Rx     RCC_AHB1Periph_GPIOB

#define GPIO_USART3_Tx               GPIOB
#define GPIO_USART3_Rx               GPIOB

#define GPIO_Pin_USART3_Tx           GPIO_Pin_10
#define GPIO_Pin_USART3_Rx           GPIO_Pin_11

#define GPIO_PinSource_USART3_Tx     GPIO_PinSource10
#define GPIO_PinSource_USART3_Rx     GPIO_PinSource11

/************************************************************/


void USART3_Init(uint32_t BaudRate, uint16_t USART_Mode, uint16_t USART_IT, uint16_t USART_DMAReq);
void USART3_DMA_TX_Init(uint32_t DMA_Memory0BaseAddr, uint16_t DMA_BufferSize);
void USART3_DMA_RX_Init(uint32_t DMA_Memory0BaseAddr, uint16_t DMA_BufferSize);
void USART3_DMA_TX(uint16_t DMA_BufferSize);
void USART3_DMA_RX(uint16_t DMA_BufferSize);
void USART3_OUT(uint8_t* Buffer, uint16_t BufferSize);
void USART3_IN(void);

#endif 	/* __USART3_H */
