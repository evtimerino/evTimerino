#include "common.h"
#include <paper.h>

Paper::Paper(Buzzer& b, Display& d) : buzz(b), display(d) {}

Paper::~Paper() {}

void Paper::setEnabled(bool state) {
    enabled = state;
    cycleFinished = false;
    this->state = Dev::OFF;
    lastAnnouncedTimeCounter = 0xFFFF;
    lastSecondAnnouncedTimeCounter = 0xFFFF;
    if (enabled) {
        timeCounter = devTimeCounter;
        devTimeCounterActive = true;
        stopTimeCounterActive = false;
        fixerTimeCounterActive = false;
    } else {
        timeCounter = 0;
        devTimeCounterActive = false;
        stopTimeCounterActive = false;
        fixerTimeCounterActive = false;
    }
}

void Paper::setDevTimeCounterUp() {
    devTimeCounter += 50;
}

void Paper::setDevTimeCounterDown() {
    if (devTimeCounter > 50) devTimeCounter -= 50;
}

void Paper::setDevTimeCounter(uint16_t t) {
    devTimeCounter = (t < 50) ? 50 : t;
}

void Paper::setStopTimeCounterUp() {
    stopTimeCounter += 50;

}

void Paper::setStopTimeCounterDown() {
    if (stopTimeCounter > 50) stopTimeCounter -= 50;
}

void Paper::setStopTimeCounter(uint16_t t) {
    stopTimeCounter = (t < 50) ? 50 : t;
}

void Paper::setFixerTimeCounterUp() {
    fixerTimeCounter += 50;
}

void Paper::setFixerTimeCounterDown() {
    if (fixerTimeCounter > 50) fixerTimeCounter -= 50;
}

void Paper::setFixerTimeCounter(uint16_t t) {
    fixerTimeCounter = (t < 50) ? 50 : t;
}

void Paper::setFactorIdxUp() {
    factorIdx++;
}

void Paper::setFactorIdxDown() {
    factorIdx--;
}

void Paper::run() {
    currentMillis = millis();

    if (previousMillis == 0) {
        previousMillis = currentMillis;
        return;
    }

    unsigned long elapsed = currentMillis - previousMillis;
    if (elapsed >= 100) {
        uint16_t ticks = elapsed / 100;
        previousMillis += static_cast<unsigned long>(ticks) * 100;

        if (ticks >= timeCounter) {
            timeCounter = 0;
        } else {
            timeCounter -= ticks;
        }
    }

    if (timeCounter > 0 && timeCounter <= 100 && timeCounter % 10 == 0 && timeCounter != lastSecondAnnouncedTimeCounter) {
        buzz.metronome();
        lastSecondAnnouncedTimeCounter = timeCounter;
    } else if (timeCounter > 0 && timeCounter % 300 == 0 && timeCounter != lastAnnouncedTimeCounter) {
        buzz.doubleBuzz();
        lastAnnouncedTimeCounter = timeCounter;
    }

    if (timeCounter == 0) {
        buzz.endExposure();
        lastAnnouncedTimeCounter = 0xFFFF;
        lastSecondAnnouncedTimeCounter = 0xFFFF;
        timeCounter = 0;
        if (devTimeCounterActive) {
            devTimeCounterActive = false;
            timeCounter = stopTimeCounter;
            stopTimeCounterActive = true;
            lastAnnouncedTimeCounter = 0xFFFF;
            lastSecondAnnouncedTimeCounter = 0xFFFF;
        } else if (stopTimeCounterActive) {
            stopTimeCounterActive = false;
            timeCounter = fixerTimeCounter;
            fixerTimeCounterActive = true;
            lastAnnouncedTimeCounter = 0xFFFF;
            lastSecondAnnouncedTimeCounter = 0xFFFF;
        } else if (fixerTimeCounterActive) {
            fixerTimeCounterActive = false;
            state = Dev::OFF;
            cycleFinished = true;
            timeCounter = devTimeCounter;
            lastAnnouncedTimeCounter = 0xFFFF;
            lastSecondAnnouncedTimeCounter = 0xFFFF;
        }
    }
}

uint16_t Paper::getTimeCounter() {
    return timeCounter;
}

uint16_t Paper::getDevTimeCounter() {
    return devTimeCounter;
}

uint16_t Paper::getStopTimeCounter() {
    return stopTimeCounter;
}

uint16_t Paper::getFixerTimeCounter() {
    return fixerTimeCounter;
}

bool Paper::getEnabled() {
    return enabled;
}

void Paper::startDevelopment() {
    previousMillis = millis();
    state = Dev::ON;
    cycleFinished = false;
    timeCounter = devTimeCounter;
    devTimeCounterActive = true;
    lastAnnouncedTimeCounter = 0xFFFF;
    lastSecondAnnouncedTimeCounter = 0xFFFF;
}

void Paper::reset() {
    previousMillis = millis();
    state = Dev::OFF;
    cycleFinished = false;
    timeCounter = devTimeCounter;
    devTimeCounterActive = false;
    stopTimeCounterActive = false;
    fixerTimeCounterActive = false;
    lastAnnouncedTimeCounter = 0xFFFF;
    lastSecondAnnouncedTimeCounter = 0xFFFF;
}

Dev Paper::getState() {
    return state;
}

bool Paper::getPaperDevelopmentActive() {
    return fixerTimeCounterActive || stopTimeCounterActive || devTimeCounterActive;
}

bool Paper::consumeCycleFinished() {
    bool finished = cycleFinished;
    cycleFinished = false;
    return finished;
}

DevType Paper::getDevType() {
    if (devTimeCounterActive) return DevType::DEVELOPER;
    if (stopTimeCounterActive) return DevType::STOP_BATH;
    if (fixerTimeCounterActive) return DevType::FIXER;
    return DevType::DEVELOPER;
}