/*
 * Delay.c
 *
 *  Created on: Jun 10, 2023
 *      Author: mahon
 */
#include <stm32f429xx.h>
#include "delay.h"

void Delay(uint32_t delay_ms)
{
    volatile uint32_t counter = delay_ms * (16000000 / 1000);
    while (counter--)
        ;
}
