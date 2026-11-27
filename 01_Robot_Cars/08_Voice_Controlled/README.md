# Voice Controlled Robot

**Difficulty:** Beginner

Say "forward", "back", "left", "right" or "stop" into your phone. A voice app converts your speech to text and sends it over Bluetooth.

## Components

- Arduino UNO or Nano
- L298N motor driver
- 2 or 4 BO / TT gear motors + wheels
- Robot chassis
- 7.4 V Li-ion pack (2x 18650) or 6x AA
- Jumper wires, switch
- HC-05 Bluetooth module
- 1k + 2k resistors

## Wiring

| Part pin | Arduino pin | Note |
|---|---|---|
| HC-05 TX | D2 |  |
| HC-05 RX | D3 | Through 1k/2k divider |
| HC-05 VCC / GND | 5V / GND |  |
| L298N ENA | D5 | Left motor speed (remove jumper) |
| L298N IN1 | D7 | Left motor direction |
| L298N IN2 | D8 | Left motor direction |
| L298N IN3 | D9 | Right motor direction |
| L298N IN4 | D10 | Right motor direction |
| L298N ENB | D6 | Right motor speed (remove jumper) |
| L298N GND | GND | Common ground with battery |

## Libraries

- SoftwareSerial (comes with Arduino IDE)

## Phone app

BT Voice Control for Arduino (Play Store), or any app that sends recognized text

## How to use

1. Build and wire the robot using the table above.
2. Open the `.ino` file in the Arduino IDE (or copy-paste the code into a new sketch).
3. Install any libraries listed above (Sketch > Include Library > Manage Libraries).
4. Select your board and port, then click Upload.
5. Unplug USB, switch on the battery and test.

## Tips

- Change `turnTime` so "left" and "right" make 90 degree turns.
- Open Serial Monitor to see exactly what text the robot heard.
- You can add your own words inside `runCommand()`.

[Back to category](../README.md) · [All projects](../../README.md)
