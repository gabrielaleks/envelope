#pragma once

#include <RadioLib.h>

#include "common_config.h"
#include "packet.h"

class LoRaRadio {
public:
    LoRaRadio();
    int init();
    int8_t getRSSI();

    template <typename T>
    int send(const T& data) {
        return radio.transmit((uint8_t*)&data, sizeof(T));
    }

    template <typename T>
    int receive(T& data, int timeoutMs = LORA_RECEIVE_TIMEOUT_MS) {
        return radio.receive((uint8_t*)&data, sizeof(T), timeoutMs);
    }

private:
    SX1276 radio;
};