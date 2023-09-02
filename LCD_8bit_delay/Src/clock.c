/*
 * clock.c
 *
 *  Created on: Apr 12, 2023
 *      Author: mahon
 */

#include "main.h"


static int delay_complete = 1;  // assume the delay is complete

/* 16 MHz SYSCLK */
void delayMs(int n) {
    int i;
    for (; n > 0; n--)
        for (i = 0; i < 3195; i++) ;
}



void delayMs1(uint16_t ms) {
    // Enable the TIM2 clock
    RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;

    // Configure the TIM2 timer
    TIM2->PSC = (16000000/1000000)-1;  // Set the prescaler to achieve 1us resolution, assuming the system clock frequency is set to 16 MHz
    TIM2->ARR = ms * 1000;  // Set the auto-reload value to the desired delay in ms
    TIM2->CR1 |= TIM_CR1_ARPE;  // Enable auto-reload preload
    TIM2->CR1 &= ~TIM_CR1_DIR;  // Count up

    // Enable interrupt for TIM2 update event
    TIM2->DIER |= TIM_DIER_UIE;
    NVIC_EnableIRQ(TIM2_IRQn);

    // Start the TIM2 timer
    TIM2->CR1 |= TIM_CR1_CEN;

    // Wait for the TIM2 timer to complete the delay
    while (delay_complete == 0);

    // Disable the TIM2 timer
    TIM2->CR1 &= ~TIM_CR1_CEN;

    // Disable interrupt for TIM2 update event
    TIM2->DIER &= ~TIM_DIER_UIE;
    NVIC_DisableIRQ(TIM2_IRQn);

    delay_complete = 0;
}

void TIM2_IRQHandler(void) {
    // Clear the TIM2 timer interrupt flag
    TIM2->SR &= ~TIM_SR_UIF;

    // Set the flag to indicate that the delay has completed
    delay_complete = 1;
}

