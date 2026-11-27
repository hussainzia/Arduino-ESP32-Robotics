# Gesture Controlled Robot (hand glove)

**Difficulty:** Advanced

Tilt your hand to drive the car. A glove with an MPU6050 sensor sends commands wirelessly through nRF24L01 radios. This project has TWO sketches: `Gesture_Transmitter` (glove) and `Gesture_Receiver` (car).

## Components

- 2x Arduino Nano/UNO
- MPU6050 accelerometer
- 2x nRF24L01 radio module
- 2x 10uF capacitor
- L298N + motors + chassis
- Battery for the glove (9V or power bank)

## Wiring

| Part pin | Arduino pin | Note |
|---|---|---|
| **Glove:** MPU6050 SDA / SCL | A4 / A5 |  |
| **Both:** nRF24 CE / CSN | D9 / D10 |  |
| **Both:** nRF24 SCK / MOSI / MISO | D13 / D11 / D12 |  |
| **Both:** nRF24 VCC | 3.3V | NOT 5V, add 10uF cap |
| **Car:** L298N ENA / ENB | D5 / D6 |  |
| **Car:** L298N IN1 / IN2 | D7 / D8 |  |
| **Car:** L298N IN3 / IN4 | D4 / D2 | Different from other projects! |

## Libraries

- RF24 by TMRh20 (install from Library Manager)
- Wire, SPI (come with Arduino IDE)

## How to use

1. Build and wire the robot using the table above.
2. Open the `.ino` file in the Arduino IDE (or copy-paste the code into a new sketch).
3. Install any libraries listed above (Sketch > Include Library > Manage Libraries).
4. Select your board and port, then click Upload.
5. Unplug USB, switch on the battery and test.

## Tips

- Open the transmitter's Serial Monitor and tilt your hand to see which command is sent.
- If the radios don't talk, the 10uF capacitor on the nRF24 power pins usually fixes it.
- The car stops automatically if the glove signal is lost for 0.5 seconds.

[Back to category](../README.md) · [All projects](../../README.md)
