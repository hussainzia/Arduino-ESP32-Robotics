/*
  ============================================================
  02 - SENSOR TEST  (check sensors before building a robot)
  ============================================================
  Board : Arduino UNO / Nano
  Prints live readings to Serial Monitor (9600 baud):
    - 5 IR sensors on A0..A4  (1 = HIGH, 0 = LOW)
    - Analog value of A0..A4   (useful for LDR / flame sensors)
    - HC-SR04 ultrasonic distance in cm

  Wiring:
    IR sensor OUT -> A0, A1, A2, A3, A4   (VCC 5V, GND GND)
    HC-SR04 TRIG  -> D12
    HC-SR04 ECHO  -> D11
    HC-SR04 VCC   -> 5V,  GND -> GND

  Typical IR module (TCRT5000 / FC-51):
    White / object detected  -> LOW  (0), LED on module ON
    Black / nothing detected -> HIGH (1)
    Turn the small blue potentiometer to adjust sensitivity.
  ============================================================
*/

#define TRIG 12
#define ECHO 11

const int sensorPins[5] = {A0, A1, A2, A3, A4};

void setup() {
  Serial.begin(9600);
  for (int i = 0; i < 5; i++) pinMode(sensorPins[i], INPUT);
  pinMode(TRIG, OUTPUT);
  pinMode(ECHO, INPUT);
  Serial.println("Sensor test ready");
}

void loop() {
  Serial.print("Digital: ");
  for (int i = 0; i < 5; i++) {
    Serial.print(digitalRead(sensorPins[i]));
    Serial.print(" ");
  }

  Serial.print(" | Analog: ");
  for (int i = 0; i < 5; i++) {
    Serial.print(analogRead(sensorPins[i]));
    Serial.print(" ");
  }

  Serial.print(" | Distance: ");
  Serial.print(readDistanceCM());
  Serial.println(" cm");

  delay(300);
}

// Returns distance in cm, or 999 if nothing is in range
long readDistanceCM() {
  digitalWrite(TRIG, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG, LOW);
  long duration = pulseIn(ECHO, HIGH, 30000UL);  // 30 ms timeout (~5 m)
  if (duration == 0) return 999;
  return duration / 58;
}
