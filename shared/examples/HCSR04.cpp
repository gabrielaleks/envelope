#include <Arduino.h>

const int ECHO_PIN = 41;
const int TRIGGER_PIN = 45;

void setup() {
    Serial.begin(115200);
    delay(2000);

    pinMode(TRIGGER_PIN, OUTPUT);
    pinMode(ECHO_PIN, INPUT);
}

int ping(int triggerPin, int echoPin) {
    long duration, distanceCm;

    digitalWrite(triggerPin, LOW);
    delayMicroseconds(4);
    digitalWrite(triggerPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(triggerPin, LOW);

    duration = pulseIn(echoPin, HIGH, 300000);

    if (duration == 0)
        return -1;  // no echo received

    distanceCm = duration * 10 / 292 / 2;  // convert to distance, in cm
    return distanceCm;
}

void loop() {
    int cm = ping(TRIGGER_PIN, ECHO_PIN);
    Serial.printf("distance: %d\n", cm);
    delay(50);
}
