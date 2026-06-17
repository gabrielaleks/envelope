#pragma once

#include <string>

#define NTP_SERVER "pool.ntp.org"
#define TIMEZONE "UTC0"

class WifiManager {
public:
    WifiManager();
    int connect();
    void syncTime();
    std::string getTime();
    bool isConnected();

private:
};