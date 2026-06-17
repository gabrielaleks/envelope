#pragma once

#include <PubSubClient.h>
#include <WiFiClient.h>

#include <queue>
#include <string>

#include "packet.h"

#define MQTT_EVENT_TOPIC "envelope/event"
#define MQTT_CLIENT_ID "envelope-receiver"

class MQTTClient {
public:
    MQTTClient();
    int connect();
    void publish(const Packet& packet, int rssi, const std::string& timestamp);
    void loop();

private:
    void reconnect();
    std::string serialize(const Packet& packet, int rssi, const std::string& timestamp);

    std::queue<std::string> _queue;
    PubSubClient _client;
    WiFiClient _wifiClient;
};
