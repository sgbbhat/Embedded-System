/*
 * GccApplication3.c
 *
 * Created: 1/30/2016 5:14:52 PM
 * Author : Shreeganeesh
 */ 

#include <avr/io.h>
#include<util/delay.h>
#include<avr/interrupt.h>

int cliFlag = 0;
int buttonPushCounterC = 0 ;
int currentButton = 0;
int previousButton = 0 ;

#define BV(x)				(1<<x)
#define setBit(P,B)			(P |= BV(B))
#define clearBit(P,B)		(P &= ~BV(B))
#define toggleBit(P,B)		(P ~ = BV(B))

int main(void)
{
	DDRC = 0x80;				//Configure PortA as an Output port
	DDRD = 0x24;  				//Configure PortD pin 7 as an output port
	DDRB = 0x00;				//Configure PortB for input
	PORTB |= 1 << PORTB3;		// when pin's direction is INPUT, setting the pin to HIGH will turn on internal pull up	
	PORTB |= 1 << PORTB0;	
	setBit(PCICR,0);
	setBit(PCMSK0,3);
		
    while (1)
    {		
		if(cliFlag==0)
		{		
			currentButton = (PINB & (1<<PORTB0));
			
			if(currentButton != previousButton)   // Maintain a counter and increment it after every press
			{
				buttonPushCounterC = buttonPushCounterC + 1;
			}
				
			if(buttonPushCounterC == 0)
			{
				;
			}
			else if(buttonPushCounterC == 1)
			{
				clearBit(PORTD,5);
				setBit(PORTC,7);
				_delay_ms(20000);
				setBit(PORTD,5);
				clearBit(PORTC,7);
				_delay_ms(20000);;
			}
			sei();
		currentButton = previousButton;
		}
		else
		{
			cli();
			_delay_ms(500);
			cliFlag = 0;
		}
    }
}

// interrupt service routine handler
ISR(PCINT0_vect)
{
	for(int i=0;i<10;i++)
	{
		setBit(PORTD,2);
		_delay_ms(5000);
		clearBit(PORTD,2);
		_delay_ms(5000);
	}
	cliFlag =1;
}
