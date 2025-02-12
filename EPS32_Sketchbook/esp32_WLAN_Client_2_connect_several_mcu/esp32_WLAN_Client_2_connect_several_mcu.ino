// esp32_WLAN_Client_2_connect_several_mcu.ino
//------------------------------------------------------------------
//vgl.: https://wolles-elektronikkiste.de/wlan-mit-esp8266-und-esp32
//------------------------------------------------------------------
// ESP32 als Client ins WLAN bringen; 
// Verbindung mit einem anderen ESP32 der als Server läuft => esp32_WLAN_Sensor2Browser.ino 
// Liest die dortige Webseite mit GET


#include <WiFi.h> // for ESP32
#include <WebServer.h> // for ESP32
#include "ssid_pw.h"
const char* ssid = SECRET_SSID;
const char* pass = SECRET_PASS ;
const char* host = "192.168.178.41";  //Die IP auf der der Server die Webseite hosted

#define PORT 80
WebServer server(PORT); // WebServer server(PORT); for ESP32

// Selber festlegen der IP-Adresse
IPAddress ip(192,168,178,42);  // Host hatte 41
IPAddress gateway(192,168,178,1);
IPAddress subnet(255,255,255,0);

void setup() {
  Serial.begin(115200);
  delay(10);
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
 
  WiFi.config(ip, gateway, subnet); 
  WiFi.begin(ssid, pass);
   
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
 
  Serial.println("");
  Serial.println("WiFi connected");  
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  Serial.println();
}
 
void loop() {
  Serial.print("connecting to ");
  Serial.println(host);
  WiFiClient client;
  const int httpPort = 80;
  if (!client.connect(host, httpPort)) {  //Der Client verbindet sich mit dem Server („host“):
    Serial.println("connection failed");
    return;
  }
  String url = "/";
  Serial.print("Requesting URL: ");
  Serial.println(url);
   
  client.print(String("GET ") + url + " HTTP/1.1\r\n" +
               "Host: " + host + "\r\n" + 
               "Connection: close\r\n\r\n");
  unsigned long lasttime = millis();

  //Eine Wartezeit ist wichtig, da der Host unter Umständen gerade einen anderen Client (hier: der PC) bedient
  while (!client.available() && ((millis() - lasttime) < 3000)){  
    delay(1);
  }  
  //Die Antwort des Hosts wird zeilenweise ausgelesen und auf dem seriellen Monitor ausgegeben:
  while (client.available()) {
    String line = client.readStringUntil('\r');
    Serial.println(line);
  }
  Serial.println();
  Serial.println("closing connection");
  delay(60000);
  // Jetzt könnten wir die Nachricht weiter auswerten und den analogen Messwert extrahieren. Das tun wir aber nicht, denn dieser Sketch sollte nur das Prinzip verdeutlichen.
}