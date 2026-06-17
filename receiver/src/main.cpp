#include <Arduino.h>
#include <MQTTClient.h>
#include <WifiManager.h>

#include "Display.h"
#include "LoRaRadio.h"
#include "esp_task_wdt.h"
#include "packet.h"

LoRaRadio _lora;
Display _display;
WifiManager _wifiManager;
MQTTClient _mqttClient;

void setup() {
    esp_task_wdt_init(30, true);  // reset if hung for 30s
    esp_task_wdt_add(NULL);

    Serial.begin(115200);
    delay(2000);

    Log::init(LOG_ALL);
    _display.init();
    Log::setDisplay(_display);

    _lora.init();

    _wifiManager.connect();
    _wifiManager.syncTime();

    _mqttClient.connect();
}

void loop() {
    esp_task_wdt_reset();

    _mqttClient.loop();

    Packet packet;
    auto state = _lora.receive(packet, 5000);

    if (state == RADIOLIB_ERR_NONE && packet.magic == PACKET_MAGIC) {
        _display.clear();
        auto rssi = _lora.getRSSI();

        debugPrint(packet);

        Log::displayln("Packet received");
        Log::displayln("seqNumber: %d", packet.seqNumber);
        Log::displayln("flap_opened: %d", packet.wasFlapOpened);
        Log::displayln("box_opened: %d", packet.wasBoxOpened);
        Log::displayln("distance: %d cm", packet.distanceCm);
        Log::displayln("voltage: %.2f V", packet.batteryVoltage);

        Ack ack = {.seqNumber = packet.seqNumber, .success = true};
        _lora.send(ack);
        Log::displayln("ACK sent");

        _mqttClient.publish(packet, rssi, _wifiManager.getTime());
    }
}
