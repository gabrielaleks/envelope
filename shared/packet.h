#pragma once

#include <stdint.h>

struct __attribute__((packed)) Packet {
    int seq_number;
    bool was_photoresistor_triggered;
    bool was_movement_detected;
    bool was_flap_opened;
    bool was_box_opened;
};