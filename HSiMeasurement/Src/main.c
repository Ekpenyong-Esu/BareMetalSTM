/**
 ******************************************************************************
 * @file           : main.c
 * @author         : Auto-generated by STM32CubeIDE
 * @brief          : Main program body
 ******************************************************************************
 * @attention
 *
 * Copyright (c) 2023 STMicroelectronics.
 * All rights reserved.
 *
 * This software is licensed under terms that can be found in the LICENSE file
 * in the root directory of this software component.
 * If no LICENSE file comes with this software, it is provided AS-IS.
 *
 *
 * 1. look for the base address of RCC
 * 2. look for the offset of RCC_CFGR_offset
 * 3. look for
 *
 ******************************************************************************
 */

#include <stdint.h>

#define RCC_Base_ADDR       0x40023800UL

#define RCC_CFGR_OFFSET     0x08UL

#define RCC_CFGR_REG_ADDR   (RCC_Base_ADDR + RCC_CFGR_OFFSET)

#define RCC_AHB1ENR_OFFSET   0x30UL

#define RCC_AHB1ENR_REG_ADD  (RCC_Base_ADDR + RCC_AHB1ENR_OFFSET)

#define GPIOA_MODE_REG_ADDR  (0x40020000UL + 0x00)



#if !defined(__SOFT_FP__) && defined(__ARM_FP)
  #warning "FPU is not initialized, but the project is compiling for an FPU. Please initialize the FPU before use."
#endif

int main(void)
{

	// take in the base address and the base address of clock configuration register as an address using a pointer
	uint32_t volatile *RCC_CFGR_REG = (uint32_t *)RCC_CFGR_REG_ADDR;

	//since the HSI is connected to MC01 in the clock tree. hence, for us to enable HSI we clear bit 21 and bit 22
	*RCC_CFGR_REG &= ~(0x3 << 21); 			// note that 0x03 = 0011



	*RCC_CFGR_REG |= (0x4 << 24);
	// since PA8 is connected to MCO1 externally, to access the HSI, we need to configure the alternate mode of PA8 that is alternate mode 0
	//to achieve this, we need to act=ivate the clock of PA8 first
	//PA8 is connected to AHB1 bus

	uint32_t *RCC_AHB1ENR_REG = (uint32_t *)RCC_AHB1ENR_REG_ADD;        // take in the address using pointer
	*RCC_AHB1ENR_REG |= (1 << 0);

	//Make the mode register off port A pin 8 as output by clearing and the setting the bit position to access the alternate function mode
	uint32_t *GPIOA_MODE_REG = (uint32_t *)GPIOA_MODE_REG_ADDR;

	*GPIOA_MODE_REG &= ~(0x3 << 16);  // clearing
	*GPIOA_MODE_REG |=  (0x2 << 16);  // setting the alternate function usin 2 == 10 in binary

	// We now set the alternate function register to 0 mde that will access MCO1
	uint32_t *GPIOA_AFRH = (uint32_t *)(GPIOA_MODE_REG_ADDR + 0x24);

	// setting the mode to 0
	*GPIOA_AFRH &= ~(0xf << 0);



    /* Loop forever */
	for(;;);
}
