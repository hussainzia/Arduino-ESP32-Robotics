# Self Balancing Robot

**Difficulty:** Advanced

A two-wheeled robot that balances itself upright like a Segway. The MPU6050 measures tilt and a PID controller drives the wheels under the robot to stop it falling.

## Components

- Arduino UNO or Nano
- MPU6050
- L298N
- 2x geared DC motors (high torque) + wheels
- Tall 2-3 level chassis
- Battery mounted on top

## Wiring

| Part pin | Arduino pin | Note |
|---|---|---|
| MPU6050 SDA / SCL | A4 / A5 |  |
| MPU6050 VCC / GND | 5V / GND |  |
| L298N ENA | D5 | Left motor speed (remove jumper) |
| L298N IN1 | D7 | Left motor direction |
| L298N IN2 | D8 | Left motor direction |
| L298N IN3 | D9 | Right motor direction |
| L298N IN4 | D10 | Right motor direction |
| L298N ENB | D6 | Right motor speed (remove jumper) |
| L298N GND | GND | Common ground with battery |

## Libraries

- Wire (comes with Arduino IDE)

## How to use

1. Build and wire the robot using the table above.
2. Open the `.ino` file in the Arduino IDE (or copy-paste the code into a new sketch).
3. Install any libraries listed above (Sketch > Include Library > Manage Libraries).
4. Select your board and port, then click Upload.
5. Unplug USB, switch on the battery and test.

## Tips

- Follow the STEP 1 and STEP 2 instructions at the top of the code.
- Tuning takes patience; change one value at a time.
- Keep the robot still for 2 seconds after power-on (gyro calibration).
- Geared motors with little backlash (e.g. N20 or JGA25) work much better than yellow TT motors.

[Back to category](../README.md) · [All projects](../../README.md)
