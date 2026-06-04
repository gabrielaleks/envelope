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
    int state = _lora.receive(packet, 5000);

    if (state == RADIOLIB_ERR_NONE) {
        _display.clear();
        Serial.println("Packet received:");
        debugPrint(packet);
        _display.println("Packet received");

        Ack ack = {.seq_number = packet.seq_number, .success = true};
        _lora.send(ack);
        _display.println("ACK sent");
    }
}
