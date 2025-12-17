#include <U8g2lib.h>
#include <common.h>

#ifndef DISPLAY_H
#define DISPLAY_H

class Display
{
private:
    U8G2_SSD1309_128X64_NONAME0_F_HW_I2C &oled;

    bool powerSaveState = false;
    uint8_t brightnessLevel = 0;

    enum
    {
        SegA = 0x01,
        SegB = 0x02,
        SegC = 0x04,
        SegD = 0x08,
        SegE = 0x10,
        SegF = 0x20,
        SegG = 0x40,
    };

    int segDigit[10] = {
        SegA | SegB | SegC | SegD | SegE | SegF,        // 0
        SegB | SegC,                                    // 1
        SegA | SegB | SegD | SegE | SegG,               // 2
        SegA | SegB | SegC | SegD | SegG,               // 3
        SegB | SegC | SegF | SegG,                      // 4
        SegA | SegC | SegD | SegF | SegG,               // 5
        SegA | SegC | SegD | SegE | SegF | SegG,        // 6
        SegA | SegB | SegC,                             // 7
        SegA | SegB | SegC | SegD | SegE | SegF | SegG, // 8
        SegA | SegB | SegC | SegD | SegF | SegG,        // 9
    };

    uint8_t menuSelection = 2;

    void drawDigit(uint8_t x, uint8_t y, uint8_t digit);
    void drawSmallDigit(uint8_t x, uint8_t y, uint8_t digit);
    void drawSign(uint8_t x, uint8_t y, bool positive);
    void drawAdj(uint8_t x, uint8_t y, uint8_t value);
    void setSSD1309VcomDeselect(uint8_t v);
    void setSSD1309PreChargePeriod(uint8_t p1, uint8_t p2);

public:
    Display(U8G2_SSD1309_128X64_NONAME0_F_HW_I2C &o);
    ~Display();
    void drawLogo();
    void drawMain(uint16_t timeCounter, uint8_t precision, uint8_t dodgeCount, uint8_t burnCount, bool prepare);
    void drawAdjustment(Adjustment type, uint16_t timeCounter, uint8_t value, uint8_t area);
    void drawNewAdjustment(uint16_t timeCounter, int8_t value, Adjustment type, uint8_t number);
    void drawTestStrip(Teststrip mode, uint16_t timeCounter, uint8_t testStripIdx);
    void drawTestStripSplitGrade(uint16_t timeCounter, uint8_t testStripIdx);
    void drawPrecision(uint8_t precision);
    void drawExposure(uint16_t timeCounter);
    void drawMetronome(uint16_t timeCounter);
    void drawPrepare();
    void drawFocus();
    void drawPause();
    void powerSave(bool activate);
    void clearDisplay();
    void menuSelectionUp();
    void menuSelectionDown();
    void updateBrightness(uint8_t level);
    uint8_t getBrightnessLevel();
};

#endif