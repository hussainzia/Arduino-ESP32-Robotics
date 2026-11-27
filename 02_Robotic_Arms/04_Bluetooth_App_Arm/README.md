# Bluetooth Controlled Robotic Arm (phone app)

**Level:** Intermediate &nbsp;|&nbsp; **Board:** Arduino UNO / Nano

## About

Control each joint of the arm from your phone. Works with a simple terminal app or your own MIT App Inventor app with 4 sliders. Also has a saved "wave hello" animation.

## Components

- Arduino UNO
- HC-05 / HC-06 Bluetooth module, 1k + 2k resistors
- 4x servo + robotic arm kit, external 5-6 V supply

## Wiring

| From | To | Note |
|---|---|---|
| HC-05 TX | D2 |  |
| HC-05 RX | D4 | through 1k/2k voltage divider |
| HC-05 VCC / GND | 5V / GND |  |
| Base servo | D3 |  |
| Shoulder servo | D5 |  |
| Elbow servo | D6 |  |
| Gripper servo | D9 |  |

## Libraries

- Servo, SoftwareSerial (built in)

## Phone app

"Serial Bluetooth Terminal" (Android) - type commands and press send. Or build an MIT App Inventor app with sliders that send the same text.

## How it works

Send a letter + angle, ending with a new line: B90  = base to 90        S120 = shoulder to 120 E60  = elbow to 60       G30  = gripper to 30 H    = home position     W    = wave hello Joints move smoothly to the new angle.

## Tips & troubleshooting

- In Serial Bluetooth Terminal, set "Newline" to LF in settings.

## How to upload

1. Build the circuit using the wiring table.
2. Open the `.ino` file in the Arduino IDE (or copy-paste the code into a new sketch).
3. Select **Tools > Board > Arduino UNO** (or Nano) and the right port.
4. Install the libraries listed above from **Sketch > Include Library > Manage Libraries**.
5. Change any settings at the top of the code (WiFi name, thresholds, pins), then click **Upload**.
6. Open the Serial Monitor (baud rate shown in `Serial.begin(...)`) to see what the project is doing.

---
[⬅ Back to category](../README.md) · [🏠 All projects](../../README.md)
