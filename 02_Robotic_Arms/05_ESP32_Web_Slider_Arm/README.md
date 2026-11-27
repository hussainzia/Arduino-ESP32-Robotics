# WiFi Robotic Arm with Web Sliders (ESP32)

**Level:** Intermediate &nbsp;|&nbsp; **Board:** ESP32 Dev Module

## About

The ESP32 creates its own WiFi network. Open the page on any phone and move 4 sliders to control the arm. No app, no router. Buttons for HOME and a saved PICK & PLACE demo.

## Components

- ESP32 dev board
- 4x servo (SG90 / MG90S) + robotic arm kit
- External 5-6 V 2 A supply for servos

## Wiring

| From | To | Note |
|---|---|---|
| Base servo signal | GPIO13 |  |
| Shoulder servo signal | GPIO14 |  |
| Elbow servo signal | GPIO27 |  |
| Gripper servo signal | GPIO26 |  |
| Servo red / brown | external 5-6 V + / - |  |
| External supply - | ESP32 GND | common ground! |

## Libraries

- ESP32Servo by Kevin Harrington (Library Manager)
- WiFi, WebServer (ESP32 board package)

## How it works

1. Connect your phone to WiFi "RoboArm" (password 12345678).
2. Open 192.168.4.1.
3. Each slider sends /set?j=<joint>&a=<angle>; the ESP32 moves the servo smoothly toward that angle.

## Tips & troubleshooting

- Do not use GPIO 6-11 (flash) or GPIO 12 (boot pin) for servos.
- Servos twitch at power-on: that is normal for cheap servos.

## How to upload

1. Build the circuit using the wiring table.
2. Open the `.ino` file in the Arduino IDE (or copy-paste the code into a new sketch).
3. Install the ESP32 board package if you haven't (see the main README), then select **ESP32 Dev Module**.
4. Install the libraries listed above from **Sketch > Include Library > Manage Libraries**.
5. Change any settings at the top of the code (WiFi name, thresholds, pins), then click **Upload**.
6. Open the Serial Monitor (baud rate shown in `Serial.begin(...)`) to see what the project is doing.

---
[⬅ Back to category](../README.md) · [🏠 All projects](../../README.md)
