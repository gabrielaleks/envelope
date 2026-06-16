#pragma once

#include <Adafruit_SSD1306.h>
#include <Wire.h>

#include "common_config.h"

class Display {
public:
    Display();
    int init();
    void print(const char* format, ...);
    void println(const char* format, ...);
    void clear();

private:
    Adafruit_SSD1306 _display;
};