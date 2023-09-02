#include "stm32f4xx.h"
#include <stdio.h>

// Function prototypes
void UART5_Init(void);
void UART5_SendChar(char ch);
char UART5_ReceiveChar(void);

int main(void) {
    UART5_Init();

    while (1) {
        // Send data via UART5
        char dataToSend[] = "Hello from STM32!\n";
        for (int i = 0; dataToSend[i] != '\0'; i++) {
            UART5_SendChar(dataToSend[i]);
        }

        // Receive data via UART5 and echo it
        char receivedChar = UART5_ReceiveChar();
        if (receivedChar != '\0') {
            UART5_SendChar(receivedChar); // Echo received character
        }
    }
}

// Initialize UART5
void UART5_Init(void) {
    // Enable UART5 and GPIOC/GPIOD clock
    RCC->APB1ENR |= RCC_APB1ENR_UART5EN;
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOCEN | RCC_AHB1ENR_GPIODEN;

    // Configure TX (PC12) pin
    GPIOC->MODER &= ~GPIO_MODER_MODER12;
    GPIOC->MODER |= GPIO_MODER_MODER12_1;
    GPIOC->AFR[1] &= ~(0xF << ((12 - 8) * 4));
    GPIOC->AFR[1] |= (8 << ((12 - 8) * 4));

    // Configure RX (PD2) pin
    GPIOD->MODER &= ~GPIO_MODER_MODER2;
    GPIOD->MODER |= GPIO_MODER_MODER2_1;
    GPIOD->AFR[0] &= ~(8 << (2 * 4));
    GPIOD->AFR[0] |= (8 << (2 * 4));

    // Configure UART5
    UART5->BRR = 16000000/115200;  // 115200 baud @ 16MHz
    UART5->CR1 = USART_CR1_TE | USART_CR1_RE | USART_CR1_UE;
}

// Send a character via UART5
void UART5_SendChar(char ch) {
    while (!(UART5->SR & USART_SR_TXE));
    UART5->DR = ch;
}

// Receive a character via UART5
char UART5_ReceiveChar(void) {
    if (UART5->SR & USART_SR_RXNE) {
        char receivedChar = UART5->DR;
        return receivedChar;
    }
    return '\0';  // No character received
}


void delayMs(int n) {
	int i;
	for (; n > 0; n--)
		for (i = 0; i < 2000; i++);
}


