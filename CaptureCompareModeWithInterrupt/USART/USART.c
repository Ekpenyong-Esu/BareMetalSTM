#include "USART.h"


void UART1_Init(void) {

	// Enable UART1 and GPIOA clocks
	  RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;
	  RCC->APB2ENR |= RCC_APB2ENR_USART1EN;

	  // Configure PA9 and PA10 pins for UART1 TX and RX
	  GPIOA->AFR[1] &= ~GPIO_AFRH_AFSEL9;
	  GPIOA->AFR[1] |= 7 << GPIO_AFRH_AFSEL9_Pos; // ALT7 for UART1_TX

	  GPIOA->AFR[1] &= ~GPIO_AFRH_AFSEL10;
	  GPIOA->AFR[1] |= 7 << GPIO_AFRH_AFSEL10_Pos; // ALT7 for UART1_RX

	  GPIOA->MODER &= ~(GPIO_MODER_MODER9 | GPIO_MODER_MODER10);
	  GPIOA->MODER |= GPIO_MODER_MODER9_1 | GPIO_MODER_MODER10_1; // Enable alternate function

	  // Configure UART1
	  USART1->BRR = 16000000/9600; // 9600 baud @ 16MHz
	  USART1->CR1 = USART_CR1_TE | USART_CR1_RE | USART_CR1_UE; // Enable TX, RX, UART
	  USART1->CR2 = 0x0000; //USART_CR2_STOP_1 ; // 1 stop bit
	  USART1->CR3 = 0x0000; // No flow control
}


// Function to send a character via UART1
void UART1_SendChar(char c) {
    while (!(USART1->SR & USART_SR_TXE));  // Wait for TX buffer to be empty
    USART1->DR = c;                        // Send character
}

// Function to receive a character via UART1
char UART1_ReceiveChar(void) {
    while (!(USART1->SR & USART_SR_RXNE)); // Wait for RX buffer to have data
    return USART1->DR;                     // Read and return received character
}

// Function to send a string via UART1
void UART1_SendString(const char* str) {
    for (int i = 0; i < strlen(str); i++) {
        UART1_SendChar(str[i]);
    }
}

// Function to receive a string via UART1 (terminates on '\r' character)
void UART1_ReceiveString(char* buffer, int maxLength) {
    int i = 0;
    char receivedChar;

    do {
        receivedChar = UART1_ReceiveChar();
        buffer[i++] = receivedChar;
    } while (receivedChar != '\r' && i < maxLength - 1);

    buffer[i] = '\0'; // Null-terminate the string
}
