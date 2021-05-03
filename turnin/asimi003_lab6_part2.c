/*      Author: Angelica Simityan
 *  Partner(s) Name: 
 *      Lab Section:
 *      Assignment: Lab #  Exercise #
 *      Exercise Description: [optional - include for your own benefit]
 *
 *     I acknowledge all content contained herein, excluding template or example
 *     code, is my own original work.
 *    Demo: https://drive.google.com/drive/folders/12kmxRGg9vPK05ZZhQIA1T9mxeqPqj4Yi?usp=sharing
 */

#include <avr/io.h>
#include <avr/interrupt.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif

volatile unsigned char TimerFlag=0;
unsigned long _avr_timer_M=1;
unsigned long _avr_timer_cntcurr=0;

void TimerOn()
{
	TCCR1B=0x0B;
	OCR1A=125;
	TIMSK1=0x02;
	TCNT1=0;
	_avr_timer_cntcurr=_avr_timer_M;
	SREG |=0x80;
}

void TimerOff()
{
	TCCR1B=0x00;
}

void TimerISR() { TimerFlag = 1;}

ISR(TIMER1_COMPA_vect)
{
	_avr_timer_cntcurr--;
	
	if(_avr_timer_cntcurr==0)
	{
		TimerISR();
		_avr_timer_cntcurr=_avr_timer_M;
	}
}

void TimerSet(unsigned long M)
{
	_avr_timer_M=M;
	_avr_timer_cntcurr=_avr_timer;
} 

enum States {Add, AddWait, Sub, SubWait, Wait, Zero} State;

void TickCounter()
{
	switch(State)
	{
		case Wait:
        		if(~PINA == 0x01)
		{
       		 	State = Add;
     		}
		else if(~PINA == 0x02)
		{
			State = Sub;
		}
		else if(~PINA == 0x03)
		{
			State = Zero;
		}
        		break;

        		case Add:
		if(~PINA == 0x03)
		{
			State = Zero;
		}
		else if(~PINA == 0x02)
		{
			State = Sub;
		}
		else if(~PINA == 0x00)
		{
			State = Wait;
		}
		else
		{
			State = AddWait;
		}
		break;

		case AddWait:
		if(~PINA == 0x00)
		{
			State = Wait;
			i = 0x00;
		}
		else if(~PINA == 0x02)
		{
			State = Sub;
			i = 0x00;
		}
		else if(~PINA == 0x03)
		{
			State = Zero;
			i =0x00;
		}
		else if(~PINA == 0x01 && i < 9)
		{
			State = AddWait;
			++i;
		}
		else if(~PINA == 0x01 && i <= 9)
		{
			State = Add;
			i = 0x00;
		}
		break;

		case Sub:
		if(~PINA == 0x03)
		{
			State = Zero;
		}
		else if(~PINA == 0x01)
		{
			State = Add;
		}
		else if(~PINA == 0x00)
		{
			State = Wait;
		}
		else
		{
			State = SubWait;
		}
		break;

		case SubWait:
		if(~PINA == 0x00)
		{
			State = Wait;
			i = 0x00;
		}
		else if(~PINA == 0x01)
		{
			State = Add;
			i = 0x00;
		}
		else if (~PINA == 0x03)
		{
			State = Zero;
			i = 0x00;
		}
		else if(~PINA == 0x02 && i < 9)
		{
			State = SubWait;
			++i;
		}
		else if(~PINA == 0x02 && i <= 9)
		{
        			State = Sub;
        			i = 0x00;
		}
		break;

		case Zero:
		State = Wait;	
		if(~PINA == 0x01)
		{
        			State = Add;
		}
        		else if(~PINA == 0x02)
		{
        			State = Sub;
        		}
        		else if(~PINA == 0x03)
		{
			State = Zero;
        		}
        		break;

        		default:
        			break;
        }

	switch(State)
	{
		case Add:
		if(PORTB < 9)
		{
			++PORTB;
		}
		break;

		case Sub:
		if(PORTB > 0)
		{
			--PORTB;
		}
		break;

		case Zero:
		PORTB = 0x00;
		break;

		case Wait:
		break;

		case AddWait:
		break;

		case SubWait:
		break;

		default:
		break;
	}

}



int main(void) {
    /* Insert DDR and PORT initializations */
	DDRA = 0x00; PORTA = 0xFF;
	DDRB = 0xFF; PORTB = 0x07;
	State = Wait;

	TimerSet(100);
	TimerOn();

    /* Insert your solution below */
    while (1) {
	TickCounter();
	
	while(!TimerFlag);
	TimerFlag = 0;
    }
    return 0;
}
