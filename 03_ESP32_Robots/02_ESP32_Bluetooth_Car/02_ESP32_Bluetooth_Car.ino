/*
  ============================================================
  PROJECT : ESP32 Bluetooth Car (built-in Bluetooth, no HC-05)
  LEVEL   : Beginner
  BOARD   : ESP32 Dev Module (original ESP32 only)
  ============================================================
  ABOUT
    The ESP32 has Bluetooth built in, so no extra module is needed.
    Works with the same phone apps as the Arduino + HC-05 car.

  PARTS
    - ESP32 dev board (original ESP32 - NOT S2 / S3 / C3, they have no Classic Bluetooth)
    - L298N, motors, chassis, battery

  WIRING
    L298N ENA -> GPIO14
    L298N IN1 -> GPIO27
    L298N IN2 -> GPIO26
    L298N IN3 -> GPIO25
    L298N IN4 -> GPIO33
    L298N ENB -> GPIO32
    L298N 5V / GND -> ESP32 VIN / GND

  LIBRARIES
    - BluetoothSerial (ESP32 board package)

  APP
    "Arduino Bluetooth RC Car" or any app sending F B L R G I H J S and 0-9.

  HOW IT WORKS
    1. Upload, then pair your phone with "ESP32-BT-Car".
    2. Commands: F forward, B back, L left, R right, G/I forward-left/right,
       H/J back-left/right, S stop, 0-9 speed.

  TIPS
    - Error "Bluetooth is not enabled"? Your board is not an original ESP32.
    - Car stops by itself after 1 s? Your app sends only one command per tap: set SIGNAL_TIMEOUT to 0.
  ============================================================
*/

#include <BluetoothSerial.h>

#define ENA 14
#define IN1 27
#define IN2 26
#define IN3 25
#define IN4 33
#define ENB 32

#define SIGNAL_TIMEOUT 1000   // ms, 0 = disabled

BluetoothSerial SerialBT;
int carSpeed = 200;
unsigned long lastCommand = 0;

void setup() {
  Serial.begin(115200);
  pinMode(ENA, OUTPUT); pinMode(ENB, OUTPUT);
  pinMode(IN1, OUTPUT); pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT); pinMode(IN4, OUTPUT);
  setMotor(0, 0);
  SerialBT.begin("ESP32-BT-Car");
  Serial.println("Pair with ESP32-BT-Car");
}

void loop() {
  if (SerialBT.available()) {
    char cmd = SerialBT.read();
    lastCommand = millis();
    if (cmd >= '0' && cmd <= '9') {
      carSpeed = map(cmd - '0', 0, 9, 100, 255);
      return;
    }
    switch (cmd) {
      case 'F': setMotor(carSpeed, carSpeed);              break;
      case 'B': setMotor(-carSpeed, -carSpeed);            break;
      case 'L': setMotor(-carSpeed, carSpeed);             break;
      case 'R': setMotor(carSpeed, -carSpeed);             break;
      case 'G': setMotor(carSpeed / 3, carSpeed);          break;
      case 'I': setMotor(carSpeed, carSpeed / 3);          break;
      case 'H': setMotor(-carSpeed / 3, -carSpeed);        break;
      case 'J': setMotor(-carSpeed, -carSpeed / 3);        break;
      case 'S': setMotor(0, 0);                            break;
    }
  }
  if (SIGNAL_TIMEOUT > 0 && millis() - lastCommand > SIGNAL_TIMEOUT) setMotor(0, 0);
}

void setMotor(int leftSpeed, int rightSpeed) {
  if (leftSpeed >= 0) { digitalWrite(IN1, HIGH); digitalWrite(IN2, LOW); }
  else                { digitalWrite(IN1, LOW);  digitalWrite(IN2, HIGH); leftSpeed = -leftSpeed; }
  if (rightSpeed >= 0) { digitalWrite(IN3, HIGH); digitalWrite(IN4, LOW); }
  else                 { digitalWrite(IN3, LOW);  digitalWrite(IN4, HIGH); rightSpeed = -rightSpeed; }
  analogWrite(ENA, constrain(leftSpeed, 0, 255));
  analogWrite(ENB, constrain(rightSpeed, 0, 255));
}
