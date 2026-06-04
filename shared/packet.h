#pragma once

#include <stdint.h>

struct __attribute__((packed)) Packet {
    uint8_t seq_number;

    // event flags
    bool was_flap_opened;
    bool was_box_opened;

    // raw sensor readings
    uint16_t light_level;  // raw ADC value
    uint16_t distance_cm;
    bool flap_magnet_present;
    bool box_magnet_present;

    // diagnostics
    float battery_voltage;
};

inline void debugPrint(const Packet& p) {
    Serial.println("--- Packet information ---");
    Serial.printf("-> seq_number: %d\n", p.seq_number);
    Serial.printf("-> was_flap_opened: %s\n", p.was_flap_opened ? "true" : "false");
    Serial.printf("-> was_box_opened: %s\n", p.was_box_opened ? "true" : "false");
    Serial.println("Sensor readings");
    Serial.printf("-> light_level: %d\n", p.light_level);
    Serial.printf("-> distance_cm: %d\n", p.distance_cm);
    Serial.printf("-> flap_magnet_present: %d\n", p.flap_magnet_present);
    Serial.printf("-> box_magnet_present: %d\n", p.box_magnet_present);
    Serial.printf("-> battery_voltage: %f\n", p.battery_voltage);
}

struct __attribute__((packed)) Ack {
    uint8_t seq_number;
    bool success;
};

inline void debugPrint(const Ack& a) {
    Serial.printf("-> seq_number: %d\n", a.seq_number);
    Serial.printf("-> success: %s\n", a.success ? "true" : "false");
    Serial.println();
}