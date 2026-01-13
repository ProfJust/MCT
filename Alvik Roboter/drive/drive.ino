/*
    This file is part of the Arduino_Alvik library.

    Copyright (c) 2024 Arduino SA

    This Source Code Form is subject to the terms of the Mozilla Public
    License, v. 2.0. If a copy of the MPL was not distributed with this
    file, You can obtain one at http://mozilla.org/MPL/2.0/.
    
*/


// Fährt ein Acht , OJ 26.09.25
/* C++ von der Arduino IDE flashen:
Langsamer Doppelklick auf RST Butto neben dem USB Kabel (LED Flashes all colours)
Nochmal drücken => LED leuchtet Grün (nur grün!!)  */

#include "Arduino_Alvik.h"

Arduino_Alvik alvik;

void setup() {
  alvik.begin();
}

void loop() {
  alvik.drive(10, 45);
  delay(10000);
  alvik.drive(10, -45);
  delay(10000);
}