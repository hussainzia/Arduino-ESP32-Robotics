# Light Following Robot

**Difficulty:** Beginner

Follows a torch light. Two light sensors (LDRs) compare brightness on the left and right and the robot steers toward the brighter side.

## Components

- Arduino UNO or Nano
- L298N motor driver
- 2 or 4 BO / TT gear motors + wheels
- Robot chassis
- 7.4 V Li-ion pack (2x 18650) or 6x AA
- Jumper wires, switch
- 2x LDR (light dependent resistor)
- 2x 10k resistor

## Wiring

| Part pin | Arduino pin | Note |
|---|---|---|
| Left LDR divider | A0 | 5V - LDR - A0 - 10k - GND |
| Right LDR divider | A1 | 5V - LDR - A1 - 10k - GND |
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

- Keep the torch off for the first 2 seconds (room light calibration).
- A small cardboard wall between the two LDRs improves steering.
- Reverse the logic (swap turnLeft/turnRight) to make a light AVOIDING robot.

[Back to category](../README.md) · [All projects](../../README.md)
