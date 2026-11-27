/*
  ============================================================
  ROBOTIC ARM (4 servos controlled by 4 potentiometers)
  ============================================================
  Board  : Arduino UNO / Nano
  Parts  : 4x servo (SG90 / MG90S / MG996R), 4x 10k potentiometer,
           external 5-6 V power supply for the servos (2 A or more)

  Each knob moves one joint: Base, Shoulder, Elbow, Gripper.
  Movement is smoothed so the arm doesn't shake.

  Wiring:
    Base     pot -> A0     Base     servo -> D3
    Shoulder pot -> A1     Shoulder servo -> D5
    Elbow    pot -> A2     Elbow    servo -> D6
    Gripper  pot -> A3     Gripper  servo -> D9
    Pot outer pins -> 5V and GND, middle pin -> A0..A3

    Servo RED (+)   -> external 5-6 V supply +
    Servo BROWN (-) -> external supply -  AND Arduino GND
    Never power 4 servos from the Arduino 5V pin!
  ============================================================
*/

#include <Servo.h>

const int potPins[4]   = {A0, A1, A2, A3};
const int servoPins[4] = {3, 5, 6, 9};

// Safe angle limits for each joint (change for your arm)
const int minAngle[4] = {0,   15,  15,  10};
const int maxAngle[4] = {180, 165, 165, 90};

Servo joints[4];
float currentAngle[4];

void setup() {
  Serial.begin(9600);
  for (int i = 0; i < 4; i++) {
    joints[i].attach(servoPins[i]);
    currentAngle[i] = readTarget(i);
    joints[i].write((int)currentAngle[i]);
  }
}

void loop() {
  for (int i = 0; i < 4; i++) {
    int target = readTarget(i);
    // smooth movement: move 15% of the way each loop
    currentAngle[i] += (target - currentAngle[i]) * 0.15;
    joints[i].write((int)currentAngle[i]);

    Serial.print((int)currentAngle[i]);
    Serial.print(i < 3 ? "\t" : "\n");
  }
  delay(20);
}

int readTarget(int i) {
  int value = analogRead(potPins[i]);
  return map(value, 0, 1023, minAngle[i], maxAngle[i]);
}
