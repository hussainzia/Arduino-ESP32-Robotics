/*
  ============================================================
  08 - VOICE CONTROLLED ROBOT (HC-05 + Android voice app)
  ============================================================
  Board  : Arduino UNO / Nano
  App    : "BT Voice Control for Arduino" (or "Arduino Voice
           Control"). The app turns your speech into text and
           sends it over Bluetooth, e.g. "*forward#".

  Say:
    "forward" / "go"      -> drive forward until "stop"
    "back" / "backward"   -> drive backward until "stop"
    "left"                -> turn left ~90 degrees, then stop
    "right"               -> turn right ~90 degrees, then stop
    "stop"                -> stop

  Wiring:
    HC-05 TX  -> D2,  HC-05 RX -> D3 (through 1k/2k voltage divider)
    HC-05 VCC -> 5V,  GND -> GND
    L298N: ENA D5, IN1 D7, IN2 D8, IN3 D9, IN4 D10, ENB D6
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

int carSpeed = 170;
int turnTime = 450;        // ms for about 90 degrees - adjust for your robot

String voice = "";

void setup() {
  Serial.begin(9600);
  bt.begin(9600);
  pinMode(ENA, OUTPUT); pinMode(ENB, OUTPUT);
  pinMode(IN1, OUTPUT); pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT); pinMode(IN4, OUTPUT);
  stopRobot();
  Serial.println("Voice robot ready");
}

void loop() {
  while (bt.available()) {
    char c = bt.read();
    if (c == '#' || c == '\n' || c == '\r') {   // end of a command
      if (voice.length() > 0) runCommand(voice);
      voice = "";
    } else if (c != '*') {                       // '*' marks the start
      voice += c;
    }
    delay(2);
  }
}

void runCommand(String cmd) {
  cmd.trim();
  cmd.toLowerCase();
  Serial.print("Heard: ");
  Serial.println(cmd);

  if (cmd.indexOf("stop") >= 0) {
    stopRobot();
  } else if (cmd.indexOf("forward") >= 0 || cmd == "go") {
    forward(carSpeed);
  } else if (cmd.indexOf("back") >= 0) {
    backward(carSpeed);
  } else if (cmd.indexOf("left") >= 0) {
    turnLeft(carSpeed);
    delay(turnTime);
    stopRobot();
  } else if (cmd.indexOf("right") >= 0) {
    turnRight(carSpeed);
    delay(turnTime);
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
