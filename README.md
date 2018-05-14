# Real_Time_Task_Scheduling
The overall purpose of this project is to implement a traffic light using the TS-7250 board and a auxiliary board. This auxiliary board contains three lights which represent the signals for one direction, the other direction, and pedestrians. When a light is turned on it represents a green light or WALK sign and when turned off it represents a red light. Also, the far left button on the auxiliary board represents the push button for the pedestrians to use when they wish to cross the street. The lights on the board are connected to the top three pins of Port B on the board. Be sure to only change the direction of these three pins when you use the lights.

Part 1. Polled Schedulling
For part 1, set up a single real time task that acts as a scheduler. It should turn on the light corresponding to one direction, then the light for the other direction, and then check if the pedestrian light needs to be turned on by checking the status of the button.

Part 2. Priority Scheduling
For Part 2, create three real time tasks, where each task is responsible for turning on and off one of the lights. Because the lights are a shared resource for all tasks, it needs to implement some sort of protection to ensure that no more than one light can be on at any given time.
