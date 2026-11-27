# IR Remote Controlled Car

**Difficulty:** Beginner

Control the car with a normal TV-style IR remote. Keys 2/8/4/6 drive and 5 stops.

## Components

- Arduino UNO or Nano
- L298N motor driver
- 2 or 4 BO / TT gear motors + wheels
- Robot chassis
- 7.4 V Li-ion pack (2x 18650) or 6x AA
- Jumper wires, switch
- IR receiver (VS1838B or KY-022)
- Any IR remote (21-key Car MP3 remote by default)

## Wiring

| Part pin | Arduino pin | Note |
|---|---|---|
| IR receiver OUT | D2 |  |
| IR receiver VCC / GND | 5V / GND |  |
| L298N ENA | D5 | Left motor speed (remove jumper) |
| L298N IN1 | D7 | Left motor direction |
| L298N IN2 | D8 | Left motor direction |
| L298N IN3 | D9 | Right motor direction |
| L298N IN4 | D10 | Right motor direction |
| L298N ENB | D6 | Right motor speed (remove jumper) |
| L298N GND | GND | Common ground with battery |

## Libraries

- IRremote by ArminJo, version 4.x (install from Library Manager)

## How to use

1. Build and wire the robot using the table above.
2. Open the `.ino` file in the Arduino IDE (or copy-paste the code into a new sketch).
3. Install any libraries listed above (Sketch > Include Library > Manage Libraries).
4. Select your board and port, then click Upload.
5. Unplug USB, switch on the battery and test.

## Tips

- Using another remote? Open Serial Monitor, press each key and copy the printed codes into the KEY_ lines.
- Point the remote at the receiver; sunlight can block IR signals.

[Back to category](../README.md) · [All projects](../../README.md)
