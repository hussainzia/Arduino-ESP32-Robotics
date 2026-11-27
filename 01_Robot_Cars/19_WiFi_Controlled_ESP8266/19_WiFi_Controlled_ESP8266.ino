/*
  ============================================================
  19 - WiFi CONTROLLED CAR (NodeMCU ESP8266, no app needed)
  ============================================================
  Board  : NodeMCU 1.0 (ESP-12E)   <-- NOT Arduino UNO
  Setup  : Arduino IDE -> File -> Preferences -> Additional boards URL:
           http://arduino.esp8266.com/stable/package_esp8266com_index.json
           Then Tools -> Board -> Boards Manager -> install "esp8266".

  How to use:
    1. Upload, then on your phone connect to WiFi "RobotCar"
       (password 12345678).
    2. Open a browser and go to  192.168.4.1
    3. Hold a button to drive, release to stop.

  Wiring (NodeMCU -> L298N):
    D1 -> ENA      D5 -> IN1     D6 -> IN2
    D2 -> ENB      D7 -> IN3     D0 -> IN4
    GND -> GND (common ground)
    Power NodeMCU from L298N 5V output -> NodeMCU VIN
  ============================================================
*/

#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

// ---------- Motor driver (L298N) pins ----------
#define ENA D1
#define ENB D2
#define IN1 D5
#define IN2 D6
#define IN3 D7
#define IN4 D0

const char* WIFI_NAME = "RobotCar";
const char* WIFI_PASS = "12345678";   // at least 8 characters

ESP8266WebServer server(80);
int carSpeed = 200;

// ---------- Web page shown on the phone ----------
const char PAGE[] PROGMEM = R"HTML(
<!DOCTYPE html><html><head>
<meta name="viewport" content="width=device-width, initial-scale=1, user-scalable=no">
<title>Robot Car</title>
<style>
 body{font-family:sans-serif;text-align:center;background:#1e293b;color:#fff;margin:0;padding:20px;
      user-select:none;-webkit-user-select:none;touch-action:none}
 .grid{display:grid;grid-template-columns:repeat(3,90px);gap:12px;justify-content:center;margin-top:30px}
 button{height:90px;font-size:32px;border:none;border-radius:16px;background:#3b82f6;color:#fff}
 button:active{background:#1d4ed8}
 .stop{background:#ef4444}
 input{width:80%;margin-top:30px}
</style></head><body>
<h2>Robot Car</h2>
<div class="grid">
 <div></div><button data-c="F">&#9650;</button><div></div>
 <button data-c="L">&#9664;</button><button class="stop" data-c="S">&#9632;</button><button data-c="R">&#9654;</button>
 <div></div><button data-c="B">&#9660;</button><div></div>
</div>
<p>Speed</p>
<input type="range" min="100" max="255" value="200" onchange="fetch('/speed?v='+this.value)">
<script>
 function send(c){fetch('/cmd?go='+c);}
 document.querySelectorAll('button').forEach(function(b){
   b.addEventListener('pointerdown',function(){send(b.dataset.c);});
   b.addEventListener('pointerup',function(){send('S');});
   b.addEventListener('pointerleave',function(e){if(e.buttons)send('S');});
 });
</script>
</body></html>
)HTML";

void setup() {
  Serial.begin(115200);
  pinMode(ENA, OUTPUT); pinMode(ENB, OUTPUT);
  pinMode(IN1, OUTPUT); pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT); pinMode(IN4, OUTPUT);
  analogWriteRange(255);   // PWM 0-255 like Arduino UNO
  stopRobot();

  WiFi.softAP(WIFI_NAME, WIFI_PASS);
  Serial.print("Connect to WiFi '");
  Serial.print(WIFI_NAME);
  Serial.print("' and open http://");
  Serial.println(WiFi.softAPIP());

  server.on("/", []() {
    server.send_P(200, "text/html", PAGE);
  });

  server.on("/cmd", []() {
    String c = server.arg("go");
    if      (c == "F") forward(carSpeed);
    else if (c == "B") backward(carSpeed);
    else if (c == "L") turnLeft(carSpeed);
    else if (c == "R") turnRight(carSpeed);
    else               stopRobot();
    server.send(200, "text/plain", "OK");
  });

  server.on("/speed", []() {
    carSpeed = constrain(server.arg("v").toInt(), 0, 255);
    server.send(200, "text/plain", "OK");
  });

  server.begin();
}

void loop() {
  server.handleClient();
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
