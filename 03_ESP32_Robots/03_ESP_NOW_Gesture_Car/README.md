# ESP-NOW Gesture Car

**Level:** Advanced

This project has more than one program:

- [`Gesture_Car_RX/Gesture_Car_RX.ino`](Gesture_Car_RX/Gesture_Car_RX.ino) — ESP-NOW Gesture Car - CAR (receiver) (ESP32 Dev Module)
- [`Gesture_Glove_TX/Gesture_Glove_TX.ino`](Gesture_Glove_TX/Gesture_Glove_TX.ino) — ESP-NOW Gesture Car - GLOVE (transmitter) (ESP32 Dev Module (or any ESP32 / ESP32-C3))

## ESP-NOW Gesture Car - CAR (receiver)

**Board:** ESP32 Dev Module

### About

Receives tilt packets from the glove over ESP-NOW and drives with proportional speed. Stops if the glove signal is lost.

### Components

- ESP32 dev board (car)
- L298N, motors, chassis, battery

### Wiring

| From | To | Note |
|---|---|---|
| L298N ENA | GPIO14 |  |
| L298N IN1 | GPIO27 |  |
| L298N IN2 | GPIO26 |  |
| L298N IN3 | GPIO25 |  |
| L298N IN4 | GPIO33 |  |
| L298N ENB | GPIO32 |  |
| L298N 5V / GND | ESP32 VIN / GND |  |

### Libraries

- WiFi, esp_now (ESP32 board package 2.x or 3.x - both supported)

### How it works

1. onReceive() runs whenever a packet arrives and stores x, y.
2. loop() mixes x/y into left/right motor speeds.
3. No packet for 500 ms -> stop.

### Tips & troubleshooting

- CAR_ID must match the glove.

## ESP-NOW Gesture Car - GLOVE (transmitter)

**Board:** ESP32 Dev Module (or any ESP32 / ESP32-C3)

### About

Tilt your hand to drive. The glove ESP32 reads an MPU6050 and sends tilt values to the car over ESP-NOW: fast, long range (100 m+), no router and no radio module needed. Tilt a little = slow, tilt more = fast.

### Components

- ESP32 dev board (glove)
- MPU6050 module
- Small power bank or 3.7 V Li-ion + charger board

### Wiring

| From | To | Note |
|---|---|---|
| MPU6050 VCC | 3.3V |  |
| MPU6050 GND | GND |  |
| MPU6050 SDA | GPIO21 |  |
| MPU6050 SCL | GPIO22 |  |

### Libraries

- WiFi, esp_now, Wire (ESP32 board package)

### How it works

1. Pitch (forward/back) and roll (left/right) are calculated from gravity.
2. A packet {carId, x, y} is broadcast 20 times a second.
3. The car only obeys packets with ITS carId, so many cars can drive in one room.

### Tips & troubleshooting

- Change CAR_ID (here AND in the car code) if several teams use this project.
- Open Serial Monitor at 115200 to see the values being sent.

## How to upload

1. Build the circuit using the wiring table.
2. Open the `.ino` file in the Arduino IDE (or copy-paste the code into a new sketch).
3. Install the ESP32 board package if you haven't (see the main README), then select **ESP32 Dev Module**.
4. Install the libraries listed above from **Sketch > Include Library > Manage Libraries**.
5. Change any settings at the top of the code (WiFi name, thresholds, pins), then click **Upload**.
6. Open the Serial Monitor (baud rate shown in `Serial.begin(...)`) to see what the project is doing.

---
[⬅ Back to category](../README.md) · [🏠 All projects](../../README.md)
