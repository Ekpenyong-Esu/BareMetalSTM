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

void GPIO_INIT(void);

//void TIM2_INIT(void);


// Function to initialize ADC1 for reading the internal temperature sensor
void ADC1_Init(void);

int main(void) {
    int result;
    double temp;

    // Initialize GPIO, ADC and USART
    GPIO_INIT();
    //TIM2_INIT();
    USART1_Init();
    ADC1_Init();

    // Print a message to indicate the program has started
    printf("LM34 Temperature Sensor\r\n");
    printf("\r\n");

    while (1)
    {
    	// Start the conversion
		ADC1->CR2 |= ADC_CR2_SWSTART;

        // Wait for the ADC conversion to complete
        while (!(ADC1->SR & ADC_SR_EOC));

        // Read the converted value
        result = ADC1->DR;

        // Convert the ADC value to voltage and then multiply by 330 since the highest temperature 300
        //
        temp = ((double)result / 4095) * 330;  // the raw ADC is divide by 12 bits which is 2^12 - 1 and
        									//the multiply by the reference voltage. This convert the raw ADC value to voltage value


        // Print the ADC value and temperature over USART
        printf("The RAW ADC Value = %d\r\n", result);
        printf("\r\n");

        printf("The Internal TEMP value is = %.2f\370C\r\n", temp); //\370C This the ASCII value for degree Celsius symbol (°C)
        printf("\r\n");

        delayMs(1000);
    }

}

// Function to introduce a delay in milliseconds
void delayMs(int n) {
    int i;
    for (; n > 0; n--)
        for (i = 0; i < 3195; i++) ;
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
	ADC1->SQR3 = 10; // Channel 10 is the first and only conversion

	// Enable ADC
	ADC1->CR2 |= ADC_CR2_ADON;
}

/// Implement the _write function for printf redirection
int _write(int file, char *ptr, int len) {
    int i;
    for (i = 0; i < len; i++) {
        USART1_WriteChar(ptr[i]);
    }
    return len;
}

void GPIO_INIT(void)
{
	// Enable Gpio C clock
	RCC->AHB1ENR |=RCC_AHB1ENR_GPIOCEN;

	//clear The analog mode
	GPIOC->MODER &= ~GPIO_MODER_MODER0;

	// Set the mode register
	GPIOC->MODER |= GPIO_MODER_MODER0;
}
