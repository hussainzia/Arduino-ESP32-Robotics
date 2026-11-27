# Robotic Arm (4 servos + potentiometers)

**Difficulty:** Intermediate

A 4-joint robotic arm (base, shoulder, elbow, gripper). Turn a knob and the matching joint follows smoothly.

## Components

- Arduino UNO or Nano
- 4x servo (SG90 / MG90S / MG996R)
- 4x 10k potentiometer
- Robotic arm kit (acrylic or 3D-printed)
- 5-6 V 2 A+ power supply for the servos

## Wiring

| Part pin | Arduino pin | Note |
|---|---|---|
| Potentiometers (middle pin) | A0 / A1 / A2 / A3 | Base / Shoulder / Elbow / Gripper |
| Servo signals | D3 / D5 / D6 / D9 | Base / Shoulder / Elbow / Gripper |
| Servo power + | External 5-6 V | Not the Arduino 5V pin |
| Servo power - | External GND + Arduino GND | Common ground |

## Libraries

- Servo (comes with Arduino IDE)

## How to use

1. Build and wire the robot using the table above.
2. Open the `.ino` file in the Arduino IDE (or copy-paste the code into a new sketch).
3. Install any libraries listed above (Sketch > Include Library > Manage Libraries).
4. Select your board and port, then click Upload.
5. Unplug USB, switch on the battery and test.

## Tips

- Set `minAngle` / `maxAngle` so joints never hit the frame (prevents burnt servos).
- Jittery servos almost always mean a weak power supply.

[Back to category](../README.md) · [All projects](../../README.md)
