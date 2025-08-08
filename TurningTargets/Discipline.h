#include <Arduino.h>
#ifndef Discipline_h
#define Discipline_h

class Discipline {
  private:
    int* intervals;
    int intervalIndex;
    unsigned long nextIntervalMillis;

  public:
    bool isActive;
    Discipline(int* intervals);
    void start();
    void stop();
    bool hasIntervalPassed();
    void beginNextInterval();
};

#endif