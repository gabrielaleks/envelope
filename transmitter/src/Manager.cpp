#include "Manager.h"

#include "Battery.h"
#include "esp_task_wdt.h"

Manager::Manager()
    : _photoresistor(PIN_PHOTORESISTOR, PHOTORESISTOR_SAMPLES),
      _flapSwitch(PIN_REED_FLAP),
      _boxSwitch(PIN_REED_BOX),
      _ultrasonic(PIN_ULTRASONIC_TRIGGER, PIN_ULTRASONIC_ECHO, ULTRASONIC_TIMEOUT_US) {
    analogSetAttenuation(ADC_11db);
    Log::init(LOG_ON);
}

void Manager::run() {
    _display.init();
    Log::setDisplay(_display);
    _lora.init();
    _photoresistor.init();
    _flapSwitch.init();
    _boxSwitch.init();
    _ultrasonic.init();

    int errorCount = 0;

    unsigned long start = millis();

    uint8_t seq = 1;
    while (millis() - start < MEASUREMENT_WINDOW_MS) {
        esp_task_wdt_reset();
        _display.clear();

        auto light = _photoresistor.getMeasurement();

        auto isFlapMagnetOn = _flapSwitch.isMagnetConnected();
        auto isBoxMagnetOn = _boxSwitch.isMagnetConnected();

        auto distance = _ultrasonic.getMeasurement();

        Packet packet = {
            .seq_number = seq,
            .was_flap_opened = !isFlapMagnetOn,
            .was_box_opened = !isBoxMagnetOn,
            .light_level = light,
            .distance_cm = distance,
            .flap_magnet_present = isFlapMagnetOn,
            .box_magnet_present = isBoxMagnetOn,
            .battery_voltage = Battery::getVoltage(),
        };

        debugPrint(packet);

        auto sendState = _lora.send(packet);
        if (sendState != RADIOLIB_ERR_NONE) {
            Log::serialln("Send failed: %d", sendState);
            if (++errorCount > 10) {
                _lora.init();
                errorCount = 0;
            }
        } else {
            Log::displayln("LoRa packet sent");

            seq++;

            Ack ack;
            auto receiveState = _lora.receive(ack);

            if (receiveState == RADIOLIB_ERR_NONE) {
                debugPrint(ack);
                Log::displayln("ACK received");
                errorCount = 0;
            } else if (receiveState != RADIOLIB_ERR_RX_TIMEOUT) {
                if (++errorCount > 10) {
                    _lora.init();
                    errorCount = 0;
                }
            } else {
                Log::displayln("ACK not received");
                Log::serialln("No ACK received (error: %d)", receiveState);
            }
        }

        delay(1000);
    }
}
