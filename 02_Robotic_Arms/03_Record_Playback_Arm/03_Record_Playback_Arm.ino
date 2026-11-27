/*
  ============================================================
  PROJECT : Teach & Repeat Robotic Arm (record and playback)
  LEVEL   : Intermediate
  BOARD   : Arduino UNO / Nano
  ============================================================
  ABOUT
    Teach the arm a job, then it repeats it by itself - just like
    factory robots. Move the arm with the knobs, press RECORD to
    save each position, then press PLAY. The steps are stored in
    EEPROM, so the program survives power-off.

  PARTS
    - Arduino UNO
    - 4x servo + robotic arm kit
    - 4x 10k potentiometer
    - 2x push button, LED (or use built-in D13)
    - External 5-6 V 2 A supply for servos

  WIRING
    Pots (middle pin) -> A0, A1, A2, A3   (Base, Shoulder, Elbow, Gripper)
    Servos            -> D3, D5, D6, D9
    RECORD button     -> D2   (other leg to GND)
    PLAY button       -> D4   (other leg to GND)
    Status LED        -> D13  (built-in)

  LIBRARIES
    - Servo, EEPROM (built in)

  HOW IT WORKS
    TEACH mode : knobs move the arm. Short press RECORD = save step (LED blinks).
                 Hold RECORD 2 s = erase all steps.
    PLAY mode  : press PLAY - arm smoothly moves through all steps
                 in a loop. Press PLAY again to stop.

  TIPS
    - Up to 40 steps. Add 2 identical steps to make the arm pause.
    - Change MOVE_TIME for faster/slower playback.
  ============================================================
*/

#include <Servo.h>
#include <EEPROM.h>

const int potPins[4]   = {A0, A1, A2, A3};
const int servoPins[4] = {3, 5, 6, 9};
const int minAngle[4]  = {0,   20,  20,  10};
const int maxAngle[4]  = {180, 160, 160, 90};

#define REC_BUTTON  2
#define PLAY_BUTTON 4
#define LED_PIN     13
#define MAX_STEPS   40
#define MOVE_TIME   800    // ms to move between two steps

// EEPROM layout: address 0 = number of steps, then 4 bytes per step
Servo joints[4];
byte steps[MAX_STEPS][4];
int stepCount = 0;
float current[4];
bool playing = false;

void setup() {
  Serial.begin(9600);
  pinMode(REC_BUTTON, INPUT_PULLUP);
  pinMode(PLAY_BUTTON, INPUT_PULLUP);
  pinMode(LED_PIN, OUTPUT);

  loadFromEEPROM();
  for (int i = 0; i < 4; i++) {
    current[i] = readKnob(i);
    joints[i].attach(servoPins[i]);
    joints[i].write((int)current[i]);
  }
  Serial.print("Steps in memory: ");
  Serial.println(stepCount);
}

void loop() {
  if (buttonPressed(PLAY_BUTTON)) {
    playing = !playing;
    Serial.println(playing ? "PLAY" : "TEACH");
  }

  if (playing) {
    if (stepCount == 0) { playing = false; return; }
    for (int s = 0; s < stepCount && playing; s++) {
      moveSmooth(steps[s]);
      if (buttonPressed(PLAY_BUTTON)) { playing = false; Serial.println("TEACH"); }
    }
    return;
  }

  // ----- TEACH mode -----
  for (int i = 0; i < 4; i++) {
    current[i] += (readKnob(i) - current[i]) * 0.2;
    joints[i].write((int)current[i]);
  }

  if (digitalRead(REC_BUTTON) == LOW) {
    unsigned long start = millis();
    while (digitalRead(REC_BUTTON) == LOW) {
      if (millis() - start > 2000) {          // long press = erase
        stepCount = 0;
        saveToEEPROM();
        Serial.println("Memory erased");
        blink(5);
        while (digitalRead(REC_BUTTON) == LOW);
        return;
      }
    }
    if (stepCount < MAX_STEPS) {               // short press = record
      for (int i = 0; i < 4; i++) steps[stepCount][i] = (byte)current[i];
      stepCount++;
      saveToEEPROM();
      Serial.print("Saved step ");
      Serial.println(stepCount);
      blink(1);
    } else {
      Serial.println("Memory full");
    }
  }
  delay(20);
}

int readKnob(int i) {
  return map(analogRead(potPins[i]), 0, 1023, minAngle[i], maxAngle[i]);
}

// Moves all joints together so they arrive at the same time
void moveSmooth(byte target[4]) {
  float start[4];
  for (int i = 0; i < 4; i++) start[i] = current[i];
  int frames = MOVE_TIME / 20;
  for (int f = 1; f <= frames; f++) {
    for (int i = 0; i < 4; i++) {
      current[i] = start[i] + (target[i] - start[i]) * f / (float)frames;
      joints[i].write((int)current[i]);
    }
    delay(20);
  }
  delay(200);
}

bool buttonPressed(int pin) {
  if (digitalRead(pin) == LOW) {
    delay(30);
    while (digitalRead(pin) == LOW);
    return true;
  }
  return false;
}

void blink(int times) {
  for (int i = 0; i < times; i++) {
    digitalWrite(LED_PIN, HIGH); delay(100);
    digitalWrite(LED_PIN, LOW);  delay(100);
  }
}

void saveToEEPROM() {
  EEPROM.update(0, stepCount);
  for (int s = 0; s < stepCount; s++)
    for (int i = 0; i < 4; i++)
      EEPROM.update(1 + s * 4 + i, steps[s][i]);
}

void loadFromEEPROM() {
  stepCount = EEPROM.read(0);
  if (stepCount > MAX_STEPS) stepCount = 0;   // empty / new EEPROM reads 255
  for (int s = 0; s < stepCount; s++)
    for (int i = 0; i < 4; i++)
      steps[s][i] = EEPROM.read(1 + s * 4 + i);
}
