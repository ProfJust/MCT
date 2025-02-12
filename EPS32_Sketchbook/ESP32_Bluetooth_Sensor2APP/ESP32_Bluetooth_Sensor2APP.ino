#include "BluetoothSerial.h"
//------------------------------------------------------------------------------------
// OJ am 11.02.2025
// ESP32 sendet Daten (ggf. von einem Sensor, hier Zufallswerte) an eine APP via Bluetooh
//------------------------------------------------------------------------------------

BluetoothSerial SerialBT;

void setup() {
  SerialBT.begin("ESP32-Bluetooth #1");
  SerialBT.println("Hallo von ESP32!"); // Daten senden
  // Serial Monitor in der Arduino IDE, ggf. RST Button zum Start
  Serial.begin(9600);
  Serial.println("ESP32 - Bluetooth #1");
  // Zufallszahlengenerator initialisieren mit einem Seed-Wert
  srand(analogRead(A0)); // A0 liefert einen "zufälligen" Startwert
}

void loop() {
  int sensorValue =   rand() % 100; // Zahl zwischen 0 und 99
  char str[6] = "$077\n"; //$ Zeichen zum Trennen der Daten
  sprintf(str,"$%04d\n",sensorValue); //$ Zeichen zum Trennen der Daten
  Serial.print(str);
  SerialBT.print(str);
   
  delay(1000);
}
