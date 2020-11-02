#include "MQTT.h"

String deviceName = "guiltless-pain";
bool setupMode = false;

// MQTT client("excellent-hairdresser.cloudmqtt.com", 1883, callback);
MQTT client("test.mosquitto.org", 1883, callback);
void callback(char *topic, byte *payload, unsigned int length) {}

// Delcare metod with defaults here. Implementation is firhter down
void sendAndReadResponse(String command,
                         int extraWaitInMillisecondsForResponse = 200);
void setup() {

  client.connect("HÅÅÅÅÅK");

  Particle.function("dashboardrequest", handleDashboardRequest);

  Serial.begin(115200);
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
  delay(20);
  Serial1.print(char(26));
}

void initS7600() {}

char response[200];

void sendAndReadResponse(String command,
                         int extraWaitInMillisecondsForResponse) {
  // Send the command
  Serial1.println(command);

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

    int i = 0;
    // wait a little longer so we get the whole response
    delay(extraWaitInMillisecondsForResponse);

    // Print out the response to Serial monitor
    while (Serial1.available()) {
      char ch = Serial1.read();
      if (ch) {
        response[i] = ch;
        Serial.print(ch);
        i++;
      }
    }
    Serial.println("\n>");
  }
}

void sendHttpRequest() {

  sendAndReadResponse("AT+CHTTPACT=\"dweet.io\",80");
  delay(200);
  Serial1.print("GET http://dweet.io/dweet/for/");
  Serial1.print(deviceName);
  Serial1.println("?gaybig=false HTTP/1.1\n");
  Serial1.println("Host: dweet.io\n");
  Serial1.println("User-Agent: mozilly\n");
  Serial1.println("Content-Length: 0\n");
  Serial1.print(char(26));
  delay(20);
  Serial1.print(char(26));
}

bool getGPS = false;
char buf[300];
char lat[300];
char lng[300];

void getCoordinates() {
  sendAndReadResponse("AT+CGPSINFO");

  int i = 25;
  int j = 0;

  for (i = 25; i < 36; i++, j++) {
    lat[j] = response[i];
  }

  j = 0;
  for (i = 40; i < 50; i++, j++) {
    lng[j] = response[i];
  }
}

void requestGPS() {
  Serial1.println("AT+CGPSINFO");

  int requestStartTime = millis();
  int millisecondsSinceRequestStarted = 0;
  bool wegotResponse = false;

  while (!wegotResponse || millisecondsSinceRequestStarted < 2000) {
    millisecondsSinceRequestStarted = millis() - requestStartTime;
    if (Serial1.available() > 0) {
      wegotResponse = true;
    }
  }

  // Print out the results
  if (millisecondsSinceRequestStarted < 2000) {
    Serial.print("\nCommand: '");
    Serial.print("AT+CGPSINFO");
    Serial.println(" timed out?\n");
  } else if (Serial1.available() > 0) {
    Serial.print("<\n");

    // wait a little longer so we get the whole response
    delay(200);

    // Print out the response to Serial monitor
    int i = 0;

    while (Serial1.available()) {
      char ch = Serial1.read();
      if (ch) {
        buf[i] = ch;
        Serial.print(ch);
        i++;
      }
    }
    Serial.println("\n>");
  }
}

// In the main loop, you can keep entering AT commands from your Serial Monitor
void loop() {

  // Publish our message to the test server
  client.publish("BBBBBBBBBBBBBBB", "B");
  delay(5000);

  // CALL THIS at the end of your loop
  client.loop();

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

int handleDashboardRequest(String command) {

  if (command == "1") {
    getCoordinates();
  }

  if (command == "3") {
    sendHttpRequest();
  }

  if (command == "2") {
    Serial.print("Latitude: ");
    Serial.print(parse_degrees(lat));
    Serial.println(" ");
    Serial.print("Longitude: ");
    Serial.print(parse_degrees(lng));
  }
}

long parse_degrees(char *p) {
  char *minutes;
  char deg[] = {0, 0, 0, 0};
  // up to 3 digits character  string for integer degrees

  double x;
  long d;

  if (strlen(p) == 0)
    return 0L; // invalid input string

  if ((minutes = strchr(p, '.')) == NULL)
    return 0L; //'.' not found, invalid

  minutes -=
      2; // back up pointer to include two digits of minutes+decimal fraction
  x = strtod(minutes, NULL); // make into double
  memcpy(deg, p,
         minutes - p); // copy degrees, works OK with zero length string
  d = strtol(deg, NULL, 10) * 1000000UL; // degrees to long int
  d += (long)(x * 16666.667 + 0.5);      //*1.e6/60.  add in minutes

  return d;
}
