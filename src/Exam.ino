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
  MQTT.publish("true", "system/publishing");
  MQTT.publish(SIM7600.getPosition(), "boat/position");
  MQTT.publish(String(boat.getFuelLevel()), "boat/fuel_level");
  MQTT.publish("false", "system/publishing");
}

void loop() {

  if (Particle.connected()) {
    Alarm.delay(1); // Alarms need this to work lmao

    handleSubscription();

    /*
        if (Serial.available() > 0) {
          Serial.print(">");
          delay(100);
          while (Serial.available()) {
            char ch = Serial.read();
            Serial.print(ch);
            Serial1.print(ch);
          }
        }


            if (Serial1.available() > 0) {
              Serial.print(":");
              delay(10);
              while (Serial1.available()) {
                char ch = Serial1.read();
                if (ch) {
                  Serial.print(ch);
                }
              }
            } */
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

// Oil level? Er det nok olje?
// Er det strøm på batteriet?
//

void setUpTimers() { Alarm.timerRepeat(120, publishTelemetry); }
