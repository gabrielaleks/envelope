#pragma once

#include <Arduino.h>

#include "config.h"

class Ultrasonic {
  public:
    Ultrasonic(int triggerPin, int echoPin);
    int init();
    int getMeasurement();

  private:
    int _triggerPin;
    int _echoPin;
};