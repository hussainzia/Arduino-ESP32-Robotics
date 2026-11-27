/*
  ============================================================
  05 - OBSTACLE AVOIDING ROBOT (Ultrasonic + Servo)
  ============================================================
  Board  : Arduino UNO / Nano
  Parts  : HC-SR04 mounted on an SG90 servo ("head")

  Logic:
    1. Look straight and drive forward.
    2. If something is closer than STOP_DISTANCE:
         stop -> back up a little -> look right -> look left
         -> turn toward the side with more free space.

  Wiring:
    HC-SR04 TRIG -> D12
    HC-SR04 ECHO -> D11
    Servo signal -> D3   (servo red 5V, brown/black GND)
    L298N: ENA D5, IN1 D7, IN2 D8, IN3 D9, IN4 D10, ENB D6

  Note: Servo library is included with the Arduino IDE.
  ============================================================
*/

#include <Servo.h>

// ---------- Motor driver (L298N) pins ----------
#define ENA 5
#define IN1 7
#define IN2 8
#define IN3 9
#define IN4 10
#define ENB 6

// ---------- Sensor / servo pins ----------
#define TRIG 12
#define ECHO 11
#define SERVO_PIN 3

#define STOP_DISTANCE 25   // cm
#define LOOK_FRONT 90      // servo angles - adjust if your servo
#define LOOK_RIGHT 20      // is mounted differently
#define LOOK_LEFT  160

int driveSpeed = 150;
int turnSpeed  = 170;
int turnTime   = 400;      // ms for about a 90 degree turn

Servo head;

void setup() {
  Serial.begin(9600);
  pinMode(ENA, OUTPUT); pinMode(ENB, OUTPUT);
  pinMode(IN1, OUTPUT); pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT); pinMode(IN4, OUTPUT);
  pinMode(TRIG, OUTPUT);
  pinMode(ECHO, INPUT);

  head.attach(SERVO_PIN);
  head.write(LOOK_FRONT);
  stopRobot();
  delay(2000);
}

void loop() {
  long distance = readDistanceCM();
  Serial.println(distance);

  if (distance > STOP_DISTANCE) {
    forward(driveSpeed);
    delay(50);
    return;
  }

  // ----- obstacle found -----
  stopRobot();
  delay(200);
  backward(driveSpeed);
  delay(300);
  stopRobot();

  head.write(LOOK_RIGHT);
  delay(500);
  long rightDist = readDistanceCM();

  head.write(LOOK_LEFT);
  delay(700);
  long leftDist = readDistanceCM();

  head.write(LOOK_FRONT);
  delay(400);

  if (rightDist >= leftDist) {
    turnRight(turnSpeed);
  } else {
    turnLeft(turnSpeed);
  }
  delay(turnTime);
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
