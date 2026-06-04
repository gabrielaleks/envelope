#include "Log.h"

Display* Log::_display = nullptr;
int Log::_level = LOG_OFF;

void Log::init(int level) {
    _level = level;
}

void Log::setDisplay(Display& display) {
    _display = &display;
}

void Log::serial(const char* format, ...) {
    if (_level < LOG_ALL)
        return;

    char buffer[128];
    va_list args;
    va_start(args, format);
    vsnprintf(buffer, sizeof(buffer), format, args);
    va_end(args);

    Serial.print(buffer);
}

void Log::serialln(const char* format, ...) {
    if (_level < LOG_ALL)
        return;

    char buffer[128];
    va_list args;
    va_start(args, format);
    vsnprintf(buffer, sizeof(buffer), format, args);
    va_end(args);

    Serial.println(buffer);
}

void Log::display(const char* format, ...) {
    if (_level < LOG_DISPLAY)
        return;

    if (_display == nullptr) {
        Log::serialln("display is not defined");
        return;
    }

    char buffer[128];
    va_list args;
    va_start(args, format);
    vsnprintf(buffer, sizeof(buffer), format, args);
    va_end(args);

    _display->print(buffer);
}

void Log::displayln(const char* format, ...) {
    if (_level < LOG_DISPLAY)
        return;

    if (_display == nullptr) {
        Log::serialln("display is not defined");
        return;
    }

    char buffer[128];
    va_list args;
    va_start(args, format);
    vsnprintf(buffer, sizeof(buffer), format, args);
    va_end(args);

    _display->println(buffer);
}