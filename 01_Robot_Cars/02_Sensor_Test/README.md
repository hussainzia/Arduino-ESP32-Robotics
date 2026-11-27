# Sensor Test

**Difficulty:** Beginner

Prints IR sensor values (digital and analog) and ultrasonic distance in the Serial Monitor. Use it to check sensors and find threshold values before building a robot.

## Components

- Arduino UNO or Nano
- 1-5 IR sensor modules (TCRT5000 / FC-51)
- HC-SR04 ultrasonic sensor

## Wiring

| Part pin | Arduino pin | Note |
|---|---|---|
| IR sensors OUT | A0 ... A4 |  |
| HC-SR04 TRIG | D12 |  |
| HC-SR04 ECHO | D11 |  |
| HC-SR04 VCC / GND | 5V / GND |  |

## Libraries

- None (only the built-in Arduino functions)

## How to use

1. Build and wire the robot using the table above.
2. Open the `.ino` file in the Arduino IDE (or copy-paste the code into a new sketch).
3. Install any libraries listed above (Sketch > Include Library > Manage Libraries).
4. Select your board and port, then click Upload.
5. Unplug USB, switch on the battery and test.

## Tips

- Open Tools > Serial Monitor and set it to 9600 baud.
- Turn the blue potentiometer on an IR module to change its sensitivity.
- Also useful for LDR and flame sensor values (analog column).

[Back to category](../README.md) · [All projects](../../README.md)
