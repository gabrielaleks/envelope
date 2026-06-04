#include <Arduino.h>
#include <Manager.h>

#include "esp_task_wdt.h"

void setup() {
    esp_task_wdt_init(30, true);  // reset if hung for 30s
    esp_task_wdt_add(NULL);

#if DEBUG >= LOG_ALL
    Serial.begin(115200);
    delay(2000);
#endif

    esp_sleep_enable_ext1_wakeup(WAKEUP_MASK, ESP_EXT1_WAKEUP_ANY_LOW);

    if (esp_sleep_get_wakeup_cause() == ESP_SLEEP_WAKEUP_EXT1) {
        Manager manager;
        manager.run();

        while (gpio_get_level(PIN_REED_FLAP) == 0 || gpio_get_level(PIN_REED_BOX) == 0) {
            esp_task_wdt_reset();
            delay(100);
        }
    }

    esp_deep_sleep_start();
}

void loop() {
}