#include "Manager.h"

Manager::Manager()
    : _photoresistor(PIN_PHOTORESISTOR), _flapSwitch(PIN_REED_FLAP), _boxSwitch(PIN_REED_BOX), _ultrasonic(PIN_ULTRASONIC_TRIGGER, PIN_ULTRASONIC_ECHO) {
    analogSetAttenuation(ADC_11db);
}

void Manager::run() {
    _display.init();
    _lora.init();
    _photoresistor.init();
    _flapSwitch.init();
    _boxSwitch.init();
    _ultrasonic.init();

    unsigned long start = millis();

    uint8_t seq = 1;
    while (millis() - start < MEASUREMENT_WINDOW_MS) {
        _display.clear();

        auto light = _photoresistor.getMeasurement();

        auto isFlapMagnetOn = _flapSwitch.isMagnetConnected();
        auto isBoxMagnetOn = _boxSwitch.isMagnetConnected();

        auto distance = _ultrasonic.getMeasurement();

        Serial.println();
        Serial.printf("photoresistor: %d\n", light);
        Serial.printf("box magnet on: %s\n", isBoxMagnetOn ? "true" : "false");
        Serial.printf("flap magnet on: %s\n", isFlapMagnetOn ? "true" : "false");
        Serial.printf("distance: %d\n", distance);
        Serial.println();

        _display.println("Measurements made");

        Packet packet = {
            .seq_number = seq,
            .was_photoresistor_triggered = light > 500 ? true : false,
            .was_movement_detected = distance < 10 ? true : false,
            .was_flap_opened = !isFlapMagnetOn,
            .was_box_opened = !isBoxMagnetOn,
        };

        _lora.send(packet);
        debugPrint(packet);

        _display.println("LoRa packet sent");

        seq++;

        Ack ack;
        int receiveState = _lora.receive(ack);
        if (receiveState == RADIOLIB_ERR_NONE) {
            debugPrint(ack);
            _display.println("ACK received");
        } else {
            _display.println("ACK not received");
            Serial.printf("No ACK received (error: %d)\n", receiveState);
        }

        delay(1000);
    }
}
