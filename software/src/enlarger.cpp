#include <enlarger.h>

Enlarger::Enlarger(Display& d, Buzzer& b, Exposure& e) : display(d), buzzer(b), exposure(e) {
    pinMode(RELAY_PIN, OUTPUT);
    pinMode(SAFELIGHT_PIN, OUTPUT);
    
    if (relay == Relay::ACTIVE_LOW) {
        digitalWrite(RELAY_PIN, HIGH); 
        digitalWrite(SAFELIGHT_PIN, HIGH);
    } else {
        digitalWrite(RELAY_PIN, LOW); 
        digitalWrite(SAFELIGHT_PIN, LOW);
    }
}

Enlarger::~Enlarger() {
}

void Enlarger::startExposure() {
    timeCounter = exposure.getTimeCounter();
    state = Lamp::ON;
    
    updateRelay();
    updateSafeLight();
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
        updateRelay();
        updateSafeLight();
        buzzer.endExposure();
        exposure.next();
        return;
    }
}

void Enlarger::switchOn() {
    state = Lamp::ON;
    updateRelay();
    updateSafeLight(); 
}

void Enlarger::switchOff() {
    state = Lamp::OFF;
    updateRelay();
    updateSafeLight();
}

Lamp Enlarger::getState() {
    return state;
}

Lamp Enlarger::getStatePrepare() {
    if (relay == Relay::ACTIVE_LOW && digitalRead(RELAY_PIN) == LOW) return Lamp::ON;
    if (relay == Relay::ACTIVE_HIGH && digitalRead(RELAY_PIN) == HIGH) return Lamp::ON;
    return Lamp::OFF;
}

uint16_t Enlarger::getTimeCounter() {
    return timeCounter;
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
    safelight = sl;
    updateSafeLight(); 
}

void Enlarger::updateSafeLight() {
    switch (safelight)
    {
    case Safelight::OFF:
        if (relay == Relay::ACTIVE_LOW) digitalWrite(SAFELIGHT_PIN, HIGH);
        if (relay == Relay::ACTIVE_HIGH) digitalWrite(SAFELIGHT_PIN, LOW);
        break;
    case Safelight::ALWAYS_ON:
        if (relay == Relay::ACTIVE_LOW) digitalWrite(SAFELIGHT_PIN, LOW);
        if (relay == Relay::ACTIVE_HIGH) digitalWrite(SAFELIGHT_PIN, HIGH);
        break;
    case Safelight::OFF_ON_EXPOSURE:
        if (state == Lamp::ON) {
            if (relay == Relay::ACTIVE_LOW) digitalWrite(SAFELIGHT_PIN, HIGH);
            if (relay == Relay::ACTIVE_HIGH) digitalWrite(SAFELIGHT_PIN, LOW);
        } else {
            if (relay == Relay::ACTIVE_LOW) digitalWrite(SAFELIGHT_PIN, LOW);
            if (relay == Relay::ACTIVE_HIGH) digitalWrite(SAFELIGHT_PIN, HIGH);
        }
        break;
    default:
        break;
    }
}

void Enlarger::updateRelay() {
    if (state == Lamp::ON && relay == Relay::ACTIVE_LOW) digitalWrite(RELAY_PIN, LOW);
    if (state == Lamp::ON && relay == Relay::ACTIVE_HIGH) digitalWrite(RELAY_PIN, HIGH);
    if (state == Lamp::OFF && relay == Relay::ACTIVE_LOW) digitalWrite(RELAY_PIN, HIGH);
    if (state == Lamp::OFF && relay == Relay::ACTIVE_HIGH) digitalWrite(RELAY_PIN, LOW);
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