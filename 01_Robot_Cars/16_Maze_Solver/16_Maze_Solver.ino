/*
  ============================================================
  16 - MAZE SOLVING ROBOT (left-hand rule, 3 ultrasonic sensors)
  ============================================================
  Board  : Arduino UNO / Nano
  Parts  : 3x HC-SR04 looking FRONT, LEFT and RIGHT

  Left-hand rule (always keep your left hand on the wall):
    1. If LEFT is open  -> turn left, then move forward
    2. Else if FRONT is open -> go forward
    3. Else if RIGHT is open -> turn right
    4. Else (dead end) -> U-turn
  This solves any maze whose walls are all connected.

  Calibrate TURN_90_TIME:
    Put the robot on the floor, change the number until one
    turnLeft() makes exactly a 90 degree turn.

  Wiring:
    FRONT HC-SR04: TRIG -> D12, ECHO -> D11
    LEFT  HC-SR04: TRIG -> A0,  ECHO -> A1
    RIGHT HC-SR04: TRIG -> A2,  ECHO -> A3
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

#define FRONT_TRIG 12
#define FRONT_ECHO 11
#define LEFT_TRIG  A0
#define LEFT_ECHO  A1
#define RIGHT_TRIG A2
#define RIGHT_ECHO A3

#define OPEN_SIDE   25   // cm - side gap bigger than this = open path
#define OPEN_FRONT  20   // cm - front gap bigger than this = can go
#define TURN_90_TIME 420 // ms - calibrate for your robot!
#define CLEAR_TIME   350 // ms - drive forward after a turn to enter the new path

int driveSpeed = 130;
int turnSpeed  = 160;

void setup() {
  Serial.begin(9600);
  pinMode(ENA, OUTPUT); pinMode(ENB, OUTPUT);
  pinMode(IN1, OUTPUT); pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT); pinMode(IN4, OUTPUT);
  pinMode(FRONT_TRIG, OUTPUT); pinMode(FRONT_ECHO, INPUT);
  pinMode(LEFT_TRIG, OUTPUT);  pinMode(LEFT_ECHO, INPUT);
  pinMode(RIGHT_TRIG, OUTPUT); pinMode(RIGHT_ECHO, INPUT);
  stopRobot();
  delay(2000);
}

void loop() {
  long front = readDistanceCM(FRONT_TRIG, FRONT_ECHO); delay(10);
  long left  = readDistanceCM(LEFT_TRIG,  LEFT_ECHO);  delay(10);
  long right = readDistanceCM(RIGHT_TRIG, RIGHT_ECHO);

  Serial.print("F: "); Serial.print(front);
  Serial.print("  L: "); Serial.print(left);
  Serial.print("  R: "); Serial.println(right);

  if (left > OPEN_SIDE) {
    // let the robot body pass the corner before turning
    forward(driveSpeed); delay(150);
    stopRobot();         delay(100);
    turnLeft(turnSpeed); delay(TURN_90_TIME);
    forward(driveSpeed); delay(CLEAR_TIME);
  } else if (front > OPEN_FRONT) {
    forward(driveSpeed);
    delay(60);
  } else if (right > OPEN_SIDE) {
    stopRobot();          delay(100);
    turnRight(turnSpeed); delay(TURN_90_TIME);
    forward(driveSpeed);  delay(CLEAR_TIME);
  } else {
    // dead end -> U-turn
    stopRobot();          delay(100);
    turnRight(turnSpeed); delay(TURN_90_TIME * 2);
  }
}

long readDistanceCM(int trigPin, int echoPin) {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  long duration = pulseIn(echoPin, HIGH, 20000UL);
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
