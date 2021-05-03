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
	ADCSRA |= (1 << ADEN) | (1 << ADSC) | (1 << ADATE);
}

int main(void)
{
    /* Insert DDR and PORT initializations */
	
	DDRB = 0xFF; PORTB = 0x00; 
	DDRD = 0xFF; PORTD = 0x00;
    
	ADC();
    
	while (1)
	{
		unsigned short n = ADC;

		PORTB = (char)n;
		PORTD= (char)(n >> 8);
   	 }
    
	return 1;
}
