#include <common.h>
#include <display.h>
#include <buzzer.h>
#include <exposure.h>

#ifndef ENLARGER_H
#define ENLARGER_H

class Enlarger
{
private:
    Display& display;
    Buzzer& buzzer;
    Exposure& exposure;

    Lamp state = Lamp::OFF;

    Safelight safelight = Safelight::OFF;

    unsigned long int currentMillis;
    unsigned long int previousMillis;

    bool prepare = false;

    uint16_t timeCounter;
    uint16_t metronomeTimeCounter = 0;
public:
    Enlarger(Display& d, Buzzer& b, Exposure& e);
    ~Enlarger();
    
    void startExposure();
    void run();
    
    Lamp getStatePrepare();
    Lamp getState();
    void switchOn();
    void switchOff();
    
    void runMetronome();
    void stopMetronome();
    uint16_t getMetronomeTimeCounter();
    
    void setSafelight(Safelight sl);
    Safelight getSafelight();
    
    void setPrepare(bool p);
    bool getPrepare();
};


#endif