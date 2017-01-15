#include "LEDs.h"
#include "timer.h"
#include "usart.h"
#include "menu.h"

#include <avr/io.h>         //gives us names for registers
#include <avr/interrupt.h>

//Gives us uintX_t (e.g. uint32_t - unsigned 32 bit int)
//On the ATMega128 int is actually 16 bits, so it is better to use
//  the int32_t or int16_t so you know exactly what is going on
#include <inttypes.h> //gives us uintX_t

// useful stuff from libc
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(void) {
	// -------------------------------------------------------------
	
	//communication: initialize serial port 1, and open up stdout/stdin
	init_USART1();
	fdevopen(USART1_stdio_send, USART1_stdio_get);

	// initialize everything else
	init_menu();
	
	//enable interrupts
	sei();
		    
	while (1) {

		/* BEGIN with a simple toggle using for-loops. No interrupt timers */

		// toggle the LED. Increment a counter.
		LED_TOGGLE(RED);
		G_red_toggles++;
		printf("%d ",G_red_toggles);
		delay_ms(50);

		if(USART1_input_ready()) {
			menu();
		}
					
	} //end while loop
} //end main

