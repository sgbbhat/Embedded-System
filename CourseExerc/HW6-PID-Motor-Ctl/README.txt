Code Set Up
Set up a pwm to drive motor 2. (The direction pin for motor 2 is PE2.)
Set up PCINT ISR to track encoder counts. See CodeExamples/PololuWheelEncoders.h.
Set up ISR based on frequency of control task to generate motor drive signal (i.e. set duty cycle based on reference position). Do not run motor at full speed (maybe max out at 60 or 75% duty cycle). If you want to be clever, you can do stepwise increments of the duty cycle until you get close to the reference position, then gradually decrease the duty cycle until you arrive at your destination, but not required.
Watch for Button A and Button C press, OR watch for user key input ‘f and ‘r to move motor “forward” or “reverse” (which are arbitrary directions).
Create a trajectory buffer to store user commands input through button/key press. Implement the buffer in whichever way makes sense to you.

Hardware Set Up
Plug motor into motor 2 (green header, middle 2).
Plug encoder channel A and channel B into PCINT 4 and 5 (Arduino pins 8, 9). You don’t have to match channel with a specific pin, as either will work.

Code Functionality
Create a calibration routine that establishes “forward” and “reverse” on the motor, in which forward means that encoder values are increasing and reverse means encoder values are decreasing. You can do this by setting the direction of the motor, turn the motor on, and see if encoder values are increasing or decreasing.
Upon a button or key press, store the command in a buffer. Button A or key ‘f = forward; Button C or key ‘r = reverse.
While there are commands in the trajectory buffer, execute as follows (probably in main loop):
if “f” (you don’t have to use “f” - it might make more sense to store the direction value of 0 or 1)
set reference position = current position+360 deg (or use encoder counts).
remove command from buffer
move motor forward until reference position reached (don’t attempt to reverse if you went past it).
turn off pwm and let it coast to a stop using a delay.

if “r” (you don’t have to use “r”)
set reference position = current position-360 deg (or use encoder counts).
remove command from buffer
move motor backward until reference position reached (don’t attempt to go forward if you went past it).
turn off pwm and let it coast to a stop using a delay.


EXAMPLE of FUNCTIONALITY using button presses.

User presses button A.
‘f’ or its equivalent is stored in buffer.
main loop sets refP = mzP + 360*deg2enc based on the buffer.
ISR for motor control fires and sets duty cycle of PWM, as well as forward direction to get motor moving.
While moving forward, user presses button A again, followed by button C, which results in “f” and “r” being stored in buffer.
ISR continues to fire throughout, setting the PWM appropriately.
When motor has reached at least refP and coasted to a stop, main loop recognizes a new refP is needed, and sets refP = mzP + 360*deg2enc.
… process continues … ISR consistently firing .... user possibly adding more to trajectory buffer ...
When motor has reached at least refP and coasted to a stop, main loop sets refP = mzP-360*deg2enc.
