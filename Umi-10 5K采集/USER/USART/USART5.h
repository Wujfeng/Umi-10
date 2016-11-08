/****************************************************************
 * Copyright (C) 2016, Yagro, all right reserved.
 * File name:    USART5.h
 * Date:         2016.03.22
 * Description:  USART5 Driver
*****************************************************************/

#ifndef __USART5_H
#define __USART5_H

#include "stm32f4xx.h"
#include <string.h>


#define USART5_BufferSize_Tx  ((uint16_t)512)
#define USART5_BufferSize_Rx  ((uint16_t)512)


extern uint8_t USART5_Buffer_Tx[USART5_BufferSize_Tx];
extern uint8_t USART5_Buffer_Rx[USART5_BufferSize_Rx];

/***************** USART5 Configuration *********************/

#define RCC_AHB1Periph_USART5_Tx     RCC_AHB1Periph_GPIOC
#define RCC_AHB1Periph_USART5_Rx     RCC_AHB1Periph_GPIOD

#define GPIO_USART5_Tx               GPIOC
#define GPIO_USART5_Rx               GPIOD

#define GPIO_Pin_USART5_Tx           GPIO_Pin_12
#define GPIO_Pin_USART5_Rx           GPIO_Pin_2

#define GPIO_PinSource_USART5_Tx     GPIO_PinSource12
#define GPIO_PinSource_USART5_Rx     GPIO_PinSource2

/************************************************************/


void USART5_Init(uint32_t BaudRate, uint16_t USART_Mode, uint16_t USART_IT, uint16_t USART_DMAReq);
void USART5_DMA_TX_Init(uint32_t DMA_Memory0BaseAddr, uint16_t DMA_BufferSize);
void USART5_DMA_RX_Init(uint32_t DMA_Memory0BaseAddr, uint16_t DMA_BufferSize);
void USART5_DMA_TX(uint16_t DMA_BufferSize);
void USART5_DMA_RX(uint16_t DMA_BufferSize);
void USART5_OUT(uint8_t* Buffer, uint16_t BufferSize);
void USART5_IN(void);

#endif 	/* __USART5_H */
