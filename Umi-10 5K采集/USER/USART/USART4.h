/****************************************************************
 * Copyright (C) 2016, Yagro, all right reserved.
 * File name:    USART4.h
 * Date:         2016.03.22
 * Description:  USART4 Driver
*****************************************************************/

#ifndef __USART4_H
#define __USART4_H

#include "stm32f4xx.h"
#include <string.h>


#define USART4_BufferSize_Tx  ((uint16_t)512)
#define USART4_BufferSize_Rx  ((uint16_t)512)


extern uint8_t USART4_Buffer_Tx[USART4_BufferSize_Tx];
extern uint8_t USART4_Buffer_Rx[USART4_BufferSize_Rx];


/***************** USART4 Configuration *********************/

#define RCC_AHB1Periph_USART4_Tx     RCC_AHB1Periph_GPIOC
#define RCC_AHB1Periph_USART4_Rx     RCC_AHB1Periph_GPIOC

#define GPIO_USART4_Tx               GPIOC
#define GPIO_USART4_Rx               GPIOC

#define GPIO_Pin_USART4_Tx           GPIO_Pin_10
#define GPIO_Pin_USART4_Rx           GPIO_Pin_11

#define GPIO_PinSource_USART4_Tx     GPIO_PinSource10
#define GPIO_PinSource_USART4_Rx     GPIO_PinSource11

/************************************************************/


void USART4_Init(uint32_t BaudRate, uint16_t USART_Mode, uint16_t USART_IT, uint16_t USART_DMAReq);
void USART4_DMA_TX_Init(uint32_t DMA_Memory0BaseAddr, uint16_t DMA_BufferSize);
void USART4_DMA_RX_Init(uint32_t DMA_Memory0BaseAddr, uint16_t DMA_BufferSize);
void USART4_DMA_TX(uint16_t DMA_BufferSize);
void USART4_DMA_RX(uint16_t DMA_BufferSize);
void USART4_OUT(uint8_t* Buffer, uint16_t BufferSize);
void USART4_IN(void);

#endif 	/* __USART4_H */
