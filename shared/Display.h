#pragma once

#include <Adafruit_SSD1306.h>
#include <Wire.h>

#include "display_config.h"

class Display {
public:
    Display();
    int init();
    void print(const char* format, ...);
    void println(const char* format, ...);
    void clear();
    void br();

private:
    Adafruit_SSD1306 _display;
};