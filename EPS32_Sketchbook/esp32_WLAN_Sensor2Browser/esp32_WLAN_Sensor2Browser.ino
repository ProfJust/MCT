// esp32_WLAN_Sensor2Browser.ino
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

void setup(){
  pinMode(LEDPIN, OUTPUT);
  digitalWrite(LEDPIN, LOW);
  srand(analogRead(A0)); // A0 liefert einen "zufälligen" Startwert
  Serial.begin(115200); 
  Serial.println("Minimal Program to switch one LED");
  Serial.print("Connecting to: ");
  Serial.println(ssid);
  
  // IP-Adresse festlegen
  WiFi.config(ip, gateway, subnet); 
  WiFi.begin(ssid, pass);
  
  //mit WLAN Verbinden
  while(WiFi.status() != WL_CONNECTED){
    delay(500); 
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.print("IP-Address of ESP32 module: ");
  Serial.println(WiFi.localIP());
  
  // Die "Callback"-Funktionen des Servers bekannt machen
  server.on("/",startSeite);  //ruft Funktion auf mittels Zeiger auf Funktion
  server.on("/led_on", ledonSeite);
  server.on("/led_off", ledoffSeite);
  server.begin();
}
  
void loop(){
  server.handleClient();  // Server bedient die Clients
}

// Die Callback -Funktionen des Servers für Webseiten
// --------------------------------------------------
// Für ein regelmäßiges Update sorgt <head><meta http-equiv=\"refresh\" content=\"5\"></head>. 
// Dadurch wird die Webseite alle 5 Sekunden neu aufgerufen.
String headAndTitle = "<head><meta http-equiv=\"refresh\" content=\"5\"></head>"
                      "<h1>Control your ESP32 by Browser </h1>"
                      "SwitchLED and get a measured value, minimal version</BR></BR>";

void startSeite() {
  /*String message="<h1>Control your ESP32 by your Browser</h1>";  
  */
  String message="";
  message += headAndTitle;
  message += "Minimal version, just one LED - Startseite </BR></BR>";
  message += "<a href=\"/led_on\">LED On</a>";;
  server.send(200, "text/html", message);
}
void ledonSeite(){
  //digitalWrite(LEDPIN, HIGH);
  Serial.println("LED ON");

  float measuredValue = rand() % 100;// analogRead(A0)/1024.0 * 3.3; // adjust to your board
  Serial.println(measuredValue); 

  String message="";
  message += headAndTitle;
  //message +="<a href=\"/led_off\">LED Off</a>";
  // Bunter mit CSS
  message += "<a href=\"/led_off\"><button style=\"background: yellow; color: black; font-size: x-large; \">LED is On</button></a>";
  message += "</BR></BR>Voltage [V]: ";
  message += String(measuredValue, 2); // float to String, two decimal places
  server.send(200, "text/html", message);     
}

void ledoffSeite(){
  //digitalWrite(LEDPIN, LOW);
  Serial.println("LED OFF");
   float measuredValue = rand() % 100;// analogRead(A0)/1024.0 * 3.3; // adjust to your board
  Serial.println(measuredValue); 

  String message="";
  message += headAndTitle;
  //message += "<a href=\"/led_on\">LED On</a>";
  // Bunter mit CSS
  message +="<a href=\"/led_on\"><button style=\"background: grey; color: white; font-size: x-large; \">LED is Off</button></a>";
  message += "</BR></BR>Voltage [V]: ";
  message += String(measuredValue, 2); // float to String, two decimal places
  server.send(200, "text/html", message);  
}
