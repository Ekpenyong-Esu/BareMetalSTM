#include "stm32f4xx.h"

#define STEP_PIN_PIN 0
#define DIR_PIN_PIN  1

void delay_ms(uint32_t ms) {
	// Simple delay function
	volatile uint32_t i, j;
	for (i = 0; i < ms; ++i)
		for (j = 0; j < 20000; ++j);
}

void GPIO_Init(void) {
	// Enable GPIOB clock
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOBEN;

	// Configure GPIOB_PIN_0 and GPIOB_PIN_1 as output
	GPIOB->MODER |= (1 << (STEP_PIN_PIN * 2)) | (1 << (DIR_PIN_PIN * 2));

	// Ensure push-pull mode
	GPIOB->OTYPER &= ~((1 << STEP_PIN_PIN) | (1 << DIR_PIN_PIN));

	// Ensure high-speed mode
	GPIOB->OSPEEDR |= (3 << (STEP_PIN_PIN * 2)) | (3 << (DIR_PIN_PIN * 2));

	// Ensure no pull-up/pull-down
	GPIOB->PUPDR &= ~((3 << (STEP_PIN_PIN * 2)) | (3 << (DIR_PIN_PIN * 2)));
}

void stepper_rotate(uint8_t direction, uint16_t steps, uint16_t delay) {
	// Set direction pin
	if (direction == 1) {
		GPIOB->BSRR |= (1 << DIR_PIN_PIN);  // Set DIR_PIN
	} else {
		GPIOB->BSRR |= (1 << (DIR_PIN_PIN + 16));  // Reset DIR_PIN
	}

	// Generate steps
	for (uint16_t i = 0; i < steps; ++i) {
		GPIOB->BSRR |= (1 << STEP_PIN_PIN);  // Set STEP_PIN
		delay_ms(1);  // Adjust delay for desired speed
		GPIOB->BSRR |= (1 << (STEP_PIN_PIN + 16));  // Reset STEP_PIN
		delay_ms(delay);  // Adjust delay for desired speed
	}
}

int main(void) {
	// Initialize GPIO
	GPIO_Init();

	// Main loop
	while (1) {
		// Rotate stepper motor clockwise
		stepper_rotate(1, 200, 10);  // 200 steps, 10ms delay

		// Rotate stepper motor counterclockwise
		stepper_rotate(0, 200, 10);  // 200 steps, 10ms delay
	}
}