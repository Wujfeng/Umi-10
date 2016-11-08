/****************************************************************
 * Copyright (C) 2016, Yagro, all right reserved.
 * File name:    USART6.h
 * Date:         2016.03.22
 * Description:  USART6 Driver
*****************************************************************/

#ifndef __USART6_H
#define __USART6_H

#include "stm32f4xx.h"
#include <string.h>


#define USART6_BufferSize_Tx  ((uint16_t)512)
#define USART6_BufferSize_Rx  ((uint16_t)512)


extern uint8_t USART6_Buffer_Tx[USART6_BufferSize_Tx];
extern uint8_t USART6_Buffer_Rx[USART6_BufferSize_Rx];


/***************** USART6 Configuration *********************/

#define RCC_AHB1Periph_USART6_Tx     RCC_AHB1Periph_GPIOA
#define RCC_AHB1Periph_USART6_Rx     RCC_AHB1Periph_GPIOA

#define GPIO_USART6_Tx               GPIOA
#define GPIO_USART6_Rx               GPIOA

#define GPIO_Pin_USART6_Tx           GPIO_Pin_11
#define GPIO_Pin_USART6_Rx           GPIO_Pin_12

#define GPIO_PinSource_USART6_Tx     GPIO_PinSource11
#define GPIO_PinSource_USART6_Rx     GPIO_PinSource12

/************************************************************/


void USART6_Init(uint32_t BaudRate, uint16_t USART_Mode, uint16_t USART_IT, uint16_t USART_DMAReq);
void USART6_DMA_TX_Init(uint32_t DMA_Memory0BaseAddr, uint16_t DMA_BufferSize);
void USART6_DMA_RX_Init(uint32_t DMA_Memory0BaseAddr, uint16_t DMA_BufferSize);
void USART6_DMA_TX(uint16_t DMA_BufferSize);
void USART6_DMA_RX(uint16_t DMA_BufferSize);
void USART6_OUT(uint8_t* Buffer, uint16_t BufferSize);
void USART6_IN(void);

#endif 	/* __USART6_H */
