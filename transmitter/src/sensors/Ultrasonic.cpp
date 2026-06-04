#include "Ultrasonic.h"

#include "Log.h"

Ultrasonic::Ultrasonic(int triggerPin, int echoPin) : _triggerPin(triggerPin), _echoPin(echoPin) {
}

int Ultrasonic::init() {
    Log::displayln("Initializing ultrasonic sensor on pins trigger %d, echo %d...", _triggerPin, _echoPin);

    pinMode(_triggerPin, OUTPUT);
    pinMode(_echoPin, INPUT);

    Log::displayln("Ultrasonic sensor initialized!");
    return 0;
}

uint16_t Ultrasonic::getMeasurement() {
    long duration, distanceCm;

    digitalWrite(_triggerPin, LOW);
    delayMicroseconds(4);
    digitalWrite(_triggerPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(_triggerPin, LOW);

    duration = pulseIn(_echoPin, HIGH, ULTRASONIC_TIMEOUT_US);

    // no echo received
    if (duration == 0) {
        return -1;
    }

    // convert to distance in cm
    distanceCm = duration * 10 / 292 / 2;
    return distanceCm;
}