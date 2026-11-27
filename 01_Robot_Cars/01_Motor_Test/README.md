# Motor Test

**Difficulty:** Beginner

Upload this first on every new robot. It runs forward, backward, left and right so you can check that both motors spin and turn the right way.

## Components

- Arduino UNO or Nano
- L298N motor driver
- 2 or 4 BO / TT gear motors + wheels
- Robot chassis
- 7.4 V Li-ion pack (2x 18650) or 6x AA
- Jumper wires, switch

## Wiring

| Part pin | Arduino pin | Note |
|---|---|---|
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

- If a wheel spins backwards, swap that motor's two wires on the L298N output terminals.
- If nothing moves, check that the ENA/ENB jumpers are removed and the wires go to D5/D6.
- The Arduino GND and battery GND must be connected together.

[Back to category](../README.md) · [All projects](../../README.md)
