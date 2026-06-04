#include "LoRaRadio.h"

LoRaRadio::LoRaRadio() : radio(new Module(PIN_LORA_CS, PIN_LORA_DIO0, PIN_LORA_RST, PIN_LORA_DIO1)) {
}

int LoRaRadio::init() {
    Log::serialln("Initializing LoRa...");

    SPI.begin(PIN_LORA_SCLK, PIN_LORA_MISO, PIN_LORA_MOSI, PIN_LORA_CS);

    int state = radio.begin();

    if (state != RADIOLIB_ERR_NONE) {
        Log::serialln("LoRa init failed: %d", state);
        return state;
    }

    if (radio.setFrequency(LORA_FREQUENCY) == RADIOLIB_ERR_INVALID_FREQUENCY) {
        Log::serialln("Selected frequency is invalid for this module: %f", LORA_FREQUENCY);
        return RADIOLIB_ERR_INVALID_FREQUENCY;
    }

    if (radio.setBandwidth(LORA_BANDWIDTH) == RADIOLIB_ERR_INVALID_BANDWIDTH) {
        Log::serialln("Selected bandwidth is invalid for this module: %f", LORA_BANDWIDTH);
        return RADIOLIB_ERR_INVALID_BANDWIDTH;
    }

    if (radio.setSpreadingFactor(LORA_SPREADING_FACTOR) == RADIOLIB_ERR_INVALID_SPREADING_FACTOR) {
        Log::serialln("Selected spreading factor is invalid for this module: %d", LORA_SPREADING_FACTOR);
        return RADIOLIB_ERR_INVALID_SPREADING_FACTOR;
    }

    if (radio.setCodingRate(LORA_CODING_RATE) == RADIOLIB_ERR_INVALID_CODING_RATE) {
        Log::serialln("Selected coding rate is invalid for this module: %d", LORA_CODING_RATE);
        return RADIOLIB_ERR_INVALID_CODING_RATE;
    }

    if (radio.setSyncWord(LORA_SYNC_WORD) != RADIOLIB_ERR_NONE) {
        Log::serialln("Unable to set sync word: %d", LORA_SYNC_WORD);
        return -1;
    }

    if (radio.setOutputPower(LORA_OUTPUT_POWER) == RADIOLIB_ERR_INVALID_OUTPUT_POWER) {
        Log::serialln("Selected output power is invalid for this module: %d", LORA_OUTPUT_POWER);
        return RADIOLIB_ERR_INVALID_OUTPUT_POWER;
    }

    if (radio.setCurrentLimit(LORA_CURRENT_LIMIT) == RADIOLIB_ERR_INVALID_CURRENT_LIMIT) {
        Log::serialln("Selected current limit is invalid for this module: %d", LORA_CURRENT_LIMIT);
        return RADIOLIB_ERR_INVALID_CURRENT_LIMIT;
    }

    if (radio.setPreambleLength(LORA_PREAMBLE_LENGTH) == RADIOLIB_ERR_INVALID_PREAMBLE_LENGTH) {
        Log::serialln("Selected preamble length is invalid for this module: %d", LORA_PREAMBLE_LENGTH);
        return RADIOLIB_ERR_INVALID_PREAMBLE_LENGTH;
    }

    if (radio.setCRC(LORA_RCR) == RADIOLIB_ERR_INVALID_CRC_CONFIGURATION) {
        Log::serialln("Selected CRC is invalid for this module: %d", LORA_RCR);
        return RADIOLIB_ERR_INVALID_CRC_CONFIGURATION;
    }

    Log::serialln("LoRa initialized!\n");

    return 0;
}

int8_t LoRaRadio::getRSSI() {
    return (int8_t)radio.getRSSI();
}