#include "MQTTHandler.h"

int uniqueID = 71;

String publishTopic = "boats/" + String(uniqueID); // Publish Topic
String Subscribe = "boats";                        // Subscribe Topic

String latitude;
String longitude;

void MQTTHandler::init() {
  // add error checking, what if not connected?
  // maybe return error

  // AT Commands for setting up the client id and Server
  // Need to be executed once -- Open serial terminal doe seeing the debug
  // messages
  Serial.println("Connecting To Server........");
  delay(2000);
  Serial1.println("AT+CMQTTSTART");
  // Establishing MQTT Connection
  delay(2000);
  Serial1.println("AT+CMQTTACCQ=0,\"01\"");
  // Client ID - change this for each client as this need to be unique
  delay(2000);
  Serial1.println("AT+CMQTTCONNECT=0,\"tcp://test.mosquitto.org:1883\",90,1");
  // MQTT Server Name for connecting this client
  delay(2000);

  // SUBSCRIBE MESSAGE
  // Need to be executed once
  Serial1.println("AT+CMQTTSUBTOPIC=0,32,1"); // AT Command for Setting up the
  // Subscribe Topic Name
  Serial1.print(char(26));

  delay(2000);
  Serial1.println(Subscribe); // Topic Name
  delay(2000);
  Serial1.println("AT+CMQTTSUB=0,4,32,1"); // Length of message
  Serial1.print(char(26));
  delay(20);
  Serial1.print(char(26));
}

void MQTTHandler::sendMQTT(String payload, String attribute) {

  String topic = publishTopic + "/" + attribute;

  String payloadLength = String(payload.length());
  String topicLength = String(topic.length());

  Serial1.println("AT+CMQTTTOPIC=0," + topicLength);
  Serial1.print(char(26));

  // AT Command for Setting up the Publish Topic Name
  delay(2000);
  Serial1.println(topic); // Topic Name
  delay(2000);
  Serial1.println("AT+CMQTTPAYLOAD=0," + payloadLength); // Payload length
  Serial1.print(char(26));
  delay(2000);

  Serial1.println(payload); // Payload message
  delay(2000);
  Serial1.println("AT+CMQTTPUB=0,1,60"); // Acknowledgment
  delay(2000);
}