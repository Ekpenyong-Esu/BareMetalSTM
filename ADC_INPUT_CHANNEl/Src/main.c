
#include "stm32f4xx.h"

void delayMs(int n);



void ADC1_Init(void)
{
	// Enable ADC1 clock
	RCC->APB2ENR |= RCC_APB2ENR_ADC1EN;

	//Setting ADC1->CR2 to 0 at the beginning of your ADC initialization
	//function is a common practice to ensure that the register is in a known state
	//before you start configuring specific settings.
	//By clearing all the bits, you're initializing the register to its default values.
	ADC1->CR2 = 0;

	// In the code snippet ADC1->CR1 = 0;, you are configuring the ADC control register CR1 to
	// an initial value of 0. This is a common practice at the beginning of an ADC configuration to ensure
	// that the register starts in a well-defined state before you set specific configuration bits.
	ADC1->CR1 = 0;

	// Set resolution to 12 bits
	//ADC1->CR1 |= ADC_CR1_RES_1;

	// Set sample time for channel 1
	ADC1->SMPR2 = ~ADC_SMPR2_SMP1_Msk; // the default sampling time is use 000 for 3 cycles

	// Configure regular channel sequence length
	ADC1->SQR1 &= ~ADC_SQR1_L;


	// Configure regular channel sequence
	ADC1->SQR3 = ADC_SQR3_SQ1_0; // Channel 1 is the first and only conversion

	// Enable ADC
	ADC1->CR2 |= ADC_CR2_ADON;
}



int main(void) {

	int result;

	ADC1_Init();

	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOGEN;            // enable GPIOG clock 0x1 << 6U

	GPIOG->MODER &= ~(GPIO_MODER_MODE14);    // 0x3 << 28
	GPIOG->MODER |=  GPIO_MODER_MODE14_0;    // set pin to output mode 0x1 << 28


	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;            // enable GPIOG clock 0x1 << 0U

	GPIOG->MODER &= ~(GPIO_MODER_MODE1);    // 0x3 << 2UL
	GPIOG->MODER |= GPIO_MODER_MODE1;    // 0x3 << 2   The mode register is in analog mode


	while(1) {

		// Start the conversion
		ADC1->CR2 |= ADC_CR2_SWSTART;

		// Wait for the conversion to complete
		while (!(ADC1->SR & ADC_SR_EOC));

		// Read the converted value
		result = ADC1->DR;

		if (result & (0x100))
		{
			GPIOG->ODR |=  GPIO_ODR_OD14;  // turn on LED  0x1 << 14
		}
		else
		{
			GPIOG->ODR &= ~(GPIO_ODR_OD14);  /* turn off LED */
		}
		delayMs(1);

	}
}

/* 16 MHz SYSCLK */
void delayMs(int n) {
	int i;
	for (; n > 0; n--)
		for (i = 0; i < 3195; i++) ;
}
