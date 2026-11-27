/*
  ============================================================
  PROJECT : ESP32 Smart Car - Manual + Autonomous mode with live dashboard
  LEVEL   : Advanced
  BOARD   : ESP32 Dev Module
  ============================================================
  ABOUT
    One car, two brains. MANUAL: drive with arrow buttons on your
    phone. AUTO: the car explores by itself and avoids obstacles.
    The dashboard shows live distance and mode. Headlights turn on
    automatically in auto mode. Great for exhibitions.

  PARTS
    - ESP32 dev board
    - L298N, motors, chassis, battery
    - HC-SR04 ultrasonic sensor + 1k and 2k resistors
    - 2 white LEDs + 220 ohm resistors (headlights)

  WIRING
    L298N ENA -> GPIO14
    L298N IN1 -> GPIO27
    L298N IN2 -> GPIO26
    L298N IN3 -> GPIO25
    L298N IN4 -> GPIO33
    L298N ENB -> GPIO32
    HC-SR04 TRIG -> GPIO19
    HC-SR04 ECHO -> GPIO18   (through 1k/2k divider: ECHO -1k- GPIO18 -2k- GND)
    HC-SR04 VCC  -> VIN (5V)
    Headlight LEDs -> GPIO2   (through 220 ohm)

  LIBRARIES
    - WiFi, WebServer (ESP32 board package)

  HOW IT WORKS
    1. WiFi "SmartCar" / 12345678, open 192.168.4.1.
    2. AUTO uses a small state machine (DRIVE -> BACK -> TURN) that never
       blocks, so the web page stays responsive.
    3. MANUAL also has a safety brake: forward stops if an obstacle is < 15 cm.

  TIPS
    - The ECHO divider is required: the HC-SR04 outputs 5 V, the ESP32 accepts only 3.3 V.
  ============================================================
*/

#include <WiFi.h>
#include <WebServer.h>

#define ENA 14
#define IN1 27
#define IN2 26
#define IN3 25
#define IN4 33
#define ENB 32
#define TRIG 19
#define ECHO 18
#define LIGHTS 2

#define STOP_DISTANCE 25
#define BRAKE_DISTANCE 15

enum AutoState { DRIVE, BACK, TURN };

WebServer server(80);
bool autoMode = false;
char manualCmd = 'S';
int carSpeed = 180;
long distanceCm = 999;
AutoState state = DRIVE;
unsigned long stateUntil = 0;
unsigned long lastCommand = 0;

const char PAGE[] PROGMEM = R"HTML(
<!DOCTYPE html><html><head><meta name="viewport" content="width=device-width,initial-scale=1,user-scalable=no">
<title>Smart Car</title><style>
body{font-family:sans-serif;background:#0f172a;color:#e2e8f0;text-align:center;margin:0;padding:14px;user-select:none;touch-action:none}
.g{display:grid;grid-template-columns:repeat(3,84px);gap:10px;justify-content:center;margin:16px}
button{height:84px;font-size:28px;border:0;border-radius:14px;background:#3b82f6;color:#fff}
#mode{width:260px;height:56px;font-size:20px;background:#a855f7}
.big{font-size:40px;font-weight:bold}
</style></head><body><h2>Smart Car</h2>
<div>Distance</div><div class="big"><span id="d">--</span> cm</div>
<button id="mode" onclick="fetch('/mode').then(u)">MODE: MANUAL</button>
<div class="g"><div></div><button data-c="F">&#9650;</button><div></div>
<button data-c="L">&#9664;</button><button data-c="S" style="background:#ef4444">&#9632;</button><button data-c="R">&#9654;</button>
<div></div><button data-c="B">&#9660;</button><div></div></div>
<script>
document.querySelectorAll('[data-c]').forEach(function(b){
 b.addEventListener('pointerdown',function(){fetch('/cmd?c='+b.dataset.c);});
 b.addEventListener('pointerup',function(){fetch('/cmd?c=S');});});
function u(){fetch('/status').then(function(r){return r.json();}).then(function(s){
 document.getElementById('d').innerText=s.d>=999?'--':s.d;
 document.getElementById('mode').innerText='MODE: '+(s.auto?'AUTO':'MANUAL');});}
setInterval(u,300);
setInterval(function(){fetch('/ping');},400);
</script></body></html>
)HTML";

void setup() {
  Serial.begin(115200);
  pinMode(ENA, OUTPUT); pinMode(ENB, OUTPUT);
  pinMode(IN1, OUTPUT); pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT); pinMode(IN4, OUTPUT);
  pinMode(TRIG, OUTPUT); pinMode(ECHO, INPUT);
  pinMode(LIGHTS, OUTPUT);
  setMotor(0, 0);

  WiFi.softAP("SmartCar", "12345678");
  Serial.println(WiFi.softAPIP());

  server.on("/", []() { server.send_P(200, "text/html", PAGE); });
  server.on("/cmd", []() {
    manualCmd = server.arg("c").charAt(0);
    lastCommand = millis();
    server.send(200, "text/plain", "OK");
  });
  server.on("/ping", []() { lastCommand = millis(); server.send(200, "text/plain", "OK"); });
  server.on("/mode", []() {
    autoMode = !autoMode;
    manualCmd = 'S';
    state = DRIVE;
    setMotor(0, 0);
    digitalWrite(LIGHTS, autoMode ? HIGH : LOW);
    server.send(200, "text/plain", "OK");
  });
  server.on("/status", []() {
    server.send(200, "application/json",
                "{\"d\":" + String(distanceCm) + ",\"auto\":" + String(autoMode ? 1 : 0) + "}");
  });
  server.begin();
}

unsigned long lastMeasure = 0;

void loop() {
  server.handleClient();

  if (millis() - lastMeasure > 60) {
    lastMeasure = millis();
    distanceCm = readDistanceCM();
  }

  if (autoMode) runAuto();
  else          runManual();
}

void runManual() {
  if (millis() - lastCommand > 1000) manualCmd = 'S';   // phone disconnected
  switch (manualCmd) {
    case 'F':
      if (distanceCm < BRAKE_DISTANCE) setMotor(0, 0);   // safety brake
      else setMotor(carSpeed, carSpeed);
      break;
    case 'B': setMotor(-carSpeed, -carSpeed);      break;
    case 'L': setMotor(-carSpeed, carSpeed);       break;
    case 'R': setMotor(carSpeed, -carSpeed);       break;
    default:  setMotor(0, 0);                      break;
  }
}

void runAuto() {
  unsigned long now = millis();
  switch (state) {
    case DRIVE:
      if (distanceCm < STOP_DISTANCE) {
        state = BACK;
        stateUntil = now + 350;
        setMotor(-160, -160);
      } else {
        setMotor(160, 160);
      }
      break;
    case BACK:
      if (now > stateUntil) {
        state = TURN;
        stateUntil = now + random(300, 650);
        if (random(2)) setMotor(-170, 170);
        else           setMotor(170, -170);
      }
      break;
    case TURN:
      if (now > stateUntil) state = DRIVE;
      break;
  }
}

long readDistanceCM() {
  digitalWrite(TRIG, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG, LOW);
  long duration = pulseIn(ECHO, HIGH, 25000UL);
  if (duration == 0) return 999;
  return duration / 58;
}

void setMotor(int leftSpeed, int rightSpeed) {
  if (leftSpeed >= 0) { digitalWrite(IN1, HIGH); digitalWrite(IN2, LOW); }
  else                { digitalWrite(IN1, LOW);  digitalWrite(IN2, HIGH); leftSpeed = -leftSpeed; }
  if (rightSpeed >= 0) { digitalWrite(IN3, HIGH); digitalWrite(IN4, LOW); }
  else                 { digitalWrite(IN3, LOW);  digitalWrite(IN4, HIGH); rightSpeed = -rightSpeed; }
  analogWrite(ENA, constrain(leftSpeed, 0, 255));
  analogWrite(ENB, constrain(rightSpeed, 0, 255));
}
