#include "Manager.h"

#include "Battery.h"
#include "esp_task_wdt.h"

Manager::Manager()
    : _photoresistor(PIN_PHOTORESISTOR, PHOTORESISTOR_SAMPLES),
      _flapSwitch(PIN_REED_FLAP),
      _boxSwitch(PIN_REED_BOX),
      _ultrasonic(PIN_ULTRASONIC_TRIGGER, PIN_ULTRASONIC_ECHO, ULTRASONIC_TIMEOUT_US) {
    analogSetAttenuation(ADC_11db);
    Log::init(DEBUG);
}

RTC_DATA_ATTR uint8_t seqNumber = 1;

void Manager::run() {
    _display.init();
    Log::setDisplay(_display);
    _lora.init();
    _photoresistor.init();
    _flapSwitch.init();
    _boxSwitch.init();
    _ultrasonic.init();

    int errorCount = 0;

    esp_task_wdt_reset();
    _display.clear();

    auto light = _photoresistor.getMeasurement();
    auto distance = _ultrasonic.getMeasurement();
    auto isFlapMagnetOn = _flapSwitch.isMagnetConnected();
    auto isBoxMagnetOn = _boxSwitch.isMagnetConnected();

    auto result = Manager::classifyEvent(
        light,
        distance,
        isFlapMagnetOn,
        isBoxMagnetOn);

    Packet packet = {
        .seqNumber = seqNumber,
        .wasFlapOpened = result.wasFlapOpened,
        .wasBoxOpened = result.wasBoxOpened,
        .lightLevel = light,
        .distanceCm = distance,
        .flapMagnetPresent = isFlapMagnetOn,
        .boxMagnetPresent = isBoxMagnetOn,
        .batteryVoltage = Battery::getVoltage(),
    };

    debugPrint(packet);

    bool ackReceived = false;
    for (int attempt = 1; attempt <= LORA_RETRY_QUANTITY && !ackReceived; attempt++) {
        auto sendState = _lora.send(packet);
        if (sendState != RADIOLIB_ERR_NONE) {
            Log::serialln("Send failed: %d", sendState);
            if (++errorCount > 10) {
                _lora.init();
                errorCount = 0;
            }
            break;
        }

        Log::displayln("LoRa packet sent");

        Ack ack;
        auto receiveState = _lora.receive(ack);

        if (receiveState == RADIOLIB_ERR_NONE) {
            debugPrint(ack);
            Log::displayln("ACK received");
            errorCount = 0;
            ackReceived = true;
        } else if (receiveState == RADIOLIB_ERR_RX_TIMEOUT) {
            Log::displayln("No ACK (%d/%d)", attempt, LORA_RETRY_QUANTITY);
            Log::serialln("No ACK %d/%d", attempt, LORA_RETRY_QUANTITY);
        } else {
            if (++errorCount > 10) {
                _lora.init();
                errorCount = 0;
            }
            break;
        }
    }
    seqNumber++;
}

/**
 * | Box reed switch | Flap reed switch | Photoresistor | Ultrasonic sensor | Meaning |
 * |---|---|---|---|---|
 * | false | false | false | false | nothing |
 * | false | false | false | true | nothing |
 * | false | false | true | false | nothing |
 * | false | false | true | true | flap opened (80%-90%) |
 * | false | true | false | false | flap opened |
 * | false | true | false | true | flap opened |
 * | false | true | true | false | flap opened |
 * | false | true | true | true | flap opened |
 * | true | false | false | false | box opened |
 * | true | false | false | true | box opened |
 * | true | false | true | false | box opened |
 * | true | false | true | true | box opened |
 * | true | true | false | false | box opened |
 * | true | true | false | true | box opened |
 * | true | true | true | false | box opened |
 * | true | true | true | true | box opened |
 */
EventResult Manager::classifyEvent(
    uint16_t light,
    uint16_t distance,
    bool isFlapMagnetOn,
    bool isBoxMagnetOn) {
    EventResult result;

    if (!isBoxMagnetOn) {
        return {.wasFlapOpened = false, .wasBoxOpened = true};
    }

    if (!isFlapMagnetOn) {
        return {.wasFlapOpened = true, .wasBoxOpened = false};
    }

    if (light > LIGHT_THRESHOLD && distance < DISTANCE_THRESHOLD) {
        return {.wasFlapOpened = true, .wasBoxOpened = false};
    }

    return {.wasFlapOpened = false, .wasBoxOpened = false};
}