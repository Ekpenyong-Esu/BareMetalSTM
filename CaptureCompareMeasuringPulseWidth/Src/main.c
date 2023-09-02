
/**************
 * This program configure timer 2 CH1 to toggle PA5 and the TIM3 is set to do input capture
 * from PA6. The program wait for capture flag CCIF to set and then store the value in a global
 * variable to be watch by a deugger
 */

#include "stm32F4xx.h"
#include <stdio.h>
#include "USART.h"
#include <stdlib.h>

void delayMs(int n);

// Variables to store the captured values
uint32_t capture_value_rising = 0;
uint32_t capture_value_falling = 0;

uint32_t period;
float frequency;

int main(void)
{
	// Character buffer to store received data from UART
	 char txBuffer[100];

	 UART1_Init();

	 RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN; /* enable GPIOG clock */

	 // Configure PA5 as output of TIM2 CH1
	 GPIOA->MODER &= ~GPIO_MODER_MODE5; /* clear pin 5 of portA mode */

	 GPIOA->MODER |= GPIO_MODER_MODE5_1; /* set pin to alternate mode */

	 GPIOA->AFR[0] &= ~GPIO_AFRL_AFSEL5;  // Clear the alternate funtion fir pin 5
	 GPIOA->AFR[0] |= (1UL << GPIO_AFRL_AFSEL5_Pos); // AF1 (Alternate Function 1) = 001


	 /* Configure Timer2  */
	 RCC->APB1ENR |=RCC_APB1ENR_TIM2EN; // enable the timer 2 clock

	 // Note that system clock is 16'000'000 MHz
	 TIM2->PSC = 1600 -1;   //divide the system clock by 1600
	 TIM2->ARR = 10000 - 1; // THIS is use to divide the remaining value of the system clock and th clock run at
	 // 1Hz per 1 second

	 TIM2->CCMR1 |= 0x3UL << 4UL;  // Toggle Mode on CH1 when match with TIM2->CCR1 value
	 TIM2->CCR1 = 0;             // This is for the capture compare value when match

	 TIM2->CCER |= TIM_CCER_CC1E;  // Enable the output for Channel 1 of the chanel compare register
	 TIM2->CNT = 0; 				// THis is use to clear the timer

	 TIM2->CR1 |= TIM_CR1_CEN; // This enable tim2 in the control register

	 // Configure PA6 as input
	 GPIOA->MODER &= ~GPIO_MODER_MODE6; /* clear pin 6 of portA mode to make it input */
	 GPIOA->MODER |= GPIO_MODER_MODE6_1; /* set pin to alternate mode */

	 GPIOA->AFR[0] &= ~GPIO_AFRL_AFSEL6;  // Clear the alternate funtion fir pin 5
	 GPIOA->AFR[0] |= (2U << GPIO_AFRL_AFSEL6_Pos); // AF2 (Alternate Function 2) = 0010 -> tim3 is in alternate  function 3


	//Configure TIM3  to do input capture with prescaler
	RCC->APB1ENR |=RCC_APB1ENR_TIM3EN;  // Enable TIMER3 clock
	TIM3->PSC = 16000-1;				// Divide by 16000
	TIM3->CCMR1 = TIM_CCMR1_CC1S_0;     // Capture using channel 1


	// Configure the input capture as both edges triggered

	TIM3->CCER |= TIM_CCER_CC1NP; // CC1NP = 1: Capture on rising edge (negative polarity)
	TIM3->CCER |= TIM_CCER_CC1P; // CC1P = 1: Capture on falling edge

	TIM3->CCER |= TIM_CCER_CC1E;		// Enable input capture for CH1

	TIM3->CR1 |= TIM_CR1_CEN;           // Enable channel 3 using the control register



	//delayMs(10);

	while (1)
	{
		while (!(TIM3->SR & TIM_SR_CC1IF));		// this bit will be raise when there is a capture
		capture_value_rising = TIM3->CCR1;    	// Read the captured counter value

		// Clear the capture flag for the rising edge
		 TIM3->SR &= ~TIM_SR_CC1IF;

//		period = current - last;	     // This calculate the period
//		//delayMs(10);
//		last = current;                 // assign the current value to last for a new iteration
//		frequency = 1000.0f / period;
//		last = current;

		// Use memset to clear the txBuffer before sprintf
		memset(txBuffer, 0, sizeof(txBuffer));

		// Use sprintf to collect the integer into the character buffer
		//delayMs(10);
		UART1_SendChar('\n'); // Newline character
		UART1_SendChar('\r'); // Carriage return character
		sprintf(txBuffer, "The rising edge value is: %lu\n\r", capture_value_rising ); // Convert integer to string
		UART1_SendString(txBuffer);


		// Wait for capture event on Timer 3 (falling edge)
		while (!(TIM3->SR & TIM_SR_CC1IF));
		capture_value_falling = TIM3->CCR1;

		// Use memset to clear the txBuffer before sprintf
		memset(txBuffer, 0, sizeof(txBuffer));

		//delayMs(10);
		UART1_SendChar('\n'); // Newline character
		UART1_SendChar('\r'); // Carriage return character
		sprintf(txBuffer, "The falling edge value is: %lu\n\r", capture_value_falling); // Convert integer to string
		UART1_SendString(txBuffer);

		// Use memset to clear the txBuffer before sprintf
		memset(txBuffer, 0, sizeof(txBuffer));

		period = abs(capture_value_falling - capture_value_rising);
		// Calculate the period
		//period = (capture_value_falling > capture_value_rising) ? (capture_value_falling - capture_value_rising)
		//						: (capture_value_rising - capture_value_falling);

		// delayMs(10);
		UART1_SendChar('\r'); // Newline character
		UART1_SendChar('\n'); // Carriage return character
		sprintf(txBuffer, "The difference value is: %lu\n\r", period); // Convert integer to string
		UART1_SendString(txBuffer);

		UART1_SendChar('\r'); // Newline character
		UART1_SendChar('\n'); // Carriage return character

		// Clear the capture flag for the falling edge
		TIM3->SR &= ~TIM_SR_CC1IF;


	}
}









