#include <Arduino_Alvik.h>
Arduino_Alvik alvik;
uint16_t blink_delay = 400;
uint16_t now_time = 0;
uint16_t last_time = 0;

// Tasten werden nur selten abgefragt ==== >>> Milis nutzen


void setup() {
  alvik.begin();
  // Optional: RGB-LED zunächst ausschalten
  alvik.left_led.set_color(0, 0, 0); // RGB: aus  
}

void loop() {
  now_time = millis();
  if (now_time >= last_time + blink_delay ){
     last_time = now_time;
    // LED auf Rot schalten
    alvik.left_led.set_color(255, 0, 0);
    alvik.right_led.set_color(0, 0, 0);
  }

  // LED ausschalten
  alvik.left_led.set_color(0, 0, 0);
  alvik.right_led.set_color(0, 255, 0);
  delay(blink_delay);

  if ( alvik.get_touch_up()){
    Serial.println("UP-Taste wurde gedrückt!");
    Serial.print("Battery :");
    Serial.println(alvik.get_battery_charge());
    blink_delay += 100; 
  }

  if ( alvik.get_touch_down()){
    Serial.println("Down-Taste wurde gedrückt!");
    if(blink_delay >100)
        blink_delay -= 100; 
  }
}

