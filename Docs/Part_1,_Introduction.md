# Part 1, Introduction
MR-4 is a small tracked robot to start learning about mobile robots.
Created by [Tim Clark](https://www.thingiverse.com/timmiclark), and is licensed under cc. the project can be found at https://www.thingiverse.com/thing:1906831

I’ve built the robot with the sensor body option and the body default parts.
I printed the bulk of the larger parts in PLA with the tracks in TPE.
The purpose of building this small robot was to study the concepts of Simultaneous Localization And Mapping. SLAM

As robots move around, they have a bit of an idea of where they want to go, but real-world conditions mean that the robot doesn’t always end up where it is intended to go.
Problems can occur where one or both wheels or track slip and the robot turns a little bit while traveling or doesn’t travel as far as it had intended.

One of the terms we will use is Pose and Pose estimation.
Our robot here doesn’t have and arms or legs, so how can it pose?
In the case of our tracked robot, the pose is both the X-Y coordinates and the angle the robot is facing.


# SLAM 

tries to address this issue by recording a map of the surroundings as seen by the robot's sensors, then comparing the sensor readings after a move and the expected position.
As part of the comparison, the robot should be able to detect an error in the actual position and then update the map with new information gathered by its sensors.
In this way, the robot can correct its course to place the robot back on track to reach its destination.


# Maps.

How do we record the map?
There are several possible solutions, each taking a different amount of memory depending on how it is recorded.

## Grid Map.

The simplest method is to create a grid of map points and place a value to represent the location, this could be a simple 1 or 0 for there is an object in the way, to an altitude, or a type of object.
The downside of this type of map is the storage space needed to record the info. The higher the resolution the greater the storage space that is needed.  This is okay for a very small map like in a tic-tack-toe game, but not for a real-world map.
A grid cell would be in the form of
  `int map[int x, int y]`
where x and y are the grid coordinate.
When choosing this method you first need to work out the map resolution, the map potential size as well. 
A simple map of an empty room 4 meters x 5 meters with door in one of the walls 800mm wide (80cm) using a resolution of 1 cm would result in a map of 400 x 500 cm or 20,000,000 cm²  Assuming a small micro controller where an Integer is 8 bits in size, that room would require 20MBytes to store.  Truth is, not knowing the starting point of the room, this will have to be 4 times larger as it make sense the robot would start the mapping process thinking it is in the centre of the map.


## Point Map.

A little more complex, but uses less memory than the Grid Map.
In this method, we record everywhere we have seen an object and the coordinates of that object.
`float map[int id, int dataType]`
In this method, we would have the `dataType enum{X, Y, entryType}`
The issue here is that a wall can have many points along it.


## Line Map.

In this type of map, each object is considered to be a line that cannot be crossed.
`float map[int id, int dataType]`
In this method, we would have the `dataType enum{Xstart, Ystart, Xend, Yend, entryType}`
As the map grows, lines can be merged into a single line as the map is filled, reducing the amount of storage required to hold the map.
A simple map of an empty room 4 meters x 5 meters with door in one of the walls 800mm wide and a closed door set back 20mm would have 8 entries in the map.
The use of a float means we can set the unit at one meter, but use a resolution of 0.001 meters or 1 millimetre or smaller.


# Sensor Input.

Our sensor inputs will be in the form of an angle and a range.
Just to make things confusing, our robot will also have an angle relative to either a known reference (Magnetic compass or some other sensor) or its starting orientation and the current X-Y coordinates.
We add the two angles together to get an effective angle.
Now we can use some trigonometry to work out the offset from the current location to where an object is sensed.
This will be an X-Y point in 2D space. however most sensors will have a sensing cone, closer to the robot will be more accurate with the width of the sensed area growing the further out from the robot we get.  When the sensor detects an object, the size of the point of detection will need to be considered the width of the detection cone at that distance.  In this case we will have a line that runs perpendicular to the direction of the sensing cone.
Where sensors overlap, the detection of an object at a further distance will take priority of the nearer sensed object, there by reducing the length of the sensed line of the closer object by eliminating the over lapped portion of the sensors from the closer object.
We know the object further away can cut into the closer objects sensed portion as the closer object wasn’t seen in the cone of the sensor seeing further away without obstruction within it cone of view.
The use of a confidence value as part of the map can help with that if we are able to sweep an area with the sensor, we can find the edge of an object and increase the confidence of where the object starts.

## HC-SR04 Ultrasonic Sensor.
![HC-SR04 Ultrasonic Sensor Module Distance Measurement Component Part Front](https://i0.wp.com/randomnerdtutorials.com/wp-content/uploads/2021/06/HC-SR04-Ultrasonic-Sensor-Module-Distance-Measurement-Component-Part-Front.jpg?resize=750%2C422&quality=100&strip=all&ssl=1)


Specifications of the HC-SR04 sensor are as follows:

- Power Supply :+5V DC
- Quiescent Current : <2mA
- Working Current: 15mA
- Effectual Angle: <15°
- Ranging Distance : 2cm – 400 cm/1″ – 13ft
- Resolution : 0.3 cm
- Measuring Angle: 30 degree
- Trigger Input Pulse width: 10uS TTL pulse
- Echo Output Signal: TTL pulse proportional to the distance range
- Dimension: 45mm x 20mm x 15mm

How it works.:
The ultrasonic sensor uses sonar to determine the distance to an object. Here’s what happens:

1. The ultrasound transmitter (trig pin) emits a high-frequency sound (40 kHz).
2. The sound travels through the air. If it finds an object, it bounces back to the module.
3. The ultrasound receiver (echo pin) receives the reflected sound (echo).

The time between the transmission and reception of the signal allows us to calculate the distance to an object. This is possible because we know the sound’s velocity in the air.
`distance to an object = ((speed of sound in the air)*time)/2`
speed of sound in the air at 20ºC (68ºF) = **343m/s**

The HC-SR04 ultrasonic sensor uses sonar to determine the distance to an object. This sensor reads from 2cm to 400cm (0.8inch to 157inch) with an accuracy of 0.3cm (0.1inches), which is good for most hobbyist projects. In addition, this particular module comes with ultrasonic transmitter and receiver modules.

We could sweep from 0° to 180° stopping every 30° to give us 7 positions. 0°, 30°, 60°, 90°, 120°, 150° & 180°, but any object no mater the size within the beam, will produce the echo and that position will see the object being the full width of the beam.
A small object at 35° will be considered to be 30° wide between 30° & 60°. The further away the object is, the wider the object is seen to be. We could use overlapping ranging positions, lets say 15°, between 0° and 180° we will have 13 positions. 0°, 15°, 30°, 45°, 60°, 75°, 90°, 105°, 120°, 135°, 150°, 165° & 180°. While the beam width is still 30° an object at 35° will not be seen at 15° position or at 60° positions, narrowing down the angle to just 15° between 30° & 45°.
In truth, in most cases, our robots will see something in all directions unless the nearest object is over 5 meters away. It also means we will see in the range -15° and +195° however we will treat our data as a 13 point point cloud. Now for the fun and games. How do we compare our point cloud to the real world map we have previously recorded before? Our robot may have moved in the X and Y axis of the real world map, but is may also have rotated. If our robot has moved, then we can get the estimated direction of travel and even the estimated amount of rotation, but this data may not be all that accurate, but then our ultrasonic data may not be that accurate either. So how do we overcome these inaccuracies?
One method that can be used for the range finder is averaging or weighted averaging. In this case, we may take 4 range samples, workout the average of those 4 samples. (Averaging) We could then choose the 3 samples closet to the average, discard the remaining sample and recalculate the average on the remaining 3 samples. (Weighted Averaging) This won't eliminate the noise from the ranging sensor, but it will reduce the noise.


## MPU6050 Accelerometer and Gyroscope

This is a MEMS based device that sensors the force of acceleration on the device in the X, Y and Z axis as well as the rotational acceleration in the 3 axis at the same time.
The idea is the device by measuring the rates of acceleration can determine over time how much you have turned.
Using the linear accelerometer, the robot will know which way is down and be able to estimate how far it has moved in any direction.
This can be combined with any tacho data to estimate the current robot pose.

# Localization.

Working out where we are.
When the robot moves, its movement may not be as accurate as you might hope, over a distance of 4 meters, the robot may perform an un-planned sweeping turn of 45°.
The purpose of localization is to try and detect this issue and allow the bot to correct it.
After the robot travels a short distance, we can scan the area and see what the robot sensors see, then compare it to what it expects to see.
We have from the sensors, a view of part of the map, what we need to do is compare that view with the map.  We use the bots believed current location and orientation as a starting the starting point of a search of the map. 
The biggest problem here is the orientation as well as the position may well not be correct.



