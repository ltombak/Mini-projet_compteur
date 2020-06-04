/*
  distance.h - Library for measuring distance with HC-SR04 module.
  Created by L. Tombakdjian, May 14, 2020.
  Released into the public domain.
*/
#ifndef distance_h
#define distance_h

#include "Arduino.h"

class distance
{
public:
  distance(int triggerPin, int echoPin);
  double measureDistance();
private:
  int _triggerPin, _echoPin;
}

#endif
