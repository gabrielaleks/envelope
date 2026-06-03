#include "LoRaRadio.h"

LoRaRadio::LoRaRadio() : radio(new Module(PIN_LORA_CS, PIN_LORA_DIO0, PIN_LORA_RST, PIN_LORA_DIO1)) {}

int LoRaRadio::init() {
    Serial.println("Initializing LoRa...");

    SPI.begin(PIN_LORA_SCLK, PIN_LORA_MISO, PIN_LORA_MOSI, PIN_LORA_SCLK);

    int state = radio.begin();

    if (state != RADIOLIB_ERR_NONE) {
        Serial.printf("LoRa init failed: %d\n", state);
        return state;
    }

    if (radio.setFrequency(LORA_FREQUENCY) == RADIOLIB_ERR_INVALID_FREQUENCY) {
        Serial.printf("Selected frequency is invalid for this module: %f\n", LORA_FREQUENCY);
        return RADIOLIB_ERR_INVALID_FREQUENCY;
    }

    if (radio.setBandwidth(LORA_BANDWIDTH) == RADIOLIB_ERR_INVALID_BANDWIDTH) {
        Serial.printf("Selected bandwidth is invalid for this module: %f", LORA_BANDWIDTH);
        return RADIOLIB_ERR_INVALID_BANDWIDTH;
    }

    if (radio.setSpreadingFactor(LORA_SPREADING_FACTOR) == RADIOLIB_ERR_INVALID_SPREADING_FACTOR) {
        Serial.printf("Selected spreading factor is invalid for this module: %d\n", LORA_SPREADING_FACTOR);
        return RADIOLIB_ERR_INVALID_SPREADING_FACTOR;
    }

    if (radio.setCodingRate(LORA_CODING_RATE) == RADIOLIB_ERR_INVALID_CODING_RATE) {
        Serial.printf("Selected coding rate is invalid for this module: %d\n", LORA_CODING_RATE);
        return RADIOLIB_ERR_INVALID_CODING_RATE;
    }

    if (radio.setSyncWord(LORA_SYNC_WORD) != RADIOLIB_ERR_NONE) {
        Serial.printf("Unable to set sync word: %d\n", LORA_SYNC_WORD);
        return 1;
    }

    if (radio.setOutputPower(LORA_OUTPUT_POWER) == RADIOLIB_ERR_INVALID_OUTPUT_POWER) {
        Serial.printf("Selected output power is invalid for this module: %d\n", LORA_OUTPUT_POWER);
        return RADIOLIB_ERR_INVALID_OUTPUT_POWER;
    }

    if (radio.setCurrentLimit(LORA_CURRENT_LIMIT) == RADIOLIB_ERR_INVALID_CURRENT_LIMIT) {
        Serial.printf("Selected current limit is invalid for this module: %d\n", LORA_CURRENT_LIMIT);
        return RADIOLIB_ERR_INVALID_CURRENT_LIMIT;
    }

    if (radio.setPreambleLength(LORA_PREAMBLE_LENGTH) == RADIOLIB_ERR_INVALID_PREAMBLE_LENGTH) {
        Serial.printf("Selected preamble length is invalid for this module: %d\n", LORA_PREAMBLE_LENGTH);
        return RADIOLIB_ERR_INVALID_PREAMBLE_LENGTH;
    }

    if (radio.setCRC(LORA_RCR) == RADIOLIB_ERR_INVALID_CRC_CONFIGURATION) {
        Serial.printf("Selected CRC is invalid for this module: %d\n", LORA_RCR);
        return RADIOLIB_ERR_INVALID_CRC_CONFIGURATION;
    }

    Serial.println("LoRa initialized! \n");

    return 0;
}

int LoRaRadio::send(Packet packet) {
    Serial.println("\n-----");
    Serial.println("Sending packet");

    radio.transmit((uint8_t *)&packet, sizeof(packet));

    Serial.println("Packet sent: ");
    Serial.printf("-> seq_number: %d\n", packet.seq_number);
    Serial.println("-----");

    return 0;
}