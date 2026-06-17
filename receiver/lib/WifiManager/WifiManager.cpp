#include "WifiManager.h"

#include <WiFi.h>

#include "Log.h"
#include "secrets.h"

WifiManager::WifiManager() {
}

int WifiManager::connect() {
    Log::serialln("Connecting to WiFi...");

    WiFi.mode(WIFI_STA);
    WiFi.setAutoReconnect(true);
    WiFi.begin(
        WIFI_SSID,
        WIFI_PASSWORD);

    while (WiFi.status() != WL_CONNECTED) {
        Log::serial(".");
        delay(500);
    }

    Log::serialln("\nConnected to WiFi!");
    Log::serialln("ESP32 IP: %s", WiFi.localIP().toString().c_str());

    return 0;
}

void WifiManager::syncTime() {
    configTzTime(TIMEZONE, NTP_SERVER);
    Log::serialln("%s", getTime().c_str());
    Log::displayln("%s", getTime().c_str());
}

std::string WifiManager::getTime() {
    struct tm timeinfo;

    if (!getLocalTime(&timeinfo, 2000)) {
        Log::serialln("Failed to obtain time");
        return "";
    }

    char formattedTime[80];
    strftime(formattedTime, sizeof(formattedTime), "%Y-%m-%dT%H:%M:%SZ", &timeinfo);
    return formattedTime;
}

bool WifiManager::isConnected() {
    return WiFi.isConnected();
}