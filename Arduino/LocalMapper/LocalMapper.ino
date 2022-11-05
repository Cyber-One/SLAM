/*
 * SLAM test code.
 * 
 * Created by Cyber_One.
 * 
 * This project uses an Arduino Nano controlling 3 servos and 1 Ultrasonic HC-SR04 range finder.
 * the hardware is based on the MR-4 robot chassis created by Tim Cook.
 * 
 */
 
#include <Ultrasonic.h>
#include <Servo.h>

// Pin allocations for this project.
#define LeftServoPin 6
#define RightServoPin 7
#define SensorServoPin 3
#define SensorTriggerPin 4
#define SensorEchoPin 5

// Create all the objects we need to run our device.
Ultrasonic ultrasonic(SensorTriggerPin, SensorEchoPin);  // create the ultrasonic range finder object.
Servo sensorServo;              // create servo object to control a servo
Servo leftServo;                // create servo object to control a servo
Servo rightServo;               // create servo object to control a servo


void setup() {
  // put your setup code here, to run once:
  sensorServo.attach(SensorServoPin);
  leftServo.attach(LeftServoPin);
  rightServo.attach(RightServoPin);
}

void loop() {
  // put your main code here, to run repeatedly:

}
