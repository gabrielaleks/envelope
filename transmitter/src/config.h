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

// Reed switches
static const gpio_num_t PIN_REED_FLAP = GPIO_NUM_16;
static const gpio_num_t PIN_REED_BOX = GPIO_NUM_15;

// Photoresistor
static const int PIN_PHOTORESISTOR = 12;
static const int PHOTORESISTOR_SAMPLES = 32;
static const int LIGHT_THRESHOLD = 500;

// Ultrasonic
static const int PIN_ULTRASONIC_TRIGGER = 41;
static const int PIN_ULTRASONIC_ECHO = 45;
static const int ULTRASONIC_TIMEOUT_US = 300000;

// Running window
static const int MEASUREMENT_WINDOW_MS = 1000000;