#ifndef SerialHandler_H
#define SerialHandler_H

#include "../serial/SerialHandler.h"
#include "Particle.h"

class SerialHandler {
public:
  void send(String command, int ms = 500);
  String response;
};

#endif
