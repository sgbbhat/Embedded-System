As part of this assignment, you will need to determine a loop count to create an accurate delay. 

Create a single program that contains the following periodic tasks

A timer that increments a global variable time_ms, which records the number of ms the system has been running.
This timer should be implemented with a timer/counter interrupt at a frequency of 1000 Hz.

Toggle an LED on the board at a frequency of 1 Hz. 
This task should be implemented inside an ISR for a timer/counter set to the frequency of the toggle. 
This should be a separate timer/counter from that used to maintain time_ms.

Toggle an LED on the board at a frequency of 4 Hz. This task should be implemented in the main loop. 
Set a timer to expire based on the global variable time_ms to determine when to toggle the LED. 
You should attempt to keep this toggling at the same rate even when the computationally intensive task is running

Poll for a button press at a “reasonable” rate. If you see the button press (i.e. pin reads high low), 
add in a very brief delay (i.e. a blind cycle) for the release of the button, then release the computationally intensive task. 
A release is performed by setting a global flag that is checked in the main loop. 

The periodic polling should be implemented inside an ISR - you can use either of the timer/counters set up for the other tasks.

A computationally intense pseudo task, that is executed inside the main loop. 
It is released by the polled event of the button press (ignore release of button). It contains these steps:

Turn on a red LED “plugged into” general I/O (see note below), then spin in a for-loop for 300 ms.
Turn off the red LED from (1), and turn on a green (or yellow) LED also in I/O, then spin in a for-loop for 500 ms.
Turn off the green/yellow, turn the red back on, then spin in a for-loop for 400 ms.
Turn off the green/yellow red LED. 

