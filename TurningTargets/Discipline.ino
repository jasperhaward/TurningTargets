#include "Discipline.h"

const int DISCIPLINE_TERMINATOR = -1;

Discipline::Discipline(int* intervals): intervals(intervals) {
  isActive = false;
}

void Discipline::start() {
  isActive = true;
  activeIntervalIndex = 0;
  activeIntervalEnd = millis() + intervals[activeIntervalIndex] * 1000UL;
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
  if (nextInterval == DISCIPLINE_TERMINATOR) {
    stop();
  } else {
    activeIntervalEnd = millis() + nextInterval * 1000;
    activeIntervalIndex++;
  }
}