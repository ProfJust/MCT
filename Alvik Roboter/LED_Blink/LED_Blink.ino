#include <Arduino_Alvik.h>

Arduino_Alvik alvik;
int blink_delay = 400;

void setup() {
  alvik.begin();
  // Optional: RGB-LED zunächst ausschalten
  alvik.left_led.set_color(0, 0, 0); // RGB: aus
  
}

void loop() {
  // LED auf Rot schalten
  alvik.left_led.set_color(255, 0, 0);
  alvik.right_led.set_color(0, 0, 0);
  delay(blink_delay);

  // LED ausschalten
  alvik.left_led.set_color(0, 0, 0);
  alvik.right_led.set_color(0, 255, 0);
  delay(blink_delay);

  if ( alvik.get_touch_up()){
    Serial.println("UP-Taste wurde gedrückt!");
    blink_delay += 100; 
  }

  if ( alvik.get_touch_down()){
    Serial.println("Down-Taste wurde gedrückt!");
    if(blink_delay >100)
        blink_delay -= 100; 
  }
}

