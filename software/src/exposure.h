#include <Arduino.h>
#include <common.h>
#include <buzzer.h>

#define MAX_ADJUSTMENTS 20

#ifndef EXPOSURE_H
#define EXPOSURE_H

struct node {
    Adjustment type;
    uint8_t area;
    uint8_t value;
    uint16_t timeCounter;
    node *next;
};


class Exposure
{
private:

    Buzzer& buzzer;

    Mode mode = Mode::EXPOSURE;

    bool baseTime = true;
    uint16_t baseTimeCounter = 160;
    uint16_t baseTimeCounterAdjusted = 160;
    
    const uint8_t precisions[10] = {2, 3, 4, 6, 8, 12, 16, 24, 32, 48};
    int16_t steps = 24;
    uint8_t precision = 6;
    uint8_t newPrecisionIdx = 3;
    uint8_t precisionIdx = 3;
    uint8_t precisionMultiplier = 4;
    bool splitState = true;

    
    uint8_t size;
    uint16_t dodgeTimeCounters;
    uint8_t dodgeSize;
    uint8_t burnSize;
    node *head;
    node *tail;
    node *dodgeItr;
    node *newAdj;
    node *itr;
    uint8_t position = 0;
    void updateAdjustments();

    int8_t teststripSteps = 0;
    Teststrip teststripMode = Teststrip::SEPARATE_A;

    uint16_t linearBaseTimeCounter = 100;
    LinearPrecision linearPrecision = LinearPrecision::SECONDS;

    public:
    Exposure(Buzzer& b);
    ~Exposure();
    
    void setMode(Mode m);
    void clear();
    void remove();
    void reset();
    void resetNewAdj();
    void restart();
    uint16_t getTimeCounter();
    uint16_t getBaseTimeCounter();
    uint8_t getValue();
    uint8_t getArea();
    void next();
    void resetIndex();
    void setBaseExposureUp();
    void setBaseExposureDown();
    void nextAdjustment();
    void setAdjustmentUp();
    void setAdjustmentDown();
    void saveAdjustment();
    uint8_t getDodgeSize();
    uint8_t getBurnSize();
    uint8_t getPrecision();
    uint16_t getAdjustmentTimeCounter();
    uint8_t getAdjustmentValue();
    Adjustment getAdjustmentType();
    Adjustment getNewAdjustmentType();
    uint8_t getAdjustmentArea();
    Mode getMode();
    bool isNewAdjustment();
    bool isNewAdjustmentBurn();
    bool getBaseTime();
    void setBaseTime();
    void setPrecision(uint8_t i);
    uint16_t getTestStripTimeCounter();
    void resetTestStrip();
    void testStripNext();
    int8_t getTestStripSteps();
    void setTestStripMode(Teststrip m);
    void resetBaseTime();
    void switchBaseTime();
    Teststrip getTestStripMode();
    uint16_t getNewAdjustmentTimeCounter();
    uint8_t getNewAdjustmentValue();
    uint8_t getPrecisionIdx();
    uint8_t getSize();
    void setStartTime(uint8_t st, bool runtime = false);
    uint8_t getPrecisionMultiplier();
    void setPrecisionMultiplier(uint8_t s);
    void switchTestStripMode();
    void setPrecisionUp();
    void setPrecisionDown();
    void updatePrecision();
    uint8_t getNewPrecision();
    void splitSteps();
    void setLinearUp();
    void setLinearDown();
    void switchLinearPrecision();
    void resetLinearTimeCounter();
    uint16_t getLinearTimeCounter();
    LinearPrecision getLinearPrecision();
};

#endif