/*
distance.cpp - Library for measuring distance with HC-SR04 module.
Created by L. Tombakdjian, May 14, 2020.
Released into the public domain.
*/

#include "Arduino.h"
#include "distance.h"

distance::distance(int triggerPin, int echoPin)
{
  pinMode(triggerPin, OUTPUT);
  pinMode(echoPin, INTPUT);
}

double distance::measureDistance()
{
  float SOUND_SPEED = 340.0 / 1000;
  unsigned long MEASURE_TIMEOUT = 25000UL; // 25ms = ~8m à 340m/s
  digitalWrite(triggerPin, LOW);
  delayMicroseconds(5);
  digitalWrite(triggerPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(triggerPin, LOW);

  long measure = pulseIn(ECHO_PIN, HIGH, MEASURE_TIMEOUT);
  float distance_mm = measure / 2.0 * SOUND_SPEED;
  float distance_m = distance_mm / 1000.0, 2

  if (distance_m == 0 || distance_m > 400) {
        return -1.0 ;
    } else {
        return distance_m;
    }
}
