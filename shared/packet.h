#pragma once

#include <stdint.h>

struct __attribute__((packed)) Packet {
    uint8_t seq_number;
    bool was_photoresistor_triggered;
    bool was_movement_detected;
    bool was_flap_opened;
    bool was_box_opened;
};

inline void debugPrint(const Packet& p) {
    Serial.println("Packet sent: ");
    Serial.printf("-> seq_number: %d\n", p.seq_number);
    Serial.printf("-> was_photoresistor_triggered: %s\n", p.was_photoresistor_triggered ? "true" : "false");
    Serial.printf("-> was_movement_detected: %s\n", p.was_movement_detected ? "true" : "false");
    Serial.printf("-> was_flap_opened: %s\n", p.was_flap_opened ? "true" : "false");
    Serial.printf("-> was_box_opened: %s\n", p.was_box_opened ? "true" : "false");
    Serial.println();
}

struct __attribute__((packed)) Ack {
    uint8_t seq_number;
    bool success;
};

inline void debugPrint(const Ack& a) {
    Serial.println("Packet received: ");
    Serial.printf("-> seq_number: %d\n", a.seq_number);
    Serial.printf("-> success: %s\n", a.success ? "true" : "false");
}