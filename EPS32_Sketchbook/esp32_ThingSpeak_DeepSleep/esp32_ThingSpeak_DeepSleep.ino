//esp32_ThingSpeak_DeepSleep.ino
// Zwischen dem Senden soll der ESp32 in den Schlaf fallen.
//Tested OK

#include <WiFi.h>
#include <ThingSpeak.h>
#include "esp_sleep.h"

#include "ssid_pw.h"
/* #define SECRET_SSID "XXX"
#define SECRET_PASS "YYY"*/

const char* ssid = SECRET_SSID;    // your network SSID (name) 
const char* pass = SECRET_PASS ;  // your network password

WiFiClient  client;

unsigned long myChannelNumber =  2837696; //X;  Channel ID?
const char * myWriteAPIKey = "8XMYYU7L74S17I97";//"XXXXXXXXXXXXXXXX";

// Timer variables
//unsigned long lastTime = 0;
//unsigned long timerDelay = 30000;

// Variable to hold temperature readings
float volt;


void setup() {
  Serial.begin(115200);  //Initialize serial  
  WiFi.mode(WIFI_STA);     
  ThingSpeak.begin(client);  // Initialize ThingSpeak

  // Connect or reconnect to WiFi
  if(WiFi.status() != WL_CONNECTED){
      Serial.print("Attempting to connect");
      while(WiFi.status() != WL_CONNECTED){
        WiFi.begin(ssid, password); 
        delay(5000);     
      } 
      Serial.println("\nConnected.");
  }

    // Get a new Measurement
    volt = analogRead(A0)%100;
    Serial.print("Volt ");
    Serial.println(volt);
        
    // Write to ThingSpeak. There are up to 8 fields in a channel, allowing you to store up to 8 different
    // pieces of information in a channel.  Here, we write to field 1.
    int x = ThingSpeak.writeField(myChannelNumber, 1, volt, myWriteAPIKey);
   
  if(x == 200){
      Serial.println("Channel update successful.");
  }
  else{
      Serial.println("Problem updating channel. HTTP error code " + String(x));
  }

  esp_sleep_enable_timer_wakeup(30 * 1000000); //30sec
  esp_deep_sleep_start();
}

void loop() {}
