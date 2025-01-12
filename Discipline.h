#include <Arduino.h>
#ifndef Discipline_h
#define Discipline_h

class Discipline {
  private:
    int* _intervals;
    int _intervalIndex;
    unsigned long _nextIntervalMillis;

  public:
    bool isActive;
    Discipline(int* intervals);
    void start();
    void stop();
    bool hasIntervalPassed();
    void beginNextInterval();
};

#endif