#include "Discipline.h"

Discipline::Discipline(int* intervals) {
  isActive = false;
  _intervals = intervals;
}

void Discipline::start() {
  isActive = true;
  _intervalIndex = 0;
  _nextIntervalMillis = millis() + _intervals[0] * 1000;
}

void Discipline::stop() {
  isActive = false;
}

bool Discipline::hasIntervalPassed() {
  return isActive && millis() >= _nextIntervalMillis;
}

void Discipline::beginNextInterval() {
  int nextInterval = _intervals[_intervalIndex + 1];

  if (nextInterval == 0) {
    Discipline::stop();
  } else {
    _nextIntervalMillis = millis() + nextInterval * 1000;
    _intervalIndex++;
  }
}