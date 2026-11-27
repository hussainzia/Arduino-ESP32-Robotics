/*
  ============================================================
  20 - SIMPLE OBSTACLE AVOIDING ROBOT (no servo, cheapest build)
  ============================================================
  Board  : Arduino UNO / Nano
  Parts  : HC-SR04 fixed at the front

  Logic:
    Drive forward. If an obstacle is closer than STOP_DISTANCE:
    stop, reverse, then turn in a random direction and continue.
    If it gets stuck turning the same way, the random choice
    helps it escape corners.

  Wiring:
    HC-SR04 TRIG -> D12, ECHO -> D11
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

#define TRIG 12
#define ECHO 11
#define STOP_DISTANCE 25    // cm

int driveSpeed = 150;
int turnSpeed  = 170;

void setup() {
  pinMode(ENA, OUTPUT); pinMode(ENB, OUTPUT);
  pinMode(IN1, OUTPUT); pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT); pinMode(IN4, OUTPUT);
  pinMode(TRIG, OUTPUT);
  pinMode(ECHO, INPUT);
  randomSeed(analogRead(A5));   // unconnected pin = random noise
  stopRobot();
  delay(2000);
}

void loop() {
  if (readDistanceCM() > STOP_DISTANCE) {
    forward(driveSpeed);
    delay(40);
    return;
  }

  stopRobot();       delay(150);
  backward(driveSpeed); delay(350);

  if (random(2) == 0) turnLeft(turnSpeed);
  else                turnRight(turnSpeed);
  delay(random(300, 600));   // random turn angle
  stopRobot();
  delay(100);
}

long readDistanceCM() {
  digitalWrite(TRIG, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG, LOW);
  long duration = pulseIn(ECHO, HIGH, 30000UL);
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
void backward(int s)  { setMotor(-s, -s); }
void turnLeft(int s)  { setMotor(-s, s); }
void turnRight(int s) { setMotor(s, -s); }
void stopRobot()      { setMotor(0, 0); }
