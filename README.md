# Robotics Building Competition 2023
## Overview
**Starting Date:** 31st July, 2023

**Project Completion:** 20th August, 2023

**Motivation:** Self-motivated in order to learn about robotics

**Status:** Complete

This was our submission for the Monash [Robotics Building Competition](https://www.facebook.com/events/798509565110988) (RBC). It is a program for a simple Arduino robot to navigate a maze using ultrasonic and colour sensors. We implemented a flood-fill inspired algorithm to dynamically store the known parts of the map and calculate the optimal route.

This project was completed in collaboration with Angel Aguinaldo, Daniel Dinh, and Domico Wibowo.

![enter image description here](https://onedrive.live.com/embed?resid=BE406011F5E2A3C1!489725&authkey=!AI5N56Za__tKtoM&height=660)

## Context

The annual Monash RBC is hosted by the [SMEE](https://smee.club/) and [MECC](https://clubs.msa.monash.edu/organisation/7148/) student clubs. It is composed of three practical workshops in the weeks leading up, followed by a two day competition. At the start of the first day the challenge is revealed, and each team attempts to build an Arduino powered robot from supplied components and program it from scratch to complete the task.

This year the task was to develop a robot to navigate from the start to the end and then back. The maze was grid-based with known start and end coordinates. The ground tiles were colour coded to indicate the start and finish locations, and any tiles with a specified colour could have any adjacent walls removed before the robot returns to the start.

Before the event, I created a motor controller library in advance in order to make the build the program on top of it, as I knew that whatever the challenge was it would almost certainty involve motor movement

I was unable to attend for the first day so I decided to attempt as much programming as possible at night before coming to the second day. Together with Domico, I programmed for 6.5 hours until after 2am to completely implement a flood fill algorithm inspired by [this video](https://www.youtube.com/watch?v=ZMQbHMgK2rw). Although this concept was used, the actual implementation was completely designed and implemented by us without looking up any pseudocode.

The following day we fixed one major bug with the algorithm, integrated the colour reading code developed by Daniel and calibrated the program on the actual robot.

## Technical Description

The project was mostly coded in C++, with C headers for the custom libraries as required by the Arduino software.

The robot was controlled by an Arduino Uno microcontroller, and it used a colour sensor, 3 ultrasonic sensors, a LED, 2 simple motors, and a H-Bridge motor controller.

### Motor Controller

For the movement we were using a L298N H-Bridge to control two motors, which required 4 digital control signals and two PWM speed signals as input. This process was tedious and prone to small errors leading to lots of debugging. Instead of interacting at a signal level, I created a library to make a simpler interface to manage this process.

The interface is as intuitive as possible, with a  single `SetMotor(<LeftSpeed>, <RightSpeed>)` function. The two inputs are floats that range over [-1,1] to allow for forward and backward motion, along with stopping, to be achieved with one method.

The library also included the required initialisation functions, including a method to inverse the direction of the right of left motors through software rather than rewiring.
I decided on this because having to rewire a motor, particularly after everything else is also connected, is very tedious and often led to wasted time. These inversing methods allow this change to be done much quicker.

### Maze Navigator

The algorithm we developed stores whether there's a wall between each cell. At the start these are initialised to an unsure state.

For each step, the robot uses it's three ultrasonic sensors to check if there are walls ahead and to the sides. This data is then used to update the memory.

Next, the robot uses a recursive algorithm that gives each grid space a distance in tiles from the end goal. This is done using the following steps:

1. Set the end tile to 0
2. Set all other tiles to 100
3. For each of the four directions, check if there is a wall that way.
4. If there isn't a wall, check if the tile in that direction has a distance more than this tile's distance + 1
5. If this is true, set the tile in that direction to this tile's distance + 1, and repeat steps 3-5 on that new tile

This process should result in a array similar to the following image:
![enter image description here](https://onedrive.live.com/embed?resid=BE406011F5E2A3C1!489723&authkey=!ABroKkA8K8MkHX4&width=256)

Next the robot checks for an adjacent tile with a number one less than it's own without a wall in the way, and moves to that tile. This concludes a single step, and the entire process of scanning, calculating distances, and moving is repeated until the robot is in the target tile.

The major benefit of this approach is that backtracking is minimised as the robot remembers where it has already checked.

In addition, after reaching the end of the maze, all the tiles it passed that were the required colour to have walls removed are 'forgotten', and each of the surrounding walls are set back to a uncertain state. This allows most of the memory to be retained to optimise the way back to the start, while still checking the potentially new openings and walls.


## Results and Potential Improvements

The algorithm performed excellently in the text-based simulator we used to test the program. However, when it was run on the physical robot it completely failed. After a long time tinkering, we were unable to get the robot to the point where it could qualify due to a lack of precision.

This was due to inaccuracy of the motors, as they tended to veer to the side even when given the same PWM speed. Attempts to compensate by reducing one of the motor's speeds took several hours, but ultimately failed to achieve the required precision. We also rebuilt the robot from scratch to eliminate any friction on the wheel axles, which was a minor improvement but not significant enough.

A base assumption I made when conceptualising the algorithm was that the robot could move forward from the centre of one square to the centre of the next, and complete perfect 90 degree turns. In reality, physical constraints such as the varying floor types and slight slopes made these assumptions incorrect.

In the future I think either having a way to detect the direction and compensate for any error is essential. A gyroscopic sensor would solve this issue, or maybe using the ultrasonic sensors to align the robot to the walls.

Another issue we encountered was how the Github repository wasn't integrating well with the Arduino software, and wasn't set up properly. This led to team members either sharing code via messages or onto their own personal repositories, which made version control very cumbersome and slowed the development process. Next time this is something I plan to work on and test in advance.

Nevertheless, I'm very proud of how much we accomplished in that short period and hope to participate in the next robotics competition.

## Running the Program

The program can be run with an Arduino Uno, however note that the final version has a lot of hardcoded values such as pin maps, PWM speeds, and colour calibrations that are different for each robot.

A text version of the algorithm was developed for testing, and it can be seen by running [MazeNavigator.exe](https://github.com/ShmuelNeumann/robotics-comp-2023/blob/main/Main/MazeNavigator.exe) in a terminal.
