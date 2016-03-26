// required for _delay_ms()
#define F_CPU 16000000ul

//Include necessary files
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include<string.h>
#include<stdlib.h>
#include "ftoa.h"
#include "uart.h"

static char global_last_m2a_val;
static char global_last_m2b_val;

static char last_m2a_val;
static char last_m2b_val;

static int global_counts_m2;
static char global_error_m2;

volatile int32_t target_degree = 360 ;
volatile int Kp = 20;
volatile float Kd = 3.5;
volatile int32_t time_ms;

volatile int32_t count_time1 = 0 ;
volatile int32_t count_time2 = 0 ;

void timer0_init()
{
	//Timer 0 initialization
	TCNT0 = 0;						// initialize timer 0 counter
	TIMSK0 |= (1 << OCIE0A);		// enable  interrupt
	TCCR0A |= (1 << WGM01);
	TCCR0B |= (1 << CS01);
	TCCR0B |= (1 << CS00);
	OCR0A = 62;
}
//Tracking time in ms increments
ISR(TIMER0_COMPA_vect)
{
	time_ms++;
	
	if((time_ms % 6)== 0)
	{
		count_time1 = global_counts_m2;
	}
	else if((time_ms % 6)== 3)
	{
		count_time2 = global_counts_m2;
	}
}

void pwm_init(void);

int main()
{
	// Initialize serial communication
	uart_init();
	pwm_init();
	timer0_init();
	// Setting motor 2 PMW as output
	DDRB |= (1<<PORTB6) ;
	
	// Enabling interrupt at PB4 and PB5
	PCMSK0 |= (1<<PCINT4) ;
	PCMSK0 |= (1<<PCINT5) ;
	
	// Enabling external interrupt
	PCICR |= (1<<PCIE0);
	sei();
	
	// Buffer for serial communication
	char str[10];
	int printed = 0;
	
	// Buffer for keeping count when motor started
	unsigned char receivedChar = '0';
	int run_motor = 2;
	int32_t error = 2 ;
	int print_menu = 0 ;
	int32_t target_count = target_degree*7;
	int32_t T; 
	
	uart_print( "Welcome\n" );
	uart_print("Target position in degree : ");
	itoa(target_degree,str,10);
	uart_print(str);
	uart_print( "\n" );
	uart_print("Target position in counts : ");
	itoa(target_count,str,10);
	uart_print(str);
	uart_print( "\n" );
	uart_print("Current position in count : ");
	itoa(global_counts_m2,str,10);
	uart_print(str);
	uart_print( "\n" );
	
	while(1)
	{
		error = target_count - global_counts_m2;
		if(print_menu == 0)
		{
			uart_print( "Enter 'R' to increase the reference by 10 degree\n" );
			uart_print( "Enter 'r' to increase the reference by 10 degree\n" );
			uart_print( "Enter 'P' to increase Kp by 1 \n" );
			uart_print( "Enter 'p' to decrease Kp by 1 \n" );
			uart_print( "Enter 'D' to increase Kd by 0.5\n" );
			uart_print( "Enter 'd' to decrease Kd by 0.5\n" );
			uart_print( "Enter 'V' to view current values of Kd, Kp, Pr, Pm \n" );
			uart_print( "Enter 't' to Start the trajectory" );
			print_menu = 1;
		}
		
		if(uart_dataAvailable())
		{
			receivedChar = uart_receive();
			switch(receivedChar)
			{
				case 'R':
				if(printed == 0)
				{
					target_count = target_count + 70 ;
					target_degree = target_degree + 10;
					uart_print( "New reference position set to(in degrees) : \n" );
					itoa(target_degree, str, 10);
					uart_print(str);
				}
				break;
				case 'r':
				if(printed == 0)
				{
					target_count = target_count - 70;
					target_degree = target_degree - 10;
					uart_print( "New reference position set to : \n" );
					itoa(target_degree, str, 10);
					uart_print(str);
				}
				break;
				case 'P':
				if(printed == 0)
				{
					Kp = Kp + 1;
					uart_print( "New value of kp : \n" );
					ftoa(Kp, str, 1);
					uart_print(str);
					//printed = 1;
				}
				break;
				case 'p':
				if(printed == 0)
				{
					Kp = Kp -1 ;
					uart_print( "New value of kp : \n" );
					ftoa(Kp, str, 1);
					uart_print(str);
					//printed = 1;
				}
				break;
				case 'D':
				if(printed == 0)
				{
					Kd = Kd + 0.5;
					uart_print( "New value of kd : " );
					ftoa(Kd, str, 1);
					uart_print(str);
					//printed = 1;
				}
				break;
				case 'd':
				if(printed == 0)
				{
					Kd = Kd - 0.5;
					uart_print( "New value of kd : \n" );
					ftoa(Kd, str, 1);
					uart_print(str);
					//printed = 1;
				}
				break;
				case 'V':
				if(printed == 0)
				{
					uart_print( "Current value of kp : " );
					ftoa(Kp, str, 1);
					uart_print(str);
					uart_print( "\n" );
					uart_print( "Current value of kd : " );
					ftoa(Kd, str, 1);
					uart_print(str);
					uart_print("\n");
					uart_print( "Measured position 'Pm' in count : " );
					itoa(global_counts_m2,str,10);
					uart_print(str);
					uart_print( "\n" );
					uart_print( "Reference position in count : " );
					itoa(target_count,str,10);
					uart_print(str);
					uart_print( "\n" );
					uart_print( "Reference position in degree : " );
					itoa(target_degree,str,10);
					uart_print(str);
				}
				break;
				case 't':
				if(printed == 0)
				{
					if(target_count != global_counts_m2)
					{
						run_motor = 1;
					}
				}
				break;
			}
		}
		
		if(run_motor == 1)
		{
			error = abs(global_counts_m2 - (target_count)) ;
			if(global_counts_m2 > target_count)
			{
				T = error*Kp + Kd*abs((count_time2 - count_time1)/(0.004));
				if(T < 10000)
				{
					OCR1B = error*Kp + Kd*abs((count_time2 - count_time1)/(0.004));
					PORTE |= (1<<PORTE2);
				}
				else
				{
					OCR1B = 10000;
					PORTE |= (1<<PORTE2);
				}

			}
			else if(global_counts_m2 < target_count)
			{
				T = error*Kp + Kd*abs((count_time2 - count_time1)/(0.004));
				if(T < 10000)
				{
					OCR1B = error*Kp + Kd*abs((count_time2 - count_time1)/(0.004));
					PORTE &= ~(1<<PORTE2);					
				}
				else
				{
					OCR1B = 10000;
					PORTE &= ~(1<<PORTE2);					
				}

			}
		}
		if(error < 5)
		{
			printed = 0;
		}		
	}
	return 0;
}

// Reading
ISR(PCINT0_vect)
{
	unsigned char m2a_val;
	unsigned char m2b_val;
	char plus_m2;
	char minus_m2 ;
	
	// Determine the current value of all channels.
	m2a_val = ((PINB & (1<<PINB4))>>PINB4);
	m2b_val = ((PINB & (1<<PINB5))>>PINB5);
	
	// Determine if the last read of one channel is different from the current read of the other channel
	// which tells you whether you need to add or subtract from encoder count
	
	plus_m2 = m2a_val^last_m2b_val;
	minus_m2 = m2b_val^last_m2a_val;
	
	// add and/or subtract as determined above
	if(plus_m2)
	{
		global_counts_m2++;
	}
	if(minus_m2)
	{
		global_counts_m2--;
	}
	// do some error checking to see if you missed an interrupt or something is wrong
	if(m2a_val != global_last_m2a_val && m2b_val != global_last_m2b_val)
	{
		global_error_m2 = 1;
	}
	// save state for next interrupt
	last_m2a_val = m2a_val;
	last_m2b_val = m2b_val;
}

//Initialize PWM
void pwm_init()
{
	TCNT1 = 0;						//Timer 1 initialization
	TCCR1A |= (1 << WGM10) ;		//Fast PWM mode - Mode 15
	TCCR1A |= (1 << WGM11) ;
	TCCR1B |= (1 << WGM12) ;
	TCCR1B |= (1 << WGM13) ;
	TCCR1B |= (1 << CS10) ;			// set up timer with pre-scaler = 64
	TCCR1A |= (1 << COM1B1) ;
	OCR1A = 10000;
}