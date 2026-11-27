# WiFi Controlled Car (NodeMCU ESP8266)

**Difficulty:** Intermediate

The car creates its own WiFi network and serves a control web page. Drive from any phone browser; no app to install.

## Components

- NodeMCU ESP8266 (instead of Arduino)
- L298N motor driver
- Motors, chassis, battery

## Wiring

| Part pin | Arduino pin | Note |
|---|---|---|
| L298N ENA / ENB | D1 / D2 | NodeMCU pins |
| L298N IN1 / IN2 | D5 / D6 |  |
| L298N IN3 / IN4 | D7 / D0 |  |
| L298N 5V | VIN | Powers the NodeMCU |
| L298N GND | GND |  |

## Libraries

- ESP8266 board package (Boards Manager): ESP8266WiFi + ESP8266WebServer are included

## How to use

1. Build and wire the robot using the table above.
2. Open the `.ino` file in the Arduino IDE (or copy-paste the code into a new sketch).
3. Install any libraries listed above (Sketch > Include Library > Manage Libraries).
4. Select your board and port, then click Upload.
5. Unplug USB, switch on the battery and test.

## Tips

- Board setting: Tools > Board > NodeMCU 1.0 (ESP-12E Module).
- Connect phone to WiFi `RobotCar` (password `12345678`) and open `192.168.4.1`.
- Change `WIFI_NAME` if several cars are in the same room.

[Back to category](../README.md) · [All projects](../../README.md)
