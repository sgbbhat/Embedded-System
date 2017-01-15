// required for _delay_ms()
#define F_CPU 16000000ul

//Include necessary files
#include <avr/io.h>
#include <util/delay.h>
#include <math.h>
#include <avr/interrupt.h>
#include<string.h>
#include<stdlib.h>

static char global_last_m2a_val;
static char global_last_m2b_val;

static char last_m2a_val;
static char last_m2b_val;

static int global_counts_m2;
static char global_error_m2;

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
	OCR1B = 1000;
}

void stop_pwm()
{
	TCCR1B &= ~(1 << CS10);
	TCCR1B &= ~(1 << CS11);
	TCCR1B &= ~(1 << CS12);
	OCR1A = 0;
	OCR1B = 0;
	TCNT1 = 0;						
	TCCR1A &= ~(1 << WGM10) ;		
	TCCR1A &= ~(1 << WGM11) ;
	TCCR1B &= ~(1 << WGM12) ;
	TCCR1B &= ~(1 << WGM13) ;
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

int main()
{
	// Initialize serial communication
	uart_init();
	
	// Setting motor 2 PMW as output
	DDRB |= (1<<PORTB6) ;
	
	// Setting PORTB3 as input - Clockwise Rotation - Button A and Pull up register
	DDRB &= ~(1<< PORTB3) ;
	PORTB |= (1<<PORTB3) ;
	
	// Setting PORTB0 as input - Anticlockwise Rotation - Button C and Pull up register
	DDRB &= ~(1<< PORTB0) ;
	PORTB |= (1<<PORTB0) ;
	
	// Enabling interrupt at PB4 and PB5
	PCMSK0 |= (1<<PCINT4) ;
	PCMSK0 |= (1<<PCINT5) ;
	
	// Enabling external interrupt
	PCICR |= (1<<PCIE0);
	sei();
	
	// Buffer for serial communication
	char buffer[20];
	
	// Buffer for keeping count when motor started
	int32_t forward_count = 0;
	int32_t reverse_count = 0;
	int32_t target_count  = 0;
	int motor_running_forward = 0;
	int motor_running_backward = 0;
	
	uart_print( "Welcome\n" );
	uart_print( "Press Button A to move motor BACKWARD\n" );
	uart_print( "Press Button C to move motor FORWARD\n" );
	
	while(1)
	{
		if(PINB & (1<<PINB3))			//Button A for reverse rotation
		{
		}
		else
		{
			if(motor_running_backward == 0)
			{
				_delay_ms(100);
				reverse_count = global_counts_m2;
				target_count = reverse_count - 2150 ;
				pwm_init();
				PORTE |= (1<<PORTE2);
				motor_running_backward = 1;
			}
		}
		
		if(PINB & (1<<PINB0))				//Button C for forward rotation
		{
		}
		else
		{
			if(motor_running_forward == 0)
			{
				_delay_ms(100);
				forward_count = global_counts_m2;
				target_count = forward_count + 2150;
				pwm_init();
				PORTE &= ~(1<<PORTE2);
				motor_running_forward = 1;
			}
		}
		
		//Keeping track of current rotation and stop when target is reached while running forward
		if((motor_running_forward == 1) & (target_count == global_counts_m2))
		{
			_delay_ms(100);
			stop_pwm();
			uart_print( "Count :" );
			itoa(global_counts_m2,buffer,10);
			uart_print(buffer);
			motor_running_forward = 0;
		}
		
		//Keeping track of current rotation and stop when target is reached while running backward
		if((motor_running_backward == 1) & (target_count == global_counts_m2))
		{
			_delay_ms(100);
			stop_pwm();
			uart_print( "Count :" );
			itoa(global_counts_m2,buffer,10);
			uart_print(buffer);
			motor_running_backward = 0;
		}
	}
	return 0;
}
