/*
  ============================================================
  10 - EDGE AVOIDING ROBOT (won't fall off the table)
  ============================================================
  Board  : Arduino UNO / Nano
  Parts  : 2 IR sensors at the front-left and front-right
           corners, pointing DOWN at the table (about 1 cm high).

  How it works:
    On the table the IR light reflects back  -> sensor LOW
    Past the edge there is nothing to reflect -> sensor HIGH
    When an edge is found: stop, back up, turn away, continue.

  Wiring:
    Left  IR OUT -> A0
    Right IR OUT -> A1
    L298N: ENA D5, IN1 D7, IN2 D8, IN3 D9, IN4 D10, ENB D6

  SAFETY: test slowly with your hand ready to catch the robot!
  ============================================================
*/

// ---------- Motor driver (L298N) pins ----------
#define ENA 5
#define IN1 7
#define IN2 8
#define IN3 9
#define IN4 10
#define ENB 6

#define LEFT_IR  A0
#define RIGHT_IR A1
#define NO_FLOOR HIGH     // sensor value when it sees the edge

int driveSpeed = 120;     // keep it slow for safety
int backTime   = 400;     // ms
int turnTime   = 450;     // ms

void setup() {
  pinMode(ENA, OUTPUT); pinMode(ENB, OUTPUT);
  pinMode(IN1, OUTPUT); pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT); pinMode(IN4, OUTPUT);
  pinMode(LEFT_IR, INPUT);
  pinMode(RIGHT_IR, INPUT);
  stopRobot();
  delay(2000);
}

void loop() {
  bool leftEdge  = (digitalRead(LEFT_IR)  == NO_FLOOR);
  bool rightEdge = (digitalRead(RIGHT_IR) == NO_FLOOR);

  if (!leftEdge && !rightEdge) {
    forward(driveSpeed);
    return;
  }

  // Edge found - react immediately
  stopRobot();
  delay(100);
  backward(driveSpeed);
  delay(backTime);

  if (leftEdge && !rightEdge) {
    turnRight(150);           // edge on left -> go right
  } else if (rightEdge && !leftEdge) {
    turnLeft(150);            // edge on right -> go left
  } else {
    turnRight(150);           // edge straight ahead -> turn around
    delay(turnTime);
  }
  delay(turnTime);
  stopRobot();
  delay(100);
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
