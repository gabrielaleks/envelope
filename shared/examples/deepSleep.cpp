#include <Arduino.h>

#include "driver/rtc_io.h"

const int LED_PIN = 38;

#define WAKEUP_GPIO_1 GPIO_NUM_15
#define WAKEUP_GPIO_2 GPIO_NUM_16

#define USE_EXT0_WAKEUP 0

uint64_t wakeup_mask = (1ULL << WAKEUP_GPIO_1) | (1ULL << WAKEUP_GPIO_2);

RTC_DATA_ATTR int bootCount = 0;

void print_wakeup_reason() {
    esp_sleep_wakeup_cause_t wakeup_reason = esp_sleep_get_wakeup_cause();

    switch (wakeup_reason) {
    case ESP_SLEEP_WAKEUP_EXT0:
        Serial.println("Wakeup caused by external signal using RTC_IO");
        break;
    case ESP_SLEEP_WAKEUP_EXT1:
        Serial.println("Wakeup caused by external signal using RTC_CNTL");
        break;
    case ESP_SLEEP_WAKEUP_TIMER:
        Serial.println("Wakeup caused by timer");
        break;
    case ESP_SLEEP_WAKEUP_TOUCHPAD:
        Serial.println("Wakeup caused by touchpad");
        break;
    case ESP_SLEEP_WAKEUP_ULP:
        Serial.println("Wakeup caused by ULP program");
        break;
    default:
        Serial.printf("Wakeup was not caused by deep sleep: %d\n", wakeup_reason);
        break;
    }
}

void handle_ext0_wakeup() {
    digitalWrite(LED_PIN, HIGH);
    delay(500);
    digitalWrite(LED_PIN, LOW);
}

void handle_ext1_wakeup() {
    uint64_t status = esp_sleep_get_ext1_wakeup_status();

    if (status & (1ULL << WAKEUP_GPIO_1)) {
        digitalWrite(LED_PIN, HIGH);
        delay(500);
        digitalWrite(LED_PIN, LOW);
    } else if (status & (1ULL << WAKEUP_GPIO_2)) {
        digitalWrite(LED_PIN, HIGH);
        delay(500);
        digitalWrite(LED_PIN, LOW);
        delay(500);
        digitalWrite(LED_PIN, HIGH);
        delay(500);
        digitalWrite(LED_PIN, LOW);
    }
}

void setup() {
    Serial.begin(115200);

    delay(1000);

    ++bootCount;
    Serial.println("Boot number: " + String(bootCount));

    pinMode(LED_PIN, OUTPUT);

    print_wakeup_reason();

#if USE_EXT0_WAKEUP
    handle_ext0_wakeup();
    esp_sleep_enable_ext0_wakeup(WAKEUP_GPIO_1, 1);
#else
    handle_ext1_wakeup();
    esp_sleep_enable_ext1_wakeup(wakeup_mask, ESP_EXT1_WAKEUP_ANY_HIGH);
#endif

    Serial.println("Going to sleep now");
    Serial.flush();
    esp_deep_sleep_start();
}

void loop() {
}