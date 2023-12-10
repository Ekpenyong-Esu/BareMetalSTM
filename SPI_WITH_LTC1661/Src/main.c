#include "stm32f4xx.h"

void SPI1_Init(void);
void DAC_Write(uint16_t data);

int main(void) {
	uint16_t i;

	SPI1_Init();

	while(1) {
		for (i = 0; i < 1024; i++) {
			DAC_Write(i);
		}
	}
}

/* Initialize SPI1 as a master with software slave select */
void SPI1_Init(void) {
	/* Enable the GPIOA and SPI1 peripheral clocks */
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;
	RCC->APB2ENR |= RCC_APB2ENR_SPI1EN;

	/* Configure GPIOA pins for SPI1 (PA7: MOSI, PA5: SCK) */
	GPIOA->MODER &= ~GPIO_MODER_MODE5_Msk & ~GPIO_MODER_MODE7_Msk; // Clear the mode bits
	GPIOA->MODER |= GPIO_MODER_MODE5_1 | GPIO_MODER_MODE7_1; // Set alternate function mode
	GPIOA->AFR[0] &= ~(GPIO_AFRL_AFSEL5_Msk | GPIO_AFRL_AFSEL7_Msk); // Clear the alternate function bits
	GPIOA->AFR[0] |= (5 << GPIO_AFRL_AFSEL5_Pos) | (5 << GPIO_AFRL_AFSEL7_Pos); // Set alternate function to SPI1

	/* Configure PA4 as GPIO output for SPI1 slave select (SS) */
	GPIOA->MODER &= ~GPIO_MODER_MODE4_Msk; // Clear the mode bits
	GPIOA->MODER |= GPIO_MODER_MODE4_0; // Set as output

	/* Reset SPI1 */
	SPI1->CR1 = 0;

	/* Set the baud rate to 1 MHz */
	SPI1->CR1 |= SPI_CR1_SSM | SPI_CR1_SSI  | SPI_CR1_BR_2 | SPI_CR1_BR_0;

	/* Configure as a master device, 8-bit data frame format, and CPOL=0, CPHA=0 (SPI mode 0) */
	SPI1->CR1 |= SPI_CR1_MSTR;

	/* Enable the SPI1 module */
	SPI1->CR1 |= SPI_CR1_SPE;
}

/* This function sends data to the DAC */
void DAC_Write(uint16_t data) {
	data &= 0x03FF; // Make sure data is 10-bit
	GPIOA->BSRR = GPIO_BSRR_BR_4; // Deassert slave select (PA4)

	while (!(SPI1->SR & SPI_SR_TXE)) {} // Wait until the transmit buffer is empty

	// Note that 0x90 is a command byte
	SPI1->DR = 0x90 | (data >> 6); // Send command and upper 4 bits of data

	while (!(SPI1->SR & SPI_SR_TXE)) {} // Wait until the transmit buffer is empty

	SPI1->DR = (data << 2) & 0xFF; // Send lower 8 bits of data

	while (SPI1->SR & SPI_SR_BSY) {} // Wait until SPI is not busy

	GPIOA->BSRR = GPIO_BSRR_BS_4; // Assert slave select (PA4)
}

