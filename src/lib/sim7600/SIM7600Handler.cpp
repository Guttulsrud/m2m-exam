#include "SIM7600Handler.h"

void SIM7600Handler::requestCoordinates() {
  sendAndReadResponse("AT+CGPSINFO");

  if (response.substring(40, 51).length() > 2) {

    char latitude[40];
    char longitude[40];

    response.substring(25, 36).toCharArray(latitude, 40);
    response.substring(40, 51).toCharArray(longitude, 40);

    // Send new
    MQTT.sendMQTT(String(parse_degrees(latitude)), "latitude");
    MQTT.sendMQTT(String(parse_degrees(longitude)), "longitude");
  } else {
    activePositionSignal = false;
    MQTT.sendMQTT("59.903900", "latitude");
    MQTT.sendMQTT("10.737192", "longitude");
  }
}

void SIM7600Handler::init() {
  Serial1.begin(115200);
  delay(8000);

  Serial1.println("AT+IPR=19200");
  delay(1000);
  Serial1.begin(19200);
  delay(1000);

  sendAndReadResponse("AT");
  sendAndReadResponse("AT+CPIN=7043");
  sendAndReadResponse("AT+CFUN=1");
  sendAndReadResponse("AT+CGACT=1,1");
  sendAndReadResponse("AT+CGDCONT=1,\"IP\",\"ice.net\"");
  sendAndReadResponse("AT+CGPS=1");
  sendAndReadResponse("AT+CGREG?");
  sendAndReadResponse("AT+NETOPEN");
  sendAndReadResponse("AT+IPADDR");
  Serial1.print(char(26));
  MQTT.init();
}

void SIM7600Handler::sendAndReadResponse(
    String command, int extraWaitInMillisecondsForResponse) {
  // Send the command
  Serial1.println(command);

  response = "";

  // Setup a timeout
  int requestStartTime = millis();
  int millisecondsSinceRequestStarted = 0;
  bool wegotResponse = false;

  // Wait until we get a response (or timeout)
  while (!wegotResponse || millisecondsSinceRequestStarted < 2000) {
    millisecondsSinceRequestStarted = millis() - requestStartTime;
    if (Serial1.available() > 0) {
      wegotResponse = true;
    }
  }

  // Print out the results
  if (millisecondsSinceRequestStarted < 2000) {
    Serial.print("\nCommand: '");
    Serial.print(command);
    Serial.println(" timed out?\n");
  } else if (Serial1.available() > 0) {
    Serial.print("<\n");

    // wait a little longer so we get the whole response
    delay(extraWaitInMillisecondsForResponse);

    // Print out the response to Serial monitor
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

long SIM7600Handler::parse_degrees(char *p) {
  char *minutes;
  char deg[] = {0, 0, 0,
                0}; // up to 3 digits character  string for integer degrees
  double x;
  long d;

  if (strlen(p) == 0)
    return 0L; // invalid input string

  if ((minutes = strchr(p, '.')) == NULL)
    return 0L; //'.' not found, invalid

  minutes -=
      2; // back up pointer to include two digits of minutes+decimal fraction
  x = strtod(minutes, NULL);   // make into double
  memcpy(deg, p, minutes - p); // copy degrees, works OK with zero length string
  d = strtol(deg, NULL, 10) * 1000000UL; // degrees to long int
  d += (long)(x * 16666.667 + 0.5);      //*1.e6/60.  add in minutes

  return d;
}
