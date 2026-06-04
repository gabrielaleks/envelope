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

    _lora.init();
    _display.init();
}

void loop() {
    esp_task_wdt_reset();

    Packet packet;
    auto state = _lora.receive(packet, 5000);

    if (state == RADIOLIB_ERR_NONE) {
        _display.clear();
        auto rssi = _lora.getRSSI();

        debugPrint(packet);
        Serial.printf("RSSI: %d\n", rssi);

        _display.println("Packet received");
        _display.println("seq_number: %d", packet.seq_number);
        _display.println("flap_opened: %d", packet.was_flap_opened);
        _display.println("box_opened: %d", packet.was_box_opened);
        _display.println("voltage: %.2f", packet.battery_voltage);
        _display.println("RSSI: %d dBm", rssi);
        Serial.println();

        Ack ack = {.seq_number = packet.seq_number, .success = true};
        _lora.send(ack);
        _display.println("");
        _display.println("ACK sent");
    }
}
