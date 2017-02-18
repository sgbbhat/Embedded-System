REQUIREMENTS:
Modify the brightness of one LED using PWM. Blink that same LED using an ISR. 
Blink another LED (full brightness) by releasing a task.

Set up a pwm to drive motor 2. (The direction pin for motor 2 is PE2.)
Set up PCINT ISR to track encoder counts. 
Set up ISR based on frequency of control task to generate motor drive signal (i.e. set duty cycle based on reference position). 

Do not run motor at full speed (maybe max out at 60 or 75% duty cycle). If you want to be clever, 
you can do stepwise increments of the duty cycle until you get close to the reference position, 
then gradually decrease the duty cycle until you arrive at your destination, but not required.
Watch for Button A and Button C press, OR watch for user key input ‘f and ‘r to move motor “forward” or “reverse” 
(which are arbitrary directions). Create a trajectory buffer to store user commands input through button/key press. 
Implement the buffer in whichever way makes sense to you.
