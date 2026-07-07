#include <timer.h>

using voidfunc = void (Timer::*)();

Timer::Timer(Display& d, Keypad& k, Buzzer& b, Exposure& e, Enlarger& i, TimerMenu::Menu& m, Storage& s, Paper& p) : display(d), keypad(k), buzzer(b), exposure(e), enlarger(i), menu(m), storage(s), paper(p) {
}

Timer::~Timer() {}

Timer::voidfunc Timer::run[] = {
    &Timer::state_main_run,
    &Timer::state_teststrip_run,
    &Timer::state_menu_run,
    &Timer::state_adjustment_run,
    &Timer::state_adjustment_phase_run,
    &Timer::state_focus_run,
    &Timer::state_prepare_run,
    &Timer::state_metronome_run,
    &Timer::state_pause_run,
    &Timer::state_lampusage_run,
    &Timer::state_precision_run,
    &Timer::state_linear_run,
    &Timer::state_paper_run,
};

const Timer::transition_t Timer::transitions[] = {
    {State::MAIN, Event::NO_EVENT, State::MAIN},
    {State::MENU, Event::NO_EVENT, State::MENU},
    {State::FOCUS, Event::NO_EVENT, State::FOCUS},
    {State::PAUSE, Event::NO_EVENT, State::PAUSE},
    {State::ADJUSTMENT, Event::NO_EVENT, State::ADJUSTMENT},
    {State::ADJUSTMENT_PHASE, Event::NO_EVENT, State::ADJUSTMENT_PHASE},
    {State::TESTSTRIP, Event::NO_EVENT, State::TESTSTRIP},
    {State::PREPARE, Event::NO_EVENT, State::PREPARE},
    {State::LAMPUSAGE, Event::NO_EVENT, State::LAMPUSAGE},
    {State::METRONOME, Event::NO_EVENT, State::METRONOME},
    {State::PRECISION, Event::NO_EVENT, State::PRECISION},
    {State::LINEAR, Event::NO_EVENT, State::LINEAR},
    {State::MAIN, Event::RELEASED_FOCUS, State::FOCUS},
    {State::FOCUS, Event::MOVE_TO_MAIN, State::MAIN},
    {State::MAIN, Event::RELEASED_ADJUSTMENT, State::ADJUSTMENT},
    {State::MAIN, Event::MOVE_TO_ADJUSTMENT_PHASE, State::ADJUSTMENT_PHASE},
    {State::ADJUSTMENT_PHASE, Event::RELEASED_ADJUSTMENT, State::ADJUSTMENT},
    {State::MAIN, Event::MOVE_TO_MENU, State::MENU},
    {State::ADJUSTMENT, Event::MOVE_TO_MAIN, State::MAIN},
    {State::ADJUSTMENT, Event::MOVE_TO_ADJUSTMENT_PHASE, State::ADJUSTMENT_PHASE},
    {State::ADJUSTMENT_PHASE, Event::MOVE_TO_MAIN, State::MAIN},
    {State::MENU, Event::RELEASED_EXIT, State::MAIN},
    {State::MENU, Event::MOVE_TO_MAIN, State::MAIN},
    {State::MAIN, Event::RELEASED_TESTSTRIP, State::TESTSTRIP},
    {State::TESTSTRIP, Event::MOVE_TO_MAIN, State::MAIN},
    {State::MAIN, Event::MOVE_TO_PREPARE, State::PREPARE},
    {State::PREPARE, Event::MOVE_TO_MAIN, State::MAIN},
    {State::MAIN, Event::LONGPRESS_TESTSTRIP, State::METRONOME},
    {State::ADJUSTMENT_PHASE, Event::RELEASED_TESTSTRIP, State::METRONOME},
    {State::METRONOME, Event::MOVE_TO_MAIN, State::MAIN},
    {State::METRONOME, Event::MOVE_TO_ADJUSTMENT_PHASE, State::ADJUSTMENT_PHASE},
    {State::MAIN, Event::MOVE_TO_PAUSE, State::PAUSE},
    {State::TESTSTRIP, Event::MOVE_TO_PAUSE, State::PAUSE},
    {State::PAUSE, Event::MOVE_TO_MAIN, State::MAIN},
    {State::PAUSE, Event::MOVE_TO_TESTSTRIP, State::TESTSTRIP},
    {State::MAIN, Event::MOVE_TO_LAMPUSAGE, State::LAMPUSAGE},
    {State::LAMPUSAGE, Event::MOVE_TO_MAIN, State::MAIN},
    {State::MAIN, Event::LONGPRESS_UP, State::PRECISION},
    {State::PRECISION, Event::MOVE_TO_MAIN, State::MAIN},
    {State::MAIN, Event::MOVE_TO_LINEAR, State::LINEAR},
    {State::LINEAR, Event::MOVE_TO_MAIN, State::MAIN},
    {State::LINEAR, Event::RELEASED_TESTSTRIP, State::METRONOME},
    {State::METRONOME, Event::MOVE_TO_LINEAR, State::LINEAR},
    {State::LINEAR, Event::MOVE_TO_PAUSE, State::PAUSE},
    {State::PAUSE, Event::MOVE_TO_LINEAR, State::LINEAR},
    {State::LINEAR, Event::RELEASED_FOCUS, State::FOCUS},
    {State::FOCUS, Event::MOVE_TO_LINEAR, State::LINEAR},
    {State::MAIN, Event::MOVE_TO_PAPER, State::PAPER},
    {State::ADJUSTMENT_PHASE, Event::MOVE_TO_PAPER, State::PAPER},
    {State::PAPER, Event::NO_EVENT, State::PAPER},
    {State::PAPER, Event::MOVE_TO_MAIN, State::MAIN},
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
    if (currentEvent == Event::RELEASED_FOOTSWITCH2 && currentState != State::PAPER) {
        currentEvent = Event::RELEASED_FOCUS;
    }

    if (currentState == State::MAIN && enlarger.getState() == Lamp::ON && currentEvent != Event::RELEASED_START && currentEvent != Event::PRESSED_START) {
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
    for (const auto& transition : transitions) {
        if (currentState == transition.from && nextEvent == transition.event) {
            if (transition.from == State::ADJUSTMENT_PHASE && transition.to == State::ADJUSTMENT) {
                adjustmentEnteredFromFase = true;
            }

            if (transition.from != transition.to) {
                previousState = currentState;
            }
            currentState = transition.to;
            nextEvent = Event::NO_EVENT;
            return;
        }
    }
}

void Timer::state_main_run() {

    if (enlarger.getIsExposureFinished()) {
        if (paper.getEnabled() && exposure.getMode() == Mode::EXPOSURE && exposure.getBaseTime()) {
            if (exposure.getSize() == 0) {
                insertEvent(Event::MOVE_TO_ADJUSTMENT_PHASE);
            } else {
                paper.reset();
                insertEvent(Event::MOVE_TO_PAPER);
            }
        }
        enlarger.setIsExposureFinished(false);
        return;
    }

    if (enlarger.getLampUsage()) {
        uint16_t counter = enlarger.getLampUsageCounter(false);
        if (counter != 0) {
            storage.saveLampUsage(counter);
        }
    }
    if (exposure.getBaseTime() && !prepareState) prepareState = true;
    if (!exposure.getBaseTime() && enlarger.getState() == Lamp::OFF && enlarger.getPrepare() && prepareState) {
        insertEvent(Event::MOVE_TO_PREPARE);
        return;
    }
    if (enlarger.getState() == Lamp::OFF) {
        if (exposure.getBaseTime()) display.drawMain(exposure.getBaseTimeCounter(), exposure.getPrecision(), exposure.getDodgeSize(), exposure.getBurnSize(), enlarger.getPrepare());
        else display.drawAdjustment(exposure.getAdjustmentType(), exposure.getAdjustmentTimeCounter(), exposure.getAdjustmentArea(), exposure.getAdjustmentValue(), exposure.getAdjPrecision());
    }

    switch (nextEvent)
    {
    case Event::PRESSED_START:
    case Event::RELEASED_START:
        if (enlarger.getState() == Lamp::OFF) {
            enlarger.startExposure();
            if (enlarger.getPrepare() && !exposure.getBaseTime()) {
                prepareState = true;
            }
        } else {
            enlarger.switchOff();
            insertEvent(Event::MOVE_TO_PAUSE);
        }
        break;
    case Event::RELEASED_UP:
        exposure.setBaseExposureUp();
        exposure.resetTestStrip();
        break;
    case Event::RELEASED_DOWN:
        exposure.setBaseExposureDown();
        exposure.resetTestStrip();
        break;
    case Event::LONGPRESS_DOWN:
        if (enlarger.getState() == Lamp::OFF && exposure.splitSteps()) {
            buzzer.doubleBuzz();
        }
        break;
    case Event::LONGPRESS_EXIT:
        exposure.clear();
        buzzer.doubleBuzz();
        display.drawExposureRestart();
        break;
    case Event::LONGPRESS_MENU:
        if (enlarger.getLampUsage()) {
            insertEvent(Event::MOVE_TO_LAMPUSAGE);
            buzzer.doubleBuzz();
        }
        break;
    case Event::LONGPRESS_ADJ:
        exposure.setMode(Mode::LINEAR);
        buzzer.doubleBuzz();
        insertEvent(Event::MOVE_TO_LINEAR);
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
        if (exposure.getMode() == Mode::EXPOSURE) insertEvent(Event::MOVE_TO_MAIN);
        if (exposure.getMode() == Mode::LINEAR) insertEvent(Event::MOVE_TO_LINEAR);
        return;
    }
}

void Timer::state_adjustment_run() {
    if (enlarger.getState() == Lamp::OFF) {
        if (exposure.isNewAdjustment()) display.drawNewAdjustment(exposure.getNewAdjustmentTimeCounter(), exposure.getNewAdjustmentValue(), exposure.getNewAdjustmentType(), exposure.getSize(), exposure.getAdjPrecision());
        else display.drawAdjustment(exposure.getAdjustmentType(), exposure.getAdjustmentTimeCounter(), exposure.getAdjustmentArea(), exposure.getAdjustmentValue(), exposure.getAdjPrecision());
        switch (nextEvent)
        {
        case Event::RELEASED_DOWN:
            exposure.setAdjustmentDown();
            break;
        case Event::RELEASED_UP:
            exposure.setAdjustmentUp();
            break;
        case Event::LONGPRESS_ADJ:
            if (!adjustmentEnteredFromFase) {
                buzzer.doubleBuzz();
                exposure.saveAdjustment();
                display.drawAdjustmentAdded();
            }
            break;
        case Event::RELEASED_ADJUSTMENT:
            exposure.nextAdjustment();
            break;
        case Event::RELEASED_EXIT:
            {
                const unsigned long now = millis();
                if (now - lastAdjustmentExitMs < adjustmentExitDebounceMs) {
                    break;
                }
                lastAdjustmentExitMs = now;
            }
            exposure.resetIndex();
            exposure.setMode(Mode::EXPOSURE);
            if (adjustmentEnteredFromFase) {
                adjustmentEnteredFromFase = false;
                insertEvent(Event::MOVE_TO_ADJUSTMENT_PHASE);
            } else {
                insertEvent(Event::MOVE_TO_MAIN);
            }
            break;
        case Event::RELEASED_MENU:
            exposure.switchAdjPrecision();
            buzzer.doubleBuzz();
            break;
        case Event::PRESSED_START:
        case Event::RELEASED_START:
            exposure.setMode(Mode::ADJUSTMENT);
            if (exposure.isNewAdjustment() && exposure.isNewAdjustmentBurn() && enlarger.getState() == Lamp::OFF) {
                enlarger.startExposure();
                if (enlarger.getPrepare() && !exposure.getBaseTime()) {
                    prepareState = true;
                }
            } else {
                enlarger.switchOff();
                insertEvent(Event::MOVE_TO_PAUSE);
            }
            break;
        case Event::LONGPRESS_EXIT:
            exposure.remove();
            buzzer.doubleBuzz();
            display.drawAdjustmentRemoved();
            break;
        default:
            break;
        }
    }
    if (enlarger.getState() == Lamp::ON) enlarger.run();
}

void Timer::state_adjustment_phase_run() {
    if (enlarger.getState() == Lamp::OFF) {
        display.drawAdjustmentPhase();
    }

    if (nextEvent == Event::PRESSED_START || nextEvent == Event::RELEASED_START) {
        paper.reset();
        insertEvent(Event::MOVE_TO_PAPER);
        return;
    }

    if (nextEvent == Event::RELEASED_EXIT) {
        const unsigned long now = millis();
        if (now - lastAdjustmentPhaseExitMs < adjustmentPhaseExitDebounceMs) {
            return;
        }
        lastAdjustmentPhaseExitMs = now;
        exposure.resetIndex();
        exposure.setMode(Mode::EXPOSURE);
        insertEvent(Event::MOVE_TO_MAIN);
        return;
    }
}

void Timer::state_teststrip_run(){
    if (enlarger.getState() == Lamp::OFF) {
        display.drawTestStrip(exposure.getTestStripMode(), exposure.getTestStripTimeCounter(), exposure.getTestStripSteps(), exposure.getTestStripPrecision());
    }
    switch (nextEvent)
    {
    case Event::PRESSED_START:
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
    case Event::RELEASED_TESTSTRIP:
        exposure.switchTestStripMode();
        break;
    case Event::RELEASED_UP:
        exposure.setTestStripPrecisionUp();
        break;
    case Event::RELEASED_DOWN:
        exposure.setTestStripPrecisionDown();
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
    if (nextEvent == Event::RELEASED_START || nextEvent == Event::PRESSED_START) {
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
    case Event::PRESSED_START:
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
        if (exposure.getMode() == Mode::LINEAR) {
            insertEvent(Event::MOVE_TO_LINEAR);
        } else if (previousState == State::ADJUSTMENT_PHASE) {
            insertEvent(Event::MOVE_TO_ADJUSTMENT_PHASE);
        } else if (enlarger.getState() == Lamp::OFF) insertEvent(Event::MOVE_TO_MAIN);
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
    if (nextEvent == Event::RELEASED_START || nextEvent == Event::PRESSED_START) {
        enlarger.switchOn();
        if (exposure.getMode() == Mode::EXPOSURE) insertEvent(Event::MOVE_TO_MAIN);
        if (exposure.getMode() == Mode::TESTSTRIP) insertEvent(Event::MOVE_TO_TESTSTRIP);
        if (exposure.getMode() == Mode::LINEAR) insertEvent(Event::MOVE_TO_LINEAR);
    }
    if (nextEvent == Event::RELEASED_EXIT && (exposure.getMode() == Mode::EXPOSURE || exposure.getMode() == Mode::LINEAR)) {
        storage.saveLampUsage(enlarger.getLampUsageCounter(true));
        exposure.restart();
        if (exposure.getMode() == Mode::EXPOSURE) insertEvent(Event::MOVE_TO_MAIN);
        if (exposure.getMode() == Mode::LINEAR) insertEvent(Event::MOVE_TO_LINEAR);
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
        display.drawLampUsageRestart();
        break;
    default:
        break;
    }
}

void Timer::state_precision_run() {
    display.drawPrecision(exposure.getNewPrecision());
    switch (nextEvent)
    {
    case Event::RELEASED_UP:
        exposure.setPrecisionUp();
        break;
    case Event::RELEASED_DOWN:
        exposure.setPrecisionDown();
        break;
    case Event::RELEASED_EXIT:
        exposure.updatePrecision();
        insertEvent(Event::MOVE_TO_MAIN);
        break;
    default:
        break;
    }
}

void Timer::state_linear_run() {
    if (enlarger.getLampUsage()) {
        uint16_t counter = enlarger.getLampUsageCounter(false);
        if (counter != 0) {
            storage.saveLampUsage(counter);
        }
    }
    if (enlarger.getState() == Lamp::OFF) {
        display.drawLinear(exposure.getLinearTimeCounter(), exposure.getLinearPrecision());
    }
    switch (nextEvent)
    {
    case Event::PRESSED_START:
    case Event::RELEASED_START:
        if (enlarger.getState() == Lamp::OFF) {
            enlarger.startExposure();
        } else {
            enlarger.switchOff();
            insertEvent(Event::MOVE_TO_PAUSE);
        }
        break;
    case Event::RELEASED_UP:
        exposure.setLinearUp();
        break;
    case Event::RELEASED_DOWN:
        exposure.setLinearDown();
        break;
    case Event::LONGPRESS_DOWN:
        exposure.resetLinearTimeCounter();
        buzzer.doubleBuzz();
        break;
    case Event::RELEASED_ADJUSTMENT:
        exposure.switchLinearPrecision();
        break;
    case Event::LONGPRESS_EXIT:
        exposure.setMode(Mode::EXPOSURE);
        buzzer.doubleBuzz();
        insertEvent(Event::MOVE_TO_MAIN);
        break;
    default:
        break;
    }
    if (enlarger.getState() == Lamp::ON) enlarger.run();
}

void Timer::state_paper_run() {
    display.drawPaper(paper.getTimeCounter(), paper.getDevType());
    switch (nextEvent)    {
    case Event::RELEASED_EXIT:
        paper.reset();
        enlarger.setIsExposureFinished(false);
        insertEvent(Event::MOVE_TO_MAIN);
        break;
    case Event::PRESSED_START:
    case Event::RELEASED_START:
    case Event::RELEASED_FOOTSWITCH2:
        paper.handleStartPress();
        break;
    default:
        break;
    }
    if (paper.getState() == Dev::ON) {
        paper.run();
        if (paper.consumeCycleFinished()) {
            enlarger.setIsExposureFinished(false);
            insertEvent(Event::MOVE_TO_MAIN);
        }
    }
}