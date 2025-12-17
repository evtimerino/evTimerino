#include <Arduino.h>
#include <common.h>

#ifndef KEYPAD_H
#define KEYPAD_H

class PushButton {
private:
    
    Button state = Button::IDLE;
    uint16_t longPressMS = 800;
    bool pressed = false;
    unsigned long timePressed = 0;

    uint8_t pin;
    byte lastButtonState = LOW;
    unsigned long debounceDuration = 20; 
    unsigned long lastMainButtonStateChange = 0;
    
public:
    PushButton(uint8_t p);
    ~PushButton();
    void scan();
    void scanLongPress();
    Button released();
};

class Keypad 
{
private:
    
    enum {
        FOCUS,
        START,
        FOOTSWITCH,
        UP,
        DOWN,
        MENU,
        ADJUSTMENT,
        EXIT,
        TESTSTRIP
    };

    PushButton focus = PushButton(FOCUS_PIN);
    PushButton start = PushButton(START_PIN);
    PushButton footswitch = PushButton(FOOTSWITCH_PIN);
    PushButton up = PushButton(UP_PIN);
    PushButton down = PushButton(DOWN_PIN);
    PushButton precision = PushButton(MENU_PIN);
    PushButton adjustment = PushButton(ADJUSTMENT_PIN);
    PushButton exit = PushButton(EXIT_PIN);
    PushButton teststrip = PushButton(TESTSTRIP_PIN);
    PushButton buttons[9] = {focus, start, footswitch, up, down, precision, adjustment, exit, teststrip}; 
public:
    Event fetchKeypadEvent();
    void tick();
    Keypad();
    ~Keypad();
    
};

#endif