#pragma once

#include <Arduino.h>

#include "config.h"

class Ultrasonic {
public:
    Ultrasonic(int triggerPin, int echoPin, uint32_t timeoutUs = 30000);
    int init();
    uint16_t getMeasurement();

private:
    int _triggerPin;
    int _echoPin;
    uint32_t _timeoutUs;
};