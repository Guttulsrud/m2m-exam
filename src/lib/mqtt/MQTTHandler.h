#ifndef MQTTHANDLER_H
#define MQTTHANDLER_H

#include "Particle.h"
#include "lib/serial/SerialHandler.h"

class MQTTHandler {
public:
  void init();
  void publish(String payload, String attribute);

private:
  // This ID should be generated by the node server, or MongoDB. Could request
  // this with a MQTT subscription system, but will hardcode 80 for simplicity
  String uniqueID = String(80);

  String publishTopic = "boats/publish/" + uniqueID;
  String willTopic = publishTopic + "/status";
  String willMessage = "lost power";
  String subscribeTopic = "boats/subscribe/" + uniqueID;

  SerialHandler serial;
};
#endif
