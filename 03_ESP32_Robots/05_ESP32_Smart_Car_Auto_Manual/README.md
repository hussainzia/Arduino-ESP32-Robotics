# ESP32 Smart Car - Manual + Autonomous mode with live dashboard

**Level:** Advanced &nbsp;|&nbsp; **Board:** ESP32 Dev Module

## About

One car, two brains. MANUAL: drive with arrow buttons on your phone. AUTO: the car explores by itself and avoids obstacles. The dashboard shows live distance and mode. Headlights turn on automatically in auto mode. Great for exhibitions.

## Components

- ESP32 dev board
- L298N, motors, chassis, battery
- HC-SR04 ultrasonic sensor + 1k and 2k resistors
- 2 white LEDs + 220 ohm resistors (headlights)

## Wiring

| From | To | Note |
|---|---|---|
| L298N ENA | GPIO14 |  |
| L298N IN1 | GPIO27 |  |
| L298N IN2 | GPIO26 |  |
| L298N IN3 | GPIO25 |  |
| L298N IN4 | GPIO33 |  |
| L298N ENB | GPIO32 |  |
| HC-SR04 TRIG | GPIO19 |  |
| HC-SR04 ECHO | GPIO18 | through 1k/2k divider: ECHO -1k- GPIO18 -2k- GND |
| HC-SR04 VCC | VIN | 5V |
| Headlight LEDs | GPIO2 | through 220 ohm |

## Libraries

- WiFi, WebServer (ESP32 board package)

## How it works

1. WiFi "SmartCar" / 12345678, open 192.168.4.1.
2. AUTO uses a small state machine (DRIVE -> BACK -> TURN) that never blocks, so the web page stays responsive.
3. MANUAL also has a safety brake: forward stops if an obstacle is < 15 cm.

## Tips & troubleshooting

- The ECHO divider is required: the HC-SR04 outputs 5 V, the ESP32 accepts only 3.3 V.

## How to upload

1. Build the circuit using the wiring table.
2. Open the `.ino` file in the Arduino IDE (or copy-paste the code into a new sketch).
3. Install the ESP32 board package if you haven't (see the main README), then select **ESP32 Dev Module**.
4. Install the libraries listed above from **Sketch > Include Library > Manage Libraries**.
5. Change any settings at the top of the code (WiFi name, thresholds, pins), then click **Upload**.
6. Open the Serial Monitor (baud rate shown in `Serial.begin(...)`) to see what the project is doing.

---
[⬅ Back to category](../README.md) · [🏠 All projects](../../README.md)
