#pragma once
#include <Arduino.h>
#include <common.h>

class Buzzer {
public:
    Buzzer();
    ~Buzzer();

    void exposure();
    void metronome();
    void endExposure();
    void doubleBuzz();
    void tripleBuzz();

    void update();

    void setState(Tone t);
    Tone getState();

private:
    struct Step {
        uint16_t frequency;
        uint16_t durationMs;
        uint16_t gapMs;
    };

    static constexpr uint8_t MAX_STEPS = 6;

    void startSequence(const Step* steps, uint8_t len);
    void playCurrentStep();

    Tone state = Tone::OFF;

    Step sequence[MAX_STEPS]{};
    uint8_t sequenceLen = 0;
    uint8_t sequenceIndex = 0;
    bool sequenceActive = false;
    unsigned long nextStepAtMs = 0;
};