#include "Display.h"

#include "Log.h"

Display::Display() : _display(PIN_OLED_WIDTH, PIN_OLED_HEIGHT, &Wire, -1) {
}

int Display::init() {
    Log::serialln("Initializing display...");
    Wire.begin(PIN_OLED_SDA, PIN_OLED_SCL);
    if (!_display.begin(SSD1306_SWITCHCAPVCC, PIN_OLED_ADDR)) {
        Log::serialln("Failed to initialize display");
        return -1;
    }

    Log::serialln("Display initialized!\n");

    _display.clearDisplay();
    _display.setTextSize(1);
    _display.println();
    _display.setTextColor(SSD1306_WHITE);
    _display.setCursor(0, 0);
    _display.display();

    return 0;
}

void Display::clear() {
    _display.clearDisplay();
    _display.setCursor(0, 0);
    _display.display();
}

void Display::print(const char* format, ...) {
    char buffer[128];
    va_list args;
    va_start(args, format);
    vsnprintf(buffer, sizeof(buffer), format, args);
    va_end(args);

    _display.print(buffer);
    _display.display();
}

void Display::println(const char* format, ...) {
    char buffer[128];
    va_list args;
    va_start(args, format);
    vsnprintf(buffer, sizeof(buffer), format, args);
    va_end(args);

    _display.println(buffer);
    _display.display();
}