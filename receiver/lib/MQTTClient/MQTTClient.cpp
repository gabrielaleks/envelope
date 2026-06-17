#include "MQTTClient.h"

#include <ArduinoJson.h>
#include <WiFi.h>

#include "Log.h"
#include "secrets.h"

MQTTClient::MQTTClient() : _client(_wifiClient) {
}

int MQTTClient::connect() {
    Log::serialln("Connecting to the MQTT broker...");

    _client.setServer(MQTT_BROKER, MQTT_PORT);

    if (_client.connect(MQTT_CLIENT_ID, MQTT_USERNAME, MQTT_PASSWORD)) {
        Log::serialln("Connected to the MQTT broker!\n");
        return 0;
    } else {
        Log::serialln("Failed to connect to the MQTT broker: %d\n", _client.state());
        return -1;
    }
}

void MQTTClient::loop() {
    if (WiFi.status() == WL_CONNECTED) {
        if (!_client.connected()) {
            reconnect();
        }

        _client.loop();

        while (!_queue.empty() && _client.connected()) {
            _client.publish(MQTT_EVENT_TOPIC, _queue.front().c_str());
            _queue.pop();
        }
    }
}

void MQTTClient::reconnect() {
    if (_client.connect(MQTT_CLIENT_ID, MQTT_USERNAME, MQTT_PASSWORD)) {
        Log::serialln("Reconnected to the MQTT broker!");
    } else {
        Log::serialln("MQTT reconnect failed, state: %d", _client.state());
    }
}

void MQTTClient::publish(const Packet& packet, int rssi, const std::string& timestamp) {
    auto payload = serialize(packet, rssi, timestamp);

    if (WiFi.status() == WL_CONNECTED && _client.connected()) {
        _client.publish(MQTT_EVENT_TOPIC, payload.c_str());
    } else {
        _queue.push(payload);
    }
}

std::string MQTTClient::serialize(const Packet& packet, int rssi, const std::string& timestamp) {
    JsonDocument doc;
    doc["seqNumber"] = packet.seqNumber;
    doc["wasFlapOpened"] = packet.wasFlapOpened;
    doc["wasBoxOpened"] = packet.wasBoxOpened;
    doc["lightLevel"] = packet.lightLevel;
    doc["distanceCm"] = packet.distanceCm;
    doc["flapMagnetPresent"] = packet.flapMagnetPresent;
    doc["boxMagnetPresent"] = packet.boxMagnetPresent;
    doc["batteryVoltage"] = packet.batteryVoltage;
    doc["rssi"] = rssi;
    doc["timestamp"] = timestamp;

    std::string output;
    serializeJson(doc, output);
    return output;
}
