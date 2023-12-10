
#include "stm32f4xx.h"

void delayMs(int n);
void GPIO_Init(void);
void SPI1_Init(void);
void SPI1_Write(unsigned const char data);

int main(void)
{
   char c = 'A';

   GPIO_Init();
   SPI1_Init();

   while(1)
   {
	   SPI1_Write(c);
	   delayMs(20);

	   // Check if c has reached 'Z' and reset it to 'A'
	   if (c == 'Z')
	   {
		   c = 'A';
	   }
	   else
	   {
		   c++; // Increment c to the next character
	   }

   }
}

/* 16 MHz SYSCLK */
void delayMs(int n) {
    int i;
    for (; n > 0; n--)
        for (i = 0; i < 3195; i++) ;
}

void GPIO_Init(void)
{
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOGEN;     // enable GPIOG clock 0x1 << 6U

	GPIOG->MODER &= ~(GPIO_MODER_MODE14);    // 0x3 << 28
	GPIOG->MODER |=  GPIO_MODER_MODE14_0;    // set pin to output mode 0x1 << 28
}

void SPI1_Init(void)
{
	// Enable SPI1 and GPIO clocks
	RCC->APB2ENR |= RCC_APB2ENR_SPI1EN;

	// Enable the GPIOA clock
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;

	// Set the alternate function of SPI serial clock sclk PA5 and MOSI PA7
	// Clear it first
	 GPIOA->MODER &=  ~(GPIO_MODER_MODER5 | GPIO_MODER_MODER7);

	 // Set it
	 GPIOA->MODER |= GPIO_MODER_MODER5_1 | GPIO_MODER_MODER7_1;

	 // Set the Alternate function, know that the alternate function is alt 5
	 // Set AF5 for PA5 (SCK), and PA7 (MOSI)
	 GPIOA->AFR[0] |= (5 << GPIO_AFRL_AFSEL5_Pos) | (5 << GPIO_AFRL_AFSEL7_Pos);

	 // Make GPIO PA4 as output for slave sellect
	 //clear the register
	 GPIOA->MODER &= ~GPIO_MODER_MODER4;

	 // Set the register
	 GPIOA->MODER |= GPIO_MODER_MODER4_0;

	 // Set the SPI settings
	 // Set SPI1 as master, software NSS, baudrate is 0X3UL = 011 = 16000000/16, 8 bit data = DFF = 0
	 // clock polarity = 0, and clock phase = 0;
	 SPI1->CR1 |= SPI_CR1_SSM | SPI_CR1_SSI | (3 << SPI_CR1_BR_Pos)| SPI_CR1_MSTR;

	 SPI1->CR2 = 0;

	 // Enable SPI
	 SPI1->CR1 |= SPI_CR1_SPE;
}


void SPI1_Write(unsigned const char data)
{
	// Set NSS low to select the slave device
	GPIOA->BSRR |= GPIO_BSRR_BR_4;

	while (!(SPI1->SR & SPI_SR_TXE)){}

	// Set LED to ON
	GPIOG->ODR ^= GPIO_BSRR_BS_14;

	SPI1->DR = data;

	while ((SPI1->SR & SPI_SR_BSY)){}

	// Set LED to OFF
	//GPIOG->BSRR |= GPIO_BSRR_BR_14;

	// Set NSS high to deselect the slave device
	GPIOA->BSRR |= GPIO_BSRR_BS_4;

}