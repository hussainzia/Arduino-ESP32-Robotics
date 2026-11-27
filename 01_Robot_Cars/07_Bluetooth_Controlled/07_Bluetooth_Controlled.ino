/*
  ============================================================
  07 - BLUETOOTH CONTROLLED CAR (HC-05 / HC-06 + Android app)
  ============================================================
  Board  : Arduino UNO / Nano
  App    : "Arduino Bluetooth RC Car" (or any app that sends
           single characters). Pair with HC-05, PIN 1234 or 0000.

  Commands received:
    F = Forward      B = Back       L = Left      R = Right
    G = Forward-Left I = Forward-Right
    H = Back-Left    J = Back-Right S = Stop
    0..9 = speed (0 slow ... 9 fast),  q = full speed

  Wiring:
    HC-05 TX  -> D2   (Arduino receives)
    HC-05 RX  -> D3   through a voltage divider
                      (D3 -> 1k -> HC-05 RX -> 2k -> GND)
    HC-05 VCC -> 5V,  GND -> GND
    L298N: ENA D5, IN1 D7, IN2 D8, IN3 D9, IN4 D10, ENB D6

  SoftwareSerial is used, so you do NOT have to unplug the
  module while uploading code.
  ============================================================
*/

#include <SoftwareSerial.h>

// ---------- Motor driver (L298N) pins ----------
#define ENA 5
#define IN1 7
#define IN2 8
#define IN3 9
#define IN4 10
#define ENB 6

SoftwareSerial bt(2, 3);   // RX, TX

int carSpeed = 200;
unsigned long lastCommandTime = 0;
// Stop if no command arrives for this many ms (app closed / out of range).
// The "Arduino Bluetooth RC Car" app keeps sending while a button is held.
// If YOUR app sends a command only once per tap, set this to 0 (disabled).
#define SIGNAL_TIMEOUT 1000

void setup() {
  Serial.begin(9600);
  bt.begin(9600);            // HC-05 default baud rate
  pinMode(ENA, OUTPUT); pinMode(ENB, OUTPUT);
  pinMode(IN1, OUTPUT); pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT); pinMode(IN4, OUTPUT);
  stopRobot();
  Serial.println("Bluetooth car ready");
}

void loop() {
  if (bt.available()) {
    char cmd = bt.read();
    Serial.println(cmd);
    lastCommandTime = millis();

    if (cmd >= '0' && cmd <= '9') {
      carSpeed = map(cmd - '0', 0, 9, 100, 255);
    } else {
      switch (cmd) {
        case 'F': forward(carSpeed);                       break;
        case 'B': backward(carSpeed);                      break;
        case 'L': turnLeft(carSpeed);                      break;
        case 'R': turnRight(carSpeed);                     break;
        case 'G': setMotor(carSpeed / 3, carSpeed);        break;
        case 'I': setMotor(carSpeed, carSpeed / 3);        break;
        case 'H': setMotor(-carSpeed / 3, -carSpeed);      break;
        case 'J': setMotor(-carSpeed, -carSpeed / 3);      break;
        case 'S': stopRobot();                             break;
        case 'q': carSpeed = 255;                          break;
      }
    }
  }

  // Safety: stop if no command for a while (app closed / out of range)
  if (SIGNAL_TIMEOUT > 0 && millis() - lastCommandTime > SIGNAL_TIMEOUT) {
    stopRobot();
  }
}

// ================= MOTOR FUNCTIONS =================
void setMotor(int leftSpeed, int rightSpeed) {
  if (leftSpeed >= 0) { digitalWrite(IN1, HIGH); digitalWrite(IN2, LOW); }
  else                { digitalWrite(IN1, LOW);  digitalWrite(IN2, HIGH); leftSpeed = -leftSpeed; }
  if (rightSpeed >= 0) { digitalWrite(IN3, HIGH); digitalWrite(IN4, LOW); }
  else                 { digitalWrite(IN3, LOW);  digitalWrite(IN4, HIGH); rightSpeed = -rightSpeed; }
  analogWrite(ENA, constrain(leftSpeed, 0, 255));
  analogWrite(ENB, constrain(rightSpeed, 0, 255));
}
void forward(int s)   { setMotor(s, s); }
void backward(int s)  { setMotor(-s, -s); }
void turnLeft(int s)  { setMotor(-s, s); }
void turnRight(int s) { setMotor(s, -s); }
void stopRobot()      { setMotor(0, 0); }
