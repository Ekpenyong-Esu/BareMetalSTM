#include "stm32f4xx.h"
#include <stdio.h>
#include "delay.h"

#define ROWS_GPIO_PORT     GPIOC
#define COLS_GPIO_PORT     GPIOB

#define ROW1_PIN           GPIO_PIN_0
#define ROW2_PIN           GPIO_PIN_1
#define ROW3_PIN           GPIO_PIN_2
#define ROW4_PIN           GPIO_PIN_3

#define COL1_PIN           GPIO_PIN_4
#define COL2_PIN           GPIO_PIN_5
#define COL3_PIN           GPIO_PIN_6
#define COL4_PIN           GPIO_PIN_7

void Keyboard_Init(void)
{
    // Enable GPIO peripheral clocks
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOBEN | RCC_AHB1ENR_GPIOCEN;

    // Configure ROW pins as inputs
    GPIOC->MODER &= ~(GPIO_MODER_MODE0 | GPIO_MODER_MODE1 | GPIO_MODER_MODE2 | GPIO_MODER_MODE3);

    // Enable pull-up resistors for ROW pins
    GPIOC->PUPDR |= GPIO_PUPDR_PUPD0_0 | GPIO_PUPDR_PUPD1_0 | GPIO_PUPDR_PUPD2_0 | GPIO_PUPDR_PUPD3_0;

    // Configure COL pins as outputs
    GPIOB->MODER &= ~(GPIO_MODER_MODE4 | GPIO_MODER_MODE5 | GPIO_MODER_MODE6 | GPIO_MODER_MODE7);
    GPIOB->MODER |= GPIO_MODER_MODE4_0 | GPIO_MODER_MODE5_0 | GPIO_MODER_MODE6_0 | GPIO_MODER_MODE7_0;
    GPIOB->OTYPER &= ~(GPIO_OTYPER_OT4 | GPIO_OTYPER_OT5 | GPIO_OTYPER_OT6 | GPIO_OTYPER_OT7);
    GPIOB->OSPEEDR |= GPIO_OSPEEDR_OSPEED4 | GPIO_OSPEEDR_OSPEED5 | GPIO_OSPEEDR_OSPEED6 | GPIO_OSPEEDR_OSPEED7;
}

uint8_t Keyboard_Scan(void)
{
    uint8_t keys[4][4] = {
        { '1', '2', '3', 'A' },
        { '4', '5', '6', 'B' },
        { '7', '8', '9', 'C' },
        { '*', '0', '#', 'D' }
    };

    for (uint8_t i = 0; i < 4; i++)
    {
        // Select a column
        uint16_t col_pin = 1 << (i + 4);

        // Set column pin to logic low
        GPIOB->BSRR = (col_pin << 16);

        // Read rows
        for (uint8_t j = 0; j < 4; j++)
        {
            uint16_t row_pin = 1 << j;

            if ((GPIOC->IDR & row_pin) == 0)  // If the row reads low, a key is pressed
            {
                // Set column pin back to logic high
                GPIOB->BSRR = col_pin;

                // Return the key that is pressed
                return keys[j][i];
            }
        }

        // Set column pin back to logic high
        GPIOB->BSRR = col_pin;
    }

    return 0;  // No key pressed
}

void USART1_Init(void)
{
    // Enable peripheral clocks
    RCC->APB2ENR |= RCC_APB2ENR_USART1EN;
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;

    GPIOA->AFR[1] |= 0x7 << (4 * 1); // AF7 for USART1 on PA9 (TX)
    GPIOA->AFR[1] |= 0x7 << (4 * 2); // AF7 for USART1 on PA10 (RX)

    // Configure PA9 (USART1_TX) and PA10 (USART1_RX) as alternate function
    GPIOA->MODER &= ~(GPIO_MODER_MODER9 | GPIO_MODER_MODER10);
    GPIOA->MODER |= GPIO_MODER_MODER9_1 | GPIO_MODER_MODER10_1;

    // Configure USART1: 9600 baud, 8 data bits, 1 stop bit, no parity, transmit enable, USART enable
    USART1->BRR = 0x0683; // 16000000 / 9600; // 9600 baud rate
    USART1->CR1 = USART_CR1_TE | USART_CR1_RE | USART_CR1_UE;
}


int USART1_WriteChar(int ch)
{
    while (!(USART1->SR & USART_SR_TXE))
        ;
    USART1->DR = ch;
    return ch;
}

int USART1_WriteString(const char* str)
{
    while (*str)
    {
        if (USART1_WriteChar(*str++) == EOF)
            return EOF;
    }
    return 0;
}

int main(void)
{
    // Enable peripheral clock for SYSCFG
    RCC->APB2ENR |= RCC_APB2ENR_SYSCFGEN;

    // Initialize peripherals and GPIO pins
    Keyboard_Init();
    USART1_Init();

    while (1)
    {
        uint8_t key = Keyboard_Scan();

        if (key != 0)
        {
            // Key pressed, send it to the terminal
            USART1_WriteChar(key);
            Delay(40);
        }
    }
}
