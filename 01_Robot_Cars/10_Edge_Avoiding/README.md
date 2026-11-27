# Edge Avoiding Robot (Table Edge Detector)

**Difficulty:** Beginner

Drives around on a table without falling off. Down-facing IR sensors notice when the table ends and the robot backs away.

## Components

- Arduino UNO or Nano
- L298N motor driver
- 2 or 4 BO / TT gear motors + wheels
- Robot chassis
- 7.4 V Li-ion pack (2x 18650) or 6x AA
- Jumper wires, switch
- 2x IR sensor (TCRT5000) facing down

## Wiring

| Part pin | Arduino pin | Note |
|---|---|---|
| Left IR OUT | A0 | Front-left corner |
| Right IR OUT | A1 | Front-right corner |
| L298N ENA | D5 | Left motor speed (remove jumper) |
| L298N IN1 | D7 | Left motor direction |
| L298N IN2 | D8 | Left motor direction |
| L298N IN3 | D9 | Right motor direction |
| L298N IN4 | D10 | Right motor direction |
| L298N ENB | D6 | Right motor speed (remove jumper) |
| L298N GND | GND | Common ground with battery |

## Libraries

- None (only the built-in Arduino functions)

## How to use

1. Build and wire the robot using the table above.
2. Open the `.ino` file in the Arduino IDE (or copy-paste the code into a new sketch).
3. Install any libraries listed above (Sketch > Include Library > Manage Libraries).
4. Select your board and port, then click Upload.
5. Unplug USB, switch on the battery and test.

## Tips

- Mount sensors in front of the wheels so the edge is seen early.
- Test slowly and keep a hand ready to catch the robot.
- Very dark or glossy tables may need the sensor potentiometer adjusted.

[Back to category](../README.md) · [All projects](../../README.md)
