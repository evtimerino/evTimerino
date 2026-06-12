#include <buzzer.h>

Buzzer::Buzzer() {
    pinMode(BUZZER_PIN, OUTPUT);
}

Buzzer::~Buzzer()
{
}

void Buzzer::exposure() {
    if (state == Tone::ON) tone(BUZZER_PIN, 500, 100);
}

void Buzzer::metronome() {
    tone(BUZZER_PIN, 500, 100);
}

void Buzzer::playCurrentStep() {
    if (!sequenceActive || sequenceIndex >= sequenceLen) {
        sequenceActive = false;
        return;
    }

    const Step& s = sequence[sequenceIndex];
    tone(BUZZER_PIN, s.frequency, s.durationMs);
    nextStepAtMs = millis() + s.durationMs + s.gapMs;
}

void Buzzer::startSequence(const Step* steps, uint8_t len) {
    if (len == 0) return;
    if (len > MAX_STEPS) len = MAX_STEPS;

    for (uint8_t i = 0; i < len; ++i) {
        sequence[i] = steps[i];
    }

    sequenceLen = len;
    sequenceIndex = 0;
    sequenceActive = true;
    playCurrentStep();
}

void Buzzer::update() {
    if (!sequenceActive) return;

    if ((long)(millis() - nextStepAtMs) >= 0) {
        ++sequenceIndex;
        if (sequenceIndex < sequenceLen) {
            playCurrentStep();
        } else {
            sequenceActive = false;
        }
    }
}

void Buzzer::endExposure() {

    static const Step steps[] = {
        {300, 120, 0},
        {600, 120, 0},
        {300, 120, 0}
    };
    startSequence(steps, 3);
}

void Buzzer::doubleBuzz() {
    static const Step steps[] = {
        {300, 60, 90},
        {300, 60, 140}
    };
    startSequence(steps, 2);
}

void Buzzer::tripleBuzz() {
    static const Step steps[] = {
        {300, 60, 90},
        {300, 60, 140},
        {300, 60, 140}
    };
    startSequence(steps, 3);
}

void Buzzer::setState(Tone t) {
    state = t;
}

Tone Buzzer::getState() {
    return state;
}