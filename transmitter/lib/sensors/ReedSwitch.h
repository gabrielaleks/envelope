#pragma once

#include <Arduino.h>

#include "config.h"

class ReedSwitch {
public:
    ReedSwitch(int pin);
    int init();
    bool isMagnetConnected();

private:
    int _pin;
};