/*
  ============================================================
  PROJECT : ESP-NOW Gesture Car - CAR (receiver)
  LEVEL   : Advanced
  BOARD   : ESP32 Dev Module
  ============================================================
  ABOUT
    Receives tilt packets from the glove over ESP-NOW and drives
    with proportional speed. Stops if the glove signal is lost.

  PARTS
    - ESP32 dev board (car)
    - L298N, motors, chassis, battery

  WIRING
    L298N ENA -> GPIO14
    L298N IN1 -> GPIO27
    L298N IN2 -> GPIO26
    L298N IN3 -> GPIO25
    L298N IN4 -> GPIO33
    L298N ENB -> GPIO32
    L298N 5V / GND -> ESP32 VIN / GND

  LIBRARIES
    - WiFi, esp_now (ESP32 board package 2.x or 3.x - both supported)

  HOW IT WORKS
    1. onReceive() runs whenever a packet arrives and stores x, y.
    2. loop() mixes x/y into left/right motor speeds.
    3. No packet for 500 ms -> stop.

  TIPS
    - CAR_ID must match the glove.
  ============================================================
*/

#include <WiFi.h>
#include <esp_now.h>

#define ENA 14
#define IN1 27
#define IN2 26
#define IN3 25
#define IN4 33
#define ENB 32

#define CAR_ID 7

typedef struct {
  uint8_t carId;
  int8_t x;
  int8_t y;
} ControlPacket;

volatile int8_t joyX = 0, joyY = 0;
volatile unsigned long lastPacket = 0;

// The callback signature changed in ESP32 core 3.x - this handles both
#if ESP_ARDUINO_VERSION_MAJOR >= 3
void onReceive(const esp_now_recv_info_t *info, const uint8_t *data, int len) {
#else
void onReceive(const uint8_t *mac, const uint8_t *data, int len) {
#endif
  if (len != sizeof(ControlPacket)) return;
  ControlPacket p;
  memcpy(&p, data, sizeof(p));
  if (p.carId != CAR_ID) return;
  joyX = p.x;
  joyY = p.y;
  lastPacket = millis();
}

void setup() {
  Serial.begin(115200);
  pinMode(ENA, OUTPUT); pinMode(ENB, OUTPUT);
  pinMode(IN1, OUTPUT); pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT); pinMode(IN4, OUTPUT);
  setMotor(0, 0);

  WiFi.mode(WIFI_STA);
  if (esp_now_init() != ESP_OK) {
    Serial.println("ESP-NOW init failed");
    return;
  }
  esp_now_register_recv_cb(onReceive);
  Serial.println("Car ready, waiting for glove...");
}

void loop() {
  if (millis() - lastPacket > 500) {
    setMotor(0, 0);
    return;
  }
  int left  = constrain(joyY + joyX, -100, 100);
  int right = constrain(joyY - joyX, -100, 100);
  setMotor(left * 255 / 100, right * 255 / 100);
  delay(20);
}

void setMotor(int leftSpeed, int rightSpeed) {
  if (leftSpeed >= 0) { digitalWrite(IN1, HIGH); digitalWrite(IN2, LOW); }
  else                { digitalWrite(IN1, LOW);  digitalWrite(IN2, HIGH); leftSpeed = -leftSpeed; }
  if (rightSpeed >= 0) { digitalWrite(IN3, HIGH); digitalWrite(IN4, LOW); }
  else                 { digitalWrite(IN3, LOW);  digitalWrite(IN4, HIGH); rightSpeed = -rightSpeed; }
  analogWrite(ENA, constrain(leftSpeed, 0, 255));
  analogWrite(ENB, constrain(rightSpeed, 0, 255));
}
