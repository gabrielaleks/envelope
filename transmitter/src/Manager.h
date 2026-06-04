#pragma once

#include <Photoresistor.h>
#include <ReedSwitch.h>
#include <Ultrasonic.h>

#include "Display.h"
#include "LoRaRadio.h"
#include "packet.h"

struct EventResult {
    bool wasFlapOpened;
    bool wasBoxOpened;
};

class Manager {
public:
    Manager();
    void run();
    static EventResult classifyEvent(
        uint16_t light,
        uint16_t distance,
        bool isFlapMagnetOn,
        bool isBoxMagnetOn);

private:
    LoRaRadio _lora;
    Display _display;
    Photoresistor _photoresistor;
    ReedSwitch _flapSwitch;
    ReedSwitch _boxSwitch;
    Ultrasonic _ultrasonic;
};