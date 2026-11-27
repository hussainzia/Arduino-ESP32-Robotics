# Fire Fighting Robot

**Difficulty:** Intermediate

Finds a flame using three flame sensors, drives toward it, then sprays water from a small pump while sweeping the nozzle until the flame goes out.

## Components

- Arduino UNO or Nano
- L298N motor driver
- 2 or 4 BO / TT gear motors + wheels
- Robot chassis
- 7.4 V Li-ion pack (2x 18650) or 6x AA
- Jumper wires, switch
- 3x IR flame sensor module
- 5V relay module
- Mini DC water pump + pipe + small water tank
- SG90 servo (nozzle)
- Separate battery for the pump

## Wiring

| Part pin | Arduino pin | Note |
|---|---|---|
| Left flame AO | A0 |  |
| Center flame AO | A1 |  |
| Right flame AO | A2 |  |
| Relay IN | D4 | Relay switches the pump |
| Servo signal | D3 |  |
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

- Use 02_Sensor_Test with a candle to find good `FLAME_DETECT` and `FLAME_CLOSE` values.
- If the pump runs all the time, your relay is active LOW. Swap PUMP_ON and PUMP_OFF.
- Safety: small candle only, adult present, keep water away from electronics.

[Back to category](../README.md) · [All projects](../../README.md)
