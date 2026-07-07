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
    bool paperOpenedFromTeststrip = false;
    bool paperOpenedFromLinear = false;
            
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
    bool adjustmentEnteredFromFase = false;
    bool adjustmentPhaseFromLinear = false;
    bool ignorePrecisionReleaseUp = false;
    bool stateCountup = false;
    unsigned long lastAdjustmentExitMs = 0;
    static constexpr unsigned long adjustmentExitDebounceMs = 120;
    unsigned long lastAdjustmentPhaseExitMs = 0;
    static constexpr unsigned long adjustmentPhaseExitDebounceMs = 120;
    
    Event currentEvent;
    Event nextEvent;

    void processInput();
    void processEvent();

    void state_main_run();
    void state_teststrip_run();
    void state_menu_run();
    void state_adjustment_run();
    void state_adjustment_phase_run();
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