/*
  ============================================================
  12 - WALL FOLLOWING ROBOT (2 ultrasonic sensors)
  ============================================================
  Board  : Arduino UNO / Nano
  Parts  : 2x HC-SR04
           FRONT sensor looks forward
           SIDE  sensor looks to the RIGHT (at the wall)

  Logic:
    Keeps WALL_DISTANCE cm away from the right-hand wall using
    proportional (P) control. If a wall appears in front, it
    turns left. If the right wall disappears (open corner),
    it curves right to find it again.

  Wiring:
    FRONT HC-SR04: TRIG -> D12, ECHO -> D11
    SIDE  HC-SR04: TRIG -> A2,  ECHO -> A3
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
#define SIDE_TRIG  A2
#define SIDE_ECHO  A3

#define WALL_DISTANCE  15   // cm - wanted distance from right wall
#define FRONT_LIMIT    20   // cm - wall ahead, turn left
#define LOST_WALL      50   // cm - wall is gone (corner)

int baseSpeed = 140;
float Kp = 6.0;             // steering strength

void setup() {
  Serial.begin(9600);
  pinMode(ENA, OUTPUT); pinMode(ENB, OUTPUT);
  pinMode(IN1, OUTPUT); pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT); pinMode(IN4, OUTPUT);
  pinMode(FRONT_TRIG, OUTPUT); pinMode(FRONT_ECHO, INPUT);
  pinMode(SIDE_TRIG, OUTPUT);  pinMode(SIDE_ECHO, INPUT);
  stopRobot();
  delay(2000);
}

void loop() {
  long front = readDistanceCM(FRONT_TRIG, FRONT_ECHO);
  delay(10);   // avoid echo interference between sensors
  long side  = readDistanceCM(SIDE_TRIG, SIDE_ECHO);

  Serial.print("Front: "); Serial.print(front);
  Serial.print("  Side: "); Serial.println(side);

  if (front < FRONT_LIMIT) {
    turnLeft(160);                        // wall ahead
    delay(250);
  } else if (side > LOST_WALL) {
    setMotor(baseSpeed, baseSpeed / 3);   // corner: curve right
  } else {
    int error = side - WALL_DISTANCE;     // + = too far, - = too close
    int correction = (int)(Kp * error);
    correction = constrain(correction, -100, 100);
    setMotor(baseSpeed + correction, baseSpeed - correction);
  }
  delay(30);
}

long readDistanceCM(int trigPin, int echoPin) {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  long duration = pulseIn(echoPin, HIGH, 30000UL);
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
void turnLeft(int s)  { setMotor(-s, s); }
void stopRobot()      { setMotor(0, 0); }
