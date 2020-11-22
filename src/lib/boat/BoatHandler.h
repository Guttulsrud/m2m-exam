#ifndef BOATHANDLER_H
#define BOATHANDLER_H

#include "../mqtt/MQTTHandler.h"
#include "Particle.h"

class BoatHandler {
public:
  void init();
  void lockBoat();
  void unlockBoat();
  int getFuelLevel();
  boolean isFuelTankFull();

private:
};
#endif
