#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Arduino.h>
#include <Wire.h>

// T3S3 V1.x pin assignments (from LilyGo utilities.h)
#define LED_PIN 37  // GPIO LED, HIGH = on
#define OLED_SDA 18
#define OLED_SCL 17
#define OLED_WIDTH 128
#define OLED_HEIGHT 64
#define OLED_ADDR 0x3C

Adafruit_SSD1306 display(OLED_WIDTH, OLED_HEIGHT, &Wire, -1);

void setup() {
    Serial.begin(115200);
    delay(2000);
    Serial.println("=== T3S3 Diagnostic ===");
    Serial.println("MCU: alive");

    // LED test
    pinMode(LED_PIN, OUTPUT);
    for (int i = 0; i < 6; i++) {
        digitalWrite(LED_PIN, i % 2);
        delay(300);
    }
    Serial.println("LED: blink done");

    // Display test
    Wire.begin(OLED_SDA, OLED_SCL);
    if (!display.begin(SSD1306_SWITCHCAPVCC, OLED_ADDR)) {
        Serial.println("DISPLAY: FAILED");
    } else {
        Serial.println("DISPLAY: OK");
        display.clearDisplay();
        display.setTextSize(1);
        display.setTextColor(SSD1306_WHITE);
        display.setCursor(0, 0);
        display.println("T3S3 Diagnostic");
        display.println("Display: OK");
        display.println("Name: Gabriel");
        display.display();
    }

    Serial.println("=== Diagnostic complete ===");
}

void loop() {
    digitalWrite(LED_PIN, HIGH);
    delay(1000);
    digitalWrite(LED_PIN, LOW);
    delay(1000);
}
