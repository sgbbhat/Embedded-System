REQUIREMENTS:

Blink an LED (plugged into general I/O) using the main loop at a rate of .5 Hz. 
Use the LEDs on the board, setting them up using DDRx and turning off and on with PORTx.
ISR based on button A. If button A is pushed and released, then blink a different LED (general I/O) 
10 times on/off with an empty for-loop for delay (time it for about 4 Hz). 
This happens inside the ISR. NOTE: ISR will fire on both press and release of button. 

The LED should blink only after the release of the button.
Create rudimentary menu that changes which color is blinking (or port) at 4 Hz. 
Currently there is no terminal-based UI capability, thus you can use button presses as a 
means to change LEDs (I will leave it as your choice to change what you like - color, frequency, or 
maybe an interesting pattern of blinking LEDs.)
