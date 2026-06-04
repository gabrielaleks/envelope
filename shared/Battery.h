#pragma once

#include <Arduino.h>

#include <cmath>

#include "common_config.h"

class Battery {
public:
    inline static uint8_t getPercentage() {
        float v = getVoltage();
        if (v >= 4.2)
            return 100;
        if (v <= 3.0)
            return 0;
        return (uint8_t)((v - 3.0) / (4.2 - 3.0) * 100);
    }

    inline static float getVoltage() {
        int raw = analogRead(PIN_BATTERY_ADC);
        float voltage = (raw / 4095.0) * 3.3 * 2.0;
        return roundf(voltage * 100.0f) / 100.0f;
    }
};