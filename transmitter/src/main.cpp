#include <Arduino.h>
#include <Manager.h>

#include "esp_task_wdt.h"

void setup() {
    esp_task_wdt_init(30, true);  // reset if hung for 30s
    esp_task_wdt_add(NULL);

    Serial.begin(115200);
    delay(2000);

    Manager manager;
    manager.run();
}

void loop() {
}