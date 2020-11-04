#ifndef MQTTHANDLER_H
#define MQTTHANDLER_H

#include "Particle.h"

class MQTTHandler {
public:
  void init();
  void sendMQTT(String payload, String attribute);
};
#endif
