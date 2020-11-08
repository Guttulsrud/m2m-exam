#include "TimeAlarms/TimeAlarms.h"
#include "lib/mqtt/MQTTHandler.h"
#include "lib/sim7600/SIM7600Handler.h"

// Handlers
SIM7600Handler SIM7600;
MQTTHandler MQTT;

int fuelLevelPercent;
bool poweredOn = false;
void setup() {

  pinMode(D2, OUTPUT);
  pinMode(D1, INPUT);
  Serial.println("Starting...");
  Particle.function("dashboardrequest", handleDashboardRequest);
  Serial.begin(19200);
  setUpTimers();

  SIM7600.init();
}

void loop() {

  if (Particle.connected()) {
    SIM7600.requestCoordinates();
    delay(20000);
    Alarm.delay(1);
  }

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
  }
}

void setUpTimers() { Alarm.timerRepeat(400, measureFuelLevel); }

int handleDashboardRequest(String command) {

  if (command == "1") {
    powerOnEngine();
  }
  if (command == "2") {
    powerOffEngine();
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

void powerOnEngine() {
  poweredOn = true;
  digitalWrite(D2, HIGH);
}

void powerOffEngine() {
  poweredOn = false;
  digitalWrite(D2, LOW);
}
