/*
  ============================================================
  PROJECT : WiFi Robotic Arm with Web Sliders (ESP32)
  LEVEL   : Intermediate
  BOARD   : ESP32 Dev Module
  ============================================================
  ABOUT
    The ESP32 creates its own WiFi network. Open the page on any
    phone and move 4 sliders to control the arm. No app, no router.
    Buttons for HOME and a saved PICK & PLACE demo.

  PARTS
    - ESP32 dev board
    - 4x servo (SG90 / MG90S) + robotic arm kit
    - External 5-6 V 2 A supply for servos

  WIRING
    Base servo signal     -> GPIO13
    Shoulder servo signal -> GPIO14
    Elbow servo signal    -> GPIO27
    Gripper servo signal  -> GPIO26
    Servo red / brown     -> external 5-6 V + / -
    External supply -     -> ESP32 GND   (common ground!)

  LIBRARIES
    - ESP32Servo by Kevin Harrington (Library Manager)
    - WiFi, WebServer (ESP32 board package)

  HOW IT WORKS
    1. Connect your phone to WiFi "RoboArm" (password 12345678).
    2. Open 192.168.4.1.
    3. Each slider sends /set?j=<joint>&a=<angle>; the ESP32 moves the
       servo smoothly toward that angle.

  TIPS
    - Do not use GPIO 6-11 (flash) or GPIO 12 (boot pin) for servos.
    - Servos twitch at power-on: that is normal for cheap servos.
  ============================================================
*/

#include <WiFi.h>
#include <WebServer.h>
#include <ESP32Servo.h>

const int servoPins[4] = {13, 14, 27, 26};
const int minAngle[4]  = {0,   20,  20,  10};
const int maxAngle[4]  = {180, 160, 160, 90};
const int homeAngle[4] = {90,  90,  90,  45};

Servo joints[4];
float current[4];
int target[4];
WebServer server(80);

const char PAGE[] PROGMEM = R"HTML(
<!DOCTYPE html><html><head><meta name="viewport" content="width=device-width,initial-scale=1">
<title>Robo Arm</title><style>
body{font-family:sans-serif;background:#0f172a;color:#e2e8f0;text-align:center;padding:16px}
.card{background:#1e293b;border-radius:14px;padding:14px;margin:12px auto;max-width:420px}
input[type=range]{width:100%;height:36px}
button{font-size:18px;padding:12px 22px;margin:6px;border:0;border-radius:10px;background:#3b82f6;color:#fff}
</style></head><body><h2>Robotic Arm</h2>
<div id="s"></div>
<button onclick="fetch('/home')">HOME</button><button onclick="fetch('/demo')">PICK &amp; PLACE</button>
<script>
var names=["Base","Shoulder","Elbow","Gripper"],mins=[0,20,20,10],maxs=[180,160,160,90],vals=[90,90,90,45];
var h="";for(var i=0;i<4;i++){h+='<div class="card">'+names[i]+': <b id="v'+i+'">'+vals[i]+'</b>&deg;'+
'<input type="range" min="'+mins[i]+'" max="'+maxs[i]+'" value="'+vals[i]+'" oninput="mv('+i+',this.value)"></div>';}
document.getElementById("s").innerHTML=h;
var t=0;function mv(j,a){document.getElementById("v"+j).innerText=a;clearTimeout(t);
t=setTimeout(function(){fetch("/set?j="+j+"&a="+a);},40);}
</script></body></html>
)HTML";

void setup() {
  Serial.begin(115200);
  for (int i = 0; i < 4; i++) {
    joints[i].setPeriodHertz(50);
    joints[i].attach(servoPins[i], 500, 2400);
    current[i] = target[i] = homeAngle[i];
    joints[i].write(homeAngle[i]);
  }

  WiFi.softAP("RoboArm", "12345678");
  Serial.print("Open http://");
  Serial.println(WiFi.softAPIP());

  server.on("/", []() { server.send_P(200, "text/html", PAGE); });
  server.on("/set", []() {
    int j = server.arg("j").toInt();
    int a = server.arg("a").toInt();
    if (j >= 0 && j < 4) target[j] = constrain(a, minAngle[j], maxAngle[j]);
    server.send(200, "text/plain", "OK");
  });
  server.on("/home", []() {
    for (int i = 0; i < 4; i++) target[i] = homeAngle[i];
    server.send(200, "text/plain", "OK");
  });
  server.on("/demo", []() {
    server.send(200, "text/plain", "OK");
    pickAndPlace();
  });
  server.begin();
}

void loop() {
  server.handleClient();
  smoothStep();
  delay(15);
}

void smoothStep() {
  for (int i = 0; i < 4; i++) {
    current[i] += (target[i] - current[i]) * 0.12;
    joints[i].write((int)current[i]);
  }
}

void goTo(int b, int s, int e, int g) {
  target[0] = b; target[1] = s; target[2] = e; target[3] = g;
  for (int k = 0; k < 70; k++) { smoothStep(); delay(15); }
}

void pickAndPlace() {
  goTo(40, 90, 90, 80);    // above object, gripper open
  goTo(40, 130, 60, 80);   // lower
  goTo(40, 130, 60, 15);   // close gripper
  goTo(40, 90, 90, 15);    // lift
  goTo(140, 90, 90, 15);   // rotate
  goTo(140, 130, 60, 15);  // lower
  goTo(140, 130, 60, 80);  // release
  goTo(homeAngle[0], homeAngle[1], homeAngle[2], homeAngle[3]);
}
