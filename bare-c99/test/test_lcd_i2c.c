/*
lcdpcf8574 lib sample

copyright (c) Davide Gironi, 2013

Released under GPLv3.
Please refer to LICENSE file for licensing information.
*/


#include <avr/io.h>
#include <stdio.h>
#include <stdlib.h>
// #include <avr/pgmspace.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#include "lcdpcf8574.h"


int main(void)
{
	//init uart
	// uart_init( UART_BAUD_SELECT(UART_BAUD_RATE,F_CPU) );

    sei();

    // uart_puts("starting...");

    //init lcd
    lcd_init(LCD_DISP_ON);

    //lcd go home
    lcd_home();

    uint8_t led = 0;
    lcd_led(led); //set led

	lcd_clrscr();
	/* int i = 0;
	for(i=0; i<40; i++) {
		lcd_command((1<<LCD_DDRAM)+LCD_START_LINE1+i);
		_delay_ms(500);
	}
	i = 64;
	for(i=64; i<104; i++) {
		lcd_command((1<<LCD_DDRAM)+LCD_START_LINE1+i);
		_delay_ms(500);
	} */

	lcd_gotoxy(5,0);
	lcd_puts("EPSEM  UPC");

	lcd_gotoxy(5,1);
	lcd_puts("VENTILATOR");

	lcd_gotoxy(0,2);
	lcd_puts("FREQ");

	lcd_gotoxy(7,2);
	lcd_puts("RATE");

	lcd_gotoxy(1,3);
	lcd_puts("10");

	lcd_gotoxy(8,3);
	lcd_puts("1:2");

    while(1) {
    	// lcd_led(led); //set led
    	// led = !led; //invert led for next loop

		

    	//test loop
    	/* int i = 0;
    	int line = 0;
    	for(i=0; i<10; i++) {
    		char buf[10];
    		itoa(i, buf, 10);
    		lcd_gotoxy(1, line);
    		lcd_puts("i= ");
    		itoa(i, buf, 10);
    		lcd_gotoxy(4, line);
    		lcd_puts(buf);
    		line++;
    		line %= 2;
        	// uart_puts(buf);
        	// uart_puts("\r\n");
    		_delay_ms(100);
    	} */
    }
}


