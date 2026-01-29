#include <timer.h>

using voidfunc = void (Timer::*)();

Timer::Timer(Display& d, Keypad& k, Buzzer& b, Exposure& e, Enlarger& i, TimerMenu::Menu& m, Storage& s) : display(d), keypad(k), buzzer(b), exposure(e), enlarger(i), menu(m), storage(s) {
}

Timer::~Timer() {}

Timer::voidfunc Timer::run[] = {
    &Timer::state_main_run,
    &Timer::state_teststrip_run,
    &Timer::state_menu_run,
    &Timer::state_adjustment_run,
    &Timer::state_focus_run,
    &Timer::state_prepare_run,
    &Timer::state_metronome_run,
    &Timer::state_pause_run,
    &Timer::state_lampusage_run,
};

void Timer::init(){
    currentEvent = Event::NO_EVENT;
    currentState = State::MAIN;
    (this->*run[static_cast<int>(currentState)])();
}

void Timer::insertEvent(Event event){
    currentEvent = event;
}

void Timer::processInput() {
    if (currentState == State::MAIN && enlarger.getState() == Lamp::ON && currentEvent != Event::RELEASED_START) {
        nextEvent = currentEvent = Event::NO_EVENT; 
        return;
    }
    if (currentEvent == Event::LONGPRESS_TESTSTRIP) buzzer.doubleBuzz();
    if (currentState == State::MAIN && currentEvent == Event::RELEASED_TESTSTRIP) {
        exposure.setMode(Mode::TESTSTRIP);
    }
    if (currentEvent == Event::LONGPRESS_FOCUS) {
        display.powerSave(true);
        buzzer.doubleBuzz();
    }
    nextEvent = currentEvent;
    currentEvent = Event::NO_EVENT;
}

void Timer::Run(){
    keypad.tick();
    Event event = keypad.fetchKeypadEvent();
    if (event != Event::NO_EVENT) insertEvent(event);
    processInput();
    processEvent();
    (this->*run[static_cast<int>(currentState)])();
}

void Timer :: processEvent() {
    for (uint8_t i = 0; i < EVENTS; i++) {
        if (currentState == transitions[i].from && nextEvent == transitions[i].event) {
            currentState = transitions[i].to;
            nextEvent = Event::NO_EVENT;
            return;
        }
    }
}

void Timer::state_main_run() {
    if (exposure.getBaseTime() && !prepareState) prepareState = true;
    if (!exposure.getBaseTime() && enlarger.getState() == Lamp::OFF && enlarger.getPrepare() && prepareState) {
        insertEvent(Event::MOVE_TO_PREPARE);
        return;
    }
    if (enlarger.getState() == Lamp::OFF) {
        if (exposure.getBaseTime()) display.drawMain(exposure.getBaseTimeCounter(), exposure.getPrecision(), exposure.getDodgeSize(), exposure.getBurnSize(), enlarger.getPrepare());
        else display.drawAdjustment(exposure.getAdjustmentType(), exposure.getAdjustmentTimeCounter(), exposure.getAdjustmentArea(), exposure.getAdjustmentValue());
    }

    switch (nextEvent)
    {
    case Event::LONGPRESS_UP:
        if (enlarger.getLampUsage()) {
            insertEvent(Event::MOVE_TO_LAMPUSAGE);
            buzzer.doubleBuzz();
        }
        break;
    case Event::RELEASED_START:
        if (enlarger.getState() == Lamp::OFF) {
            enlarger.startExposure();
            if (enlarger.getPrepare() && !exposure.getBaseTime()) {
                prepareState = true;
            }
            if (enlarger.getLampUsage() && exposure.getBaseTime()) {
                storage.saveLampUsage(exposure.getBaseTimeCounter());
            } else if (enlarger.getLampUsage()) {
                storage.saveLampUsage(exposure.getAdjustmentTimeCounter());
            }
        } else {
            enlarger.switchOff();
            insertEvent(Event::MOVE_TO_PAUSE);
        }
        break;
    case Event::RELEASED_UP:
        exposure.setBaseExposureUp();
        break;
    case Event::RELEASED_DOWN:
        exposure.setBaseExposureDown();
        break;
    case Event::LONGPRESS_EXIT:
        exposure.clear();
        buzzer.doubleBuzz();
        break;
    case Event::LONGPRESS_MENU:
        storage.save();
        buzzer.doubleBuzz();
        break;
    case Event::RELEASED_MENU:
        if (exposure.getBaseTime()) insertEvent(Event::MOVE_TO_MENU);
        break;
    default:
        break;
    }
    if (enlarger.getState() == Lamp::ON) enlarger.run();
}

void Timer::state_focus_run() {
    display.drawFocus();
    enlarger.switchOn();
    enlarger.runFocusLampUsageCounter();
    if (nextEvent == Event::RELEASED_FOCUS) {
        enlarger.switchOff();
        if (enlarger.getLampUsage()) storage.saveLampUsage(enlarger.getFocusLampUsageCounter());
        insertEvent(Event::MOVE_TO_MAIN);
        return;
    }
}

void Timer::state_adjustment_run() {
    if (exposure.isNewAdjustment()) display.drawNewAdjustment(exposure.getNewAdjustmentTimeCounter(), exposure.getNewAdjustmentValue(), exposure.getNewAdjustmentType(), exposure.getSize());
    else display.drawAdjustment(exposure.getAdjustmentType(), exposure.getAdjustmentTimeCounter(), exposure.getAdjustmentArea(), exposure.getAdjustmentValue());
    switch (nextEvent)
    {
    case Event::RELEASED_DOWN:
        exposure.setAdjustmentDown();
        break;
    case Event::RELEASED_UP:
        exposure.setAdjustmentUp();
        break;
    case Event::LONGPRESS_ADJ:
        buzzer.doubleBuzz();
        exposure.saveAdjustment();
        break;
    case Event::RELEASED_ADJUSTMENT:
        exposure.nextAdjustment();
        break;
    case Event::RELEASED_EXIT:
        exposure.resetIndex();
        insertEvent(Event::MOVE_TO_MAIN);
        break;
    case Event::LONGPRESS_EXIT:
        exposure.remove();
        buzzer.doubleBuzz();
        break;
    default:
        break;
    }
}

void Timer::state_teststrip_run(){
    if (enlarger.getState() == Lamp::OFF) {
        display.drawTestStrip(exposure.getTestStripMode(), exposure.getTestStripTimeCounter(), exposure.getTestStripSteps());
    }
    switch (nextEvent)
    {
    case Event::RELEASED_START:
        if (enlarger.getState() == Lamp::OFF) {
            enlarger.startExposure();
            exposure.switchBaseTime();
            exposure.testStripNext();
            if (enlarger.getLampUsage()) storage.saveLampUsage(exposure.getTestStripTimeCounter());
        } else {
            enlarger.switchOff();
            insertEvent(Event::MOVE_TO_PAUSE);
            return;
        }
        break;
    case Event::RELEASED_EXIT:
        exposure.resetTestStrip();
        exposure.resetBaseTime();
        exposure.setMode(Mode::EXPOSURE);
        insertEvent(Event::MOVE_TO_MAIN);
        break;
    default:
        break;
    }
    if (enlarger.getState() == Lamp::ON) enlarger.run();
}


void Timer::state_prepare_run() {
    display.drawPrepare();
    if (nextEvent == Event::RELEASED_START) {
        buzzer.exposure();
        switch (enlarger.getStatePrepare())
        {
        case Lamp::OFF:
            enlarger.switchOn();
            break;
        case Lamp::ON:
            enlarger.switchOff();
            prepareState = false;
            insertEvent(Event::MOVE_TO_MAIN);
            break;
        default:
            break;
        }
    }
}

void Timer::state_metronome_run() {
    if (enlarger.getMetronomeTimeCounter() == 0) display.drawMetronome(enlarger.getMetronomeTimeCounter());
    switch (nextEvent)
    {
    case Event::RELEASED_START:
        if (enlarger.getState() == Lamp::OFF) {
            enlarger.switchOn();
        } else {
            enlarger.switchOff();
            if (enlarger.getLampUsage()) storage.saveLampUsage(enlarger.getMetronomeTimeCounter());
            enlarger.stopMetronome();
        }
        break;
    case Event::RELEASED_EXIT:
        if (enlarger.getState() == Lamp::OFF) insertEvent(Event::MOVE_TO_MAIN);
        break;
    default:
        break;
    }
    if (enlarger.getState() == Lamp::ON) enlarger.runMetronome();
}

void Timer::state_menu_run() {
    menu.loop();
    Event event = Event::MOVE_TO_MAIN;
    insertEvent(event);
}

void Timer::state_pause_run() {
    display.drawPause();
    if (nextEvent == Event::RELEASED_START) {
        enlarger.switchOn();
        if (exposure.getMode() == Mode::EXPOSURE) insertEvent(Event::MOVE_TO_MAIN);
        if (exposure.getMode() == Mode::TESTSTRIP) insertEvent(Event::MOVE_TO_TESTSTRIP);
    }
}

void Timer::state_lampusage_run() {
    display.drawLampUsage(storage.getLampUsageHours(), storage.getLampUsageMinutes());
    switch (nextEvent)
    {
    case Event::RELEASED_EXIT:
        insertEvent(Event::MOVE_TO_MAIN);
        break;
    case Event::LONGPRESS_EXIT:
        storage.resetLampUsage();
        buzzer.doubleBuzz();
        break;
    default:
        break;
    }
}