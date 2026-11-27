# Wall Following Robot

**Difficulty:** Intermediate

Drives along a wall at a fixed distance using a side ultrasonic sensor and proportional control. Turns when it meets a wall ahead.

## Components

- Arduino UNO or Nano
- L298N motor driver
- 2 or 4 BO / TT gear motors + wheels
- Robot chassis
- 7.4 V Li-ion pack (2x 18650) or 6x AA
- Jumper wires, switch
- 2x HC-SR04 ultrasonic sensor

## Wiring

| Part pin | Arduino pin | Note |
|---|---|---|
| Front HC-SR04 TRIG / ECHO | D12 / D11 | Looks forward |
| Side HC-SR04 TRIG / ECHO | A2 / A3 | Looks right |
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

- Change `WALL_DISTANCE` to stay closer to or farther from the wall.
- Robot wobbles? Lower `Kp`. Reacts too slowly? Raise `Kp`.
- To follow a LEFT wall, mount the side sensor on the left and swap the + and - in setMotor().

[Back to category](../README.md) · [All projects](../../README.md)
