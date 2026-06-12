#include <U8g2lib.h>
#include <display.h>
#include <common.h>
#include <keypad.h>
#include <buzzer.h>
#include <exposure.h>
#include <enlarger.h>
#include <menu.h>
#include <storage.h>
#include <paper.h>

#ifndef TIMER_H
#define TIMER_H

class Timer {

public:
    Timer(Display& d, Keypad& k, Buzzer& b, Exposure& e, Enlarger& i, TimerMenu::Menu& m, Storage& s, Paper& p);
    ~Timer();

    void insertEvent(Event event);
    void init();
    void Run();

private:
    Display& display;
    Keypad& keypad;
    Buzzer& buzzer;
    Exposure& exposure;
    Enlarger& enlarger;
    TimerMenu::Menu& menu;
    Storage& storage;
    Paper& paper;

    bool prepareState = false;
            
    // State Machine
    struct transition_t
    {
        State from;
        Event event;
        State to;
    };

    static const transition_t transitions[];

    typedef void (Timer::* voidfunc)();
    static voidfunc run[static_cast<int>(State::COUNT)];
    
    State currentState;
    State previousState = State::MAIN;
    bool stateCountup = false;

    uint16_t lastPaperDisplayCounter = 0xFFFF;
    uint8_t lastPrecisionDisplay = 0xFF;
    bool mainDisplayCacheValid = false;
    bool mainDisplayCacheBaseTime = false;
    uint16_t mainDisplayBaseTimeCounter = 0;
    uint8_t mainDisplayPrecision = 0;
    uint8_t mainDisplayDodgeCount = 0;
    uint8_t mainDisplayBurnCount = 0;
    bool mainDisplayPrepare = false;
    Adjustment mainDisplayAdjType = Adjustment::NONE;
    uint16_t mainDisplayAdjTimeCounter = 0;
    uint8_t mainDisplayAdjArea = 0;
    uint8_t mainDisplayAdjValue = 0;
    uint8_t mainDisplayAdjPrecision = 0;
    bool adjustmentDisplayCacheValid = false;
    bool adjustmentDisplayIsNew = false;
    uint16_t adjustmentDisplayTimeCounter = 0;
    uint8_t adjustmentDisplayValue = 0;
    Adjustment adjustmentDisplayType = Adjustment::NONE;
    uint8_t adjustmentDisplayNumber = 0;
    uint8_t adjustmentDisplayPrecision = 0;
    uint8_t adjustmentDisplayArea = 0;
    
    Event currentEvent;
    Event nextEvent;

    void processInput();
    void processEvent();

    void state_main_run();
    void state_teststrip_run();
    void state_menu_run();
    void state_adjustment_run();
    void state_focus_run();
    void state_prepare_run();
    void state_metronome_run();
    void state_pause_run();
    void state_lampusage_run();
    void state_precision_run();
    void state_linear_run();
    void state_paper_run();
};

#endif