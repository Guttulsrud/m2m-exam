#ifndef SIM7600Handler_H
#define SIM7600Handler_H

#include "../mqtt/MQTTHandler.h"
#include "Particle.h"

class SIM7600Handler {
public:
  void init();
  String getModuleResponse();
  void sendAndReadResponse(String command,
                           int extraWaitInMillisecondsForResponse = 200);
  void requestCoordinates();

private:
  long parse_degrees(char *p);

  String response;
  bool activePositionSignal;
  MQTTHandler MQTT;
};

#endif
