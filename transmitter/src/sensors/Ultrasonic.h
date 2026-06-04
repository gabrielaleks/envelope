#pragma once

#include <Arduino.h>

#include "config.h"

class Ultrasonic {
public:
    Ultrasonic(int triggerPin, int echoPin);
    int init();
    uint16_t getMeasurement();

private:
    int _triggerPin;
    int _echoPin;
};