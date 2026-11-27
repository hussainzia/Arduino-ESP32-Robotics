/*
  ============================================================
  15 - LINE FOLLOWER + OBSTACLE DETECTION
  ============================================================
  Board  : Arduino UNO / Nano
  Parts  : 2 IR sensors (line) + HC-SR04 (obstacle) + buzzer

  Logic:
    Follows a black line like project 03. If something blocks
    the track (closer than STOP_DISTANCE) it stops, beeps,
    and waits until the path is clear, then continues.
    Great for "smart delivery robot" / "automatic train" demos.

  Wiring:
    Left IR OUT  -> A0
    Right IR OUT -> A1
    HC-SR04 TRIG -> D12, ECHO -> D11
    Buzzer +     -> D4,  Buzzer - -> GND  (active buzzer)
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

#define LEFT_IR  A0
#define RIGHT_IR A1
#define TRIG 12
#define ECHO 11
#define BUZZER 4

#define ON_LINE HIGH
#define STOP_DISTANCE 15     // cm

int baseSpeed = 140;
int turnSpeed = 160;

void setup() {
  pinMode(ENA, OUTPUT); pinMode(ENB, OUTPUT);
  pinMode(IN1, OUTPUT); pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT); pinMode(IN4, OUTPUT);
  pinMode(LEFT_IR, INPUT);
  pinMode(RIGHT_IR, INPUT);
  pinMode(TRIG, OUTPUT);
  pinMode(ECHO, INPUT);
  pinMode(BUZZER, OUTPUT);
  stopRobot();
  delay(2000);
}

void loop() {
  // ---- obstacle check ----
  if (readDistanceCM() < STOP_DISTANCE) {
    stopRobot();
    digitalWrite(BUZZER, HIGH);
    delay(150);
    digitalWrite(BUZZER, LOW);
    delay(350);
    return;                  // keep waiting until clear
  }

  // ---- line following ----
  bool leftOnLine  = (digitalRead(LEFT_IR)  == ON_LINE);
  bool rightOnLine = (digitalRead(RIGHT_IR) == ON_LINE);

  if (!leftOnLine && !rightOnLine)      forward(baseSpeed);
  else if (leftOnLine && !rightOnLine)  turnLeft(turnSpeed);
  else if (!leftOnLine && rightOnLine)  turnRight(turnSpeed);
  else                                  stopRobot();
}

long readDistanceCM() {
  digitalWrite(TRIG, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG, LOW);
  long duration = pulseIn(ECHO, HIGH, 6000UL);   // ~1 m max, keeps loop fast
  if (duration == 0) return 999;
  return duration / 58;
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
void turnLeft(int s)  { setMotor(-s, s); }
void turnRight(int s) { setMotor(s, -s); }
void stopRobot()      { setMotor(0, 0); }
