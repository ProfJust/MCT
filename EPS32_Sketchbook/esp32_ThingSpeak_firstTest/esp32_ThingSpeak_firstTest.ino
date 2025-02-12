/*
  Adapted from WriteSingleField Example from ThingSpeak Library (Mathworks)  
  Rui Santos & Sara Santos - Random Nerd Tutorials
  Complete project details at https://RandomNerdTutorials.com/esp32-thingspeak-publish-arduino/
  Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files.
  The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
*/

#include <WiFi.h>
#include <ThingSpeak.h>
#include "ssid_pw.h"
const char* ssid = SECRET_SSID; // your network SSID (name) 
const char* pass = SECRET_PASS ; // your network password

WiFiClient  client;

unsigned long myChannelNumber =  2837696; //X;  Channel ID?
const char * myWriteAPIKey = "8XMYYU7L74S17I97";//"XXXXXXXXXXXXXXXX";

// Timer variables
unsigned long lastTime = 0;
unsigned long timerDelay = 30000;

// Variable to hold temperature readings
float volt;


void setup() {
  Serial.begin(115200);  //Initialize serial  
  WiFi.mode(WIFI_STA);     
  ThingSpeak.begin(client);  // Initialize ThingSpeak
}

void loop() {
  if ((millis() - lastTime) > timerDelay) {
    
    // Connect or reconnect to WiFi
    if(WiFi.status() != WL_CONNECTED){
      Serial.print("Attempting to connect");
      while(WiFi.status() != WL_CONNECTED){
        WiFi.begin(ssid, password); 
        delay(5000);     
      } 
      Serial.println("\nConnected.");
    }

    // Get a new temperature reading
    volt = millis()%100;
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
    lastTime = millis();
  }
}
