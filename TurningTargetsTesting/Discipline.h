#include <Arduino.h>
#ifndef Discipline_h
#define Discipline_h

class Discipline {
  private:
    int* intervals;
    int activeIntervalIndex;
    unsigned long activeIntervalEnd;

  public:
    bool isActive;
    Discipline(int* intervals);
    void start();
    void stop();
    bool isActiveIntervalComplete();
    void beginNextInterval();
};

#endif