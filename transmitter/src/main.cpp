#include <Arduino.h>
#include <Manager.h>

void setup() {
    Serial.begin(115200);
    delay(2000);

    Manager manager;
    manager.run();
}

void loop() {
}