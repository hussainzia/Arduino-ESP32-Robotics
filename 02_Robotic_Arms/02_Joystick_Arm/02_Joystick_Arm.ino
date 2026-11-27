/*
  ============================================================
  PROJECT : Joystick Controlled Robotic Arm (2 joysticks, 4 servos)
  LEVEL   : Beginner
  BOARD   : Arduino UNO / Nano
  ============================================================
  ABOUT
    Drive a 4-joint arm like a video game. Pushing a stick moves
    the joint, releasing it HOLDS the position (unlike pots).
    Push further = move faster. Press joystick 1 to go home.

  PARTS
    - Arduino UNO
    - 2x KY-023 joystick
    - 4x servo (SG90 / MG90S), robotic arm kit
    - External 5-6 V 2 A power supply for servos

  WIRING
    Joystick 1 VRx -> A0   (Base: left/right)
    Joystick 1 VRy -> A1   (Shoulder: up/down)
    Joystick 1 SW  -> D2   (Home button)
    Joystick 2 VRx -> A2   (Elbow)
    Joystick 2 VRy -> A3   (Gripper open/close)
    Joysticks +5V / GND -> 5V / GND
    Base servo     -> D3
    Shoulder servo -> D5
    Elbow servo    -> D6
    Gripper servo  -> D9
    Servo red / brown -> external 5-6 V + / -   (connect - to Arduino GND too)

  LIBRARIES
    - Servo (built in)

  HOW IT WORKS
    1. Each loop reads a stick: outside the dead zone, the angle changes
       by up to MAX_STEP degrees, depending on how far the stick is pushed.
    2. Angles are kept inside safe limits for each joint.

  TIPS
    - Joint moves the wrong way? Make its entry in `direction[]` -1.
    - Change minAngle/maxAngle so the arm never hits itself.
  ============================================================
*/

#include <Servo.h>

const int joyPins[4]   = {A0, A1, A2, A3};
const int servoPins[4] = {3, 5, 6, 9};
const int minAngle[4]  = {0,   20,  20,  10};
const int maxAngle[4]  = {180, 160, 160, 90};
const int homeAngle[4] = {90,  90,  90,  45};
const int direction[4] = {1,   1,   1,   1};   // set -1 to reverse a joint

#define HOME_BUTTON 2
#define DEAD_ZONE   80
#define MAX_STEP    3.0   // degrees per loop at full stick

Servo joints[4];
float angle[4];

void setup() {
  pinMode(HOME_BUTTON, INPUT_PULLUP);
  for (int i = 0; i < 4; i++) {
    angle[i] = homeAngle[i];
    joints[i].attach(servoPins[i]);
    joints[i].write(homeAngle[i]);
  }
}

void loop() {
  if (digitalRead(HOME_BUTTON) == LOW) {
    for (int i = 0; i < 4; i++) angle[i] = homeAngle[i];
  }

  for (int i = 0; i < 4; i++) {
    int offset = analogRead(joyPins[i]) - 512;
    if (abs(offset) > DEAD_ZONE) {
      angle[i] += direction[i] * MAX_STEP * offset / 512.0;
      angle[i] = constrain(angle[i], minAngle[i], maxAngle[i]);
    }
    joints[i].write((int)angle[i]);
  }
  delay(20);
}
