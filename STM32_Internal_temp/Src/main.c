#include "stm32f4xx.h"
#include <stdio.h>

// Function to introduce a delay in milliseconds
void delayMs(int n);

// Function to initialize USART1 for communication
void USART1_Init(void);

// Function to write a character to USART1
int USART1_WriteChar(int ch);

// Function to write a string to USART1
int USART1_WriteString(const char* str);

void TIM2_INIT(void);


// Function to initialize ADC1 for reading the internal temperature sensor
void ADC1_Init(void);

int main(void) {
    int result;
    double volt, temp;

    // Initialize ADC and USART
    TIM2_INIT();
    USART1_Init();
    ADC1_Init();

    // Print a message to indicate the program has started
    printf("ADC temperature sensor\r\n");
    printf("\r\n");

    while (1) {
        // Wait for the ADC conversion to complete
        while (!(ADC1->SR & ADC_SR_EOC));

        // Read the converted value
        result = ADC1->DR;

        // Convert the ADC value to voltage and temperature
        volt = ((double)result / 4095) * 3.3;  // the raw ADC is divide by 12 bits which is 2^12 - 1 and
        									//the multiply by the reference voltage. This convert the raw ADC value to voltage value

        //The specific value of 0.0025 indicates that for every 1 volt change, the temperature changes by 1 degree Celsius.
        //Finally, 25 degrees Celsius is added to the result.
        //This adjustment accounts for any temperature offset or calibration specific to the sensor or system.
        temp = (volt - 0.76) / 0.0025 + 25;  // substract 0.76 from the volt to adjusts the voltage reading.This adjustment is often required because temperature sensors
        									//may have an offset or reference voltage that needs correction.

        // Print the ADC value and temperature over USART
        printf("The RAW ADC Value = %d\r\n", result);
        printf("\r\n");

        printf("The Internal TEMP value is = %.2f\370C\r\n", temp); //\370C This the ASCII value for degree Celsius symbol (°C)
        printf("\r\n");
    }

}

// Function to introduce a delay in milliseconds
void delayMs(int n) {
    int i;
    for (; n > 0; n--)
        for (i = 0; i < 3195; i++) ;
}

/*************************

TIM2 Initialization
*************************/
void TIM2_INIT(void){

// Enable timer clock
RCC->APB1ENR |=RCC_APB1ENR_TIM2EN;

// Set prescaler to divide SYSCLK by 1600
TIM2->PSC = 1600 -1;

// Set auto-reload for 1s period (1Hz)
TIM2->ARR = 10000 - 1;

// Clear counter
TIM2->CNT = 0;

// Configure CH2 for PWM mode 1
TIM2->CCMR1 = TIM_CCMR1_OC2PE;
TIM2->CCMR1 |= 0x6UL << 12UL;

// Enable CH2 capture/compare
TIM2->CCER |= TIM_CCER_CC2E;

// Set CH2 compare value
TIM2->CCR2 = 500-1;

// Enable timer
TIM2->CR1 |= TIM_CR1_CEN;

}

// Function to initialize USART1 for communication
void USART1_Init(void) {
    // Enable peripheral clocks for USART1 and related GPIO
    RCC->APB2ENR |= RCC_APB2ENR_USART1EN;
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;

    // Configure alternate function for USART1_TX and USART1_RX on PA9 and PA10
    GPIOA->AFR[1] |= 0x7 << (4 * 1); // AF7 for USART1 on PA9 (TX)
    GPIOA->AFR[1] |= 0x7 << (4 * 2); // AF7 for USART1 on PA10 (RX)

    // Configure PA9 and PA10 as alternate function mode for USART1
    GPIOA->MODER &= ~(GPIO_MODER_MODER9 | GPIO_MODER_MODER10);
    GPIOA->MODER |= GPIO_MODER_MODER9_1 | GPIO_MODER_MODER10_1;

    // Configure USART1 settings: 9600 baud, 8 data bits, 1 stop bit, no parity, transmit enable, USART enable
    USART1->BRR = 0x0683; // 16000000 / 9600; // 9600 baud rate
    USART1->CR1 = USART_CR1_TE | USART_CR1_RE | USART_CR1_UE;
}

// Function to write a character to USART1
int USART1_WriteChar(int ch) {
    // Wait until the transmit buffer is empty
    while (!(USART1->SR & USART_SR_TXE));

    // Write the character to the data register
    USART1->DR = ch;
    return ch;
}

// Function to write a string to USART1
int USART1_WriteString(const char* str) {
    // Iterate through each character in the string
    while (*str) {
        // Write the current character to USART1
        if (USART1_WriteChar(*str++) == EOF)
            return EOF;
    }
    return 0;
}

// Function to initialize ADC1 for reading the internal temperature sensor
void ADC1_Init(void) {
    // Enable ADC1 clock
    RCC->APB2ENR |= RCC_APB2ENR_ADC1EN;

    // Enable the temperature sensor
    ADC->CCR |= ADC_CCR_TSVREFE;

    // Disable VBATE to enable the temperature sensor
    ADC->CCR &= ~ADC_CCR_VBATE;

    // Configure ADC settings
    ADC1->SMPR1 = ADC_SMPR1_SMP18_2; // Set sample time for channel 18 (internal temperature sensor)
    ADC1->SQR1 &= ~ADC_SQR1_L; // Configure regular channel sequence length
    ADC1->SQR1 |= ADC_SQR1_L_0; // Set sequence length to one conversion
    ADC1->SQR3 = 18; // Configure regular channel sequence for channel 18 (internal temperature sensor)
    ADC1->CR2 |= ADC_CR2_EXTEN_0; // Set ADC external trigger source to TIM2 (EXTSEL bits)
    ADC1->CR2 |= ADC_CR2_EXTSEL_0 | ADC_CR2_EXTSEL_1; // Select TIM2 TRGO event as trigger
    ADC1->CR2 |= ADC_CR2_ADON; // Enable ADC

    // Delay for ADC to stabilize
    delayMs(10);
}

/// Implement the _write function for printf redirection
int _write(int file, char *ptr, int len) {
    int i;
    for (i = 0; i < len; i++) {
        USART1_WriteChar(ptr[i]);
    }
    return len;
}
