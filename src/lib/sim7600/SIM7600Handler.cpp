#include "SIM7600Handler.h"

void SIM7600Handler::init() {
  Serial1.begin(115200);
  delay(8000);

  Serial1.println("AT+IPR=19200");
  delay(1000);
  Serial1.begin(19200);
  delay(1000);

  serial.send("AT");
  serial.send("AT+CPIN=7043");
  serial.send("AT+CFUN=1");
  serial.send("AT+CGACT=1,1");
  serial.send("AT+CGDCONT=1,\"IP\",\"ice.net\"");
  serial.send("AT+CGPS=1");
  serial.send("AT+CGREG?");
  serial.send("AT+NETOPEN");
  serial.send("AT+IPADDR");
  MQTT.init();
}

String SIM7600Handler::getPosition() {
  serial.send("AT+CGPSINFO");

  char latitude[40];
  char longitude[40];

  String response = serial.response;

  if (response.substring(40, 51).length() > 2) {
    response.substring(25, 36).toCharArray(latitude, 40);
    response.substring(40, 51).toCharArray(longitude, 40);
  }

  String lat = String(parse_degrees(latitude));
  String lng = String(parse_degrees(longitude));

  if (lat.length() > 6 && lng.length() > 6) {
    MQTT.publish("false", "boat/position/out_of_range");

    aquiredPosition = true;
    position = lat + ", " + lng;
    return position;

  } else {
    MQTT.publish("true", "boat/position/out_of_range");

    if (aquiredPosition) {
      return position;
    }

    return "59923159, 10753234";
  }
}

long SIM7600Handler::parse_degrees(char *input) {
  char *minutes;
  char deg[] = {0, 0, 0,
                0}; // up to 3 digits character  string for integer degrees
  double x;
  long d;

  if (strlen(input) == 0)
    return 0L; // invalid input string

  if ((minutes = strchr(input, '.')) == NULL)
    return 0L; //'.' not found, invalid

  minutes -=
      2; // back up pointer to include two digits of minutes+decimal fraction
  x = strtod(minutes, NULL); // make into double
  memcpy(deg, input,
         minutes - input); // copy degrees, works OK with zero length string
  d = strtol(deg, NULL, 10) * 1000000UL; // degrees to long int
  d += (long)(x * 16666.667 + 0.5);      //*1.e6/60.  add in minutes

  return d;
}
