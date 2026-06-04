#pragma once

#include <Arduino.h>

#include "config.h"

class Photoresistor {
public:
    Photoresistor(int pin);
    int init();
    uint16_t getMeasurement();

private:
    int _pin;
};