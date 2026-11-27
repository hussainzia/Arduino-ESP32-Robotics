/*
  ============================================================
  17 - GESTURE CONTROLLED ROBOT  >>> TRANSMITTER (on your hand) <<<
  ============================================================
  Board   : Arduino Nano (or UNO)
  Parts   : MPU6050 accelerometer + nRF24L01 radio, on a glove
  Library : "RF24" by TMRh20  (Tools -> Manage Libraries -> RF24)
            MPU6050 is read directly with Wire (no extra library)

  Tilt your hand:
    Forward  -> 'F'      Backward -> 'B'
    Left     -> 'L'      Right    -> 'R'
    Flat     -> 'S' (stop)

  Wiring:
    MPU6050 VCC -> 5V, GND -> GND, SDA -> A4, SCL -> A5
    nRF24L01 VCC  -> 3.3V  (NOT 5V!)  + 10uF capacitor across VCC/GND
    nRF24L01 GND  -> GND
    nRF24L01 CE   -> D9
    nRF24L01 CSN  -> D10
    nRF24L01 SCK  -> D13
    nRF24L01 MOSI -> D11
    nRF24L01 MISO -> D12

  If the directions are mixed up, open Serial Monitor (9600),
  tilt your hand and change the signs / axes in getCommand().
  ============================================================
*/

#include <Wire.h>
#include <SPI.h>
#include <RF24.h>

#define MPU_ADDR 0x68
#define TILT_LIMIT 7000      // about 25 degrees (16384 = 1 g)

RF24 radio(9, 10);           // CE, CSN
const byte address[6] = "ROBO1";   // must match the receiver

void setup() {
  Serial.begin(9600);

  // Wake up MPU6050
  Wire.begin();
  Wire.beginTransmission(MPU_ADDR);
  Wire.write(0x6B);          // power management register
  Wire.write(0);             // 0 = wake up
  Wire.endTransmission(true);

  // Radio
  if (!radio.begin()) {
    Serial.println("nRF24L01 not found - check wiring!");
  }
  radio.openWritingPipe(address);
  radio.setPALevel(RF24_PA_LOW);
  radio.stopListening();     // this one only sends
  Serial.println("Gesture transmitter ready");
}

void loop() {
  int16_t ax, ay, az;
  readAccel(ax, ay, az);

  char cmd = getCommand(ax, ay);
  radio.write(&cmd, sizeof(cmd));

  Serial.print("ax: "); Serial.print(ax);
  Serial.print("  ay: "); Serial.print(ay);
  Serial.print("  -> "); Serial.println(cmd);
  delay(50);
}

char getCommand(int16_t ax, int16_t ay) {
  if (ax >  TILT_LIMIT) return 'F';
  if (ax < -TILT_LIMIT) return 'B';
  if (ay >  TILT_LIMIT) return 'L';
  if (ay < -TILT_LIMIT) return 'R';
  return 'S';
}

void readAccel(int16_t &ax, int16_t &ay, int16_t &az) {
  Wire.beginTransmission(MPU_ADDR);
  Wire.write(0x3B);                         // first accel register
  Wire.endTransmission(false);
  Wire.requestFrom((uint8_t)MPU_ADDR, (uint8_t)6);
  ax = read16();
  ay = read16();
  az = read16();
}

// Reads one 16-bit value (high byte first) from the MPU6050
int16_t read16() {
  uint8_t high = Wire.read();
  uint8_t low  = Wire.read();
  return (int16_t)((high << 8) | low);
}
