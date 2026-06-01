#include <Arduino.h>

const int TCRT_A0_PIN = 15;
const int NUM_SAMPLES = 32;

void setup() {
    Serial.begin(115200);
    delay(2000);

    analogSetAttenuation(ADC_11db);  // Full 0–3.3V range
    pinMode(TCRT_A0_PIN, INPUT);
}

void loop() {
    long sum = 0;
    for (int i = 0; i < NUM_SAMPLES; i++) {
        sum += analogRead(TCRT_A0_PIN);
    }

    int valueRead = sum / NUM_SAMPLES;  // Raw ADC value

    Serial.printf("valueRead: %d\n", valueRead);

    delay(100);
}
