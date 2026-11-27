/*
  ============================================================
  14 - SUMO ROBOT (push the opponent out of the ring)
  ============================================================
  Board  : Arduino UNO / Nano
  Arena  : Black ring with a WHITE border line (standard dohyo)
  Parts  : HC-SR04 at the front (finds the opponent)
           2 IR sensors at front-left / front-right facing DOWN
           (find the white border so we don't drive out)

  Logic (priority order):
    1. White border under a sensor -> back up and turn away
    2. Opponent within ATTACK_RANGE -> full speed attack
    3. Otherwise -> spin slowly to search
    Waits START_DELAY ms after power-on (competition rule).

  Wiring:
    HC-SR04 TRIG -> D12, ECHO -> D11
    Left  IR OUT -> A0
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

#define TRIG 12
#define ECHO 11
#define LEFT_IR  A0
#define RIGHT_IR A1
#define WHITE LOW              // IR reads LOW on the white border

#define ATTACK_RANGE 60        // cm
#define START_DELAY  5000      // ms

int attackSpeed = 255;
int searchSpeed = 130;

void setup() {
  pinMode(ENA, OUTPUT); pinMode(ENB, OUTPUT);
  pinMode(IN1, OUTPUT); pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT); pinMode(IN4, OUTPUT);
  pinMode(TRIG, OUTPUT);
  pinMode(ECHO, INPUT);
  pinMode(LEFT_IR, INPUT);
  pinMode(RIGHT_IR, INPUT);
  stopRobot();
  delay(START_DELAY);
}

void loop() {
  bool leftBorder  = (digitalRead(LEFT_IR)  == WHITE);
  bool rightBorder = (digitalRead(RIGHT_IR) == WHITE);

  // 1. Stay inside the ring
  if (leftBorder || rightBorder) {
    backward(220);
    delay(350);
    if (leftBorder) turnRight(200);
    else            turnLeft(200);
    delay(300);
    return;
  }

  // 2. Attack if opponent is in front
  long distance = readDistanceCM();
  if (distance < ATTACK_RANGE) {
    forward(attackSpeed);
  } else {
    // 3. Search
    turnRight(searchSpeed);
  }
}

long readDistanceCM() {
  digitalWrite(TRIG, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG, LOW);
  long duration = pulseIn(ECHO, HIGH, 10000UL);   // short timeout = fast loop
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
