/*
 * uart.h
 *
 * Created: 3/21/2016 12:33:06 PM
 *  Author: Shreeganeesh
 */ 



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
	for (int c = 0; c < strlen(data); c++ )
	uart_transmit(data[c]);
}