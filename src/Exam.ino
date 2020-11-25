#include "TimeAlarms/TimeAlarms.h"
#include "lib/boat/BoatHandler.h"
#include "lib/mqtt/MQTTHandler.h"
#include "lib/sim7600/SIM7600Handler.h"

// Handlers
SIM7600Handler SIM7600;
MQTTHandler MQTT;
BoatHandler boat;

String securityToken = "abcd";
String mqttPayload;
bool mqttPayloadRecieved = false;

void setup() {
  Serial.begin(19200);
  setUpTimers();
  boat.init();
  SIM7600.init();
}

void publishTelemetry() {
  MQTT.publish(SIM7600.getPosition(), "boat/position");
  MQTT.publish(String(boat.getFuelLevel()), "boat/fuel_level");
}

void loop() {

  if (Particle.connected()) {
    Alarm.delay(1); // Alarms need this to work lmao

    handleSubscription();
  }
}

void handleSubscription() {
  if (mqttPayloadRecieved) {
    if (mqttPayload == "unlock") {
      boat.unlockBoat();
      MQTT.publish("unlocked", "boat/status");
    }

    if (mqttPayload == "lock") {
      boat.lockBoat();
      MQTT.publish("locked", "boat/status");
    }
    mqttPayloadRecieved = false;
  }

  String a;
  // Receiving subscribe response
  while (Serial1.available() > 0) {
    delay(10);
    a = Serial1.readString();
    if (a.indexOf("PAYLOAD") != -1) {
      int new1 = a.indexOf("PAYLOAD");
      String neww = a.substring(new1);
      int new2 = neww.indexOf('\n');
      String new3 = neww.substring(new2 + 1);
      int new4 = new3.indexOf('\n');
      String new5 = new3.substring(0, new4);
      mqttPayload = new5.substring(0, new5.length() - 1);
      mqttPayloadRecieved = true;
    }
  }
}

void setUpTimers() { Alarm.timerRepeat(120, publishTelemetry); }
