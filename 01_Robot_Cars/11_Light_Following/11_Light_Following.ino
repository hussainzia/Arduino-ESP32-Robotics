/*
  ============================================================
  11 - LIGHT FOLLOWING ROBOT (2 LDRs + phone torch)
  ============================================================
  Board  : Arduino UNO / Nano
  Parts  : 2 LDRs (light sensors) at front-left and front-right,
           each with a 10k resistor. A small cardboard wall
           between them makes steering much better.

  LDR circuit (one for each side):
      5V ---- LDR ----+---- 10k ---- GND
                      |
                   A0 (left) / A1 (right)
    More light -> higher reading.

  Logic:
    At startup the robot measures normal room light (keep
    the torch OFF for the first 2 seconds).
    Torch on the left  -> turn left
    Torch on the right -> turn right
    Torch in front     -> go forward
    No torch           -> stop

  Wiring:
    Left LDR  -> A0,  Right LDR -> A1
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

#define LEFT_LDR  A0
#define RIGHT_LDR A1

int lightMargin = 100;   // how much brighter than room light counts as "torch"
int sideMargin  = 60;    // left/right difference needed to turn
int driveSpeed  = 150;
int turnSpeed   = 140;

int ambientLeft, ambientRight;

void setup() {
  Serial.begin(9600);
  pinMode(ENA, OUTPUT); pinMode(ENB, OUTPUT);
  pinMode(IN1, OUTPUT); pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT); pinMode(IN4, OUTPUT);
  stopRobot();

  // Calibrate room light (average of 20 readings)
  long sumL = 0, sumR = 0;
  for (int i = 0; i < 20; i++) {
    sumL += analogRead(LEFT_LDR);
    sumR += analogRead(RIGHT_LDR);
    delay(100);
  }
  ambientLeft  = sumL / 20;
  ambientRight = sumR / 20;
  Serial.print("Room light L/R: ");
  Serial.print(ambientLeft); Serial.print(" / "); Serial.println(ambientRight);
}

void loop() {
  int left  = analogRead(LEFT_LDR)  - ambientLeft;
  int right = analogRead(RIGHT_LDR) - ambientRight;

  Serial.print("L: "); Serial.print(left);
  Serial.print("  R: "); Serial.println(right);

  if (left < lightMargin && right < lightMargin) {
    stopRobot();                         // no torch
  } else if (left - right > sideMargin) {
    turnLeft(turnSpeed);                 // brighter on left
  } else if (right - left > sideMargin) {
    turnRight(turnSpeed);                // brighter on right
  } else {
    forward(driveSpeed);                 // torch in front
  }
  delay(30);
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
