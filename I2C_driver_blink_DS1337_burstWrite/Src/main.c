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


void I2C_burstWrite(char saddr, char maddr, int n, char *data)
{
	while (I2C1->SR2 & I2C_SR2_BUSY);          /* Wait until bus not busy */

	I2C1->CR1 &= ~I2C_CR1_POS; // Disable POS (packet error checking)

    // Generate START condition
    I2C1->CR1 |= I2C_CR1_START;

    // Wait until the START condition is generated
    while (!(I2C1->SR1 & I2C_SR1_SB));

    // Send slave device address with Write bit (0)
    I2C1->DR = (saddr << 1);

    // Wait until the address is sent
    while (!(I2C1->SR1 & I2C_SR1_ADDR));
    (void)I2C1->SR2; // Clear ADDR bit

    while (!(I2C1->SR1 & I2C_SR1_TXE)); // Wait until data register is empty

    // Send the register address
    I2C1->DR = maddr;

    int i;
    // Write all the data
    for (n = 0; i < n; i++)
    {
    	while (!(I2C1->SR1 & I2C_SR1_TXE)); // Wait until data register is empty

    	I2C1->DR = data[i]; // Transmit data
    }

    //!<Byte Transfer Finished
    while (!(I2C1->SR1 & I2C_SR1_BTF)); // Wait until transfer is finished

    I2C1->CR1 |= I2C_CR1_STOP; // Generate stop

}


int main()
{
	 /*                          00    01    02    03    04    05    06 */
	char timeDateToSet[8] = {0x55, 0x58, 0x10, 0x03, 0x26, 0x09, 0x17, 0x00};
								/* 2017 September 26, Tuesday, 10:58:55 */
    I2C_Init();

    I2C_burstWrite(DS_DEVICE_ADDRESS, 0, 7, timeDateToSet);


    // Continue with your application logic

    while (1)
    {

    }
}
