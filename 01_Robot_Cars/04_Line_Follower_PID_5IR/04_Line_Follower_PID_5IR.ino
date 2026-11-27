/*
  ============================================================
  04 - FAST LINE FOLLOWER WITH PID (5 IR sensors)
  ============================================================
  Board  : Arduino UNO / Nano
  Sensor : 5-channel IR array (TCRT5000) or 5 single IR modules
  Track  : Black line on white floor

  Sensor placement (front of robot, facing down):
       S1   S2   S3   S4   S5
      (A0) (A1) (A2) (A3) (A4)
     LEFT      CENTER      RIGHT
    The line should be under S3 when the robot is centered.

  How PID works here:
    error  = where the line is  (-4 = far left, 0 = center, +4 = far right)
    P      = react to the current error
    I      = fix small error that stays for a long time
    D      = stop the robot from wobbling (zig-zag)
    left motor  = baseSpeed + correction
    right motor = baseSpeed - correction

  Tuning steps:
    1. Set Ki = 0, Kd = 0. Increase Kp until robot follows but wobbles.
    2. Increase Kd until the wobble stops.
    3. Add a very small Ki only if needed.

  Wiring:
    S1..S5 OUT -> A0..A4
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

// ---------- Sensors ----------
const int sensorPins[5]   = {A0, A1, A2, A3, A4};
const int sensorWeight[5] = {-4, -2, 0, 2, 4};
#define ON_LINE HIGH      // change to LOW if your sensors are inverted

// ---------- PID settings (tune these!) ----------
float Kp = 25.0;
float Ki = 0.0;
float Kd = 15.0;

int baseSpeed = 150;      // 0-255
int maxSpeed  = 230;      // 0-255

float lastError = 0;
float integral  = 0;

void setup() {
  Serial.begin(9600);
  pinMode(ENA, OUTPUT); pinMode(ENB, OUTPUT);
  pinMode(IN1, OUTPUT); pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT); pinMode(IN4, OUTPUT);
  for (int i = 0; i < 5; i++) pinMode(sensorPins[i], INPUT);
  stopRobot();
  delay(2000);
}

void loop() {
  int sum = 0;
  int count = 0;

  for (int i = 0; i < 5; i++) {
    if (digitalRead(sensorPins[i]) == ON_LINE) {
      sum += sensorWeight[i];
      count++;
    }
  }

  // Line lost: spin toward the side where the line was last seen
  if (count == 0) {
    if (lastError < 0) setMotor(-120, 120);   // line was on the left
    else               setMotor(120, -120);   // line was on the right
    return;
  }

  // All sensors on black: junction / finish -> stop
  if (count == 5) {
    stopRobot();
    return;
  }

  float error = (float)sum / count;

  integral += error;
  integral = constrain(integral, -100, 100);  // anti wind-up
  float derivative = error - lastError;
  float correction = Kp * error + Ki * integral + Kd * derivative;
  lastError = error;

  int leftSpeed  = constrain(baseSpeed + (int)correction, -maxSpeed, maxSpeed);
  int rightSpeed = constrain(baseSpeed - (int)correction, -maxSpeed, maxSpeed);
  setMotor(leftSpeed, rightSpeed);
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
void stopRobot() { setMotor(0, 0); }
