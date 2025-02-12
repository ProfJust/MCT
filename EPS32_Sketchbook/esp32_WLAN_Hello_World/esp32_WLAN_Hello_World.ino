//https://wolles-elektronikkiste.de/wlan-mit-esp8266-und-esp32
// ESP32 Dev Module  mit  WLAN verbinden
// freie IP Adresse möglich 
// Tested 11.02.25 by OJ as OK
#include "ssid_pw.h"   // enthält #define SECRET_SSID "XXX" und #define SECRET_PASS "YYY"
#include <WiFi.h> // for ESP32
#include <WebServer.h> // for ESP32
const char* ssid = SECRET_SSID;
const char* pass = SECRET_PASS;
#define PORT 80
WebServer server(PORT); // WebServer server(PORT); for ESP32




 
void setup(){
  Serial.begin(115200); 
  Serial.print("Connecting to: ");
  Serial.println(ssid);
 
  //---- Router soll IP-Adresse bestimmen => Auskoomentieren --
  // Selber festlegen der IP-Adresse
  //IPAddress ip(192, 168, 178, 41);
  //IPAddress gateway(192, 168, 178, 1);
  //IPAddress subnet(255, 255, 255, 0);
  //WiFi.config(ip, gateway, subnet); 
  //---- Router soll IP-Adresse bestimmen => Auskoomentieren --

  WiFi.begin(ssid, pass);
   
  while(WiFi.status() != WL_CONNECTED){
    delay(500); 
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.print("IP-Address of ESP8266 module: ");
  Serial.println(WiFi.localIP());
  
  server.begin();
}
  
void loop(){
}