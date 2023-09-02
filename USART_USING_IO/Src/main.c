#include "stm32f4xx.h"
#include <stdio.h>

//void SystemClock_Config(void);
void USART1_Init(void);
void delayMs(int ms);
int USART1_write(int ch);
int USART1_read(void);

int main(void) {
    //SystemClock_Config();
    USART1_Init();

    int n;
    char str[80];

    printf("Test stdio library console I/O functions\r\n");
    fflush(stdout); // Ensure the output is flushed before input
    fprintf(stdout, " test for stdout\r\n");
    fflush(stdout); // Ensure the output is flushed before input
    delayMs(100);
    fprintf(stderr, " test for stderr\r\n");
    fflush(stdout); // Ensure the output is flushed before input
    delayMs(100);

    while (1) {
    	printf("Please enter a number: ");
    	//fflush(stdout);
    	scanf("%d", &n);
    	getchar(); // Consume the newline character
    	delayMs(100);

        printf("The number entered is: %d\r\n", n);
        delayMs(100);
        printf("Please type a character string: ");
        delayMs(100);
        //fflush(stdout);
        //delayMs(100);
        fgets(str, sizeof(str), stdin);
        delayMs(100);

        printf("The character string entered is: %s\r\n", str);
        delayMs(100);
        printf("\r\n");
    }
}


void USART1_Init(void) {
    // Enable peripheral clocks
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;
    RCC->APB2ENR |= RCC_APB2ENR_USART1EN;

    // Configure PA9 (USART1_TX) and PA10 (USART1_RX) as alternate function
    GPIOA->MODER &= ~(GPIO_MODER_MODER9 | GPIO_MODER_MODER10);
    GPIOA->MODER |= (GPIO_MODER_MODER9_1 | GPIO_MODER_MODER10_1);

    GPIOA->AFR[1] &= ~(GPIO_AFRH_AFSEL9_Msk | GPIO_AFRH_AFSEL10_Msk);
    GPIOA->AFR[1] |= (7 << GPIO_AFRH_AFSEL9_Pos) | (7 << GPIO_AFRH_AFSEL10_Pos);

    // Configure USART1: 9600 baud, 8 data bits, 1 stop bit, no parity, transmit enable, USART enable
    USART1->BRR = (16000000 / 9600);
    USART1->CR1 |= USART_CR1_RE | USART_CR1_TE | USART_CR1_RXNEIE;
    USART1->CR1 |= USART_CR1_UE;
    NVIC_EnableIRQ(USART1_IRQn);
}

int USART1_write(int ch) {
    while (!(USART1->SR & USART_SR_TXE)) {}
    USART1->DR = (ch & 0xFF);
    return ch;
}

int USART1_read(void) {
    while (!(USART1->SR & USART_SR_RXNE)) {}
    return (USART1->DR & 0xFF);
}

int __io_putchar(int ch) {
    USART1_write(ch);
    return ch;
}

int _write(int file, char *ptr, int len) {
	(void)file;
    int DataIdx;
    for (DataIdx = 0; DataIdx < len; DataIdx++) {
        __io_putchar(*ptr++);
    }
    return len;
}





void delayMs(int ms) {
    for (volatile int i = 0; i < ms * 4000; ++i) {}
}

void USART1_IRQHandler(void) {
    if (USART1->SR & USART_SR_RXNE) {
        int receivedChar = USART1_read();
        // Handle received character here
    }
}
