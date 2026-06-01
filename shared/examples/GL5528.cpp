#include <Arduino.h>

const int LDR_PIN = 16;
const int ADC_MAX = 4096;  // ESP32-S3 is 12-bit
const int NUM_SAMPLES = 32;
const int LIGHT_THRESHOLD = 500;
const int DARK_RESISTANCE_KOHM = 1000;       // Dark resistance in KΩ
const int LIGHT_RESISTANCE_KOHM = 15;        // Light resistance in KΩ
const int CALIBRATION_RESISTANCE_KOHM = 10;  // Calibration resistance in KΩ

void setup() {
    Serial.begin(115200);
    delay(2000);

    analogSetAttenuation(ADC_11db);  // Full 0–3.3V range
    pinMode(LDR_PIN, INPUT);
}

void loop() {
    long sum = 0;
    for (int i = 0; i < NUM_SAMPLES; i++) {
        sum += analogRead(LDR_PIN);
    }

    int valueRead = sum / NUM_SAMPLES;  // Raw ADC value

    int ilum = ((long)valueRead * DARK_RESISTANCE_KOHM * 10) / ((long)LIGHT_RESISTANCE_KOHM * CALIBRATION_RESISTANCE_KOHM * (ADC_MAX - valueRead));  // lux calculation

    bool lightDetected = valueRead > LIGHT_THRESHOLD;

    Serial.printf("ilum: %d | valueRead: %d | %s\n", ilum, valueRead, lightDetected ? "light" : "dark");

    delay(100);
}
