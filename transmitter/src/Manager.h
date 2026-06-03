#pragma once

#include <sensors/Photoresistor.h>
#include <sensors/ReedSwitch.h>
#include <sensors/Ultrasonic.h>

#include "Display.h"
#include "LoRaRadio.h"
#include "packet.h"

class Manager {
public:
    Manager();
    void run();

private:
    LoRaRadio _lora;
    Display _display;
    Photoresistor _photoresistor;
    ReedSwitch _flapSwitch;
    ReedSwitch _boxSwitch;
    Ultrasonic _ultrasonic;
};