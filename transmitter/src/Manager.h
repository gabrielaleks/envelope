#pragma once

#include <sensors/Photoresistor.h>
#include <sensors/ReedSwitch.h>
#include <sensors/Ultrasonic.h>

#include "LoRaRadio.h"
#include "packet.h"

class Manager {
public:
    Manager();
    void run();

private:
    LoRaRadio _lora;
    Photoresistor _photoresistor;
    ReedSwitch _flapSwitch;
    ReedSwitch _boxSwitch;
    Ultrasonic _ultrasonic;
};