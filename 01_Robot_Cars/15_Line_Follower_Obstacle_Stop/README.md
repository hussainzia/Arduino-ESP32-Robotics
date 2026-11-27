# Line Follower + Obstacle Stop

**Difficulty:** Beginner

A line follower that stops and beeps when something blocks its path, then continues when the path is clear. Good for "automatic train" or "delivery robot" demos.

## Components

- Arduino UNO or Nano
- L298N motor driver
- 2 or 4 BO / TT gear motors + wheels
- Robot chassis
- 7.4 V Li-ion pack (2x 18650) or 6x AA
- Jumper wires, switch
- 2x IR sensor (TCRT5000)
- HC-SR04 ultrasonic sensor
- Active buzzer

## Wiring

| Part pin | Arduino pin | Note |
|---|---|---|
| Left IR OUT | A0 |  |
| Right IR OUT | A1 |  |
| HC-SR04 TRIG | D12 |  |
| HC-SR04 ECHO | D11 |  |
| HC-SR04 VCC / GND | 5V / GND |  |
| Buzzer + | D4 | Buzzer - to GND |
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

- Change `STOP_DISTANCE` to stop earlier or later.
- Same track tips as project 03.

[Back to category](../README.md) · [All projects](../../README.md)
