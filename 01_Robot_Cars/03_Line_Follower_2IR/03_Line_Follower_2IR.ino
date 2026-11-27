/*
  ============================================================
  03 - LINE FOLLOWER ROBOT (2 IR sensors)
  ============================================================
  Board  : Arduino UNO / Nano
  Track  : Black line (2-3 cm wide tape) on white floor

  Sensor placement:
    Put the two IR sensors at the front, facing DOWN, about
    1 cm above the floor. The black line should pass BETWEEN
    them, so on a straight line both sensors see white.

  Logic:
    Left = white, Right = white  -> Forward
    Left = BLACK, Right = white  -> Turn left
    Left = white, Right = BLACK  -> Turn right
    Left = BLACK, Right = BLACK  -> Stop (junction / end)

  Wiring:
    Left IR OUT  -> A0
    Right IR OUT -> A1
    L298N: ENA D5, IN1 D7, IN2 D8, IN3 D9, IN4 D10, ENB D6
  ============================================================
*/

// ---------- Motor driver (L298N) pins ----------
#define ENA 5
#define IN1 7
#define IN2 8
#define IN3 9
#define IN4 10
#define ENB 6

// ---------- Sensor pins ----------
#define LEFT_IR  A0
#define RIGHT_IR A1

// Most IR modules give HIGH on black. If your robot does the
// opposite of what it should, change HIGH to LOW here.
#define ON_LINE HIGH

int baseSpeed = 140;   // forward speed   (0-255)
int turnSpeed = 160;   // turning speed   (0-255)

void setup() {
  pinMode(ENA, OUTPUT); pinMode(ENB, OUTPUT);
  pinMode(IN1, OUTPUT); pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT); pinMode(IN4, OUTPUT);
  pinMode(LEFT_IR, INPUT);
  pinMode(RIGHT_IR, INPUT);
  stopRobot();
  delay(2000);   // time to place robot on the track
}

void loop() {
  bool leftOnLine  = (digitalRead(LEFT_IR)  == ON_LINE);
  bool rightOnLine = (digitalRead(RIGHT_IR) == ON_LINE);

  if (!leftOnLine && !rightOnLine) {
    forward(baseSpeed);
  } else if (leftOnLine && !rightOnLine) {
    turnLeft(turnSpeed);
  } else if (!leftOnLine && rightOnLine) {
    turnRight(turnSpeed);
  } else {
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
