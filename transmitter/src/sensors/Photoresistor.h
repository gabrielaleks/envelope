#pragma once

#include <Arduino.h>

#include "config.h"

class Photoresistor {
public:
    Photoresistor(int pin);
    int init();
    int getMeasurement();

private:
    int _pin;
};