#include "stm32f4xx.h"

#define SystemCoreClock 16000000
volatile uint32_t msTicks = 0;

void SysTick_Init(void) {
    SysTick->LOAD = SystemCoreClock / 1000 - 1; // Set reload value for 1 ms interval
    SysTick->VAL = 0;
    SysTick->CTRL = SysTick_CTRL_CLKSOURCE_Msk | SysTick_CTRL_TICKINT_Msk | SysTick_CTRL_ENABLE_Msk; // Enable SysTick
}

void delayMs(uint32_t ms) {
    uint32_t startTicks = msTicks;
    while ((msTicks - startTicks) < ms) {}
}

int main(void) {
    // Initialize SysTick
    SysTick_Init();

    // Enable GPIOG clock
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOGEN;

    // Configure PG13 as output
    GPIOG->MODER |= GPIO_MODER_MODE13_0;

    while (1) {
        // Toggle PG13 every 1000 ms (1 second)
        GPIOG->ODR ^= GPIO_ODR_OD13;
        delayMs(1000);
    }
}

void SysTick_Handler(void) {
    msTicks++; // Increment msTicks every 1 ms
}