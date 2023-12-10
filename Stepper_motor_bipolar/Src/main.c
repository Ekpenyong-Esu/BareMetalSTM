#include "stm32f4xx.h"

#define IN1_PIN (1 << 0)
#define IN2_PIN (1 << 0)
#define IN3_PIN (1 << 0)
#define IN4_PIN (1 << 0)

#define DELAY_TIME 1000

#define STEPS_PER_REV 200

void delay_ms(uint32_t ms) {
    volatile uint32_t i;
    for (i = 0; i < (ms * 3360); i++) {
        // Adjust this value based on your clock speed
    }
}

void init_stepper_motor() {
  // Enable the clock for GPIOE
	 RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;  // Enable GPIOA clock

  // Configure the stepper motor driver pins as outputs
	 GPIOA->MODER |= (GPIO_MODER_MODER0_0 | GPIO_MODER_MODER1_0 | GPIO_MODER_MODER2_0 | GPIO_MODER_MODER3_0);
}

void rotate_stepper_motor(int steps) {
  int i;

  for (i = 0; i < steps; i++) {
    // Set the step pattern for forward rotation
    GPIOE->ODR |= IN1_PIN;
    GPIOE->ODR &= ~IN2_PIN;
    GPIOE->ODR |= IN3_PIN;
    GPIOE->ODR &= ~IN4_PIN;

    // Delay for the specified amount of time
    delay_ms(DELAY_TIME);

    // Set the step pattern for the next step
    GPIOE->ODR &= ~IN1_PIN;
    GPIOE->ODR |= IN2_PIN;
    GPIOE->ODR &= ~IN3_PIN;
    GPIOE->ODR |= IN4_PIN;

    // Delay for the specified amount of time
    delay_ms(DELAY_TIME);
  }
}

void reverse_stepper_motor(int steps) {
  int i;

  for (i = 0; i < steps; i++) {
    // Set the step pattern for reverse rotation
    GPIOE->ODR &= ~IN1_PIN;
    GPIOE->ODR |= IN2_PIN;
    GPIOE->ODR &= ~IN3_PIN;
    GPIOE->ODR |= IN4_PIN;

    // Delay for the specified amount of time
    delay_ms(DELAY_TIME);

    // Set the step pattern for the next step
    GPIOE->ODR |= IN1_PIN;
    GPIOE->ODR &= ~IN2_PIN;
    GPIOE->ODR |= IN3_PIN;
    GPIOE->ODR &= ~IN4_PIN;

    // Delay for the specified amount of time
    delay_ms(DELAY_TIME);
  }
}

int main(void) {
  // Initialize the stepper motor
  init_stepper_motor();

  // Rotate the stepper motor for 200 steps in forward direction
  rotate_stepper_motor(200);

  // Reverse the stepper motor for 200 steps in reverse direction
  reverse_stepper_motor(200);

  while (1) {
  }
}
