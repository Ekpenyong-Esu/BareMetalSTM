#include "stm32f4xx.h"
#include <stdio.h>
#include <string.h>

#define MAX_STRING_LENGTH 1000

char rx_buffer[MAX_STRING_LENGTH]; // Buffer to store received data
volatile uint32_t rx_index = 0; // Index for received data buffer

void UART1_Init(void) {

	// Enable UART1 and GPIOA clocks
	  RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;
	  RCC->APB2ENR |= RCC_APB2ENR_USART1EN;

	 // RCC->APB2ENR |= RCC_APB2ENR_SYSCFGEN; // SYSCFG clock for interrupt

	  // Configure PA9 and PA10 pins for UART1 TX and RX
	  GPIOA->AFR[1] &= ~GPIO_AFRH_AFSEL9;
	  GPIOA->AFR[1] |= 7 << GPIO_AFRH_AFSEL9_Pos; // ALT7 for UART1_TX

	  GPIOA->AFR[1] &= ~GPIO_AFRH_AFSEL10;
	  GPIOA->AFR[1] |= 7 << GPIO_AFRH_AFSEL10_Pos; // ALT7 for UART1_RX

	  GPIOA->MODER &= ~(GPIO_MODER_MODER9 | GPIO_MODER_MODER10);
	  GPIOA->MODER |= GPIO_MODER_MODER9_1 | GPIO_MODER_MODER10_1; // Enable alternate function

	  // Configure UART1
	  USART1->BRR = 16000000/9600; // 9600 baud @ 16MHz

	  //  Enable RXNE interrupt and  Enable TXEIE interrupt



	  USART1->CR1 |=USART_CR1_RXNEIE | USART_CR1_TE | USART_CR1_RE; // Enable TX, RX, UART

	  USART1->CR2 = 0x0000; //USART_CR2_STOP_1 ; // 1 stop bit
	  USART1->CR3 = 0x0000; // No flow control

	  USART1->CR1 |= USART_CR1_UE;

	  NVIC_SetPriority(USART1_IRQn, 0); // Highest priority
	  NVIC_EnableIRQ(USART1_IRQn);


}



void USART1_Transmit(uint8_t data) {
    while ((USART1->SR & USART_SR_TXE) == 0) {} // Wait until TXE flag is set
    USART1->DR = data;
}




void USART1_TransmitString(const char *string) {
    while (*string != '\0') {
        USART1_Transmit(*string);
        string++;
    }
}


void USART1_IRQHandler(void)
{
	if ((USART1->SR & USART_SR_RXNE) != 0)
	{
		// Data received, process it
		char receivedData = USART1->DR;

		if (receivedData == '\r')
		{
			rx_buffer[rx_index++] = '\n'; // Convert '\r' to '\n'
		}
		else
		{
			rx_buffer[rx_index++] = receivedData;
		}

		//rx_buffer[rx_index++] = receivedData;

		if (rx_index >= MAX_STRING_LENGTH)
		{
			USART1->CR1 &= ~USART_CR1_RXNEIE; // Disable interrupt
			rx_index = 0; // Reset index if buffer is full
		}

		USART1->SR &= ~USART_SR_RXNE; // Reset flag
	}


}



void delayMs(int n) {
	int i;
	for (; n > 0; n--)
		for (i = 0; i < 2000; i++);
}


int main(void) {
    UART1_Init();

    USART1_TransmitString("Transmitting Using USART:\n\r");
    USART1_TransmitString("\n\r");
    USART1_TransmitString("Enter some characters:  ");
    delayMs(5000);


    while (1)
    {
    	if (rx_index > 0)
    	{


    		USART1_TransmitString("\n\r");
    		USART1_TransmitString("Received: ");
    		USART1_TransmitString(rx_buffer);
    		USART1_TransmitString("\r\n");

    		// Clear the buffer
    		memset(rx_buffer, 0, sizeof(rx_buffer));
    		rx_index = 0;
    	}

    	USART1_TransmitString("Enter some characters:  ");
    	delayMs(5000);
    	USART1_TransmitString("\n\r");

    }
}