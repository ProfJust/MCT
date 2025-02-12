//host_1_setup_without_router.ino
//----------------------------------------------
// COM4 schwarzes Kabel -ESP32-Client Host1
//----------------------------------------------
// Baut ein Netzwerk aus zwei ESP32 und dem PC
// Verbindet ESP32-Client per WLAN mit einem ESP32-Server
// Tested 11.02.25 by OJ as OK now
// Jetzt Verbindung zum Server, 
// WiFi.begin(ssid, pass); funktioniert wenn Passwort =""

#include <WiFi.h> // for ESP32
#include <WebServer.h> // for ESP32

const char* ssid = "ESP_FF45A5";
const char* pass = "";
//  Windows Terminal
// netsh wlan show profile ESP_FF45A5 key=clear
// Sicherheitsschlüssel: Nicht Vorhanden => ""
IPAddress ip(192,168,4,3); // this ESP32 // ip(192,168,4,2); for second host
IPAddress gateway(192,168,4,1); // must be the same everywhere
IPAddress subnet(255,255,255,0); // must be the same everywhere

WebServer server(80); // WebServer server(80); for ESP32
 
void setup(){
  Serial.begin(115200);
  delay(10);
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.config(ip, gateway, subnet);
  // Besser ohne ?? NEIN! sonst falsche IP
  
  //WiFi.mode(WIFI_STA); // ??
  WiFi.begin(ssid, pass);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    //Serial.print(".");
   wl_status_t status = WiFi.status();
   switch (status) {
    case WL_CONNECTED:
      Serial.println("WLAN verbunden");
      break;
    case WL_DISCONNECTED:
      Serial.println("WLAN nicht verbunden");
      break;
    case WL_NO_SSID_AVAIL:
      Serial.println("SSID nicht verfügbar");
      break;
    case WL_CONNECT_FAILED:
      Serial.println("Verbindung fehlgeschlagen (Passwort?)");
      break;
    default:
      Serial.println("Unbekannter Status");
      break;
   }
    //Serial.println();
  }
   
  //Serial.println("");
  //Serial.println("WiFi connected");  
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  Serial.println();

  server.on("/voltage",voltage); 
  server.begin();
}

void loop(){
  server.handleClient();
}

void voltage(){
  float measuredValue_1 = 22.20; //rand() % 3000 / 3.0; //analogRead(A0)/1024.0 * 3.3;
  String message = String(measuredValue_1, 2);
  server.send(200, "text/html", message);
}
