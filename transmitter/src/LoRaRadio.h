#pragma once

#include <RadioLib.h>

#include "lora_config.h"
#include "packet.h"

class LoRaRadio {
  public:
    LoRaRadio();
    int init();
    int send(Packet packet);

  private:
    SX1276 radio;
};