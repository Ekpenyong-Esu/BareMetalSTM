/*
 * main.h
 *
 *  Created on: Apr 12, 2023
 *      Author: mahon
 */

#ifndef MAIN_H_
#define MAIN_H_

#include "stm32f4xx.h"
#include "stm32f429xx.h"


void delayMs1(int n);
void delayMs(uint16_t ms);
void TIM2_IRQHandler(void);

#endif /* MAIN_H_ */
