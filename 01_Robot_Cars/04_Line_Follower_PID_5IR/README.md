# Fast Line Follower with PID (5 IR sensors)

**Difficulty:** Intermediate

A smoother and faster line follower for competitions. Five sensors find exactly where the line is and a PID controller adjusts motor speeds instead of just turning left/right.

## Components

- Arduino UNO or Nano
- L298N motor driver
- 2 or 4 BO / TT gear motors + wheels
- Robot chassis
- 7.4 V Li-ion pack (2x 18650) or 6x AA
- Jumper wires, switch
- 5-channel IR sensor array (or 5 TCRT5000 modules)

## Wiring

| Part pin | Arduino pin | Note |
|---|---|---|
| Sensor S1 (left) ... S5 (right) | A0 ... A4 |  |
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

- Tune in order: Kp first, then Kd, then a little Ki.
- Robot wobbles (zig-zag)? Increase Kd or decrease Kp.
- Robot is slow to turn on curves? Increase Kp.
- When the line is lost, the robot spins toward where it last saw it.

[Back to category](../README.md) · [All projects](../../README.md)
