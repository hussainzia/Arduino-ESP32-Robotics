/*
  ============================================================
  18 - SELF BALANCING ROBOT (2 wheels, MPU6050 + PID)
  ============================================================
  Board  : Arduino UNO / Nano
  Parts  : MPU6050, L298N, 2 geared DC motors, tall chassis
           with the battery on TOP (higher = easier to balance)
  Library: none (MPU6050 is read directly with Wire)

  How it works:
    1. Accelerometer + gyroscope are combined (complementary
       filter) to get a clean tilt angle.
    2. PID turns the tilt into motor speed: if the robot falls
       forward, the wheels drive forward to catch it.

  Mounting:
    Mount the MPU6050 flat, with its X axis pointing along the
    wheel axle (left-right). Tilting forward/back then changes
    the angle printed in Serial Monitor.

  STEP 1 - find the balance point (SETPOINT):
    Set DEBUG to true, upload, hold the robot exactly upright
    and read the "Angle" value. Put that number in SETPOINT.
  STEP 2 - tune PID (set DEBUG back to false):
    a) Ki = 0, Kd = 0. Raise Kp until it oscillates back and forth.
    b) Raise Kd until the oscillation calms down.
    c) Add a little Ki so it stops drifting away.
    If it runs AWAY from the fall instead of under it,
    swap the motor wires (or make Kp negative).

  Wiring:
    MPU6050 VCC -> 5V, GND -> GND, SDA -> A4, SCL -> A5
    L298N: ENA D5, IN1 D7, IN2 D8, IN3 D9, IN4 D10, ENB D6
  ============================================================
*/

#include <Wire.h>

// ---------- Motor driver (L298N) pins ----------
#define ENA 5
#define IN1 7
#define IN2 8
#define IN3 9
#define IN4 10
#define ENB 6

#define MPU_ADDR 0x68
#define DEBUG false            // true = print angle (robot won't balance well)

// ---------- Tune these ----------
float SETPOINT = 0.0;          // upright angle (degrees) from STEP 1
float Kp = 30.0;
float Ki = 0.0;
float Kd = 1.0;
int   MIN_PWM = 40;            // smallest PWM that makes your motors turn
#define FALL_ANGLE 45          // stop motors if tilted more than this

// ---------- Internal variables ----------
const float dt = 0.005;        // 5 ms loop = 200 Hz
float angle = 0;
float integral = 0;
float gyroOffset = 0;
unsigned long lastTime = 0;
int debugCounter = 0;

void setup() {
  Serial.begin(115200);
  pinMode(ENA, OUTPUT); pinMode(ENB, OUTPUT);
  pinMode(IN1, OUTPUT); pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT); pinMode(IN4, OUTPUT);
  setMotor(0, 0);

  Wire.begin();
  Wire.setClock(400000);
  writeMPU(0x6B, 0x00);        // wake up
  writeMPU(0x1A, 0x03);        // digital low-pass filter ~44 Hz
  writeMPU(0x1B, 0x00);        // gyro  +-250 deg/s  (131 LSB per deg/s)
  writeMPU(0x1C, 0x00);        // accel +-2 g        (16384 LSB per g)
  delay(100);

  // Gyro calibration: keep the robot STILL for 2 seconds
  Serial.println("Calibrating gyro - keep the robot still...");
  long sum = 0;
  for (int i = 0; i < 500; i++) {
    int16_t ax, ay, az, gx;
    readMPU(ax, ay, az, gx);
    sum += gx;
    delay(3);
  }
  gyroOffset = sum / 500.0;

  // Start angle from the accelerometer
  int16_t ax, ay, az, gx;
  readMPU(ax, ay, az, gx);
  angle = atan2((float)ay, (float)az) * RAD_TO_DEG;

  Serial.println("Ready!");
  lastTime = micros();
}

void loop() {
  // run exactly every 5 ms
  if (micros() - lastTime < 5000UL) return;
  lastTime += 5000UL;

  int16_t ax, ay, az, gx;
  readMPU(ax, ay, az, gx);

  float accAngle = atan2((float)ay, (float)az) * RAD_TO_DEG;
  float gyroRate = (gx - gyroOffset) / 131.0;           // deg/s

  // Complementary filter: trust gyro short-term, accel long-term
  angle = 0.98 * (angle + gyroRate * dt) + 0.02 * accAngle;

  if (DEBUG && ++debugCounter >= 40) {                   // 5 times a second
    debugCounter = 0;
    Serial.print("Angle: ");
    Serial.println(angle);
  }

  // Fallen over? stop everything
  if (fabs(angle - SETPOINT) > FALL_ANGLE) {
    setMotor(0, 0);
    integral = 0;
    return;
  }

  // ---------- PID ----------
  float error = angle - SETPOINT;
  integral += error * dt;
  integral = constrain(integral, -50, 50);
  float output = Kp * error + Ki * integral + Kd * gyroRate;

  // add MIN_PWM so small corrections still move the wheels
  int pwm = 0;
  if (output > 1)       pwm = (int)output + MIN_PWM;
  else if (output < -1) pwm = (int)output - MIN_PWM;
  pwm = constrain(pwm, -255, 255);

  setMotor(pwm, pwm);
}

// ================= MPU6050 =================
void writeMPU(byte reg, byte value) {
  Wire.beginTransmission(MPU_ADDR);
  Wire.write(reg);
  Wire.write(value);
  Wire.endTransmission(true);
}

void readMPU(int16_t &ax, int16_t &ay, int16_t &az, int16_t &gx) {
  Wire.beginTransmission(MPU_ADDR);
  Wire.write(0x3B);
  Wire.endTransmission(false);
  Wire.requestFrom((uint8_t)MPU_ADDR, (uint8_t)10);
  ax = read16();
  ay = read16();
  az = read16();
  Wire.read(); Wire.read();                 // skip temperature
  gx = read16();
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

// Reads one 16-bit value (high byte first) from the MPU6050
int16_t read16() {
  uint8_t high = Wire.read();
  uint8_t low  = Wire.read();
  return (int16_t)((high << 8) | low);
}
