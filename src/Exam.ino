#include "lib/mqtt/MQTTHandler.h"
#include "lib/sim7600/SIM7600Handler.h"

// Handlers
SIM7600Handler SIM7600;

void setup() {

  Serial.println("Starting...");
  Particle.function("dashboardrequest", handleDashboardRequest);
  Serial.begin(19200);

  SIM7600.init();
}

void loop() {

  SIM7600.requestCoordinates();

  delay(10000);

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

int handleDashboardRequest(String command) { SIM7600.requestCoordinates(); }
