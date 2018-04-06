////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//		Silvan Nägeli
//	
//		06.04.2018
//
//		main.c / I2C-scanner
//
//		github: https://github.com/Naegi88
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////




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

	


char string[30] = "";

// Variabeln Deklarieren
uint8_t address = 0;			
uint8_t yaxe = 0;
uint8_t i = 0;

int main(void)
{
	
	
	/* Backlight pin PL3, set as output, set high for 100% output */
	DDRB |= (1<<PB2);
	PORTB |= (1<<PB2);
	//PORTB &= ~(1<<PB2);
	
	DDRD &= ~((1<<PD6) | (1<<PD2) | (1<<PD5)); 	//Taster 1-3
	PORTD |= ((1<<PD6) | (1<<PD2) | (1<<PD5)); 	//PUllups für Taster einschalten
	
	// Init 
	glcd_init();
	glcd_clear();
	glcd_write();
	TWIInit();	
	
	// lcd schrift wählen
	glcd_tiny_set_font(Font5x7,5,7,32,127);
	glcd_clear_buffer();
	
	
	
	// Suchprozes
	for(i = 0; i < 255;i++)
	{	TWIStart();
		TWIWrite(address);
		if (TWIGetStatus() == 0x18)
		{
			delay_ms(100);
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