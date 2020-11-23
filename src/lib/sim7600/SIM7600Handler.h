#ifndef SIM7600Handler_H
#define SIM7600Handler_H

#include "../mqtt/MQTTHandler.h"
#include "../serial/SerialHandler.h"

#include "Particle.h"

class SIM7600Handler {
public:
  void init();
  String getPosition();

private:
  long parse_degrees(char *input);
  String position;
  bool aquiredPosition = false;
  MQTTHandler MQTT;
  SerialHandler serial;
  bool boatOpen = false;
};

#endif
