//Configuration parameters

#define GreenLEDBusyWait 0		//To set delay in Green LED ISR
#define YellowLEDBusyWait 0		//To set delay in Yellow LED ISR
#define GreenLEDPWMSP 100		//To set delay in PWM value for GREEN in ms

// required for _delay_ms()
#define F_CPU 16000000ul	
//Include necessary files
#include <avr/io.h>			
#include <util/delay.h>
#include <avr/pgmspace.h>
#include <math.h>
#include <avr/interrupt.h>
#include<stdlib.h>
#include<string.h>
#include<stdio.h>
#include "uart.h"
#include "hough.h"

//Global variable declaration
volatile uint32_t time_ms;					// variable to count which records the number of ms the system has been running
volatile uint32_t GreenLEDToggle;			//Number of toggles - GREEN LED
volatile uint32_t YellowLEDToggle;			//Number of toggles - YELLOW LED
volatile int rand_flag;						//Flag is set in case of an event
volatile int system_pause = 0;				//Pause a system in case of user input
volatile int RunHoughTr = 0;				//Flag indicating Time to run Hough Transform
int count_40Hz = 0;							//Flag to keep track of no of times 40Hz ISR executed - Turn on/off LED every 4 count
uint32_t missedRedDeadline = 0;				//Missed Deadlines counter - RED LED
uint32_t missedGreenDeadline = 0;			//Missed Deadlines counter - GREEN LED
uint32_t missedJitterDeadline = 0;			//Missed Deadlines counter - JITTER LED
uint32_t missedYellowDeadline = 0;			//Missed Deadlines counter - YELLOW LED
volatile unsigned long int _time_end = 0;	// 1ms timer counter
volatile unsigned long int _time_start = 0;	// 1ms timer counter
volatile double _actual_time = 0;			// 1ms timer counter

// transmit a char to uart
void uart_transmit( unsigned char data )
{
	// wait for empty transmit buffer
	while ( ! ( UCSR1A & ( 1 << UDRE1 ) ) )
	;
	// put data into buffer, sends data
	UDR1 = data;
}

// read a char from uart
unsigned char uart_receive(void)
{
	while (!( UCSR1A & ( 1 << RXC1) ))
	;
	return UDR1;
}

// init uart
void uart_init(void)
{
	// set baud rate
	
	UBRR1 = ((F_CPU/(16*38400)) - 1);
	
	// enable received and transmitter
	UCSR1B = ( 1 << RXEN1 ) | ( 1 << TXEN1 );
	
	// set frame format ( 8data, 2stop )
	UCSR1C = ( 1 << USBS1 ) | ( 3 << UCSZ10 );
}

// check if there are any chars to be read
int uart_dataAvailable(void)
{
	if ( UCSR1A & ( 1 << RXC1) )
	return 1;
	
	return 0;
}

// write a string to the uart
void uart_print( char data[] )
{
	int c = 0;
	
	for ( c = 0; c < strlen(data); c++ )
	uart_transmit(data[c]);
}

void timer0_init()
{
	//Timer 0 initialization
	TCNT0 = 0;						// initialize timer 0 counter
	TIMSK0 |= (1 << OCIE0A);		// enable  interrupt
	TCCR0A |= (1 << WGM01);
	TCCR0B |= (1 << CS01);
	TCCR0B |= (1 << CS00);
	OCR0A = 250;
}
//Tracking time in ms increments
ISR(TIMER0_COMPA_vect)
{
	time_ms++;
}

void timer3_init()
{
	//Timer 3 initialization
	TCNT3 = 0;						// initialize timer 0 counter
	TIMSK3 |= (1 << OCIE3A);		// enable  interrupt
	TCCR3B |= (1 << WGM32);
	TCCR3B |= (1 << CS31);
	TCCR3B |= (1 << CS30);
	OCR3A = 6250;
}
//ISR with frequency 40Hz
ISR(TIMER3_COMPA_vect)
{
	//random number genration and checking for an event
	//sei();
	if((rand() % 5) == 4)
	{
		if(rand_flag == 1)
			{
				missedJitterDeadline++;
			}
		rand_flag = 1;
		RunHoughTr = 1;
	}
	count_40Hz++;
	if(count_40Hz >= 4) 
	{
		PORTD ^= (1<<PORTD6);
		count_40Hz = 0;
		YellowLEDToggle++ ;
	}
	_delay_ms(YellowLEDBusyWait);
}

void send_char(char c)
{
	UDR1 = c;
}

void send_string(char s[])
{
	int i =0;
	
	while (s[i] != 0x00)
	{
		send_char(s[i]);
		i++;
	}
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
	TCCR1B |= (1 << CS11) ;			// set up timer with pre-scaler = 64
	TCCR1A |= (1 << COM1A0) ;
	OCR1A = (GreenLEDPWMSP * 250) ;
	TIMSK1 = 1<< OCIE1A;
}

//Timer 1 ISR
ISR(TIMER1_COMPA_vect)
{
	GreenLEDToggle++;
	_delay_ms(GreenLEDBusyWait);
}

//Pause system when menu otions are displayed
void stop_pwm()
{
	TCCR1B &= ~(1 << CS10);
	TCCR1B &= ~(1 << CS11);
	TCCR1B &= ~(1 << CS12);
}
//Pause system when menu otions are displayed
void stop_timer3()
{
	TCCR3B &= ~(1 << CS10);
	TCCR3B &= ~(1 << CS11);
	TCCR3B &= ~(1 << CS12);
}
//Pause system when menu otions are displayed
void stop_timer0()
{
	TCCR0B &= ~(1 << CS10);
	TCCR0B &= ~(1 << CS11);
	TCCR0B &= ~(1 << CS12);	
}

int main(void)
{
	unsigned char receivedChar = '0';
	char buffer[20];
	
	timer0_init() ;			//Initializing timer, counter, PWM and serial communication
	timer3_init() ;
	pwm_init() ;
	uart_init() ;
	
	DDRB |= 1<< PORTB4 ;	//GPIO RED LED
	DDRD |= 1<< PORTD6 ;	//GPIO YELLOW LED
	DDRB |= 1<< PORTB5 ;	//GREEN LED
	DDRC |= 1<< PORTC7 ;	//On-Board Yellow LED
	PORTB |= 1<< PORTB3 ;	//Pull-up

	sei();					//Enable global interrupt
	char dummyVar;
	uart_print( "Welcome :)\n" );

	while(1)
	{
		if((PINB & (1<<PORTB3)) && (system_pause == 0))
		{
			if(rand_flag)			//JITTER LED fag is high , i, e event has occurred
			{
				PORTC |= 1<< PORTC7 ;
				_delay_ms(5);
				PORTC &= ~(1 << PORTC7);
				rand_flag = 0;
			}
			if((time_ms % 200) > 100)		//RED LED toggling by keeping track of global variable time_ms
			{	
				PORTB |= 1<< PORTB4 ;
			}
			else
			{
				PORTB &= ~(1<< PORTB4) ;
			}
			
			// Hough Transform Starts
			if(RunHoughTr)
			{
				_time_start = time_ms;
				dummyVar = houghTransform( (uint16_t) &red, (uint16_t) &green, (uint16_t) &blue );
				RunHoughTr = 0;
				_time_end = time_ms;
				//WCET calculation
				_actual_time = _time_end - _time_start;
			}
			// Hough Transform Ends
			
		}
		else if(system_pause != 1)
		{
			_delay_ms(500);
			system_pause = 1;
			stop_pwm();
			stop_timer0();
			stop_timer3();
			uart_print( "Please ENTER one of the option:\n" );		//Menu options
			uart_print( "p : Print data collected for experiment\n" );
			uart_print( "e # : Set­Up this experiment number\n" );
			uart_print( "r # : Set the period of the GREEN LED Task to # ms\n" );
			uart_print( "z : Reset all variables to set up for a new experiment\n" );
			uart_print( "g : Go signal for start experiment\n" );
		}
		
		if ( uart_dataAvailable() )
		{
			// echo any received characters right back
			receivedChar = uart_receive();
			switch(receivedChar)
			{
				case 'p':
				uart_print( "Worst case Execution time of Hough Transform in ms :" );
				itoa(_actual_time,buffer,10);
				uart_print(buffer);
				uart_print( "\nNumber of Green LED toggles : " );
				itoa(GreenLEDToggle,buffer,10);
				uart_print(buffer);
				uart_print( "\nNumber of Yellow LED toggles : " );
				itoa(YellowLEDToggle,buffer,10);
				uart_print(buffer);
				missedGreenDeadline = abs((time_ms/100) - GreenLEDToggle);
				uart_print( "\nNumber of missed deadlines - GREEN LED : " );
				itoa(missedGreenDeadline,buffer,10);
				uart_print(buffer);
				missedYellowDeadline = abs((time_ms/100) - YellowLEDToggle);
				uart_print( "\nNumber of missed deadlines - YELLOW LED : " );
				itoa(missedYellowDeadline,buffer,10);
				uart_print(buffer);
				uart_print( "\nNumber of missed deadlines - JITTER LED : " );
				itoa(missedJitterDeadline,buffer,10);
				uart_print(buffer);
				break;
				case 'e':
				uart_print( "Please configure the variable GreenLEDBusyWait/YellowLEDBusyWait variable to change the experiment :\n" );
				break;
				case 'r':
				uart_print( "Please configure GreenLEDPWMSP to set PWM duty cycle time:\n" );
				break;
				case 'z':
				time_ms = 0;
				GreenLEDToggle = 0;
				TCNT1 = 0;
				missedGreenDeadline = 0;
				missedJitterDeadline = 0;
				missedRedDeadline = 0;
				missedYellowDeadline = 0;
				uart_print( "All variables set to zero :\n" );
				break;
				case 'g':
				uart_print( "Starting the experiment..\n" );
				pwm_init();
				timer0_init();
				timer3_init();
				uart_print( "Started\n" );
				break;
			}
			
			//Reinitialize after the user entry
			system_pause = 0;
			pwm_init();
			timer3_init();
			timer0_init();
		}
	}
	return 0 ;
}