#include <U8g2lib.h>
#include <display.h>
#include <common.h>
#include <keypad.h>
#include <buzzer.h>
#include <exposure.h>
#include <enlarger.h>
#include <menu.h>
#include <storage.h>

#ifndef TIMER_H
#define TIMER_H

class Timer {

public:
    Timer(Display& d, Keypad& k, Buzzer& b, Exposure& e, Enlarger& i, TimerMenu::Menu& m, Storage& s);
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

    bool prepareState = false;
            
    // State Machine
    struct transition_t
    {
        State from;
        Event event;
        State to;
    };

    transition_t transitions[EVENTS] = {
        {State::MAIN, Event::NO_EVENT, State::MAIN},
        {State::MENU, Event::NO_EVENT, State::MENU},
        {State::FOCUS, Event::NO_EVENT, State::FOCUS},
        {State::PAUSE, Event::NO_EVENT, State::PAUSE},
        {State::ADJUSTMENT, Event::NO_EVENT, State::ADJUSTMENT},
        {State::TESTSTRIP, Event::NO_EVENT, State::TESTSTRIP},
        {State::PREPARE, Event::NO_EVENT, State::PREPARE},
        {State::LAMPUSAGE, Event::NO_EVENT, State::LAMPUSAGE},
        {State::METRONOME, Event::NO_EVENT, State::METRONOME},
        {State::MAIN, Event::RELEASED_FOCUS, State::FOCUS},
        {State::FOCUS, Event::MOVE_TO_MAIN, State::MAIN},
        {State::MAIN, Event::RELEASED_ADJUSTMENT, State::ADJUSTMENT},
        {State::MAIN, Event::RELEASED_TESTSTRIP, State::TESTSTRIP},
        {State::MAIN, Event::MOVE_TO_MENU, State::MENU},
        {State::ADJUSTMENT, Event::MOVE_TO_MAIN, State::MAIN},
        {State::MENU, Event::RELEASED_EXIT, State::MAIN},
        {State::MENU, Event::MOVE_TO_MAIN, State::MAIN},
        {State::MAIN, Event::RELEASED_TESTSTRIP, State::TESTSTRIP},
        {State::TESTSTRIP, Event::MOVE_TO_MAIN, State::MAIN},
        {State::MAIN, Event::MOVE_TO_PREPARE, State::PREPARE},
        {State::PREPARE, Event::MOVE_TO_MAIN, State::MAIN},
        {State::MAIN, Event::LONGPRESS_TESTSTRIP, State::METRONOME},
        {State::METRONOME, Event::MOVE_TO_MAIN, State::MAIN},
        {State::MAIN, Event::MOVE_TO_PAUSE, State::PAUSE},
        {State::TESTSTRIP, Event::MOVE_TO_PAUSE, State::PAUSE},
        {State::PAUSE, Event::MOVE_TO_MAIN, State::MAIN},
        {State::PAUSE, Event::MOVE_TO_TESTSTRIP, State::TESTSTRIP},
        {State::MAIN, Event::MOVE_TO_LAMPUSAGE, State::LAMPUSAGE},
        {State::LAMPUSAGE, Event::MOVE_TO_MAIN, State::MAIN},
    };

    typedef void (Timer::* voidfunc)();
    static voidfunc run[static_cast<int>(State::COUNT)];
    
    State currentState;
    State previousState = State::MAIN;
    bool stateCountup = false;
    
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
    void state_lampusage_run();
    void state_pause_run();
};

#endif