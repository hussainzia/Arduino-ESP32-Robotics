/*
  ============================================================
  PROJECT : ESP32 WiFi Car with Touch Joystick (no app needed)
  LEVEL   : Intermediate
  BOARD   : ESP32 Dev Module
  ============================================================
  ABOUT
    The car makes its own WiFi hotspot and serves a web page with
    a touch joystick. Drag your thumb to drive with smooth,
    proportional speed and steering, like a real RC car.

  PARTS
    - ESP32 dev board
    - L298N motor driver, 2/4 gear motors, chassis
    - 2x 18650 battery pack (7.4 V)

  WIRING
    L298N ENA -> GPIO14   (remove ENA jumper)
    L298N IN1 -> GPIO27
    L298N IN2 -> GPIO26
    L298N IN3 -> GPIO25
    L298N IN4 -> GPIO33
    L298N ENB -> GPIO32   (remove ENB jumper)
    L298N 5V  -> ESP32 VIN
    L298N GND -> ESP32 GND

  LIBRARIES
    - WiFi, WebServer (ESP32 board package 2.x or 3.x)

  HOW IT WORKS
    1. Connect phone WiFi to "ESP32-Car" (password 12345678), open 192.168.4.1.
    2. The joystick sends x and y (-100..100) about 10 times a second.
    3. Differential mixing: left = y + x, right = y - x.
    4. Safety: if no command arrives for 600 ms, the car stops.

  TIPS
    - Car turns the wrong way? Swap the motor wires or swap the + and - of x in drive().
    - Several cars in one room? Give each a different WiFi name.
  ============================================================
*/

#include <WiFi.h>
#include <WebServer.h>

// ---------- ESP32 car standard pins (L298N) ----------
#define ENA 14
#define IN1 27
#define IN2 26
#define IN3 25
#define IN4 33
#define ENB 32

WebServer server(80);
unsigned long lastCommand = 0;

const char PAGE[] PROGMEM = R"HTML(
<!DOCTYPE html><html><head><meta name="viewport" content="width=device-width,initial-scale=1,user-scalable=no">
<title>ESP32 Car</title><style>
body{margin:0;background:#0f172a;color:#e2e8f0;font-family:sans-serif;text-align:center;touch-action:none;user-select:none}
#pad{width:260px;height:260px;border-radius:50%;background:#1e293b;margin:40px auto;position:relative;border:3px solid #334155}
#knob{width:90px;height:90px;border-radius:50%;background:#3b82f6;position:absolute;left:85px;top:85px}
</style></head><body><h2>ESP32 Car</h2><div id="pad"><div id="knob"></div></div><p id="info">x:0 y:0</p>
<script>
var pad=document.getElementById('pad'),knob=document.getElementById('knob'),x=0,y=0,active=false,last=0;
function move(e){var r=pad.getBoundingClientRect(),cx=e.clientX-r.left-130,cy=e.clientY-r.top-130,
d=Math.min(Math.hypot(cx,cy),100),a=Math.atan2(cy,cx);cx=d*Math.cos(a);cy=d*Math.sin(a);
knob.style.left=(85+cx)+'px';knob.style.top=(85+cy)+'px';x=Math.round(cx);y=Math.round(-cy);}
pad.addEventListener('pointerdown',function(e){active=true;pad.setPointerCapture(e.pointerId);move(e);});
pad.addEventListener('pointermove',function(e){if(active)move(e);});
pad.addEventListener('pointerup',function(){active=false;x=0;y=0;knob.style.left='85px';knob.style.top='85px';send();});
function send(){document.getElementById('info').innerText='x:'+x+' y:'+y;fetch('/drive?x='+x+'&y='+y).catch(function(){});}
setInterval(function(){if(active)send();},100);
</script></body></html>
)HTML";

void setup() {
  Serial.begin(115200);
  pinMode(ENA, OUTPUT); pinMode(ENB, OUTPUT);
  pinMode(IN1, OUTPUT); pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT); pinMode(IN4, OUTPUT);
  setMotor(0, 0);

  WiFi.softAP("ESP32-Car", "12345678");
  Serial.print("Open http://");
  Serial.println(WiFi.softAPIP());

  server.on("/", []() { server.send_P(200, "text/html", PAGE); });
  server.on("/drive", []() {
    int x = constrain(server.arg("x").toInt(), -100, 100);
    int y = constrain(server.arg("y").toInt(), -100, 100);
    drive(x, y);
    lastCommand = millis();
    server.send(200, "text/plain", "OK");
  });
  server.begin();
}

void loop() {
  server.handleClient();
  if (millis() - lastCommand > 600) setMotor(0, 0);   // fail-safe
}

void drive(int x, int y) {
  int left  = constrain(y + x, -100, 100);
  int right = constrain(y - x, -100, 100);
  setMotor(left * 255 / 100, right * 255 / 100);
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
