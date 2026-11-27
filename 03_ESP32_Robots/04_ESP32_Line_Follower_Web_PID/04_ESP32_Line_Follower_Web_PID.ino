/*
  ============================================================
  PROJECT : ESP32 Line Follower with Live PID Tuning from Phone
  LEVEL   : Advanced
  BOARD   : ESP32 Dev Module
  ============================================================
  ABOUT
    A competition-style PID line follower whose Kp, Kd and speed
    you tune LIVE from a phone, without re-uploading code each time.
    The page also shows the line position in real time.
    Saves hours of tuning before a competition.

  PARTS
    - ESP32 dev board
    - 5-channel IR sensor array (TCRT5000), digital outputs
    - L298N (or TB6612FNG), N20 / TT motors, light chassis

  WIRING
    IR S1 (left)   -> GPIO34
    IR S2          -> GPIO35
    IR S3 (center) -> GPIO36 (VP)
    IR S4          -> GPIO39 (VN)
    IR S5 (right)  -> GPIO4
    IR VCC / GND   -> 3.3V / GND
    L298N ENA -> GPIO14
    L298N IN1 -> GPIO27
    L298N IN2 -> GPIO26
    L298N IN3 -> GPIO25
    L298N IN4 -> GPIO33
    L298N ENB -> GPIO32

  LIBRARIES
    - WiFi, WebServer (ESP32 board package)

  HOW IT WORKS
    1. Connect to WiFi "LineBot" (12345678) and open 192.168.4.1.
    2. Press START. Move the sliders while the robot runs.
    3. PID runs every 5 ms; the web server is handled in between.

  TIPS
    - Power the IR array from 3.3V so its outputs are safe for the ESP32.
    - Write down the best values and put them as the defaults below.
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

const int sensorPins[5]   = {34, 35, 36, 39, 4};
const int sensorWeight[5] = {-4, -2, 0, 2, 4};
#define ON_LINE HIGH

float Kp = 25, Kd = 15;
int baseSpeed = 150;
bool running = false;
float lastError = 0, error = 0;
unsigned long lastControl = 0;

WebServer server(80);

const char PAGE[] PROGMEM = R"HTML(
<!DOCTYPE html><html><head><meta name="viewport" content="width=device-width,initial-scale=1">
<title>LineBot</title><style>
body{font-family:sans-serif;background:#0f172a;color:#e2e8f0;text-align:center;padding:14px}
.c{background:#1e293b;border-radius:12px;padding:12px;margin:10px auto;max-width:420px}
input{width:100%}button{font-size:20px;padding:12px 30px;margin:6px;border:0;border-radius:10px;color:#fff}
#bar{height:24px;background:#334155;border-radius:12px;position:relative}
#dot{width:24px;height:24px;border-radius:12px;background:#22c55e;position:absolute;left:calc(50% - 12px)}
</style></head><body><h2>LineBot PID Tuner</h2>
<button style="background:#16a34a" onclick="fetch('/run?on=1')">START</button>
<button style="background:#dc2626" onclick="fetch('/run?on=0')">STOP</button>
<div class="c">Line position<div id="bar"><div id="dot"></div></div></div>
<div class="c">Kp: <b id="kp">25</b><input type="range" min="0" max="80" step="0.5" value="25" oninput="s('kp',this.value)"></div>
<div class="c">Kd: <b id="kd">15</b><input type="range" min="0" max="80" step="0.5" value="15" oninput="s('kd',this.value)"></div>
<div class="c">Speed: <b id="sp">150</b><input type="range" min="60" max="255" value="150" oninput="s('sp',this.value)"></div>
<script>
var t={};function s(k,v){document.getElementById(k).innerText=v;clearTimeout(t[k]);
t[k]=setTimeout(function(){fetch('/set?'+k+'='+v);},80);}
setInterval(function(){fetch('/status').then(function(r){return r.json();}).then(function(d){
document.getElementById('dot').style.left='calc('+(50+d.e*11)+'% - 12px)';}).catch(function(){});},300);
</script></body></html>
)HTML";

void setup() {
  Serial.begin(115200);
  pinMode(ENA, OUTPUT); pinMode(ENB, OUTPUT);
  pinMode(IN1, OUTPUT); pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT); pinMode(IN4, OUTPUT);
  for (int i = 0; i < 5; i++) pinMode(sensorPins[i], INPUT);
  setMotor(0, 0);

  WiFi.softAP("LineBot", "12345678");
  Serial.println(WiFi.softAPIP());

  server.on("/", []() { server.send_P(200, "text/html", PAGE); });
  server.on("/set", []() {
    if (server.hasArg("kp")) Kp = server.arg("kp").toFloat();
    if (server.hasArg("kd")) Kd = server.arg("kd").toFloat();
    if (server.hasArg("sp")) baseSpeed = server.arg("sp").toInt();
    server.send(200, "text/plain", "OK");
  });
  server.on("/run", []() {
    running = server.arg("on") == "1";
    if (!running) setMotor(0, 0);
    server.send(200, "text/plain", "OK");
  });
  server.on("/status", []() {
    String json = "{\"e\":" + String(error, 2) + ",\"run\":" + String(running ? 1 : 0) + "}";
    server.send(200, "application/json", json);
  });
  server.begin();
}

void loop() {
  server.handleClient();
  if (micros() - lastControl < 5000) return;   // 200 Hz control loop
  lastControl = micros();
  if (running) followLine();
}

void followLine() {
  int sum = 0, count = 0;
  for (int i = 0; i < 5; i++) {
    if (digitalRead(sensorPins[i]) == ON_LINE) { sum += sensorWeight[i]; count++; }
  }
  if (count == 0) {                      // line lost: spin toward last side
    error = lastError < 0 ? -4 : 4;
    setMotor(lastError < 0 ? -130 : 130, lastError < 0 ? 130 : -130);
    return;
  }
  error = (float)sum / count;
  float correction = Kp * error + Kd * (error - lastError);
  lastError = error;
  setMotor(constrain(baseSpeed + (int)correction, -255, 255),
           constrain(baseSpeed - (int)correction, -255, 255));
}

void setMotor(int leftSpeed, int rightSpeed) {
  if (leftSpeed >= 0) { digitalWrite(IN1, HIGH); digitalWrite(IN2, LOW); }
  else                { digitalWrite(IN1, LOW);  digitalWrite(IN2, HIGH); leftSpeed = -leftSpeed; }
  if (rightSpeed >= 0) { digitalWrite(IN3, HIGH); digitalWrite(IN4, LOW); }
  else                 { digitalWrite(IN3, LOW);  digitalWrite(IN4, HIGH); rightSpeed = -rightSpeed; }
  analogWrite(ENA, constrain(leftSpeed, 0, 255));
  analogWrite(ENB, constrain(rightSpeed, 0, 255));
}
