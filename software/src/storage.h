#include <Arduino.h>
#include <U8g2lib.h>
#include <common.h>
#include <Preferences.h>
#include <keypad.h>
#include <exposure.h>
#include <enlarger.h>
#include <display.h>
#include <paper.h>

#ifndef USER_PREFERENCES_H
#define USER_PREFERENCES_H

class Storage
{
private:
    Display& display;
    Preferences& preferences;
    Exposure& exposure;
    Buzzer& buzzer;
    Enlarger& enlarger;
    Keypad& keypad;
    Paper& paper;

    uint32_t lampUsageHoursCounter = 0;
    uint8_t lampUsageMinutesCounter = 1;
    uint32_t lampUsageSecondsCounter = 0;
    uint32_t lampUsageTenthsCounter = 0;

public:
    Storage(Display& o, Preferences& p, Exposure& e, Buzzer& b, Enlarger& l, Keypad& k, Paper& pa);
    ~Storage();
    void load();
    uint16_t getLampUsageHours();
    uint8_t getLampUsageMinutes();
    void saveLampUsage(uint16_t usage);
    
    void storeLampUsage(uint8_t l);
    void storePrecision(uint8_t p);
    void storeTestStripMode(uint8_t t);
    void storeSafelight(uint8_t s);
    void storeStartTime(uint8_t s);
    void storeStartTrigger(uint8_t t);
    void storeBuzzer(uint8_t b);
    void storePrepare(uint8_t p);
    void storeBrightness(uint8_t b);
    void storePaperEnabled(uint8_t e);
    void storePaperFactorial(uint8_t f);
    void storePaperFactor(uint8_t f);
    void storePaperDevTime(uint16_t t);
    void storePaperStopTime(uint16_t t);
    void storePaperFixerTime(uint16_t t);
    
    void resetLampUsage();
};

#endif