/*
  ============================================================
  17 - GESTURE CONTROLLED ROBOT  >>> RECEIVER (on the car) <<<
  ============================================================
  Board   : Arduino UNO / Nano
  Library : "RF24" by TMRh20

  Receives 'F','B','L','R','S' from the glove and drives.
  Stops automatically if no signal for 500 ms.

  !! NOTE: motor pins are DIFFERENT from the other projects !!
  nRF24L01 needs D9-D13, so IN3/IN4 move to D4 and D2.

  Wiring:
    nRF24L01 VCC -> 3.3V (+10uF capacitor), GND -> GND
    nRF24L01 CE  -> D9,  CSN -> D10
    nRF24L01 SCK -> D13, MOSI -> D11, MISO -> D12
    L298N ENA -> D5
    L298N IN1 -> D7
    L298N IN2 -> D8
    L298N IN3 -> D4    <-- changed
    L298N IN4 -> D2    <-- changed
    L298N ENB -> D6
  ============================================================
*/

#include <SPI.h>
#include <RF24.h>

// ---------- Motor driver (L298N) pins ----------
#define ENA 5
#define IN1 7
#define IN2 8
#define IN3 4
#define IN4 2
#define ENB 6

RF24 radio(9, 10);                 // CE, CSN
const byte address[6] = "ROBO1";   // must match the transmitter

int carSpeed = 180;
unsigned long lastReceived = 0;

void setup() {
  Serial.begin(9600);
  pinMode(ENA, OUTPUT); pinMode(ENB, OUTPUT);
  pinMode(IN1, OUTPUT); pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT); pinMode(IN4, OUTPUT);
  stopRobot();

  if (!radio.begin()) {
    Serial.println("nRF24L01 not found - check wiring!");
  }
  radio.openReadingPipe(0, address);
  radio.setPALevel(RF24_PA_LOW);
  radio.startListening();          // this one only receives
  Serial.println("Gesture receiver ready");
}

void loop() {
  if (radio.available()) {
    char cmd;
    radio.read(&cmd, sizeof(cmd));
    lastReceived = millis();
    Serial.println(cmd);

    switch (cmd) {
      case 'F': forward(carSpeed);   break;
      case 'B': backward(carSpeed);  break;
      case 'L': turnLeft(carSpeed);  break;
      case 'R': turnRight(carSpeed); break;
      default:  stopRobot();         break;
    }
  }

  // Fail-safe: lost signal -> stop
  if (millis() - lastReceived > 500) {
    stopRobot();
  }
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
