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

public:
    Storage(Display& o, Preferences& p, Exposure& e, Buzzer& b, Enlarger& l);
    ~Storage();
    void load();
    void save();
};

#endif