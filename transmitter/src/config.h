/**
 * Define here anything tied to the physical setup:
 * - pin assignments
 * - timing constants
 * - sensor thresholds
 * - lora config
 * - retry countsd
 */

#pragma once

#include "driver/rtc_io.h"

// Environment
// #define DEBUG LOG_DISPLAY  // only display
#define DEBUG LOG_ALL  // full debug
// #define DEBUG LOG_OFF  // production

// Reed switches
static const gpio_num_t PIN_REED_FLAP = GPIO_NUM_16;
static const gpio_num_t PIN_REED_BOX = GPIO_NUM_15;
static const uint64_t WAKEUP_MASK = (1ULL << PIN_REED_FLAP) | (1ULL << PIN_REED_BOX);

// Photoresistor
static const int PIN_PHOTORESISTOR = 12;
static const int PHOTORESISTOR_SAMPLES = 32;
static const int LIGHT_THRESHOLD = 500;

// Ultrasonic
static const int PIN_ULTRASONIC_TRIGGER = 41;
static const int PIN_ULTRASONIC_ECHO = 45;
static const int ULTRASONIC_TIMEOUT_US = 300000;
static const int DISTANCE_THRESHOLD = 10;

// Running window
static const int MEASUREMENT_WINDOW_MS = 1000;