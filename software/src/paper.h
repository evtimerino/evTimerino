#include <arduino.h>
#include <buzzer.h>
#include <display.h>

#ifndef PAPER_H
#define PAPER_H


class Paper {
    private:
        Buzzer& buzz;
        Display& display;
        bool enabled = true;
        Dev state = Dev::OFF;
        DevType devType = DevType::DEVELOPER;
        uint16_t devTimeCounter = 300;
        uint16_t stopTimeCounter = 300;
        uint16_t fixerTimeCounter = 300;
        uint16_t timeCounter = 0;
        bool devTimeCounterActive = false;
        bool stopTimeCounterActive = false;
        bool fixerTimeCounterActive = false;
        unsigned long currentMillis;
        unsigned long previousMillis = 0;
        uint16_t lastAnnouncedTimeCounter = 0xFFFF;
        uint16_t lastSecondAnnouncedTimeCounter = 0xFFFF;
        bool cycleFinished = false;
        bool factorial = false;
        bool factorialSamplingActive = false;
        uint8_t factor[9] = {2, 3, 4, 5, 6, 7, 8, 9, 10};
        uint8_t factorIdx = 0;
        uint32_t devElapsedTicks = 0;
        uint16_t getCurrentDevTargetTicks();
        void recalcDevRemainingFromElapsed();
    public:
        Paper(Buzzer& b, Display& d);
        ~Paper();
        void run();
        void startDevelopment();
        void handleStartPress();
        void setEnabled(bool state);
        void setDevTimeCounterUp();
        void setDevTimeCounterDown();
        void setDevTimeCounter(uint16_t t);
        void setStopTimeCounterUp();
        void setStopTimeCounterDown();
        void setStopTimeCounter(uint16_t t);
        void setFixerTimeCounterUp();
        void setFixerTimeCounterDown();
        void setFixerTimeCounter(uint16_t t);
        void setFactorIdxUp();
        void setFactorIdxDown();
        void setFactorial(bool state);
        bool getFactorial();
        void setFactor(uint8_t f);
        uint8_t getFactor();
        uint16_t getTimeCounter();
        uint16_t getDevTimeCounter();
        uint16_t getStopTimeCounter();
        uint16_t getFixerTimeCounter();
        bool getEnabled();
        Dev getState();
        void reset();
        bool getPaperDevelopmentActive();
        bool consumeCycleFinished();
        DevType getDevType();
}; 
#endif