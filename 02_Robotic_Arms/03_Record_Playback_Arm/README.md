# Teach & Repeat Robotic Arm (record and playback)

**Level:** Intermediate &nbsp;|&nbsp; **Board:** Arduino UNO / Nano

## About

Teach the arm a job, then it repeats it by itself - just like factory robots. Move the arm with the knobs, press RECORD to save each position, then press PLAY. The steps are stored in EEPROM, so the program survives power-off.

## Components

- Arduino UNO
- 4x servo + robotic arm kit
- 4x 10k potentiometer
- 2x push button, LED (or use built-in D13)
- External 5-6 V 2 A supply for servos

## Wiring

| From | To | Note |
|---|---|---|
| Pots (middle pin) | A0, A1, A2, A3 | Base, Shoulder, Elbow, Gripper |
| Servos | D3, D5, D6, D9 |  |
| RECORD button | D2 | other leg to GND |
| PLAY button | D4 | other leg to GND |
| Status LED | D13 | built-in |

## Libraries

- Servo, EEPROM (built in)

## How it works

TEACH mode : knobs move the arm. Short press RECORD = save step (LED blinks). Hold RECORD 2 s = erase all steps. PLAY mode  : press PLAY - arm smoothly moves through all steps in a loop. Press PLAY again to stop.

## Tips & troubleshooting

- Up to 40 steps. Add 2 identical steps to make the arm pause.
- Change MOVE_TIME for faster/slower playback.

## How to upload

1. Build the circuit using the wiring table.
2. Open the `.ino` file in the Arduino IDE (or copy-paste the code into a new sketch).
3. Select **Tools > Board > Arduino UNO** (or Nano) and the right port.
4. Install the libraries listed above from **Sketch > Include Library > Manage Libraries**.
5. Change any settings at the top of the code (WiFi name, thresholds, pins), then click **Upload**.
6. Open the Serial Monitor (baud rate shown in `Serial.begin(...)`) to see what the project is doing.

---
[⬅ Back to category](../README.md) · [🏠 All projects](../../README.md)
