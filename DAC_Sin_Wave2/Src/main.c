#include <math.h>
#include "stm32f4xx.h"

#define TABLE_SIZE 256

uint16_t sine_table[TABLE_SIZE];

void generate_sine_table() {
    for (int i = 0; i < TABLE_SIZE; i++) {
        sine_table[i] = (uint16_t)(((sin(2 * M_PI * i / TABLE_SIZE) + 1) / 2) * 4095);
    }
}

void DAC_Init() {
    // Enable the DAC clock
    RCC->APB1ENR |= RCC_APB1ENR_DACEN;

    // Enable GPIOA clock
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;

    // Configure PA4 (DAC_OUT1) as analog
    GPIOA->MODER |= GPIO_MODER_MODER4;

    // Set the DAC channel 1 trigger to software trigger
    DAC->CR &= ~DAC_CR_TEN1;
    DAC->CR &= ~DAC_CR_TSEL1;

    // Enable DAC channel 1
    DAC->CR |= DAC_CR_EN1;

    // Generate the sine wave table
    generate_sine_table();
}

void DAC_OutputSineWave() {
    static uint16_t index = 0;

    // Read the next value from the sine wave table
    uint16_t dac_value = sine_table[index];

    // Set the DAC channel 1 output data
    DAC->DHR12R1 = dac_value;

    // Trigger DAC channel 1 which is the default pattern
    DAC->SWTRIGR |= DAC_SWTRIGR_SWTRIG1;

    // Increment the index for the next sample
    index = (index + 1) % TABLE_SIZE;
}

int main() {
    // Initialize the MCU, DAC, and the sine wave table
    DAC_Init();

    // Start a timer or use a loop to continuously update the DAC
    while (1) {
        DAC_OutputSineWave();
    }
}
