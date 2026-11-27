# ESP32 WiFi Car with Touch Joystick (no app needed)

**Level:** Intermediate &nbsp;|&nbsp; **Board:** ESP32 Dev Module

## About

The car makes its own WiFi hotspot and serves a web page with a touch joystick. Drag your thumb to drive with smooth, proportional speed and steering, like a real RC car.

## Components

- ESP32 dev board
- L298N motor driver, 2/4 gear motors, chassis
- 2x 18650 battery pack (7.4 V)

## Wiring

| From | To | Note |
|---|---|---|
| L298N ENA | GPIO14 | remove ENA jumper |
| L298N IN1 | GPIO27 |  |
| L298N IN2 | GPIO26 |  |
| L298N IN3 | GPIO25 |  |
| L298N IN4 | GPIO33 |  |
| L298N ENB | GPIO32 | remove ENB jumper |
| L298N 5V | ESP32 VIN |  |
| L298N GND | ESP32 GND |  |

## Libraries

- WiFi, WebServer (ESP32 board package 2.x or 3.x)

## How it works

1. Connect phone WiFi to "ESP32-Car" (password 12345678), open 192.168.4.1.
2. The joystick sends x and y (-100..100) about 10 times a second.
3. Differential mixing: left = y + x, right = y - x.
4. Safety: if no command arrives for 600 ms, the car stops.

## Tips & troubleshooting

- Car turns the wrong way? Swap the motor wires or swap the + and - of x in drive().
- Several cars in one room? Give each a different WiFi name.

## How to upload

1. Build the circuit using the wiring table.
2. Open the `.ino` file in the Arduino IDE (or copy-paste the code into a new sketch).
3. Install the ESP32 board package if you haven't (see the main README), then select **ESP32 Dev Module**.
4. Install the libraries listed above from **Sketch > Include Library > Manage Libraries**.
5. Change any settings at the top of the code (WiFi name, thresholds, pins), then click **Upload**.
6. Open the Serial Monitor (baud rate shown in `Serial.begin(...)`) to see what the project is doing.

---
[⬅ Back to category](../README.md) · [🏠 All projects](../../README.md)
