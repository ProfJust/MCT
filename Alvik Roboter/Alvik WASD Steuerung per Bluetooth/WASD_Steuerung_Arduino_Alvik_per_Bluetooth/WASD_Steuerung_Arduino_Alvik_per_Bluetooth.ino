#include <Arduino.h>
#include <ArduinoBLE.h>
#include <Arduino_Alvik.h>
Arduino_Alvik alvik;

// Nordic UART Service UUIDs (NUS)
const char* UART_SERVICE_UUID = "6E400001-B5A3-F393-E0A9-E50E24DCCA9E";
const char* UART_RX_UUID      = "6E400002-B5A3-F393-E0A9-E50E24DCCA9E"; // Write: PC -> Alvik
const char* UART_TX_UUID      = "6E400003-B5A3-F393-E0A9-E50E24DCCA9E"; // Notify: Alvik -> PC (optional)

BLEService uartService(UART_SERVICE_UUID);
BLECharacteristic rxChar(UART_RX_UUID, BLEWrite | BLEWriteWithoutResponse, 20);
BLECharacteristic txChar(UART_TX_UUID, BLENotify, 20); // optional

static int speedVal = 10;
static int turnDelta = 10;

static uint32_t lastCmdMs = 0;
static const uint32_t FAILSAFE_MS = 300;

//  Alvik-Motorsteuerung 
void setMotors(int left, int right) {
  left  = constrain(left,  -20, 20);
  right = constrain(right, -20, 20);

  // Differentialantrieb:
  int linear  = (left + right) / 2;
  int angular = (right - left) / 2;

  alvik.drive(linear, angular);

  // Debug-Ausgabe
  if (linear !=0  || angular !=0){
    Serial.print("drive(): linear=");
    Serial.print(linear);
    Serial.print(" angular=");
    Serial.println(angular);
  }
}

void handleKey(char c) {
  c = toupper((unsigned char)c);
  lastCmdMs = millis();

  // --- Empfangene Taste im Serial Monitor ausgeben ---
    Serial.print("RX: '");
    Serial.print(c);
    Serial.print("'  (");
    Serial.print((int)c);
    Serial.print(")  t=");
    Serial.print(millis());
    Serial.println(" ms");

  switch (c) {
    case 'W': setMotors(+speedVal, +speedVal); break;
    case 'S': setMotors(-speedVal, -speedVal); break;
    case 'A': setMotors(-turnDelta, +turnDelta); break;
    case 'D': setMotors(+turnDelta, -turnDelta); break;
    case 'Q': setMotors(+speedVal, +speedVal+5); break;
    case 'E': setMotors(+speedVal+5, +speedVal); break;
    case 'X': setMotors(0, 0); break;
    case '+': speedVal = min(20, speedVal + 2); break;
    case '-': speedVal = max(2,   speedVal - 2); break;
    default: break;
  }
}

void setup() {
  Serial.begin(115200);
  unsigned long t0 = millis();
  while (!Serial && (millis() - t0 < 2000)) { delay(10); } // max 2s warten
  Serial.println("BOOT: Sketch gestartet");

  alvik.begin();
  Serial.println("Alvik initialisiert");
  setMotors(0, 0);
  lastCmdMs = millis();

  if (!BLE.begin()) {
    Serial.println("BLE.begin() fehlgeschlagen.");
    while (1) delay(100);
  }

  BLE.setLocalName("Alvik-WASD");
  BLE.setDeviceName("Alvik-WASD");
  BLE.setAdvertisedService(uartService);

  uartService.addCharacteristic(rxChar);
  uartService.addCharacteristic(txChar);
  BLE.addService(uartService);

  BLE.advertise();
  Serial.println("BLE aktiv: Alvik-WASD (NUS).");
}

void loop() {
  BLEDevice central = BLE.central();

  if (central) {
    Serial.print("Verbunden: ");
    Serial.println(central.address());

    while (central.connected()) {
      if (rxChar.written()) {
        byte buf[20];
        int len = rxChar.valueLength();
        rxChar.readValue(buf, len);
        if (len > 0) handleKey((char)buf[0]);
        
      }

     /* // Failsafe, stoppt nach 300ms
      if (millis() - lastCmdMs > FAILSAFE_MS) {
        setMotors(0, 0);
      }*/
      delay(10);
    }

    Serial.println("Verbindung getrennt.");
    setMotors(0, 0);
  }
}
