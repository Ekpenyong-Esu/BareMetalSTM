/* p2_4.c Display number 75 on a 2-digit 7-segment common cathode LED.
 *
 * The segments are driven by Port C0-C6.
 * The digit selects are driven by PB0 and PB1.
 *
 * This program was tested with Keil uVision v5.24a with DFP v2.11.0
 */

#include "stm32f4xx.h"





#define RS 0x20     /* PB5 mask for reg select */
#define RW 0x40     /* PB6 mask for read/write */
#define EN 0x80     /* PB7 mask for enable */

void delayMs(uint16_t n);
void delayMs1(int n);
void LCD_command(unsigned char command);
void LCD_command_noPoll(unsigned char command);
void LCD_data(char data);
void LCD_init(void);
void LCD_ready(void);
void PORTS_init(void);

int main(void) {
    /* initialize LCD controller */
    LCD_init();

    while(1) {
        /* Write "Hello" on LCD */
        LCD_data('H');
        LCD_data('e');
        LCD_data('l');
        LCD_data('l');
        LCD_data('o');
        delayMs1(500);

        /* clear LCD display */
        LCD_command(1);

        delayMs1(500);
    }
}



void PORTS_init(void) {
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOBEN | RCC_AHB1ENR_GPIOCEN; //0x06;          /* enable GPIO B/C clock */

    /* PB5 for LCD R/S */
    /* PB6 for LCD R/W */
    /* PB7 for LCD EN */
    GPIOB->MODER &= ~(GPIO_MODER_MODER5_Msk | GPIO_MODER_MODER6_Msk | GPIO_MODER_MODER7_Msk);  //~0x0000FC00;    /* clear pin mode  port B pin5(moder5), pin6(moder6), pin7(moder7) */
    GPIOB->MODER |=  (GPIO_MODER_MODER5_0 | GPIO_MODER_MODER6_0 | GPIO_MODER_MODER7_0);  //0x00005400;    /* set pin output mode */
    GPIOB->BSRR = GPIO_BSRR_BR_7 | GPIO_BSRR_BR_6; //0x00C00000;       /* turn off EN and R/W */

    /* PC0-PC7 for LCD D0-D7, respectively. */
    GPIOC->MODER &= ~(GPIO_MODER_MODER0_Msk | GPIO_MODER_MODER1_Msk | GPIO_MODER_MODER2_Msk | GPIO_MODER_MODER3_Msk |
    		GPIO_MODER_MODER4_Msk | GPIO_MODER_MODER5_Msk | GPIO_MODER_MODER6_Msk | GPIO_MODER_MODER7_Msk	);//~0x0000FFFF;    /* clear pin mode */
    GPIOC->MODER |= (GPIO_MODER_MODER0_0 | GPIO_MODER_MODER1_0 | GPIO_MODER_MODER2_0 | GPIO_MODER_MODER3_0 |
    		GPIO_MODER_MODER4_0 | GPIO_MODER_MODER5_0 | GPIO_MODER_MODER6_0 | GPIO_MODER_MODER7_0 ); //0x00005555;    /* set pin output mode */
}




/* This function waits until LCD controller is ready to
 * accept a new command/data before returns.
 * It polls the busy bit of the status register of LCD controller.
 * In order to read the status register, the data port of the
 * microcontroller has to change to an input port before reading
 * the LCD. The data port of the microcontroller is return to
 * output port before the end of this function.
 */
void LCD_ready(void) {
    char status;


    /* Change to read configuration to poll the status register */
        GPIOC->MODER &= ~(GPIO_MODER_MODER0_Msk | GPIO_MODER_MODER1_Msk | GPIO_MODER_MODER2_Msk | GPIO_MODER_MODER3_Msk |
                          GPIO_MODER_MODER4_Msk | GPIO_MODER_MODER5_Msk | GPIO_MODER_MODER6_Msk | GPIO_MODER_MODER7_Msk); /* Clear pin mode for PC0-PC7 */
    GPIOB->BSRR = GPIO_BSRR_BR_5 << 16;  //RS << 16;         /* RS = 0 for status register */
    GPIOB->BSRR = GPIO_BSRR_BS_6;         //RW;               /* R/W = 1 for read */

    do {    /* stay in the loop until it is not busy */
        GPIOB->BSRR =  GPIO_BSRR_BS_7;           //EN;           /* pulse E high */
        delayMs1(1);
        status = GPIOC->IDR;        /* read status register */
        GPIOB->BSRR = GPIO_BSRR_BR_7 << 16;      //EN << 16;     /* clear E */
        delayMs1(1);
    } while (status & 0x80);        /* check busy bit */


    /* Return to default write configuration */
       GPIOB->BSRR = GPIO_BSRR_BR_6 << 16;     /* R/W = 0, LCD input */
       GPIOC->MODER |= GPIO_MODER_MODER0_0 | GPIO_MODER_MODER1_0 | GPIO_MODER_MODER2_0 | GPIO_MODER_MODER3_0 |
                       GPIO_MODER_MODER4_0 | GPIO_MODER_MODER5_0 | GPIO_MODER_MODER6_0 | GPIO_MODER_MODER7_0;  /* Port C as output */
   }




void LCD_command(unsigned char command) {
    LCD_ready();            /* wait for LCD controller ready */
    GPIOB->BSRR = (RS | RW) << 16;  /* RS = 0, R/W = 0 */
    GPIOC->ODR = command;           /* put command on data bus */
    GPIOB->BSRR = EN;               /* pulse E high */
    delayMs1(0);
    GPIOB->BSRR = EN << 16;         /* clear E */
}


/* This function is used at the beginning of the initialization
 * when the busy bit of the status register is not readable.
 */
void LCD_command_noPoll(unsigned char command) {
    GPIOB->BSRR = (RS | RW) << 16;  /* RS = 0, R/W = 0 */
    GPIOC->ODR = command;           /* put command on data bus */
    GPIOB->BSRR = EN;               /* pulse E high */
    delayMs1(0);
    GPIOB->BSRR = GPIO_BSRR_BR_7 << 16;          /* clear E */
}


/* Initialize port pins then initialize LCD controller */
void LCD_init(void) {
    PORTS_init();

    delayMs1(30);            /* initialization sequence */
    LCD_command_noPoll(0x30);   /* LCD does not respond to status poll yet */
    delayMs1(10);
    LCD_command_noPoll(0x30);
    delayMs1(1);
    LCD_command_noPoll(0x30);   /* busy flag cannot be polled before this */

    LCD_command(0x38);      /* set 8-bit data, 2-line, 5x7 font */
    LCD_command(0x06);      /* move cursor right after each char */
    LCD_command(0x01);      /* clear screen, move cursor to home */
    LCD_command(0x0F);      /* turn on display, cursor blinking */
}






void LCD_data(char data) {
    LCD_ready();            /* wait for LCD controller ready */
    GPIOB->BSRR = RS;               /* RS = 1 */
    GPIOB->BSRR = RW << 16;         /* R/W = 0 */
    GPIOC->ODR = data;              /* put data on data bus */
    GPIOB->BSRR = EN;               /* pulse E high */
    delayMs1(0);
    GPIOB->BSRR = EN << 16;         /* clear E */
}
