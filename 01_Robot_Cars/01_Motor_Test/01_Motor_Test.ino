/*
  ============================================================
  01 - MOTOR TEST  (run this FIRST on every new robot)
  ============================================================
  Board  : Arduino UNO / Nano
  Driver : L298N (or L293D module with the same pins)

  What it does:
    Forward 2 s -> Stop -> Backward 2 s -> Stop -> Left -> Right
    Open Serial Monitor (9600) to see which move is running.
    If a wheel turns the wrong way, swap that motor's two wires
    on the L298N (OUT1/OUT2 or OUT3/OUT4).

  Wiring (standard for this whole repository):
    L298N ENA -> D5   (remove the ENA jumper cap)
    L298N IN1 -> D7
    L298N IN2 -> D8
    L298N IN3 -> D9
    L298N IN4 -> D10
    L298N ENB -> D6   (remove the ENB jumper cap)
    L298N GND -> Arduino GND  (common ground is a MUST)
    L298N 12V -> Battery + (7-12 V),  Battery - -> L298N GND
  ============================================================
*/

// ---------- Motor driver (L298N) pins ----------
#define ENA 5    // Left motor speed (PWM)
#define IN1 7    // Left motor direction
#define IN2 8
#define IN3 9    // Right motor direction
#define IN4 10
#define ENB 6    // Right motor speed (PWM)

int motorSpeed = 180;   // 0 - 255

void setup() {
  Serial.begin(9600);
  pinMode(ENA, OUTPUT); pinMode(ENB, OUTPUT);
  pinMode(IN1, OUTPUT); pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT); pinMode(IN4, OUTPUT);
  stopRobot();
  Serial.println("Motor test starts in 3 seconds...");
  delay(3000);
}

void loop() {
  Serial.println("FORWARD");   forward(motorSpeed);   delay(2000);
  Serial.println("STOP");      stopRobot();           delay(1000);
  Serial.println("BACKWARD");  backward(motorSpeed);  delay(2000);
  Serial.println("STOP");      stopRobot();           delay(1000);
  Serial.println("LEFT");      turnLeft(motorSpeed);  delay(1000);
  Serial.println("STOP");      stopRobot();           delay(1000);
  Serial.println("RIGHT");     turnRight(motorSpeed); delay(1000);
  Serial.println("STOP");      stopRobot();           delay(3000);
}

// ================= MOTOR FUNCTIONS =================
// speed range: -255 (full reverse) ... 0 (stop) ... 255 (full forward)
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
void turnLeft(int s)  { setMotor(-s, s); }   // spin in place
void turnRight(int s) { setMotor(s, -s); }   // spin in place
void stopRobot()      { setMotor(0, 0); }
