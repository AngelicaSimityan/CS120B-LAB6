/*     Author: Angelica Simityan
 *  Partner(s) Name: 
 *      Lab Section:
 *      Assignment: Lab #  Exercise #
 *      Exercise Description: [optional - include for your own benefit]
 *
 *     I acknowledge all content contained herein, excluding template or example
 *     code, is my own original work.
 *    Demo: https://drive.google.com/drive/folders/1vHCaSJvltPWxV8gRPXcXScaQ_FrK0mHx?usp=sharing
 */

#include <avr/io.h>
#include <avr/interrupt.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif

void ADC()
{
	ADCSRA |= (1 << ADEN)  | (1 << ADSC) | (1 << ADATE);	
}

int Max= 255;

int main(void) {
    /* Insert DDR and PORT initializations */
	DDRA = 0x00; PORTA = 0xFF;
	DDRD = 0x00; PORTD = 0xFF;
	DDRB = 0xFF; PORTB = 0x00;

	ADC();
    
	while (1)
	{
		unsigned div = Max/8;
		unsigned short n = ADC;

		if (n >= Max)
		{
			PORTB = 0xFF;
		}
		else if (n >= ( div * 7))
		{
			PORTB = 0x7F;
		}
		else if (n >= ( div * 6))
		{
			PORTB = 0x3F;
		}
		else if (n >= ( div * 5))
		{
			PORTB = 0x1F;
		}
		else if (n >= ( div * 4))
		{
			PORTB = 0x0F;
		}
		else if (n >= ( div * 3))
		{
			PORTB = 0x07;
		}
		else if (n >= ( div * 2))
		{
			PORTB = 0x03;
		}
		else if (n >= ( div * 1))
		{
			PORTB = 0x01;
		}
		else {
			PORTB = 0x00;
		}
	}
    	return 1;
}
