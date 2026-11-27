# Obstacle Avoiding Robot (with servo)

**Difficulty:** Beginner

Drives forward until an obstacle is close, then looks left and right with a servo-mounted ultrasonic sensor and turns toward the side with more space.

## Components

- Arduino UNO or Nano
- L298N motor driver
- 2 or 4 BO / TT gear motors + wheels
- Robot chassis
- 7.4 V Li-ion pack (2x 18650) or 6x AA
- Jumper wires, switch
- HC-SR04 ultrasonic sensor
- SG90 servo + servo bracket

## Wiring

| Part pin | Arduino pin | Note |
|---|---|---|
| HC-SR04 TRIG | D12 |  |
| HC-SR04 ECHO | D11 |  |
| HC-SR04 VCC / GND | 5V / GND |  |
| Servo signal (orange) | D3 | Red 5V, brown GND |
| L298N ENA | D5 | Left motor speed (remove jumper) |
| L298N IN1 | D7 | Left motor direction |
| L298N IN2 | D8 | Left motor direction |
| L298N IN3 | D9 | Right motor direction |
| L298N IN4 | D10 | Right motor direction |
| L298N ENB | D6 | Right motor speed (remove jumper) |
| L298N GND | GND | Common ground with battery |

## Libraries

- Servo (comes with Arduino IDE)

## How to use

1. Build and wire the robot using the table above.
2. Open the `.ino` file in the Arduino IDE (or copy-paste the code into a new sketch).
3. Install any libraries listed above (Sketch > Include Library > Manage Libraries).
4. Select your board and port, then click Upload.
5. Unplug USB, switch on the battery and test.

## Tips

- Change `turnTime` so the robot turns about 90 degrees.
- If the servo looks the wrong way, swap the LOOK_LEFT and LOOK_RIGHT values.
- If the Arduino resets when the servo moves, power it from a better battery.

[Back to category](../README.md) · [All projects](../../README.md)
