#pragma once

#include <Arduino.h>

#include "Display.h"

class Log {
public:
    static void init(int level);
    static void setDisplay(Display& display);
    static void serial(const char* format, ...);
    static void serialln(const char* format, ...);
    static void display(const char* format, ...);
    static void displayln(const char* format, ...);

private:
    static Display* _display;
    static int _level;
};