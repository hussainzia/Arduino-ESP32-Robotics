/*
  ============================================================
  PROJECT : Automatic Pick & Place Arm (factory automation demo)
  LEVEL   : Intermediate
  BOARD   : Arduino UNO / Nano
  ============================================================
  ABOUT
    An IR sensor watches the pick-up spot. When an object
    arrives, the arm picks it up, drops it in the bin, counts
    it and returns home. A mini version of a real factory cell.
    Add a conveyor belt (DC motor) for an impressive exhibition.

  PARTS
    - Arduino UNO
    - 4x servo + robotic arm kit, external 5-6 V supply
    - IR obstacle sensor (FC-51)
    - 16x2 I2C LCD (optional, shows count)
    - Buzzer (optional)

  WIRING
    Base servo     -> D3
    Shoulder servo -> D5
    Elbow servo    -> D6
    Gripper servo  -> D9
    IR sensor OUT  -> D2   (aimed at the pick-up spot)
    Buzzer +       -> D8
    LCD SDA / SCL  -> A4 / A5

  LIBRARIES
    - Servo (built in)
    - LiquidCrystal I2C by Frank de Brabander

  HOW IT WORKS
    1. Waits at HOME until the IR sensor sees an object.
    2. Runs the PICK sequence, then the PLACE sequence (list of poses).
    3. Increments the counter and shows it on the LCD.

  TIPS
    - Teach your own poses: use project 01 (knobs) and Serial Monitor to read
      the angles, then copy them into the pose arrays.
  ============================================================
*/

#include <Servo.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

#define IR_PIN     2
#define BUZZER_PIN 8
#define DETECTED   LOW

const int servoPins[4] = {3, 5, 6, 9};

// Each pose = {base, shoulder, elbow, gripper}
const int HOME_POSE[4] = {90, 90, 90, 70};
const int pickPoses[][4] = {
  {30, 90, 90, 70},    // turn to pick spot, gripper open
  {30, 130, 60, 70},   // lower
  {30, 130, 60, 15},   // grip
  {30, 90, 90, 15},    // lift
};
const int placePoses[][4] = {
  {150, 90, 90, 15},   // turn to bin
  {150, 115, 75, 15},  // lower a bit
  {150, 115, 75, 70},  // release
  {150, 90, 90, 70},   // up
};

Servo joints[4];
float current[4];
LiquidCrystal_I2C lcd(0x27, 16, 2);
int itemCount = 0;

void setup() {
  pinMode(IR_PIN, INPUT);
  pinMode(BUZZER_PIN, OUTPUT);
  for (int i = 0; i < 4; i++) {
    current[i] = HOME_POSE[i];
    joints[i].attach(servoPins[i]);
    joints[i].write(HOME_POSE[i]);
  }
  lcd.init();
  lcd.backlight();
  showCount();
}

void loop() {
  if (digitalRead(IR_PIN) == DETECTED) {
    delay(500);                                   // let the object settle
    if (digitalRead(IR_PIN) != DETECTED) return;  // false alarm

    lcd.setCursor(0, 0); lcd.print("Picking...      ");
    for (unsigned int p = 0; p < sizeof(pickPoses) / sizeof(pickPoses[0]); p++)   moveTo(pickPoses[p]);
    lcd.setCursor(0, 0); lcd.print("Placing...      ");
    for (unsigned int p = 0; p < sizeof(placePoses) / sizeof(placePoses[0]); p++) moveTo(placePoses[p]);
    moveTo(HOME_POSE);

    itemCount++;
    tone(BUZZER_PIN, 1500, 120);
    showCount();
  }
}

void moveTo(const int pose[4]) {
  float start[4];
  for (int i = 0; i < 4; i++) start[i] = current[i];
  const int frames = 40;                       // 40 x 20 ms = 0.8 s per move
  for (int f = 1; f <= frames; f++) {
    for (int i = 0; i < 4; i++) {
      current[i] = start[i] + (pose[i] - start[i]) * f / (float)frames;
      joints[i].write((int)current[i]);
    }
    delay(20);
  }
  delay(150);
}

void showCount() {
  lcd.clear();
  lcd.setCursor(0, 0); lcd.print("Waiting item...");
  lcd.setCursor(0, 1); lcd.print("Sorted: ");
  lcd.print(itemCount);
}
