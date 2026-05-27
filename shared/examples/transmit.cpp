#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Arduino.h>
#include <RadioLib.h>
#include <SPI.h>
#include <Wire.h>

// T3-S3 V1.x SX1276 pins (from LilyGo utilities.h)
#define RADIO_CS_PIN 7
#define RADIO_DIO0_PIN 9
#define RADIO_DIO1_PIN 33
// #define RADIO_DIO2_PIN 34
// #define RADIO_DIO3_PIN 21
// #define RADIO_DIO4_PIN 10
// #define RADIO_DIO5_PIN 36
#define RADIO_RST_PIN 8
#define RADIO_MOSI_PIN 6
#define RADIO_MISO_PIN 3
#define RADIO_SCLK_PIN 5
#define OLED_SDA 18
#define OLED_SCL 17

// OLED parameters
#define OLED_WIDTH 128
#define OLED_HEIGHT 64
#define OLED_ADDR 0x3C

SX1276 radio = new Module(RADIO_CS_PIN, RADIO_DIO0_PIN, RADIO_RST_PIN, RADIO_DIO1_PIN);

static volatile bool transmittedFlag = false;

void setFlag(void) {
    transmittedFlag = true;
}

Adafruit_SSD1306 display(OLED_WIDTH, OLED_HEIGHT, &Wire, -1);

static String payload;
static uint32_t counter = 0;
static int transmissionState = RADIOLIB_ERR_NONE;

void setup() {
    Serial.begin(115200);

    delay(2000);

    Wire.begin(OLED_SDA, OLED_SCL);

    if (!display.begin(SSD1306_SWITCHCAPVCC, OLED_ADDR)) {
        Serial.println("DISPLAY: FAILED");
    } else {
        Serial.println("DISPLAY: OK");
        display.clearDisplay();
        display.setTextSize(1);
        display.setTextColor(SSD1306_WHITE);
        display.setCursor(0, 0);
        display.display();
    }

    SPI.begin(RADIO_SCLK_PIN, RADIO_MISO_PIN, RADIO_MOSI_PIN);

    Serial.print(F("[SX1276] Initializing ... "));
    int state = radio.begin();

    if (state == RADIOLIB_ERR_NONE) {
        Serial.println(F("success!"));
    } else {
        Serial.print(F("failed, code "));
        Serial.println(state);
        while (true)
            ;
    }

    radio.setPacketSentAction(setFlag);

    transmissionState = radio.startTransmit(String(counter).c_str());
    radio.setFrequency(868.0);
    radio.setBandwidth(125.0);
    radio.setSpreadingFactor(12);
    radio.setCodingRate(6);
    radio.setSyncWord(0xAB);
    radio.setOutputPower(14);
    radio.setCurrentLimit(140);
    radio.setPreambleLength(16);
    radio.setCRC(false);

    delay(1000);
}

void loop() {
    if (!transmittedFlag) {
        return;
    }

    payload = "#" + String(counter++);
    transmittedFlag = false;

    if (transmissionState == RADIOLIB_ERR_NONE) {
        Serial.println(F("transmission finished!"));
        display.clearDisplay();
        display.setCursor(0, 0);
        display.print("package ");
        display.print(payload);
        display.display();
    } else {
        Serial.print(F("failed, code "));
        Serial.println(transmissionState);
    }

    delay(1000);

    transmissionState = radio.startTransmit(payload);
}
