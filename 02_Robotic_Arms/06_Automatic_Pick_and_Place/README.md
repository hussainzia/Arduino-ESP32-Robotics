# Automatic Pick & Place Arm (factory automation demo)

**Level:** Intermediate &nbsp;|&nbsp; **Board:** Arduino UNO / Nano

## About

An IR sensor watches the pick-up spot. When an object arrives, the arm picks it up, drops it in the bin, counts it and returns home. A mini version of a real factory cell. Add a conveyor belt (DC motor) for an impressive exhibition.

## Components

- Arduino UNO
- 4x servo + robotic arm kit, external 5-6 V supply
- IR obstacle sensor (FC-51)
- 16x2 I2C LCD (optional, shows count)
- Buzzer (optional)

## Wiring

| From | To | Note |
|---|---|---|
| Base servo | D3 |  |
| Shoulder servo | D5 |  |
| Elbow servo | D6 |  |
| Gripper servo | D9 |  |
| IR sensor OUT | D2 | aimed at the pick-up spot |
| Buzzer + | D8 |  |
| LCD SDA / SCL | A4 / A5 |  |

## Libraries

- Servo (built in)
- LiquidCrystal I2C by Frank de Brabander

## How it works

1. Waits at HOME until the IR sensor sees an object.
2. Runs the PICK sequence, then the PLACE sequence (list of poses).
3. Increments the counter and shows it on the LCD.

## Tips & troubleshooting

- Teach your own poses: use project 01 (knobs) and Serial Monitor to read the angles, then copy them into the pose arrays.

## How to upload

1. Build the circuit using the wiring table.
2. Open the `.ino` file in the Arduino IDE (or copy-paste the code into a new sketch).
3. Select **Tools > Board > Arduino UNO** (or Nano) and the right port.
4. Install the libraries listed above from **Sketch > Include Library > Manage Libraries**.
5. Change any settings at the top of the code (WiFi name, thresholds, pins), then click **Upload**.
6. Open the Serial Monitor (baud rate shown in `Serial.begin(...)`) to see what the project is doing.

---
[⬅ Back to category](../README.md) · [🏠 All projects](../../README.md)
