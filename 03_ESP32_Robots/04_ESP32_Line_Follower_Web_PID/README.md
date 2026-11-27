# ESP32 Line Follower with Live PID Tuning from Phone

**Level:** Advanced &nbsp;|&nbsp; **Board:** ESP32 Dev Module

## About

A competition-style PID line follower whose Kp, Kd and speed you tune LIVE from a phone, without re-uploading code each time. The page also shows the line position in real time. Saves hours of tuning before a competition.

## Components

- ESP32 dev board
- 5-channel IR sensor array (TCRT5000), digital outputs
- L298N (or TB6612FNG), N20 / TT motors, light chassis

## Wiring

| From | To | Note |
|---|---|---|
| IR S1 (left) | GPIO34 |  |
| IR S2 | GPIO35 |  |
| IR S3 (center) | GPIO36 | VP |
| IR S4 | GPIO39 | VN |
| IR S5 (right) | GPIO4 |  |
| IR VCC / GND | 3.3V / GND |  |
| L298N ENA | GPIO14 |  |
| L298N IN1 | GPIO27 |  |
| L298N IN2 | GPIO26 |  |
| L298N IN3 | GPIO25 |  |
| L298N IN4 | GPIO33 |  |
| L298N ENB | GPIO32 |  |

## Libraries

- WiFi, WebServer (ESP32 board package)

## How it works

1. Connect to WiFi "LineBot" (12345678) and open 192.168.4.1.
2. Press START. Move the sliders while the robot runs.
3. PID runs every 5 ms; the web server is handled in between.

## Tips & troubleshooting

- Power the IR array from 3.3V so its outputs are safe for the ESP32.
- Write down the best values and put them as the defaults below.

## How to upload

1. Build the circuit using the wiring table.
2. Open the `.ino` file in the Arduino IDE (or copy-paste the code into a new sketch).
3. Install the ESP32 board package if you haven't (see the main README), then select **ESP32 Dev Module**.
4. Install the libraries listed above from **Sketch > Include Library > Manage Libraries**.
5. Change any settings at the top of the code (WiFi name, thresholds, pins), then click **Upload**.
6. Open the Serial Monitor (baud rate shown in `Serial.begin(...)`) to see what the project is doing.

---
[⬅ Back to category](../README.md) · [🏠 All projects](../../README.md)
