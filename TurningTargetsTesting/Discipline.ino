#include "Discipline.h"

Discipline::Discipline(int* _intervals) {
  isActive = false;
  intervals = _intervals;
}

void Discipline::start() {
  isActive = true;
  activeIntervalIndex = 0;
  activeIntervalEnd = millis() + intervals[activeIntervalIndex] * 1000;
}

void Discipline::stop() {
  isActive = false;
}

bool Discipline::isActiveIntervalComplete() {
  return isActive && millis() >= activeIntervalEnd;
}

void Discipline::beginNextInterval() {
  int nextInterval = intervals[activeIntervalIndex + 1];

  // the last interval in a discipline's sequence is -1
  if (nextInterval == -1) {
    stop();
  } else {
    activeIntervalEnd = millis() + nextInterval * 1000;
    activeIntervalIndex++;
  }
}