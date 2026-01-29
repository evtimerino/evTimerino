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

void Buzzer::endExposure() {
    tone(BUZZER_PIN, 300, 120);
    delay(120);
    tone(BUZZER_PIN, 600, 120);
    delay(120);
    tone(BUZZER_PIN, 300, 120);
    delay(120);
}

void Buzzer::doubleBuzz() {
    tone(BUZZER_PIN, 300, 60);
    delay(150);
    tone(BUZZER_PIN, 300, 60);
    delay(200);
}

void Buzzer::tripleBuzz() {
    tone(BUZZER_PIN, 300, 60);
    delay(150);
    tone(BUZZER_PIN, 300, 60);
    delay(200);
    tone(BUZZER_PIN, 300, 60);
    delay(200);
}

void Buzzer::setState(Tone t) {
    state = t;
}

Tone Buzzer::getState() {
    return state;
}