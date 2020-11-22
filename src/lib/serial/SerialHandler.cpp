#include "SerialHandler.h"

void SerialHandler::send(String command, int ms) {
  // Send the command
  Serial1.println(command);
  response = "";

  int requestStartTime = millis();
  int requestStarted = 0;
  bool wegotResponse = false;

  // Wait until we get a response (or timeout)
  while (!wegotResponse || requestStarted < 2000) {
    requestStarted = millis() - requestStartTime;
    if (Serial1.available() > 0) {
      wegotResponse = true;
    }
  }

  // Print out the results
  if (requestStarted < 2000) {
    Serial.print("\nCommand: '");
    Serial.print(command);
    Serial.println(" timed out?\n");
  } else if (Serial1.available() > 0) {
    Serial.print("<\n");

    // wait a little longer so we get the whole response
    delay(ms);

    while (Serial1.available()) {
      char ch = Serial1.read();
      if (ch) {
        Serial.print(ch);

        response += ch;
      }
    }
    Serial.println("\n>");
  }
}
