#include "BoatHandler.h"

void BoatHandler::init() {
  pinMode(D2, OUTPUT);
  pinMode(D1, INPUT);
}

void BoatHandler::unlockBoat() { digitalWrite(D2, HIGH); }

void BoatHandler::lockBoat() { digitalWrite(D2, LOW); }

int BoatHandler::getFuelLevel() {

  // Generate random fuel level percent 40-90%
  return random(60, 98);
}

boolean BoatHandler::isFuelTankFull() { return !digitalRead(D1); }
