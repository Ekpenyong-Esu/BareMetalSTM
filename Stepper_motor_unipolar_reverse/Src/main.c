#include "stm32f4xx.h"

#define STEPPER_PIN_1 (1 << 0)
#define STEPPER_PIN_2 (1 << 1)
#define STEPPER_PIN_3 (1 << 2)
#define STEPPER_PIN_4 (1 << 3)

void delay_ms(uint32_t ms) {
    volatile uint32_t i;
    for (i = 0; i < (ms * 3360); i++) {
        // Adjust this value based on your clock speed
    }
}

void stepper_init() {
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;  // Enable GPIOA clock

    GPIOA->MODER |= (GPIO_MODER_MODER0_0 | GPIO_MODER_MODER1_0 | GPIO_MODER_MODER2_0 | GPIO_MODER_MODER3_0);

    // You may need to configure other GPIO settings based on your hardware setup (e.g., pull-up/pull-down resistors)
}

void step(int step_number) {
    switch (step_number) {
        case 0:
            GPIOA->ODR |= (STEPPER_PIN_1 | STEPPER_PIN_2);
            GPIOA->ODR &= ~(STEPPER_PIN_3 | STEPPER_PIN_4);
            break;
        case 1:
            GPIOA->ODR |= (STEPPER_PIN_2 | STEPPER_PIN_3);
            GPIOA->ODR &= ~(STEPPER_PIN_1 | STEPPER_PIN_4);
            break;
        case 2:
            GPIOA->ODR |= (STEPPER_PIN_3 | STEPPER_PIN_4);
            GPIOA->ODR &= ~(STEPPER_PIN_1 | STEPPER_PIN_2);
            break;
        case 3:
            GPIOA->ODR |= (STEPPER_PIN_1 | STEPPER_PIN_4);
            GPIOA->ODR &= ~(STEPPER_PIN_2 | STEPPER_PIN_3);
            break;
    }
}

int main(void) {
    stepper_init();

    while (1) {
        // Move clockwise
        for (int i = 0; i < 4; i++) {
            step(i);
            delay_ms(500);  // Adjust the delay based on your motor and application requirements
        }

        // Add a delay between directions
        delay_ms(1000);

        // Move counterclockwise
        for (int i = 3; i >= 0; i--) {
            step(i);
            delay_ms(500);
        }

        // Add a delay between directions
        delay_ms(1000);
    }
}
