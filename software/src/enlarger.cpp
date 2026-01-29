#include <enlarger.h>

Enlarger::Enlarger(Display& d, Buzzer& b, Exposure& e) : display(d), buzzer(b), exposure(e) {
    pinMode(RELAY_PIN, OUTPUT);
    digitalWrite(RELAY_PIN, LOW);
}

Enlarger::~Enlarger()
{
}

void Enlarger::startExposure() {
    timeCounter = exposure.getTimeCounter();
    state = Lamp::ON;
    if (safelight == Safelight::OFF_ON_EXPOSURE) {
        digitalWrite(SAFELIGHT_PIN, LOW);
    }
    digitalWrite(RELAY_PIN, HIGH);
}

Lamp Enlarger::getState() {
    return state;
}

void Enlarger::run() {
    currentMillis = millis();
    
    if ((currentMillis - previousMillis) >= 100 ) {
        previousMillis = currentMillis;
        timeCounter--;    
        display.drawExposure(timeCounter);
        if (exposure.getMode() == Mode::EXPOSURE && exposure.getBaseTime() && timeCounter % 10 == 0) {
            buzzer.exposure();
        }
    }

    if (timeCounter == 0) {
        state = Lamp::OFF;
        digitalWrite(RELAY_PIN, LOW);
        if (safelight == Safelight::OFF_ON_EXPOSURE) {
            digitalWrite(SAFELIGHT_PIN, HIGH);
        }
        buzzer.endExposure();
        exposure.next();
        return;
    }
}

Lamp Enlarger::getStatePrepare() {
    uint8_t state = digitalRead(RELAY_PIN);
    if (state == HIGH) return Lamp::ON;
    return Lamp::OFF;
}

void Enlarger::switchOn() {
    digitalWrite(RELAY_PIN, HIGH);
    state = Lamp::ON;
}

void Enlarger::switchOff() {
    digitalWrite(RELAY_PIN, LOW);
    state = Lamp::OFF;
}

uint16_t Enlarger::getMetronomeTimeCounter() {
    return metronomeTimeCounter;
}

void Enlarger::runMetronome() {

    currentMillis = millis();
    
    if ((currentMillis - previousMillis) >= 100 ) {
        previousMillis = currentMillis;
        if (metronomeTimeCounter == 9999) metronomeTimeCounter = 0;
        metronomeTimeCounter++;    
        display.drawMetronome(metronomeTimeCounter);
        if (metronomeTimeCounter % 10 == 0) {
            buzzer.metronome();
        }
    }
}

void Enlarger::stopMetronome() {
    display.drawMetronome(metronomeTimeCounter);
    buzzer.endExposure();
    delay(1000);
    metronomeTimeCounter = 0;
}

void Enlarger::setSafelight(Safelight sl) {
    switch (sl)
    {
    case Safelight::OFF:
        digitalWrite(SAFELIGHT_PIN, LOW);
        break;
    case Safelight::OFF_ON_EXPOSURE:
        digitalWrite(SAFELIGHT_PIN, HIGH);
        break;
    case Safelight::ALWAYS_ON:
        digitalWrite(SAFELIGHT_PIN, HIGH);
        break;
    default:
        break;
    }
    safelight = sl;
}

Safelight Enlarger::getSafelight() {
    return safelight;
}

void Enlarger::setPrepare(bool p) {
    prepare = p;
}

bool Enlarger::getPrepare() {
    return prepare;
}

void Enlarger::setLampUsage(bool lu) {
    lampUsage = lu;
}

bool Enlarger::getLampUsage() {
    return lampUsage;
}

void Enlarger::runFocusLampUsageCounter() {
    currentMillis = millis();
    
    if ((currentMillis - previousMillis) >= 100 ) {
        previousMillis = currentMillis;
        focusLampUsageCounter++;    
    }
}

uint16_t Enlarger::getFocusLampUsageCounter() {
    uint16_t counter = focusLampUsageCounter;
    focusLampUsageCounter = 0;
    return counter;
}

