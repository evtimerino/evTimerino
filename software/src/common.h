#ifndef COMMON_H
#define COMMON_H

#define LOGO_DURATION 1500

#define BUZZER_PIN A2
#define RELAY_PIN A6
#define SAFELIGHT_PIN 17
#define SWITCH_PIN 18
#define FOOTSWITCH_PIN A3


#define FOCUS_PIN A1
#define START_PIN A0
#define UP_PIN D9
#define DOWN_PIN D10
#define MENU_PIN D11
#define ADJUSTMENT_PIN D12
#define EXIT_PIN D8
#define TESTSTRIP_PIN D7


#define EVENTS 26

enum class Event {
    NO_EVENT,
    RELEASED_FOCUS,
    RELEASED_START,
    RELEASED_UP,
    RELEASED_DOWN,
    RELEASED_MENU,
    RELEASED_ADJUSTMENT,
    RELEASED_EXIT,
    RELEASED_TESTSTRIP,
    LONGPRESS_FOCUS,
    LONGPRESS_EXIT,
    LONGPRESS_ADJ,
    LONGPRESS_TESTSTRIP,
    LONGPRESS_MENU,
    MOVE_TO_MAIN,
    MOVE_TO_PREPARE,
    MOVE_TO_MENU,
    MOVE_TO_PAUSE,
    MOVE_TO_TESTSTRIP,
};

enum class State {
    MAIN = 0,
    TESTSTRIP = 1,
    MENU = 2,
    ADJUSTMENT = 3,
    FOCUS = 4,
    PREPARE = 5,
    METRONOME = 6,
    PAUSE = 7,
    COUNT = 8,
};

enum class Mode {
  EXPOSURE,
  TESTSTRIP,
};

enum class Button {
  IDLE,
  PRESSED,
  LONG_PRESS,
  RELEASED
};

enum class Adjustment {
    DODGE,
    BURN,
    NONE,
};

enum class Teststrip {
  SEPARATE,
  SPLIT_GRADE,
  INCREMENTAL,
};

enum class Safelight {
  OFF,
  OFF_ON_EXPOSURE,
  ALWAYS_ON,
};

enum class Tone {
  ON,
  OFF,
};

enum class Lamp {
    ON,
    OFF,
};

#endif