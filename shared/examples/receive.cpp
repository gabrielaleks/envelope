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
#define RADIO_RST_PIN 8
#define RADIO_MOSI_PIN 6
#define RADIO_MISO_PIN 3
#define RADIO_SCLK_PIN 5

// OLED parameters
#define OLED_SDA 18
#define OLED_SCL 17
#define OLED_WIDTH 128
#define OLED_HEIGHT 64
#define OLED_ADDR 0x3C

Adafruit_SSD1306 display(OLED_WIDTH, OLED_HEIGHT, &Wire, -1);
SX1276 radio = new Module(RADIO_CS_PIN, RADIO_DIO0_PIN, RADIO_RST_PIN, RADIO_DIO1_PIN);

static volatile bool receivedFlag = false;
static String rssi = "0dBm";
static String snr = "0dB";
static String payload = "0";

void setFlag(void) {
    receivedFlag = true;
}

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

    radio.setPacketReceivedAction(setFlag);
    radio.setFrequency(868.0);
    radio.setBandwidth(125.0);
    radio.setSpreadingFactor(12);
    radio.setCodingRate(6);
    radio.setSyncWord(0xAB);
    radio.setOutputPower(14);
    radio.setCurrentLimit(140);
    radio.setPreambleLength(16);
    radio.setCRC(false);

    Serial.print(F("Radio Starting to listen ... "));
    state = radio.startReceive();
    if (state == RADIOLIB_ERR_NONE) {
        Serial.println(F("success!"));
    } else {
        Serial.print(F("failed, code "));
        Serial.println(state);
    }

    delay(1000);
}

void loop() {
    if (receivedFlag) {
        receivedFlag = false;

        int state = radio.readData(payload);

        if (state == RADIOLIB_ERR_NONE) {
            display.clearDisplay();
            display.setCursor(0, 0);

            rssi = String(radio.getRSSI()) + "dBm";
            snr = String(radio.getSNR()) + "dB";

            Serial.println(F("Radio Received packet!"));

            Serial.print(F("Data:\t"));
            Serial.println(String(payload));
            display.print("Data: ");
            display.println(String(payload));

            // print RSSI (Received Signal Strength Indicator)
            Serial.print(F("Radio RSSI:\t"));
            Serial.println(rssi);
            display.print(F("RSSI: "));
            display.println(rssi);

            // print SNR (Signal-to-Noise Ratio)
            Serial.print(F("Radio SNR:\t"));
            Serial.println(snr);
            display.print(F("SNR: "));
            display.println(snr);

            display.display();
        } else if (state == RADIOLIB_ERR_CRC_MISMATCH) {
            Serial.println("CRC error!");
        } else {
            Serial.print("Failed with code ");
            Serial.println(state);
        }
        radio.startReceive();
    }
}
