#include <Arduino.h>
#include <U8g2lib.h>
#include <common.h>
#include <Preferences.h>
#include <exposure.h>
#include <enlarger.h>
#include <display.h>

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

    uint32_t lampUsageHoursCounter = 0;
    uint8_t lampUsageMinutesCounter = 1;
    uint32_t lampUsageSecondsCounter = 0;
    uint32_t lampUsageTenthsCounter = 0;

public:
    Storage(Display& o, Preferences& p, Exposure& e, Buzzer& b, Enlarger& l);
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
    void storeBuzzer(uint8_t b);
    void storePrepare(uint8_t p);
    void storeBrightness(uint8_t b);
    
    void resetLampUsage();
};

#endif