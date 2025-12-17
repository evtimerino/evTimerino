#include <keypad.h>

PushButton::PushButton(uint8_t p) : pin(p) {
    pinMode(p, INPUT_PULLUP);
}

PushButton::~PushButton() {}

void PushButton::scan() {
    if (state == Button::PRESSED && digitalRead(pin) == HIGH) state = Button::RELEASED; 
    if (millis() - lastMainButtonStateChange > debounceDuration) {
        byte buttonState = digitalRead(pin);
        if (buttonState != lastButtonState) {
            lastMainButtonStateChange = millis();
            lastButtonState = buttonState;
            if (buttonState == LOW) {
                state = Button::PRESSED;
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
        state = Button::IDLE;
        event = Button::LONG_PRESS;
        break;
    case Button::RELEASED:
        state = Button::IDLE;
        event = Button::RELEASED;
        pressed = false;
        break;
    default:
        event = Button::IDLE;
        break;
    }
    return event;
}

Keypad::Keypad() {}

Keypad::~Keypad() {}

void Keypad::tick(){
    for (uint8_t i = 0; i < 9; i++) {
        buttons[i].scan();
        buttons[i].scanLongPress();
    }
}

Event Keypad::fetchKeypadEvent() {
    Event newEvent = Event::NO_EVENT;
    for (uint8_t i = 0; i < 9; i++) {
        Button state = buttons[i].released();
        if (state == Button::LONG_PRESS) {
            switch (i)
            {
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
                newEvent = Event::RELEASED_START;
                break;
            case FOOTSWITCH:
                newEvent = Event::RELEASED_START;
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