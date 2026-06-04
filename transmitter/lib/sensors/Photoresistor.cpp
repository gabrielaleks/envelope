#include "Photoresistor.h"

#include "Log.h"

Photoresistor::Photoresistor(int pin, int samples) : _pin(pin), _samples(samples) {
}

int Photoresistor::init() {
    Log::displayln("Initializing photoresistor on pin %d...", _pin);

    pinMode(_pin, INPUT);

    int val = analogRead(_pin);
    if (val < 20) {
        Log::displayln("Photoresistor not detected");
        return -1;
    }

    Log::displayln("Photoresistor initialized!");
    return 0;
}

uint16_t Photoresistor::getMeasurement() {
    long sum = 0;
    for (int i = 0; i < _samples; i++) {
        sum += analogRead(_pin);
    }

    return sum / _samples;  // Raw averaged ADC value
}