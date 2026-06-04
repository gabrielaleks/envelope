#pragma once

#include <stdint.h>

#include "Log.h"

struct __attribute__((packed)) Packet {
    uint8_t seqNumber;

    // event flags
    bool wasFlapOpened;
    bool wasBoxOpened;

    // raw sensor readings
    uint16_t lightLevel;  // raw ADC value
    uint16_t distanceCm;
    bool flapMagnetPresent;
    bool boxMagnetPresent;

    // diagnostics
    float batteryVoltage;
};

inline void debugPrint(const Packet& p) {
    Log::serialln("--- Packet information ---");
    Log::serialln("-> seqNumber: %d", p.seqNumber);
    Log::serialln("-> wasFlapOpened: %s", p.wasFlapOpened ? "true" : "false");
    Log::serialln("-> wasBoxOpened: %s", p.wasBoxOpened ? "true" : "false");
    Log::serialln("-> lightLevel: %d", p.lightLevel);
    Log::serialln("-> distance: %d cm", p.distanceCm);
    Log::serialln("-> flapMagnetPresent: %d", p.flapMagnetPresent);
    Log::serialln("-> boxMagnetPresent: %d", p.boxMagnetPresent);
    Log::serialln("-> batteryVoltage: %.2f V", p.batteryVoltage);
    Log::serialln("");
}

struct __attribute__((packed)) Ack {
    uint8_t seqNumber;
    bool success;
};

inline void debugPrint(const Ack& a) {
    Log::serialln("--- Ack information ---");
    Log::serialln("-> seqNumber: %d", a.seqNumber);
    Log::serialln("-> success: %s", a.success ? "true" : "false");
    Log::serialln("");
}