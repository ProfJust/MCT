//main_mcu_setup_without_router.ino
//----------------------------------------------
// COM5 rotes Kabel - ESP32-Server Main MCU
// Erzeugt die Webseite und holt die Daten von Host1 ab
//----------------------------------------------
// Baut ein Netzwerk aus zwei ESP32 und dem PC
// Verbindet ESP32-Client per WLAN mit einem ESP32-Server
// Tested 11.02.25 by OJ as OK

#include <WiFi.h> // for ESP32
#include <WebServer.h> // for ESP32

const char* ssid = "ESP_FF45A5";
const char* pass = "youbot"; 

#define HOST_1 0
#define HOST_2 1
const char* host_2 = "192.168.4.2";
const char* host_1 = "192.168.4.3";
String voltage[2] = {"",""};
unsigned long requestPeriod = 10000;
unsigned long startTime = 0;
IPAddress ip(192,168,4,1); //this ESP32
IPAddress gateway(192,168,4,1);
IPAddress subnet(255,255,255,0);
WebServer server(80); // WebServer server(80); for ESP32
 
String headAndTitle = "<head><style>"
                        ".blueBox {"
                          "background-color: blue;"
                          "color: white;"
                          "width: 350px;" 
                          "padding: 20px;"
                          "text-align: center;"
                          "font-size: 50px;"
                          "font-family: arial;"
                        "}"
                      "</style>"
                      "</head><meta http-equiv=\"refresh\" content=\"10\"></head>"
                      "</BR></BR><h1 align=\"center\">Get data from other ESP32 modules</h1></div>"
                      "<div align=\"center\">AP Mode, get measured values from two hosts</BR></BR></div>";
void setup(){
  WiFi.softAPConfig(ip, gateway, subnet);
  WiFi.softAP(ssid, pass);  
  
  server.on("/",handleRoot);
  server.begin();
  Serial.begin(115200);
}

void loop(){ 
  static int cntr=0;
  server.handleClient();
  if ((millis() - startTime) > requestPeriod){
    Serial.print("Request Host1 #");
    Serial.println(++cntr); 
    requestESP32(HOST_1);
    //requestESP8266(HOST_2);
    startTime = millis();  // Reset Timerperiod
  }
}
void handleRoot(){
  String message = "";
  message += headAndTitle;
  message += "<div align=\"center\";>";
  float measuredValue_1 = millis() %100; //11.1; //analogRead(A0)/1024.0 * 3.3;
  
  message += "</BR><div class=\"blueBox\">";
  message += "Voltage_1 [V]: </BR>";
  message += String(measuredValue_1, 2);
  message += "</div>";
  message += "</BR><div class=\"blueBox\">";
  message += "Voltage_2 [V]: </BR>";
  message += voltage[HOST_1];
  message += "</div>";
  
 // message += "</BR><div class=\"blueBox\">";
 // message += "Voltage_3 [V]: </BR>";
 // message += voltage[HOST_2];
 // message += "</div></div>";
  server.send(200, "text/html", message); 
}
void requestESP32(int hostNo){
  WiFiClient client;
  String host = "";
  if(hostNo == HOST_1){
    if (!client.connect(host_1, 80)) {
      host = host_1;
      return;
    }
  }
 // if(hostNo == HOST_2){
 //   if (!client.connect(host_2, 80)) {
 //     host = host_2;
 //     return;
 //   }
 // }
  String url = "/voltage";
    
  String getstr =  String("GET ") + url + " HTTP/1.1\r\n" +
               "Host: " + host + "\r\n" + 
               "Connection: close\r\n\r\n";
  client.print(getstr);
  Serial.println(getstr);
 // client.print(String("GET ") + url + " HTTP/1.1\r\n" +
 //              "Host: " + host + "\r\n" + 
 //              "Connection: close\r\n\r\n");
  unsigned long lasttime = millis();
  while (!client.available() && ((millis() - lasttime) < 3000)){
    delay(1);
  }  
  while (client.available()) {
    String line = client.readStringUntil('\r');
    if(line != ""){
      voltage[hostNo] = line;
    }
  }
}
