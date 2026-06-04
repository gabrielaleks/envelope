#pragma once

// Log
#define LOG_OFF 0      // 0b00
#define LOG_DISPLAY 1  // 0b01
#define LOG_ALL 2      // 0b10

// Display
static const int PIN_OLED_SDA = 18;
static const int PIN_OLED_SCL = 17;
static const int PIN_OLED_WIDTH = 128;
static const int PIN_OLED_HEIGHT = 64;
static const int PIN_OLED_ADDR = 0x3C;

// LoRa
static const int PIN_LORA_CS = 7;
static const int PIN_LORA_DIO0 = 9;
static const int PIN_LORA_DIO1 = 33;
static const int PIN_LORA_RST = 8;
static const int PIN_LORA_MOSI = 6;
static const int PIN_LORA_MISO = 3;
static const int PIN_LORA_SCLK = 5;
static const float LORA_FREQUENCY = 868.0;
static const float LORA_BANDWIDTH = 125.0;
static const int LORA_SPREADING_FACTOR = 7;  // When testing: 7 | On production: 9
static const int LORA_CODING_RATE = 6;
static const int LORA_SYNC_WORD = 0xAB;
static const int LORA_OUTPUT_POWER = 14;
static const int LORA_CURRENT_LIMIT = 140;
static const int LORA_PREAMBLE_LENGTH = 16;
static const int LORA_RCR = false;
static const int LORA_RECEIVE_TIMEOUT_MS = 1000;

// Battery
static const int PIN_BATTERY_ADC = 1;
