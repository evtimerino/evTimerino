#include <Arduino.h>
#include <common.h>

#ifndef BUZZER_H
#define BUZZER_H

class Buzzer
{
private:
   Tone state = Tone::OFF;
public:
    Buzzer();
    ~Buzzer();
    
    void exposure();
    void metronome();
    void endExposure();
    void doubleBuzz();
    void tripleBuzz();
    
    void setState(Tone t);
    Tone getState();
};

#endif