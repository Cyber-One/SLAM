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
#define LEFT_SERVO_PIN 6
#define RIGHT_SERVO_PIN 7
#define SENSOR_SERVO_PIN 3
#define SENSOR_TRIGGER_PIN 4
#define SENSOR_ECHO_PIN 5

// Compile time Settings for this project.
#define SENSOR_ARC_WIDTH 30     // in degrees
#define SENSOR_POSITIONS 13     // int
#define SENSOR_START_POS 0      // degrees
#define SENSOR_STOP_POS 180     // degrees
#define SENSOR_SAMPLES 4        // int

// Create all the objects we need to run our device.
Ultrasonic ultrasonic(SENSOR_TRIGGER_PIN, SENSOR_ECHO_PIN);  // create the ultrasonic range finder object.
Servo sensorServo;              // create servo object to control a servo
Servo leftServo;                // create servo object to control a servo
Servo rightServo;               // create servo object to control a servo

int pingArray[SENSOR_POSITIONS];

int ping() {
  int rangeSample[SENSOR_SAMPLES];
  int sum = 0;
  int valid = 0;
  int average = 0;
  for(int i=0; i<SENSOR_SAMPLES; i++) {
    rangeSample[i] = ultrasonic.read();
    sum =+ rangeSample[i];
    if (rangeSample[i] > 0) valid++;
  }
  if (valid > 0) {
    average = sum/valid;
  }
  return(average);
}

void scan() {
  int pos = 0;
  for (int i=0; i<SENSOR_POSITIONS; i++) {
    pos = (((SENSOR_STOP_POS - SENSOR_START_POS)/(SENSOR_POSITIONS - 1)) * i) + SENSOR_START_POS;
    sensorServo.write(pos);
    delay(10);
    pingArray[i] = ping();
  }
}

void setup() {
  // put your setup code here, to run once:
  sensorServo.attach(SENSOR_SERVO_PIN);
  leftServo.attach(LEFT_SERVO_PIN);
  rightServo.attach(RIGHT_SERVO_PIN);
}

void loop() {
  // put your main code here, to run repeatedly:
  scan();
}
