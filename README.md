# Arduino & ESP32 Robotics Projects

[![Compile Sketches](https://github.com/hussainzia/Arduino-ESP32-Robotics/actions/workflows/compile.yml/badge.svg)](https://github.com/hussainzia/Arduino-ESP32-Robotics/actions/workflows/compile.yml) ![Sketches](https://img.shields.io/badge/sketches-33-blue) ![Platform](https://img.shields.io/badge/platform-Arduino%20%7C%20ESP32%20%7C%20ESP8266-00979D) [![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](LICENSE) [![PRs Welcome](https://img.shields.io/badge/PRs-welcome-brightgreen.svg)](CONTRIBUTING.md)

Ready-to-use, well-commented code for **31 robotics projects** for school, college and hobby builders: robot cars, robotic arms and ESP32 robots.

Every project is **one self-contained sketch** with the parts list and wiring written at the top of the code, plus a README with wiring tables and troubleshooting. Copy, paste, upload.

> 📡 Looking for sensors, IoT, smart home and automation projects? See the companion repo **[Arduino-ESP32-IoT-Automation](https://github.com/hussainzia/Arduino-ESP32-IoT-Automation)**.

---

## Categories

| Folder | What's inside | Projects |
|---|---|---|
| [01_Robot_Cars](01_Robot_Cars/) | Robot Cars | 20 |
| [02_Robotic_Arms](02_Robotic_Arms/) | Robotic Arms | 6 |
| [03_ESP32_Robots](03_ESP32_Robots/) | ESP32 Robots | 5 |

## All projects

### [Robot Cars](01_Robot_Cars/) (20)

| # | Project | Level | Board |
|---|---|---|---|
| 01 | [Motor Test](01_Robot_Cars/01_Motor_Test/) | Beginner | Arduino UNO / Nano |
| 02 | [Sensor Test](01_Robot_Cars/02_Sensor_Test/) | Beginner | Arduino UNO / Nano |
| 03 | [Line Follower (2 IR sensors)](01_Robot_Cars/03_Line_Follower_2IR/) | Beginner | Arduino UNO / Nano |
| 04 | [Fast Line Follower with PID (5 IR sensors)](01_Robot_Cars/04_Line_Follower_PID_5IR/) | Intermediate | Arduino UNO / Nano |
| 05 | [Obstacle Avoiding Robot (with servo)](01_Robot_Cars/05_Obstacle_Avoiding/) | Beginner | Arduino UNO / Nano |
| 06 | [Human Following Robot](01_Robot_Cars/06_Human_Following/) | Beginner | Arduino UNO / Nano |
| 07 | [Bluetooth Controlled Car](01_Robot_Cars/07_Bluetooth_Controlled/) | Beginner | Arduino UNO / Nano |
| 08 | [Voice Controlled Robot](01_Robot_Cars/08_Voice_Controlled/) | Beginner | Arduino UNO / Nano |
| 09 | [IR Remote Controlled Car](01_Robot_Cars/09_IR_Remote_Controlled/) | Beginner | Arduino UNO / Nano |
| 10 | [Edge Avoiding Robot (Table Edge Detector)](01_Robot_Cars/10_Edge_Avoiding/) | Beginner | Arduino UNO / Nano |
| 11 | [Light Following Robot](01_Robot_Cars/11_Light_Following/) | Beginner | Arduino UNO / Nano |
| 12 | [Wall Following Robot](01_Robot_Cars/12_Wall_Following/) | Intermediate | Arduino UNO / Nano |
| 13 | [Fire Fighting Robot](01_Robot_Cars/13_Fire_Fighting/) | Intermediate | Arduino UNO / Nano |
| 14 | [Sumo Robot](01_Robot_Cars/14_Sumo_Robot/) | Intermediate | Arduino UNO / Nano |
| 15 | [Line Follower + Obstacle Stop](01_Robot_Cars/15_Line_Follower_Obstacle_Stop/) | Beginner | Arduino UNO / Nano |
| 16 | [Maze Solving Robot](01_Robot_Cars/16_Maze_Solver/) | Advanced | Arduino UNO / Nano |
| 17 | [Gesture Controlled Robot (hand glove)](01_Robot_Cars/17_Gesture_Controlled/) | Advanced | Arduino UNO / Nano |
| 18 | [Self Balancing Robot](01_Robot_Cars/18_Self_Balancing/) | Advanced | Arduino UNO / Nano |
| 19 | [WiFi Controlled Car (NodeMCU ESP8266)](01_Robot_Cars/19_WiFi_Controlled_ESP8266/) | Intermediate | NodeMCU 1.0 (ESP-12E) |
| 20 | [Simple Obstacle Avoiding Robot (no servo)](01_Robot_Cars/20_Obstacle_Avoiding_No_Servo/) | Beginner | Arduino UNO / Nano |

### [Robotic Arms](02_Robotic_Arms/) (6)

| # | Project | Level | Board |
|---|---|---|---|
| 01 | [Robotic Arm (4 servos + potentiometers)](02_Robotic_Arms/01_Potentiometer_Arm/) | Intermediate | Arduino UNO / Nano |
| 02 | [Joystick Controlled Robotic Arm (2 joysticks, 4 servos)](02_Robotic_Arms/02_Joystick_Arm/) | Beginner | Arduino UNO / Nano |
| 03 | [Teach & Repeat Robotic Arm (record and playback)](02_Robotic_Arms/03_Record_Playback_Arm/) | Intermediate | Arduino UNO / Nano |
| 04 | [Bluetooth Controlled Robotic Arm (phone app)](02_Robotic_Arms/04_Bluetooth_App_Arm/) | Intermediate | Arduino UNO / Nano |
| 05 | [WiFi Robotic Arm with Web Sliders (ESP32)](02_Robotic_Arms/05_ESP32_Web_Slider_Arm/) | Intermediate | ESP32 Dev Module |
| 06 | [Automatic Pick & Place Arm (factory automation demo)](02_Robotic_Arms/06_Automatic_Pick_and_Place/) | Intermediate | Arduino UNO / Nano |

### [ESP32 Robots](03_ESP32_Robots/) (5)

| # | Project | Level | Board |
|---|---|---|---|
| 01 | [ESP32 WiFi Car with Touch Joystick (no app needed)](03_ESP32_Robots/01_ESP32_WiFi_Joystick_Car/) | Intermediate | ESP32 Dev Module |
| 02 | [ESP32 Bluetooth Car (built-in Bluetooth, no HC-05)](03_ESP32_Robots/02_ESP32_Bluetooth_Car/) | Beginner | ESP32 Dev Module (original ESP32 only) |
| 03 | [ESP-NOW Gesture Car - CAR (receiver)](03_ESP32_Robots/03_ESP_NOW_Gesture_Car/) | Advanced | ESP32 Dev Module |
| 04 | [ESP32 Line Follower with Live PID Tuning from Phone](03_ESP32_Robots/04_ESP32_Line_Follower_Web_PID/) | Advanced | ESP32 Dev Module |
| 05 | [ESP32 Smart Car - Manual + Autonomous mode with live dashboard](03_ESP32_Robots/05_ESP32_Smart_Car_Auto_Manual/) | Advanced | ESP32 Dev Module |

---

## Standard wiring (build one car, try many projects)

All Arduino robot cars use the **same motor pins**, and all ESP32 cars share their own standard. Build the chassis once, then just upload a different sketch.

| L298N pin | Arduino UNO / Nano | ESP32 | Purpose |
|---|---|---|---|
| ENA | D5 | GPIO14 | Left motor speed (PWM). Remove the jumper cap |
| IN1 | D7 | GPIO27 | Left motor direction |
| IN2 | D8 | GPIO26 | Left motor direction |
| IN3 | D9 | GPIO25 | Right motor direction |
| IN4 | D10 | GPIO33 | Right motor direction |
| ENB | D6 | GPIO32 | Right motor speed (PWM). Remove the jumper cap |
| GND | GND | GND | **Common ground (required)** |
| 5V | VIN (optional) | VIN | Powers the board from the L298N |

| Sensor (Arduino cars) | Pin |
|---|---|
| HC-SR04 TRIG / ECHO | D12 / D11 |
| IR sensors | A0, A1, A2 ... |
| Servo | D3 |

Projects that need different pins (gesture receiver, NodeMCU car, arms) say so clearly in their code.

---

## Getting started

1. Install the free [Arduino IDE](https://www.arduino.cc/en/software).
2. Open a project folder and its `.ino` file, **or** copy the code into a new sketch.
3. Install the libraries listed in the project's README (**Sketch > Include Library > Manage Libraries**).
4. Start with **01_Robot_Cars/01_Motor_Test** to check your wheels turn the right way.

> The Arduino IDE needs the folder name and the `.ino` name to match. If you copy a single file, the IDE offers to create the folder. Click OK.

### ESP32 board setup (one time)

1. Arduino IDE > **File > Preferences** > *Additional boards manager URLs*, paste:
   `https://espressif.github.io/arduino-esp32/package_esp32_index.json`
2. **Tools > Board > Boards Manager**, search **esp32**, install *esp32 by Espressif Systems*.
3. Select **Tools > Board > ESP32 Arduino > ESP32 Dev Module**.
4. Upload stuck on `Connecting....`? Hold the **BOOT** button on the board until uploading starts.
5. Windows doesn't see the board? Install the CP2102 or CH340 USB driver (depends on your board).

---

## Common problems

| Problem | Fix |
|---|---|
| A wheel spins the wrong way | Swap that motor's two wires on the L298N |
| Motors don't move | Remove ENA/ENB jumpers, check battery, connect all GNDs |
| Board resets when motors start | Weak batteries. Use 2x 18650 Li-ion, not a 9 V battery |
| Robot does the opposite of what it should | Flip `ON_LINE` / `DETECTED` between `HIGH` and `LOW` |
| Ultrasonic always shows 999 | TRIG/ECHO swapped, or no 5 V on the sensor |
| `xxx.h: No such file or directory` | Install the library named in the README |
| Servos shake or the board restarts | Power servos from a separate 5-6 V supply (common GND) |
| ESP32 + HC-SR04 gives wrong values | Add the 1k/2k voltage divider on ECHO (5 V -> 3.3 V) |

---

## Safety

- Test new code with the wheels lifted off the table.
- Li-ion batteries can catch fire if short-circuited: use a holder with a switch and never leave charging batteries unattended.

## Contributing

New projects and fixes are welcome, see [CONTRIBUTING.md](CONTRIBUTING.md). Keep each project as one self-contained sketch with the standard header (PROJECT, LEVEL, BOARD, PARTS, WIRING, HOW IT WORKS, TIPS).

## License

[MIT](LICENSE): free to use, copy and modify for school, college and personal projects.
