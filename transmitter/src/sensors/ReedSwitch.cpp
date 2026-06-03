#include "ReedSwitch.h"

ReedSwitch::ReedSwitch(int pin) : _pin(pin) {}

int ReedSwitch::init() {
    Serial.printf("Initializing reed switch on pin %d...\n", _pin);
    pinMode(_pin, INPUT);

    Serial.println("Reed switch initialized!\n");
    return 0;
}

bool ReedSwitch::isMagnetConnected() { return digitalRead(_pin) == HIGH; }