#include "Discipline.h"

Discipline::Discipline(int* _intervals) {
  isActive = false;
  intervals = _intervals;
}

void Discipline::start() {
  isActive = true;
  intervalIndex = 0;
  nextIntervalMillis = millis() + intervals[0] * 1000;
}

void Discipline::stop() {
  isActive = false;
}

bool Discipline::hasIntervalPassed() {
  return isActive && millis() >= nextIntervalMillis;
}

void Discipline::beginNextInterval() {
  int nextInterval = intervals[intervalIndex + 1];

  // the last interval in a discipline's sequence is -1
  if (nextInterval == -1) {
    Discipline::stop();
  } else {
    nextIntervalMillis = millis() + nextInterval * 1000;
    intervalIndex++;
  }
}