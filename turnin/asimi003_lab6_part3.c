/*     Author: Angelica Simityan
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

enum States {Begin, None, One, TwiceOne, Two, WaitOne, WaitTwo} State;

void TickCounter()
{
	switch(State)
	{
		case Begin:
			State = None;
			break;

		case None:
			if(~PINA == 0x00)
			{
				State = One;
			}
			else
			{
				State = WaitOne;
			}
			break;

		case One:
			if(~PINA == 0x00)
			{
				State = Two;
			}
			else
			{
				State = WaitOne;
			}
			break;

			case Two:
			if(~PINA == 0x00)
			{
				State = TwiceOne;
			}
			else
			{
				State = WaitOne;
			}
			break;
	
		case TwiceOne:
			if(~PINA == 0x00)
			{
              	  		State = None;
               	 	}
            	        else
			{
               	 		State = WaitOne;
             		}
                		break;

			case WaitOne:
			if(~PINA == 0x00)
			{
				State = WaitTwo;
			}
			else
			{
				State = WaitOne;
			}
			break;
		
		case WaitTwo:
			if(~PINA == 0x01)
			{
				State = None;
			}
			else
			{
				State = WaitTwo;
			}
			break;

			default:
				break;
	}

	switch(State)
	{ 
		case None:
			PORTB = 0x01;
			break;

		case One:
			PORTB = 0x02;
			break;

		case TwiceOne:
			PORTB = 0x02;
			break;

		case Two:
			PORTB = 0x04;
			break;
	
		case Begin:
			break;

		case WaitOne:
			break;

		case WaitTwo:
			break;
		
		default:
			break;

	}
}

int main(void)
{
    /* Insert DDR and PORT initializations */

	DDRA = 0x00; PORTA = 0xFF;
	DDRB = 0xFF; PORTB = 0x00;

	TimerSet(300);
	TimerOn();
	State = Begin;

    /* Insert your solution below */
	while (1)
   	 {
		TickCounter();
		while(!TimerFlag);
		TimerFlag = 0;
	}
	return 0;
}
