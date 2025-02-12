// esp32_WLAN_one_LED.ino
//------------------------------------------------------------------
//vgl.: https://wolles-elektronikkiste.de/wlan-mit-esp8266-und-esp32
//------------------------------------------------------------------
// ESP32 als Server ins WLAN bringen; LED steuern
// Tested 11.02.25 by OJ as OK

#include <WiFi.h> // for ESP32
#include <WebServer.h> // for ESP32
#include "ssid_pw.h"
const char* ssid = SECRET_SSID;
const char* pass = SECRET_PASS;
#define PORT 80
WebServer server(PORT); // WebServer server(PORT); for ESP32

// Selber festlegen der IP-Adresse
IPAddress ip(192,168,178,41);
IPAddress gateway(192,168,178,1);
IPAddress subnet(255,255,255,0);

//LED bei mir nicht verfügbar #############
#define LEDPIN 2  // choose an appropriate pin
String led1= "<a href=\"/led_on\">LED On</a>";
String led0= "<a href=\"/led_off\">LED Off</a>";

void setup(){
  pinMode(LEDPIN, OUTPUT);
  digitalWrite(LEDPIN, LOW);
  Serial.begin(115200); 
  Serial.println("Minimal Program to switch one LED");
  Serial.print("Connecting to: ");
  Serial.println(ssid);
  WiFi.config(ip, gateway, subnet); 
  WiFi.begin(ssid, pass);
  
  while(WiFi.status() != WL_CONNECTED){
    delay(500); 
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.print("IP-Address of ESP32 module: ");
  Serial.println(WiFi.localIP());
  
  server.on("/",handleRoot);
  server.on("/led_on", ledon);
  server.on("/led_off", ledoff);
  server.begin();
}
  
void loop(){
  server.handleClient();
}

void handleRoot() {
  String message="<h1>Control your ESP8266 by your Browser</h1>";
  message += "Minimal version, just one LED</BR></BR>";
  message += led1;
  server.send(200, "text/html", message);
}
void ledon(){
  //digitalWrite(LEDPIN, HIGH);
  Serial.println("LED ON");
  server.send(200, "text/html", led0);
}
void ledoff(){
  //digitalWrite(LEDPIN, LOW);
  Serial.println("LED OFF");
  server.send(200, "text/html", led1);
}