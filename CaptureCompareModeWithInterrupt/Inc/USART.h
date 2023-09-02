/*
 * USART.h
 *
 *  Created on: Jul 26, 2023
 *      Author: mahon
 */

#ifndef USART_H_
#define USART_H_
#include "stm32F4xx.h"
#include <stdio.h>
#include <string.h>

// Define system clock speed - 16 MHz on STM32F429 Discovery board
#define SYS_CLOCK_SPEED 16000000

// Define desired baud rate
#define DESIRED_BAUD 9600


void UART1_Init(void);
void delayMs(int n);
void UART1_SendChar(char c);
char UART1_ReceiveChar(void) ;
void UART1_SendString(const char* str);
void UART1_ReceiveString(char* buffer, int maxLength) ;




#endif /* USART_H_ */
