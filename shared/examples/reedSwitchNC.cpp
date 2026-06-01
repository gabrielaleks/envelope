#include <Arduino.h>

const int REED_SWITCH_PIN = 38;

void setup() {
    Serial.begin(115200);
    delay(2000);

    pinMode(REED_SWITCH_PIN, INPUT_PULLUP);
}

void loop() {
    int switchState = digitalRead(REED_SWITCH_PIN);

    if (switchState == LOW) {
        Serial.println("Magnet off");
    } else {
        Serial.println("Magnet on");
    }

    delay(200);
}
