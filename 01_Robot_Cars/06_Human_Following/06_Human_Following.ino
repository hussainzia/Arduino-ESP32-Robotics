/*
  ============================================================
  06 - HUMAN FOLLOWING ROBOT (Ultrasonic + 2 IR obstacle sensors)
  ============================================================
  Board  : Arduino UNO / Nano
  Parts  : HC-SR04 at front center, two IR obstacle sensors
           (FC-51) at front-left and front-right, facing FORWARD.

  Logic:
    - Person in front between MIN and MAX distance -> follow forward
    - Person too close (< MIN)                    -> back off
    - Person seen only by LEFT IR                 -> turn left
    - Person seen only by RIGHT IR                -> turn right
    - Nothing seen                                -> stop and wait

  Wiring:
    HC-SR04 TRIG -> D12,  ECHO -> D11
    Left  IR OUT -> A0
    Right IR OUT -> A1
    L298N: ENA D5, IN1 D7, IN2 D8, IN3 D9, IN4 D10, ENB D6

  Tip: Set the IR sensor range to ~20-30 cm using the module's
  potentiometer. Walk slowly in front of the robot.
  ============================================================
*/

// ---------- Motor driver (L298N) pins ----------
#define ENA 5
#define IN1 7
#define IN2 8
#define IN3 9
#define IN4 10
#define ENB 6

// ---------- Sensors ----------
#define TRIG 12
#define ECHO 11
#define LEFT_IR  A0
#define RIGHT_IR A1
#define DETECTED LOW        // FC-51 output is LOW when it sees something

#define MIN_DISTANCE 10     // cm - closer than this: move back
#define MAX_DISTANCE 40     // cm - farther than this: person lost

int followSpeed = 150;
int turnSpeed   = 150;

void setup() {
  Serial.begin(9600);
  pinMode(ENA, OUTPUT); pinMode(ENB, OUTPUT);
  pinMode(IN1, OUTPUT); pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT); pinMode(IN4, OUTPUT);
  pinMode(TRIG, OUTPUT);
  pinMode(ECHO, INPUT);
  pinMode(LEFT_IR, INPUT);
  pinMode(RIGHT_IR, INPUT);
  stopRobot();
  delay(2000);
}

void loop() {
  long distance  = readDistanceCM();
  bool leftSees  = (digitalRead(LEFT_IR)  == DETECTED);
  bool rightSees = (digitalRead(RIGHT_IR) == DETECTED);

  Serial.print("Dist: "); Serial.print(distance);
  Serial.print("  L: ");  Serial.print(leftSees);
  Serial.print("  R: ");  Serial.println(rightSees);

  if (distance < MIN_DISTANCE) {
    backward(followSpeed);
  } else if (distance <= MAX_DISTANCE) {
    // person in front - steer while following
    if (leftSees && !rightSees)      setMotor(followSpeed / 3, followSpeed);
    else if (rightSees && !leftSees) setMotor(followSpeed, followSpeed / 3);
    else                             forward(followSpeed);
  } else if (leftSees && !rightSees) {
    turnLeft(turnSpeed);
  } else if (rightSees && !leftSees) {
    turnRight(turnSpeed);
  } else {
    stopRobot();
  }
  delay(50);
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
