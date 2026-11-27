/*
  ============================================================
  PROJECT : Bluetooth Controlled Robotic Arm (phone app)
  LEVEL   : Intermediate
  BOARD   : Arduino UNO / Nano
  ============================================================
  ABOUT
    Control each joint of the arm from your phone. Works with
    a simple terminal app or your own MIT App Inventor app with
    4 sliders. Also has a saved "wave hello" animation.

  PARTS
    - Arduino UNO
    - HC-05 / HC-06 Bluetooth module, 1k + 2k resistors
    - 4x servo + robotic arm kit, external 5-6 V supply

  WIRING
    HC-05 TX  -> D2
    HC-05 RX  -> D4   (through 1k/2k voltage divider)
    HC-05 VCC / GND -> 5V / GND
    Base servo     -> D3
    Shoulder servo -> D5
    Elbow servo    -> D6
    Gripper servo  -> D9

  LIBRARIES
    - Servo, SoftwareSerial (built in)

  APP
    "Serial Bluetooth Terminal" (Android) - type commands and press send.
    Or build an MIT App Inventor app with sliders that send the same text.

  HOW IT WORKS
    Send a letter + angle, ending with a new line:
      B90  = base to 90        S120 = shoulder to 120
      E60  = elbow to 60       G30  = gripper to 30
      H    = home position     W    = wave hello
    Joints move smoothly to the new angle.

  TIPS
    - In Serial Bluetooth Terminal, set "Newline" to LF in settings.
  ============================================================
*/

#include <Servo.h>
#include <SoftwareSerial.h>

SoftwareSerial bt(2, 4);   // RX, TX

const int servoPins[4] = {3, 5, 6, 9};
const char jointKey[4] = {'B', 'S', 'E', 'G'};
const int minAngle[4]  = {0,   20,  20,  10};
const int maxAngle[4]  = {180, 160, 160, 90};
const int homeAngle[4] = {90,  90,  90,  45};

Servo joints[4];
float current[4];
int target[4];
String command = "";

void setup() {
  Serial.begin(9600);
  bt.begin(9600);
  for (int i = 0; i < 4; i++) {
    current[i] = target[i] = homeAngle[i];
    joints[i].attach(servoPins[i]);
    joints[i].write(homeAngle[i]);
  }
  bt.println("Arm ready. Send B90, S120, E60, G30, H or W");
}

void loop() {
  while (bt.available()) {
    char c = bt.read();
    if (c == '\n' || c == '\r') {
      if (command.length() > 0) handleCommand(command);
      command = "";
    } else {
      command += c;
    }
  }
  smoothStep();
  delay(15);
}

void handleCommand(String cmd) {
  cmd.trim();
  cmd.toUpperCase();
  Serial.println(cmd);
  char key = cmd.charAt(0);

  if (key == 'H') {
    for (int i = 0; i < 4; i++) target[i] = homeAngle[i];
    return;
  }
  if (key == 'W') { waveHello(); return; }

  for (int i = 0; i < 4; i++) {
    if (key == jointKey[i]) {
      int a = cmd.substring(1).toInt();
      target[i] = constrain(a, minAngle[i], maxAngle[i]);
      bt.print(jointKey[i]); bt.print(" -> "); bt.println(target[i]);
    }
  }
}

// move each joint a little closer to its target
void smoothStep() {
  for (int i = 0; i < 4; i++) {
    current[i] += (target[i] - current[i]) * 0.1;
    joints[i].write((int)current[i]);
  }
}

void moveAndWait(int b, int s, int e, int g) {
  target[0] = b; target[1] = s; target[2] = e; target[3] = g;
  for (int k = 0; k < 60; k++) { smoothStep(); delay(15); }
}

void waveHello() {
  moveAndWait(90, 70, 40, 45);
  for (int k = 0; k < 3; k++) {
    moveAndWait(60, 70, 40, 10);
    moveAndWait(120, 70, 40, 80);
  }
  moveAndWait(homeAngle[0], homeAngle[1], homeAngle[2], homeAngle[3]);
}
