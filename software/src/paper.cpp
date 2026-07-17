#include "common.h"
#include <paper.h>

Paper::Paper(Buzzer& b, Display& d) : buzz(b), display(d) {}

Paper::~Paper() {}

uint16_t Paper::getCurrentDevTargetTicks() {
    if (factorial) {
        return static_cast<uint16_t>(devTimeCounter * factor[factorIdx]);
    }
    return devTimeCounter;
}

void Paper::recalcDevRemainingFromElapsed() {
    if (!(state == Dev::ON && devTimeCounterActive) || factorialSamplingActive) {
        return;
    }

    uint16_t target = getCurrentDevTargetTicks();
    if (devElapsedTicks >= target) {
        timeCounter = 0;
        return;
    }

    timeCounter = static_cast<uint16_t>(target - devElapsedTicks);
}

void Paper::setEnabled(bool state) {
    enabled = state;
    cycleFinished = false;
    this->state = Dev::OFF;
    lastAnnouncedTimeCounter = 0xFFFF;
    lastSecondAnnouncedTimeCounter = 0xFFFF;
    if (enabled) {
        timeCounter = factorial ? 0 : getCurrentDevTargetTicks();
        devTimeCounterActive = false;
        stopTimeCounterActive = false;
        fixerTimeCounterActive = false;
        factorialSamplingActive = false;
        devElapsedTicks = 0;
    } else {
        timeCounter = 0;
        devTimeCounterActive = false;
        stopTimeCounterActive = false;
        fixerTimeCounterActive = false;
        factorialSamplingActive = false;
        devElapsedTicks = 0;
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
    recalcDevRemainingFromElapsed();
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
    if (factorIdx < 8) {
        factorIdx++;
    }
}

void Paper::setFactorIdxDown() {
    if (factorIdx > 0) {
        factorIdx--;
    }
}

void Paper::setFactorial(bool state) {
    factorial = state;
    recalcDevRemainingFromElapsed();
}

bool Paper::getFactorial() {
    return factorial;
}

void Paper::setFactor(uint8_t f) {
    if (f < 2) f = 2;
    if (f > 10) f = 10;
    factorIdx = f - 2;
    recalcDevRemainingFromElapsed();
}

uint8_t Paper::getFactor() {
    return factor[factorIdx];
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

        if (factorialSamplingActive) {
            uint32_t next = static_cast<uint32_t>(timeCounter) + ticks;
            if (next > 65535UL) next = 65535UL;
            timeCounter = static_cast<uint16_t>(next);
            return;
        }

        if (ticks >= timeCounter) {
            if (devTimeCounterActive) {
                devElapsedTicks += timeCounter;
            }
            timeCounter = 0;
        } else {
            if (devTimeCounterActive) {
                devElapsedTicks += ticks;
            }
            timeCounter -= ticks;
        }
    }

    if (!factorialSamplingActive && timeCounter > 0 && timeCounter <= 100 && timeCounter % 10 == 0 && timeCounter != lastSecondAnnouncedTimeCounter) {
        buzz.metronome();
        lastSecondAnnouncedTimeCounter = timeCounter;
    } else if (timeCounter > 0 && timeCounter % 300 == 0 && timeCounter != lastAnnouncedTimeCounter) {
        buzz.doubleBuzz();
        lastAnnouncedTimeCounter = timeCounter;
    }

    if (timeCounter == 0 && !factorialSamplingActive) {
        buzz.endExposure();
        lastAnnouncedTimeCounter = 0xFFFF;
        lastSecondAnnouncedTimeCounter = 0xFFFF;
        timeCounter = 0;
        if (devTimeCounterActive) {
            devTimeCounterActive = false;
            timeCounter = stopTimeCounter;
            stopTimeCounterActive = true;
            devElapsedTicks = 0;
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
            timeCounter = getCurrentDevTargetTicks();
            devElapsedTicks = 0;
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
    timeCounter = getCurrentDevTargetTicks();
    devTimeCounterActive = true;
    stopTimeCounterActive = false;
    fixerTimeCounterActive = false;
    factorialSamplingActive = false;
    devElapsedTicks = 0;
    buzz.doubleBuzz();
    lastAnnouncedTimeCounter = 0xFFFF;
    lastSecondAnnouncedTimeCounter = 0xFFFF;
}

void Paper::handleStartPress() {
    if (!enabled) {
        return;
    }

    if (!factorial) {
        if (state == Dev::OFF) {
            startDevelopment();
        }
        return;
    }

    if (state == Dev::OFF) {
        previousMillis = millis();
        state = Dev::ON;
        cycleFinished = false;
        timeCounter = 0;
        devTimeCounterActive = false;
        stopTimeCounterActive = false;
        fixerTimeCounterActive = false;
        factorialSamplingActive = true;
        devElapsedTicks = 0;
        buzz.doubleBuzz();
        lastAnnouncedTimeCounter = 0xFFFF;
        lastSecondAnnouncedTimeCounter = 0xFFFF;
        return;
    }

    if (factorialSamplingActive) {
        uint32_t target = static_cast<uint32_t>(timeCounter) * factor[factorIdx];
        if (target > 65535UL) target = 65535UL;
        timeCounter = static_cast<uint16_t>(target);
        devElapsedTicks = 0;
        devTimeCounterActive = true;
        stopTimeCounterActive = false;
        fixerTimeCounterActive = false;
        factorialSamplingActive = false;
        buzz.doubleBuzz();
        lastAnnouncedTimeCounter = 0xFFFF;
        lastSecondAnnouncedTimeCounter = 0xFFFF;
    }
}

void Paper::reset() {
    previousMillis = millis();
    state = Dev::OFF;
    cycleFinished = false;
    timeCounter = factorial ? 0 : getCurrentDevTargetTicks();
    devTimeCounterActive = false;
    stopTimeCounterActive = false;
    fixerTimeCounterActive = false;
    factorialSamplingActive = false;
    devElapsedTicks = 0;
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
    if (factorialSamplingActive) return DevType::FACTORIAL_MEASURE;
    if (devTimeCounterActive) return DevType::DEVELOPER;
    if (stopTimeCounterActive) return DevType::STOP_BATH;
    if (fixerTimeCounterActive) return DevType::FIXER;
    return DevType::DEVELOPER;
}