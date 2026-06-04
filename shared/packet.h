#pragma once

#include <stdint.h>

#include "Log.h"

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
    Log::serialln("--- Packet information ---");
    Log::serialln("-> seq_number: %d", p.seq_number);
    Log::serialln("-> was_flap_opened: %s", p.was_flap_opened ? "true" : "false");
    Log::serialln("-> was_box_opened: %s", p.was_box_opened ? "true" : "false");
    Log::serialln("-> light_level: %d", p.light_level);
    Log::serialln("-> distance: %d cm", p.distance_cm);
    Log::serialln("-> flap_magnet_present: %d", p.flap_magnet_present);
    Log::serialln("-> box_magnet_present: %d", p.box_magnet_present);
    Log::serialln("-> battery_voltage: %.2f V", p.battery_voltage);
    Log::serialln("");
}

struct __attribute__((packed)) Ack {
    uint8_t seq_number;
    bool success;
};

inline void debugPrint(const Ack& a) {
    Log::serialln("--- Ack information ---");
    Log::serialln("-> seq_number: %d", a.seq_number);
    Log::serialln("-> success: %s", a.success ? "true" : "false");
    Log::serialln("");
}