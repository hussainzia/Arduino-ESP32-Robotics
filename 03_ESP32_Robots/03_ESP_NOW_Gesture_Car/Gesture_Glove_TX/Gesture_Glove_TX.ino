/*
  ============================================================
  PROJECT : ESP-NOW Gesture Car - GLOVE (transmitter)
  LEVEL   : Advanced
  BOARD   : ESP32 Dev Module (or any ESP32 / ESP32-C3)
  ============================================================
  ABOUT
    Tilt your hand to drive. The glove ESP32 reads an MPU6050 and
    sends tilt values to the car over ESP-NOW: fast, long range
    (100 m+), no router and no radio module needed.
    Tilt a little = slow, tilt more = fast.

  PARTS
    - ESP32 dev board (glove)
    - MPU6050 module
    - Small power bank or 3.7 V Li-ion + charger board

  WIRING
    MPU6050 VCC -> 3.3V
    MPU6050 GND -> GND
    MPU6050 SDA -> GPIO21
    MPU6050 SCL -> GPIO22

  LIBRARIES
    - WiFi, esp_now, Wire (ESP32 board package)

  HOW IT WORKS
    1. Pitch (forward/back) and roll (left/right) are calculated from gravity.
    2. A packet {carId, x, y} is broadcast 20 times a second.
    3. The car only obeys packets with ITS carId, so many cars can drive in one room.

  TIPS
    - Change CAR_ID (here AND in the car code) if several teams use this project.
    - Open Serial Monitor at 115200 to see the values being sent.
  ============================================================
*/

#include <WiFi.h>
#include <esp_now.h>
#include <Wire.h>

#define CAR_ID   7          // must match the car
#define MPU_ADDR 0x68
#define MAX_TILT 35.0       // degrees for full speed
#define DEAD_TILT 8.0       // ignore small hand shakes

uint8_t broadcastAddress[] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};

typedef struct {
  uint8_t carId;
  int8_t x;     // -100 (left)  .. 100 (right)
  int8_t y;     // -100 (back)  .. 100 (forward)
} ControlPacket;

ControlPacket packet;

void setup() {
  Serial.begin(115200);

  Wire.begin();
  Wire.beginTransmission(MPU_ADDR);
  Wire.write(0x6B);
  Wire.write(0);
  Wire.endTransmission(true);

  WiFi.mode(WIFI_STA);
  if (esp_now_init() != ESP_OK) {
    Serial.println("ESP-NOW init failed");
    return;
  }
  esp_now_peer_info_t peer = {};
  memcpy(peer.peer_addr, broadcastAddress, 6);
  peer.channel = 0;
  peer.encrypt = false;
  esp_now_add_peer(&peer);
  Serial.println("Glove ready");
}

void loop() {
  int16_t ax, ay, az;
  readAccel(ax, ay, az);

  float pitch = atan2((float)ax, sqrt((float)ay * ay + (float)az * az)) * RAD_TO_DEG;
  float roll  = atan2((float)ay, (float)az) * RAD_TO_DEG;

  packet.carId = CAR_ID;
  packet.y = tiltToSpeed(pitch);
  packet.x = tiltToSpeed(roll);
  esp_now_send(broadcastAddress, (uint8_t *)&packet, sizeof(packet));

  Serial.print("x: "); Serial.print(packet.x);
  Serial.print("  y: "); Serial.println(packet.y);
  delay(50);
}

int8_t tiltToSpeed(float angle) {
  if (fabs(angle) < DEAD_TILT) return 0;
  float s = (fabs(angle) - DEAD_TILT) / (MAX_TILT - DEAD_TILT) * 100.0;
  if (s > 100) s = 100;
  return (int8_t)(angle > 0 ? s : -s);
}

void readAccel(int16_t &ax, int16_t &ay, int16_t &az) {
  Wire.beginTransmission(MPU_ADDR);
  Wire.write(0x3B);
  Wire.endTransmission(false);
  Wire.requestFrom((uint8_t)MPU_ADDR, (uint8_t)6);
  ax = read16();
  ay = read16();
  az = read16();
}

int16_t read16() {
  uint8_t high = Wire.read();
  uint8_t low  = Wire.read();
  return (int16_t)((high << 8) | low);
}
