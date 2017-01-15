/*
 * Timer.c
 *
 * Created: 2/2/2016 8:23:38 PM
 * Author : Shreeganeesh Bhat
 */

#include <avr/io.h>
#include <avr/interrupt.h>
#include<util/delay.h>

//Delay using for loop
#define LOOP_COUNT_FOR_300MS 4800000			//300ms Delay using for loop
#define LOOP_COUNT_FOR_400MS 6400000			//400ms Delay using for loop
#define LOOP_COUNT_FOR_500MS 8000000			//500ms Delay using for loop

#define _NOP() do { __asm__ __volatile__ ("nop"); } while (0)

#define Loop_Count_1ms 8
#define Loop_Count_1s 40
#define Output_Compare_RegT0 250
#define Output_Compare_RegT1 50000

#define BV(x)				(1<<x)
#define setBit(P,B)			(P |= BV(B))
#define clearBit(P,B)		(P &= ~BV(B))
#define toggleBit(P,B)		(P ~ = BV(B))

volatile uint32_t T0_tot_overflow;			// variable to count the number of overflows - Timer0
volatile int T1_tot_overflow;			// variable to count the number of overflows - Timer1
volatile uint32_t time_ms;				// variable to count which records the number of ms the system has been running.
int cliFlag = 0;

// initialize timer, interrupt and variable
void timer1_init()
{
	//Timer 1 initialization
	TCNT1 = 0;						// initialize timer 1 counter
	TIMSK1 |= (1 << OCIE1A);		// enable overflow interrupt
	TCCR1B |= (1 << CS11);			// set up timer with pre-scaler = 8
	TCCR0A |= (1 << WGM11);
	OCR1A = Output_Compare_RegT1;
	T1_tot_overflow = 0;
}

void timer0_init()
{
	//Timer 0 initialization
	TCNT0 = 0;						// initialize timer 0 counter
	TIMSK0 |= (1 << OCIE0A);		// enable  interrupt
	TCCR0A |= (1 << WGM01);
	TCCR0B |= (1 << CS01);
	OCR0A = Output_Compare_RegT0;
	T0_tot_overflow = 0;
}

ISR(TIMER1_COMPA_vect)
{
	// keep a track of number of overflows
	T1_tot_overflow++;
	
	// 40 overflows = 1 seconds delay (approx.)
	if (T1_tot_overflow >= Loop_Count_1s) 
	{
		PORTD ^= (1 << 5);  
		T1_tot_overflow = 0;   // reset overflow counter
	}
}

ISR(TIMER0_COMPA_vect)
{
	// keep a track of number of overflows
	T0_tot_overflow++;
	// 64  overflows = 1 ms delay
	if(T0_tot_overflow >= Loop_Count_1ms)
	{
		time_ms++;  
		T0_tot_overflow = 0;   // reset overflow counter
	}
}

ISR(PCINT0_vect)
{
	cliFlag =1;
}

int main(void)
{
	//Setting Data Direction Registers
	DDRD = 0xFF;
	DDRC = 0x80;
	DDRB = 0x00;
	setBit(PORTB,3);		// when pin's direction is INPUT, setting the pin to HIGH will turn on internal pull up	
	setBit(PCICR,0);
	setBit(PCMSK0,3);
	
	// initialize timer
	timer1_init();
	timer0_init();
	sei();
	
	while(1)
	{
		//Toggling an LED at 4kHz, i,e. at the rate of 0.25s
		if((time_ms % 500) > 250)
		{
			setBit(PORTC,7);		
		}	
		else
		{
			clearBit(PORTC,7);
		}		
		
		//Computationally intensive task 
		if((time_ms % 3000) > 2000)		//Polled every 2sec
		{
			if(cliFlag == 1)
			{
				setBit(PORTD,1);		//Turning RED LED ON at PORT D channel 1
				for(uint32_t i= 0; i<LOOP_COUNT_FOR_300MS;i++)
				{
					_NOP() ;
				}
				clearBit(PORTD,1);		//Turning RED LED OFF at PORT D channel 1
				
				setBit(PORTD,4);		//Turning GREEN LED ON at PORT D channel 4
				for(uint32_t i= 0; i<LOOP_COUNT_FOR_500MS;i++)
				{
					_NOP() ;
				}
				clearBit(PORTD,4);		//Turning GREEN LED OFF at PORT D channel 4
				
				setBit(PORTD,1);		//Turning RED LED ON again at PORT D channel 1
				for(uint32_t i= 0; i<LOOP_COUNT_FOR_400MS;i++)
				{
					_NOP() ;
				}
				clearBit(PORTD,1);		//Turning RED LED OFF at PORT D channel 1
				cliFlag =0;				//Clearing Interrupt flag that was set by the interrupt
			}
		}
	}
}