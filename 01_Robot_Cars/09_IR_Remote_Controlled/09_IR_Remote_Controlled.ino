/*
  ============================================================
  09 - IR REMOTE CONTROLLED CAR (TV-style remote + VS1838B)
  ============================================================
  Board   : Arduino UNO / Nano
  Library : "IRremote" by shirriff / z3t0 / ArminJo  (version 4.x)
            Arduino IDE -> Tools -> Manage Libraries -> search IRremote

  Default keys (common 21-key "Car MP3" remote):
    2 = Forward   8 = Backward   4 = Left   6 = Right   5 = Stop

  Using a different remote?
    Open Serial Monitor (9600), press each button and note the
    "Command" number printed. Put those numbers below.

  Wiring:
    IR receiver (VS1838B / KY-022) OUT -> D2, VCC -> 5V, GND -> GND
    L298N: ENA D5, IN1 D7, IN2 D8, IN3 D9, IN4 D10, ENB D6
  ============================================================
*/

#include <IRremote.hpp>

// ---------- Motor driver (L298N) pins ----------
#define ENA 5
#define IN1 7
#define IN2 8
#define IN3 9
#define IN4 10
#define ENB 6

#define IR_PIN 2

// ---------- Remote button codes (change for your remote) ----------
#define KEY_FORWARD  0x18   // button 2
#define KEY_BACKWARD 0x52   // button 8
#define KEY_LEFT     0x08   // button 4
#define KEY_RIGHT    0x5A   // button 6
#define KEY_STOP     0x1C   // button 5

int carSpeed = 180;

void setup() {
  Serial.begin(9600);
  pinMode(ENA, OUTPUT); pinMode(ENB, OUTPUT);
  pinMode(IN1, OUTPUT); pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT); pinMode(IN4, OUTPUT);
  stopRobot();
  IrReceiver.begin(IR_PIN, ENABLE_LED_FEEDBACK);
  Serial.println("IR remote car ready - press a button");
}

void loop() {
  if (IrReceiver.decode()) {
    uint16_t cmd = IrReceiver.decodedIRData.command;
    Serial.print("Command: 0x");
    Serial.println(cmd, HEX);

    switch (cmd) {
      case KEY_FORWARD:  forward(carSpeed);   break;
      case KEY_BACKWARD: backward(carSpeed);  break;
      case KEY_LEFT:     turnLeft(carSpeed);  break;
      case KEY_RIGHT:    turnRight(carSpeed); break;
      case KEY_STOP:     stopRobot();         break;
    }
    IrReceiver.resume();   // ready for the next button
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
