Please execute the project in the folder in "Lab 2" to test Question No 2 of the assignment.

And execute "Lab2_interpolation" to test question 4 in the assignment.

Notes for testing question 2:

Default tearget position configured is 360. User can chnage the target position by entering R/r in the command window
'R' to increase the reference by 10 degree
Enter 
'r' to increase the reference by 10 degree

Similarly the gain values can be varied from the command window itself.
Enter 'P' to increase Kp by 1 

Enter 'p' to decrease Kp by 1 

Enter 'D' to increase Kd by 0.5

Enter 'd' to decrease Kd by 0.5


To view existing values:
Enter 'V' to view current values of Kd, Kp, Pr, Pm 

Once all the configurations are done, Enter 't' to Start the trajectory

Notes for testing question 4:

Similar to the previous problem, the gain values can be varied from the command window itself.
Enter 'P' to increase Kp by 1 

Enter 'p' to decrease Kp by 1 

Enter 'D' to increase Kd by 0.5

Enter 'd' to decrease Kd by 0.5

Interpolator is implemented by to perform following steps 
Step 1 : Rotate the motor forward 90 degrees
Step 2 : Held for 0.5 seconds
Step 3 : Rotate in reverse for 360 degree
Step 4 : Held for 0.5 seconds
Step 5 : Rotated forward for 5 degrees

Enter 't' to execute the interpolation by traversing the trajectory

Please note the the user will not be able to directly input the required reference position but he will be able to 
increase or decrease the reference position, 10 degree at a time.

All the graphs are drwan by printing the values on screen and transefering the values in to MATLAB. Matlab .m file has been attached in case anyone would like to varify the graph.s