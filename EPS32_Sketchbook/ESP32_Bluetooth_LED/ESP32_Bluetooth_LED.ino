#include "BluetoothSerial.h"

BluetoothSerial SerialBT;
const int LEDpin = 2;  // LED_BUILTIN ist am GPIO 2 oder 8 beim ESP32
// bei LEDpin = 8 kann mein Smartphone keine Bluetooth-Verbindung aufbauen. Warum???
// Doppelt-Belegung des GPIO 8 ?
// Das Problem, dass keine Bluetooth-Verbindung mehr möglich ist, wenn GPIO8 als LED_BUILTIN definiert wird, liegt daran, dass GPIO8 auf einigen ESP32-Modellen (z. B. ESP32-C3)
// spezielle Funktionen hat. Dieser Pin wird oft für interne Hardwarezwecke wie Flash-Speicherzugriff oder andere Peripheriegeräte verwendet, was Konflikte mit Bluetooth verursachen kann. 

void setup() {
  SerialBT.begin("ESP32-Bluetooth #1");
  pinMode(LEDpin, OUTPUT);
  Serial.begin(9600);
  Serial.println("ESP32 - Bluetooth");
}

void loop() {
  if (SerialBT.available()){
    char c = SerialBT.read();
    if (c=='1'){
      digitalWrite(LEDpin, HIGH);
      Serial.println("Received Bluetooth - ON");
    } else if  (c=='0'){
      digitalWrite(LEDpin, LOW);
      Serial.println("Received Bluetooth - OFF");
    }
  }
}
