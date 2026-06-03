#include "Manager.h"

Manager::Manager()
    : _photoresistor(PIN_PHOTORESISTOR), _flapSwitch(PIN_REED_FLAP), _boxSwitch(PIN_REED_BOX),
      _ultrasonic(PIN_ULTRASONIC_TRIGGER, PIN_ULTRASONIC_ECHO) {
    analogSetAttenuation(ADC_11db);
}

void Manager::run() {
    _lora.init();
    _photoresistor.init();
    _flapSwitch.init();
    _boxSwitch.init();
    _ultrasonic.init();

    unsigned long start = millis();

    while (millis() - start < MEASUREMENT_WINDOW_MS) {
        auto light = _photoresistor.getMeasurement();

        auto isFlapMagnetOn = _flapSwitch.isMagnetConnected();
        auto isBoxMagnetOn = _boxSwitch.isMagnetConnected();

        auto distance = _ultrasonic.getMeasurement();

        Serial.printf("photoresistor: %d\n", light);
        Serial.printf("box magnet on: %s\n", isBoxMagnetOn ? "true" : "false");
        Serial.printf("flap magnet on: %s\n", isFlapMagnetOn ? "true" : "false");
        Serial.printf("distance: %d\n", distance);

        Serial.println();
        delay(500);
    }

    Packet packet = {.seq_number = 1, .was_photoresistor_triggered = true};

    _lora.send(packet);
}
