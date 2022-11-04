Simultaneous Localization And Mapping (SLAM)

As robots move around, they have a bit of an idea of where they want to go, but real-world conditions mean that the robot doesn’t always end up where it is intended to go.
Problems can occur where one or both wheels or track slip and the robot turns a little bit while traveling or doesn’t travel as far as it had intended.

SLAM tries to address this issue by recording a map of the surroundings as seen by the robot's sensors, then comparing the sensor readings after a move and the expected position.
As part of the comparison, the robot should be able to detect an error in the actual position and then update the map with new information gathered by its sensors.
In this way, the robot can correct its course to place the robot back on track to reach its destination.

This repository has been created to hold documentation and sample code on a prototype SLAM program.
The test robot is based on MR-4, a small tracked robot to start learning about mobile robots.
Created by Tim Clark, and is licensed under cc. the project can be found at https://www.thingiverse.com/thing:1906831
