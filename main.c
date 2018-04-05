  /*

	Demo of glcd library with AVR8 microcontroller
	
	Tested on a custom made PCB (intended for another project)

	See ../README.md for connection details

*/

#include <avr/io.h>
#include "glcd/glcd.h"
#include <avr/interrupt.h>
#include <stdio.h>
#include <stdlib.h>
#include "glcd/fonts/Liberation_Sans15x21_Numbers.h"
#include "glcd/fonts/font5x7.h"
#include <avr/pgmspace.h>
#include "I2C.h"

#define F_CPU 16000000UL  // 1 MHz


/* Function prototypes */
static void setup(void);

uint8_t address = 0;
uint8_t yaxe = 0;

static void setup(void)
{
	/* Set up glcd, also sets up SPI and relevent GPIO pins */
	glcd_init();
}

char string[30] = "";

int main(void)
{
	
	
	/* Backlight pin PL3, set as output, set high for 100% output */
	DDRB |= (1<<PB2);
	PORTB &= ~(1<<PB2);
	//PORTB &= ~(1<<PB2);
	 
	DDRD &= ~((1<<PD6) | (1<<PD2) | (1<<PD5)); 	//Taster 1-3
	PORTD |= ((1<<PD6) | (1<<PD2) | (1<<PD5)); 	//PUllups für Taster einschalten
	
	DDRD &= ~(1<<PD4); //T0 Counter Input
	TCCR0B |= (1<<CS02) | (1<<CS01) | (1<<CS00);//Counter 0 enabled clock on rising edge
	
	//Timer 1 Configuration
	OCR1A = 0x009C;	//OCR1A = 0x3D08;==1sec
	
    TCCR1B |= (1 << WGM12);
    // Mode 4, CTC on OCR1A

    TIMSK1 |= (1 << OCIE1A);
    //Set interrupt on compare match

    TCCR1B |= (1 << CS12) | (1 << CS10);
    // set prescaler to 1024 and start the timer
	
	
	
	setup();
	
	glcd_clear();
	glcd_write();
	TWIInit();
	
	
	glcd_tiny_set_font(Font5x7,5,7,32,127);
	glcd_clear_buffer();
	
	glcd_write();
	
	
	while(address < 256) 
	{	TWIStart();
		TWIWrite(address);
		if (TWIGetStatus() == 0x18)
		{
			sprintf(string,"0x%x",address);
			glcd_draw_string_xy(0,yaxe,string);
			yaxe += 8;
		}
		TWIStop();
		address++;
		
		glcd_write();
	}//End of while
	while(1)
	{
	}
	
	
	return 0;
}//end of main