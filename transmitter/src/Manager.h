#pragma once

#include <Photoresistor.h>
#include <ReedSwitch.h>
#include <Ultrasonic.h>

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