#include <Arduino.h>

#include "Display.h"
#include "LoRaRadio.h"
#include "esp_task_wdt.h"
#include "packet.h"

LoRaRadio _lora;
Display _display;

void setup() {
    esp_task_wdt_init(30, true);  // reset if hung for 30s
    esp_task_wdt_add(NULL);

    Serial.begin(115200);
    delay(2000);

    Log::init(LOG_ON);
    _display.init();
    Log::setDisplay(_display);
    _lora.init();
}

void loop() {
    esp_task_wdt_reset();

    Packet packet;
    auto state = _lora.receive(packet, 5000);

    if (state == RADIOLIB_ERR_NONE) {
        _display.clear();
        auto rssi = _lora.getRSSI();

        debugPrint(packet);

        Log::displayln("Packet received");
        Log::displayln("seq_number: %d", packet.seq_number);
        Log::displayln("flap_opened: %d", packet.was_flap_opened);
        Log::displayln("box_opened: %d", packet.was_box_opened);
        Log::displayln("distance: %d cm", packet.distance_cm);
        Log::displayln("voltage: %.2f V", packet.battery_voltage);
        Log::displayln("RSSI: %d dBm", rssi);

        Ack ack = {.seq_number = packet.seq_number, .success = true};
        _lora.send(ack);
        Log::displayln("ACK sent");
    }
}
