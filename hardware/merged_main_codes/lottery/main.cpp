#include <Arduino.h>
#include "BarCode.h"
#include "ESP.h"

void setup() {
    setupBarCode();
    setupESP();
}

void loop() {
    loopBarCode();
    loopESP();
}
