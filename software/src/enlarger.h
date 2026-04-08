#include <common.h>
#include <display.h>
#include <buzzer.h>
#include <exposure.h>

#ifndef ENLARGER_H
#define ENLARGER_H

const Relay relay = Relay::ACTIVE_LOW;

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
    bool lampUsage = false;

    uint16_t timeCounter;
    uint16_t metronomeTimeCounter = 0;
    uint16_t lampUsageBaseTime = 0;
    uint16_t focusLampUsageCounter = 0;

    void updateRelay();
    void updateSafeLight();
    void setLampUsageBaseTime(uint16_t c);

public:
    Enlarger(Display& d, Buzzer& b, Exposure& e);
    ~Enlarger();
    
    void startExposure();
    void run();
    
    Lamp getStatePrepare();
    Lamp getState();
    void switchOn();
    void switchOff();

    void runFocusLampUsageCounter();
    uint16_t getTimeCounter();
    uint16_t getFocusLampUsageCounter();
    
    
    void runMetronome();
    void stopMetronome();
    uint16_t getMetronomeTimeCounter();
    
    void setSafelight(Safelight sl);
    Safelight getSafelight();
    
    void setPrepare(bool p);
    bool getPrepare();

    void setLampUsage(bool lu);
    bool getLampUsage();

    uint16_t getLampUsageCounter(bool pause);

};


#endif