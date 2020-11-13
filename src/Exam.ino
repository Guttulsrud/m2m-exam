#include "TimeAlarms/TimeAlarms.h"
#include "lib/mqtt/MQTTHandler.h"
#include "lib/sim7600/SIM7600Handler.h"

// Handlers
SIM7600Handler SIM7600;
MQTTHandler MQTT;

int fuelLevelPercent;
bool locked = true;

String securityToken = "abcd";

void setup() {

  pinMode(D2, OUTPUT);
  pinMode(D1, INPUT);
  Serial.println("Starting...");
  Particle.function("dashboardrequest", handleDashboardRequest);
  Serial.begin(19200);
  setUpTimers();

  SIM7600.init();
}
String res = "";

String mqttPayload = "";
bool mqttPayloadRecieved = false;

void gps() { SIM7600.requestCoordinates(); }

void loop() {

  if (Particle.connected()) {
    Alarm.delay(1); // Alarms need this to work lmao

    handleMTTPayload();

    if (Serial.available() > 0) {
      Serial.print(">");
      delay(100);
      while (Serial.available()) {
        char ch = Serial.read();
        Serial.print(ch);
        Serial1.print(ch);
      }
    }
    /*
        if (Serial1.available() > 0) {
          Serial.print(":");
          delay(10);
          while (Serial1.available()) {
            char ch = Serial1.read();
            if (ch) {
            }
          }
        }*/
    readSerialString();
  }
}

void readSerialString() {
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

void handleMTTPayload() {
  if (mqttPayloadRecieved) {

    if (mqttPayload == "unlock") {
      unlockBoat();
    }

    if (mqttPayload == "lock") {
      lockBoat();
    }

    mqttPayloadRecieved = false;
  }
}

void setUpTimers() {
  Alarm.timerRepeat(300, measureFuelLevel);
  Alarm.timerRepeat(260, gps);
}

int handleDashboardRequest(String command) {

  if (command == "1") {
    MQTT.sendMQTT("HELLO", "test");
    // powerOnEngine();
  }
}

void measureFuelLevel() {

  // Generate random fuel level percent 40-90%
  fuelLevelPercent = random(40, 90);

  if (!digitalRead(D1)) {
    // Tank is full!

  } else {
    // Tank is not full
  }

  MQTT.sendMQTT(String(fuelLevelPercent), "fuel_level");
}

void unlockBoat() {
  locked = false;
  digitalWrite(D2, HIGH);
}

void lockBoat() {
  locked = true;
  digitalWrite(D2, LOW);
}
