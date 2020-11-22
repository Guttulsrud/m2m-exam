#include "MQTTHandler.h"

void MQTTHandler::init() {
  // add error checking, what if not connected?
  // maybe return error

  // AT Commands for setting up the client id and Server
  serial.send("AT+CMQTTSTART");

  // Establishing MQTT Connection
  serial.send("AT+CMQTTACCQ=0,\"" + uniqueID + "\"");

  serial.send("AT+CMQTTWILLTOPIC=0," + String(willTopic.length()));

  serial.send(willTopic);
  serial.send("AT+CMQTTWILLMSG=0," + String(willMessage.length()) + ",1");

  serial.send(willMessage);

  serial.send("AT+CMQTTCONNECT=0,\"tcp://mqtt.eclipse.org:1883\",90,1");
  // MQTT Server Name for connecting this client

  // Subscribe
  serial.send("AT+CMQTTSUBTOPIC=0," + String(subscribeTopic.length()) + ",1");
  serial.send(subscribeTopic);        // Topic Name
  serial.send("AT+CMQTTSUB=0,4,1,1"); // Length of message
  serial.send("HAII");
}

void MQTTHandler::publish(String payload, String attribute) {

  String topic = publishTopic + "/" + attribute;

  String payloadLength = String(payload.length());
  String topicLength = String(topic.length());

  serial.send("AT+CMQTTTOPIC=0," +
              topicLength); // Command for setting up the publish topic

  serial.send(topic);
  serial.send("AT+CMQTTPAYLOAD=0," + payloadLength); // Payload length

  serial.send(payload);              // Payload message
  serial.send("AT+CMQTTPUB=0,1,60"); // Acknowledgment
}
