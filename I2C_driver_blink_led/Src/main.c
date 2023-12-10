#include "stm32f4xx.h"

#define DS_DEVICE_ADDRESS 0x68    /* 1101 000. DS1337 */


void I2C_Init()
{
    // Enable the I2C1 peripheral clock
    RCC->APB1ENR |= RCC_APB1ENR_I2C1EN;

    /* Enable GPIOB clock */
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOBEN;


    // Configure the GPIO pins (SCL and SDA) for I2C
    // This example configures pins as GPIOB Pin 6 (SCL) and GPIOB Pin 9 (SDA).
    // You need to set the corresponding GPIO pins based on your hardware connection.

    // Configure SCL
    GPIOB->MODER &= ~(GPIO_MODER_MODER8_0 | GPIO_MODER_MODER8_1); // Clear bits
    GPIOB->MODER |= (GPIO_MODER_MODER8_1); // Set to alternate function mode
    GPIOB->OTYPER |= GPIO_OTYPER_OT8;            /* Output open-drain */
    GPIOB->AFR[1] |= (4 << (4 * (8 - 8))); // AF4 for I2C1_SCL
    GPIOB->PUPDR &= ~(GPIO_PUPDR_PUPD8);              /* With pull-ups */
    GPIOB->PUPDR |= GPIO_PUPDR_PUPD8_0;



    // Configure SDA
    GPIOB->MODER &= ~(GPIO_MODER_MODER9_0 | GPIO_MODER_MODER9_1); // Clear bits
    GPIOB->MODER |= (GPIO_MODER_MODER9_1); // Set to alternate function mode
    GPIOB->AFR[1] |= (4 << (4 * (9 - 8))); // AF4 for I2C1_SDA
    GPIOB->OTYPER |= GPIO_OTYPER_OT9;             /* Output open-drain */
    GPIOB->PUPDR &= ~(GPIO_PUPDR_PUPD9);              /* With pull-ups */
    GPIOB->PUPDR |= GPIO_PUPDR_PUPD9_0;

    I2C1->CR1 |= I2C_CR1_SWRST;		/* Software reset I2C1 */
    I2C1->CR1 &= ~I2C_CR1_SWRST; 	/* Out of reset */

    // Set clock speed
    I2C1->CR2 &= ~I2C_CR2_FREQ;
    I2C1->CR2 = 0x0010; // Assuming a 16 MHz system clock

    // Configure I2C speed (Standard mode or Fast mode)
    //I2C_CCR = 16000000 / 100000
    //I2C_CCR = 160
    //I2C_CCR = 160 - 1 = 159
    //for HEX I2C1->CCR = 0x80;

    I2C1->CCR = 0x80; // For 100 kHz (Standard mode)

    // TRISE = (1000 ns / (1 / 16,000,000 Hz)) + 1 = (1000 / (1 / 16,000,000)) + 1 = 16 + 1 = 17
    I2C1->TRISE = 17;     /* Maximum rise time */

    // Enable I2C peripheral
    I2C1->CR1 |= I2C_CR1_PE;
}


void I2C_byteRead(char saddr, char maddr, char *data)
{
	while (I2C1->SR2 & I2C_SR2_BUSY);          /* Wait until bus not busy */

    // Generate START condition
    I2C1->CR1 |= I2C_CR1_START;

    // Wait until the START condition is generated
    while (!(I2C1->SR1 & I2C_SR1_SB));

    // Send device address with Write bit (0)
    I2C1->DR = (saddr << 1);

    // Wait until the address is sent
    while (!(I2C1->SR1 & I2C_SR1_ADDR));
    (void)I2C1->SR2; // Clear ADDR bit

    // Send the register address
    I2C1->DR = maddr;

    // Wait until the data register is empty
    while (!(I2C1->SR1 & I2C_SR1_TXE));

    // Generate repeated START condition
    I2C1->CR1 |= I2C_CR1_START;

    // Wait until the repeated START condition is generated
    while (!(I2C1->SR1 & I2C_SR1_SB));

    // Send device address with Read bit (1)
    I2C1->DR = (saddr << 1) | 1;

    // Wait until the address is sent
    while (!(I2C1->SR1 & I2C_SR1_ADDR));
    (void)I2C1->SR2; // Clear ADDR bit

    // Generate STOP condition after data is rceived
	I2C1->CR1 |= I2C_CR1_STOP;

    // Wait until the data register is not empty that is receive is not empty
    while (!(I2C1->SR1 & I2C_SR1_RXNE));

    // Read the data
    *data++ = I2C1->DR;
}


int main()
{
	char data;

    // Initialize the system, GPIO, and other configurations
	 RCC->AHB1ENR |= RCC_AHB1ENR_GPIOGEN;

	 GPIOG->MODER &= ~(GPIO_MODER_MODER13_0 | GPIO_MODER_MODER13_1);

	 GPIOG->MODER |= GPIO_MODER_MODER13_0;


    I2C_Init();


    // Continue with your application logic

    while (1)
    {
    	// Read from the DS RTC module
    	I2C_byteRead(DS_DEVICE_ADDRESS, 0, &data);

    	if (data & 1)
    	{
    		GPIOG->ODR |= 0x00002000;
    	}
    	else
    	{
    		GPIOG->ODR &= ~0x00002000;
    	}
    }
}