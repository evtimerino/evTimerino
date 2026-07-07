#include <keypad.h>

PushButton::PushButton(uint8_t p) : pin(p) {
    pinMode(p, INPUT_PULLUP);
}

PushButton::~PushButton() {}

void PushButton::scan() {
    if ((state == Button::PRESSED || state == Button::LONG_PRESS) && digitalRead(pin) == HIGH) state = Button::RELEASED;
    if (millis() - lastMainButtonStateChange > debounceDuration) {
        byte buttonState = digitalRead(pin);
        if (buttonState != lastButtonState) {
            lastMainButtonStateChange = millis();
            lastButtonState = buttonState;
            if (buttonState == LOW) {
                state = Button::PRESSED;
                pressEdge = true;
                longPressHandled = false;
            }
        }
    }
}
void PushButton::scanLongPress() {
    if (state == Button::PRESSED && !pressed) {
        pressed = true;
        timePressed = millis();
    }
    if (pressed && millis() - timePressed > longPressMS) {
        state = Button::LONG_PRESS;
        pressed = false;
    }
}

Button PushButton::released() {
    Button event;
    switch (state)
    {
    case Button::LONG_PRESS:
        if (!longPressHandled) {
            longPressHandled = true;
            event = Button::LONG_PRESS;
        } else {
            event = Button::IDLE;
        }
        break;
    case Button::RELEASED:
        state = Button::IDLE;
        event = Button::RELEASED;
        pressed = false;
        longPressHandled = false;
        break;
    default:
        event = Button::IDLE;
        break;
    }
    return event;
}

bool PushButton::consumePressEdge() {
    if (!pressEdge) {
        return false;
    }
    pressEdge = false;
    return true;
}

Keypad::Keypad() {}

Keypad::~Keypad() {}

void Keypad::tick(){
    for (uint8_t i = 0; i < buttonsNumber; i++) {
        buttons[i].scan();
        buttons[i].scanLongPress();
    }
}

Event Keypad::fetchKeypadEvent() {
    Event newEvent = Event::NO_EVENT;
    for (uint8_t i = 0; i < buttonsNumber; i++) {
        if (!startOnRelease) {
            if ((i == START || i == FOOTSWITCH) && buttons[i].consumePressEdge()) {
                newEvent = Event::PRESSED_START;
            }
        }

        Button state = buttons[i].released();
        if (state == Button::LONG_PRESS) {
            switch (i)
            {
            case UP:
                newEvent = Event::LONGPRESS_UP;
                break;
            case DOWN:
                newEvent = Event::LONGPRESS_DOWN;
                break;
            case FOCUS:
                newEvent = Event::LONGPRESS_FOCUS;
                break;
            case EXIT:
                newEvent = Event::LONGPRESS_EXIT;
                break;
            case ADJUSTMENT:
                newEvent = Event::LONGPRESS_ADJ;
                break;
            case TESTSTRIP:
                newEvent = Event::LONGPRESS_TESTSTRIP;
                break;
            case MENU:
                newEvent = Event::LONGPRESS_MENU;
                break;
            default:
                break;
            }
        }
        if (state == Button::RELEASED) {
            switch (i)
            {
            case FOCUS:
                newEvent = Event::RELEASED_FOCUS;
                break;
            case START:
                if (startOnRelease) newEvent = Event::RELEASED_START;
                break;
            case FOOTSWITCH:
                if (startOnRelease) newEvent = Event::RELEASED_START;
                break;
            case FOOTSWITCH2:
                newEvent = Event::RELEASED_FOCUS;
                break;
            case UP:
                newEvent = Event::RELEASED_UP;
                break;
            case DOWN:
                newEvent = Event::RELEASED_DOWN;
                break;
            case MENU:
                newEvent = Event::RELEASED_MENU;
                break;
            case ADJUSTMENT:
                newEvent = Event::RELEASED_ADJUSTMENT;
                break;
            case EXIT:
                newEvent = Event::RELEASED_EXIT;
                break;
            case TESTSTRIP:
                newEvent = Event::RELEASED_TESTSTRIP;
                break;
            default:
                break;
            }
        }
    }
    return newEvent;
}

void Keypad::setStartOnRelease(bool enabled) {
    startOnRelease = enabled;
}

bool Keypad::getStartOnRelease() {
    return startOnRelease;
}