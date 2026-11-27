# Joystick Controlled Robotic Arm (2 joysticks, 4 servos)

**Level:** Beginner &nbsp;|&nbsp; **Board:** Arduino UNO / Nano

## About

Drive a 4-joint arm like a video game. Pushing a stick moves the joint, releasing it HOLDS the position (unlike pots). Push further = move faster. Press joystick 1 to go home.

## Components

- Arduino UNO
- 2x KY-023 joystick
- 4x servo (SG90 / MG90S), robotic arm kit
- External 5-6 V 2 A power supply for servos

## Wiring

| From | To | Note |
|---|---|---|
| Joystick 1 VRx | A0 | Base: left/right |
| Joystick 1 VRy | A1 | Shoulder: up/down |
| Joystick 1 SW | D2 | Home button |
| Joystick 2 VRx | A2 | Elbow |
| Joystick 2 VRy | A3 | Gripper open/close |
| Joysticks +5V / GND | 5V / GND |  |
| Base servo | D3 |  |
| Shoulder servo | D5 |  |
| Elbow servo | D6 |  |
| Gripper servo | D9 |  |
| Servo red / brown | external 5-6 V + / - | connect - to Arduino GND too |

## Libraries

- Servo (built in)

## How it works

1. Each loop reads a stick: outside the dead zone, the angle changes by up to MAX_STEP degrees, depending on how far the stick is pushed.
2. Angles are kept inside safe limits for each joint.

## Tips & troubleshooting

- Joint moves the wrong way? Make its entry in `direction[]` -1.
- Change minAngle/maxAngle so the arm never hits itself.

## How to upload

1. Build the circuit using the wiring table.
2. Open the `.ino` file in the Arduino IDE (or copy-paste the code into a new sketch).
3. Select **Tools > Board > Arduino UNO** (or Nano) and the right port.
4. Install the libraries listed above from **Sketch > Include Library > Manage Libraries**.
5. Change any settings at the top of the code (WiFi name, thresholds, pins), then click **Upload**.
6. Open the Serial Monitor (baud rate shown in `Serial.begin(...)`) to see what the project is doing.

---
[⬅ Back to category](../README.md) · [🏠 All projects](../../README.md)
