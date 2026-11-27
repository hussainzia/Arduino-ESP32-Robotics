/*
  ============================================================
  13 - FIRE FIGHTING ROBOT (3 flame sensors + water pump)
  ============================================================
  Board  : Arduino UNO / Nano
  Parts  : 3 IR flame sensor modules (use their AO pin),
           5V relay module, small 3-6V DC water pump + pipe,
           SG90 servo to sweep the nozzle.

  Logic:
    - Drives toward the flame (the sensor with the lowest
      reading sees the strongest flame).
    - When the flame is close, it stops, switches the pump ON
      and sweeps the nozzle left-right until the flame is out.

  Calibrate first!
    Upload 02_Sensor_Test, light a candle and note the analog
    values at ~1 m (FLAME_DETECT) and ~15 cm (FLAME_CLOSE).
    Lower value = stronger flame.

  Wiring:
    Left   flame AO -> A0
    Center flame AO -> A1
    Right  flame AO -> A2
    Relay IN        -> D4   (relay switches the pump's own battery)
    Servo signal    -> D3
    L298N: ENA D5, IN1 D7, IN2 D8, IN3 D9, IN4 D10, ENB D6

  SAFETY: Only test with a small candle, on a non-flammable
  surface, with an adult present. Keep water away from the Arduino.
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

#define FLAME_LEFT   A0
#define FLAME_CENTER A1
#define FLAME_RIGHT  A2
#define PUMP_PIN     4
#define SERVO_PIN    3

#define PUMP_ON  HIGH       // many relay boards are active LOW:
#define PUMP_OFF LOW        // if so, swap HIGH and LOW here

int FLAME_DETECT = 800;     // below this = flame seen
int FLAME_CLOSE  = 250;     // below this = close enough to spray

int driveSpeed = 120;
int turnSpeed  = 130;

Servo nozzle;

void setup() {
  Serial.begin(9600);
  pinMode(ENA, OUTPUT); pinMode(ENB, OUTPUT);
  pinMode(IN1, OUTPUT); pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT); pinMode(IN4, OUTPUT);
  pinMode(PUMP_PIN, OUTPUT);
  digitalWrite(PUMP_PIN, PUMP_OFF);
  nozzle.attach(SERVO_PIN);
  nozzle.write(90);
  stopRobot();
  delay(2000);
}

void loop() {
  int left   = analogRead(FLAME_LEFT);
  int center = analogRead(FLAME_CENTER);
  int right  = analogRead(FLAME_RIGHT);

  Serial.print("L: "); Serial.print(left);
  Serial.print("  C: "); Serial.print(center);
  Serial.print("  R: "); Serial.println(right);

  if (left < FLAME_CLOSE || center < FLAME_CLOSE || right < FLAME_CLOSE) {
    stopRobot();
    extinguishFire();
  } else if (center < FLAME_DETECT && center <= left && center <= right) {
    forward(driveSpeed);
  } else if (left < FLAME_DETECT && left < right) {
    turnLeft(turnSpeed);
  } else if (right < FLAME_DETECT) {
    turnRight(turnSpeed);
  } else {
    stopRobot();          // no fire seen
  }
  delay(50);
}

bool flameStillThere() {
  return analogRead(FLAME_LEFT)   < FLAME_DETECT ||
         analogRead(FLAME_CENTER) < FLAME_DETECT ||
         analogRead(FLAME_RIGHT)  < FLAME_DETECT;
}

void extinguishFire() {
  Serial.println("FIRE! Pump ON");
  digitalWrite(PUMP_PIN, PUMP_ON);

  unsigned long start = millis();
  while (flameStillThere() && millis() - start < 10000UL) {   // max 10 s
    for (int a = 50; a <= 130; a += 5) { nozzle.write(a); delay(20); }
    for (int a = 130; a >= 50; a -= 5) { nozzle.write(a); delay(20); }
  }

  delay(500);                       // a little extra water
  digitalWrite(PUMP_PIN, PUMP_OFF);
  nozzle.write(90);
  Serial.println("Fire out. Pump OFF");
  delay(1000);
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
