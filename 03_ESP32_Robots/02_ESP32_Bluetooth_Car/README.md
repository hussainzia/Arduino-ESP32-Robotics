# ESP32 Bluetooth Car (built-in Bluetooth, no HC-05)

**Level:** Beginner &nbsp;|&nbsp; **Board:** ESP32 Dev Module (original ESP32 only)

## About

The ESP32 has Bluetooth built in, so no extra module is needed. Works with the same phone apps as the Arduino + HC-05 car.

## Components

- ESP32 dev board (original ESP32 - NOT S2 / S3 / C3, they have no Classic Bluetooth)
- L298N, motors, chassis, battery

## Wiring

| From | To | Note |
|---|---|---|
| L298N ENA | GPIO14 |  |
| L298N IN1 | GPIO27 |  |
| L298N IN2 | GPIO26 |  |
| L298N IN3 | GPIO25 |  |
| L298N IN4 | GPIO33 |  |
| L298N ENB | GPIO32 |  |
| L298N 5V / GND | ESP32 VIN / GND |  |

## Libraries

- BluetoothSerial (ESP32 board package)

## Phone app

"Arduino Bluetooth RC Car" or any app sending F B L R G I H J S and 0-9.

## How it works

1. Upload, then pair your phone with "ESP32-BT-Car".
2. Commands: F forward, B back, L left, R right, G/I forward-left/right, H/J back-left/right, S stop, 0-9 speed.

## Tips & troubleshooting

- Error "Bluetooth is not enabled"? Your board is not an original ESP32.
- Car stops by itself after 1 s? Your app sends only one command per tap: set SIGNAL_TIMEOUT to 0.

## How to upload

1. Build the circuit using the wiring table.
2. Open the `.ino` file in the Arduino IDE (or copy-paste the code into a new sketch).
3. Install the ESP32 board package if you haven't (see the main README), then select **ESP32 Dev Module**.
4. Install the libraries listed above from **Sketch > Include Library > Manage Libraries**.
5. Change any settings at the top of the code (WiFi name, thresholds, pins), then click **Upload**.
6. Open the Serial Monitor (baud rate shown in `Serial.begin(...)`) to see what the project is doing.

---
[⬅ Back to category](../README.md) · [🏠 All projects](../../README.md)
