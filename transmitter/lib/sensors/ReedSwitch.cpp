#include "ReedSwitch.h"

#include "Log.h"

ReedSwitch::ReedSwitch(int pin) : _pin(pin) {
}

int ReedSwitch::init() {
    Log::serialln("Initializing reed switch on pin %d...", _pin);

    pinMode(_pin, INPUT);

    Log::serialln("Reed switch initialized!");
    return 0;
}

bool ReedSwitch::isMagnetConnected() {
    return digitalRead(_pin) == HIGH;
}