#include "common.h"
#include <storage.h>

Storage::Storage(Display& o, Preferences& p, Exposure& e, Buzzer& b, Enlarger& l) : display(o), preferences(p), exposure(e), buzzer(b), enlarger(l) {
}

Storage::~Storage() {}


void Storage::load() {
    preferences.begin("data", false);
    enlarger.setPrepare(preferences.getInt("prepare", 0));
    enlarger.setLampUsage(preferences.getInt("lampUsage", 0));
    display.updateBrightness(preferences.getInt("brightness", 0));
    uint8_t safelightState = preferences.getInt("safelight", 0);
    uint8_t startTime = preferences.getInt("startTime", 1);
    switch (startTime)
    {
    case 0:
        exposure.setStartTime(2);
        break;
    case 1:
        exposure.setStartTime(3);
        break;
    case 2:
        exposure.setStartTime(4);
        break;
    case 3:
        exposure.setStartTime(5);
        break;
    case 4:
        exposure.setStartTime(6);
        break;
    default:
        break;
    }
    switch (safelightState)
    {
    case 0:
        enlarger.setSafelight(Safelight::OFF);
        break;
    case 1:
        enlarger.setSafelight(Safelight::OFF_ON_EXPOSURE);
        break;
    case 2:
        enlarger.setSafelight(Safelight::ALWAYS_ON);
        break;
    default:
        break;
    }
    uint8_t buzzerState = preferences.getInt("buzzer", 0);
    switch (buzzerState)
    {
    case 0:
        buzzer.setState(Tone::ON);
        break;
    case 1:
        buzzer.setState(Tone::OFF);
        break;
    default:
        break;
    }
    exposure.setPrecision(preferences.getInt("precision", 6));
    uint8_t testStripState = preferences.getInt("teststrip", 0);
    switch (testStripState)
    {
    case 0:
        exposure.setTestStripMode(Teststrip::SEPARATE_A);
        exposure.resetTestStrip();
        break;
    case 1:
        exposure.setTestStripMode(Teststrip::INCREMENTAL_A);
        exposure.resetTestStrip();
        break;
    case 2:
        exposure.setTestStripMode(Teststrip::SPLIT_GRADE);
        break;
    case 3:
        exposure.setTestStripMode(Teststrip::SEPARATE_B);
        break;
    case 4:
        exposure.setTestStripMode(Teststrip::INCREMENTAL_B);
        break;
    default:
        break;
    }
    lampUsageHoursCounter = preferences.getUInt("LUHours", 0);
    lampUsageMinutesCounter = preferences.getUInt("LUMinutes", 0);
    lampUsageSecondsCounter = preferences.getUInt("LUSeconds", 0);
    lampUsageTenthsCounter = preferences.getUInt("LUTenth", 0);
    preferences.end();
}

uint16_t Storage::getLampUsageHours() {
    return lampUsageHoursCounter;
}

uint8_t Storage::getLampUsageMinutes() {
    return lampUsageMinutesCounter;
}

void Storage::saveLampUsage(uint16_t usage) {
    preferences.begin("data", false);
    uint8_t minutes = usage / 600;
    uint8_t seconds = usage % 600;
    uint8_t tenths = seconds % 10;
    if (lampUsageTenthsCounter + tenths >= 10) {
        lampUsageSecondsCounter++;
        lampUsageTenthsCounter = (lampUsageTenthsCounter + tenths) - 10;
    } else {
        lampUsageTenthsCounter += tenths;
    }
    if (lampUsageSecondsCounter + seconds >= 60) {
        lampUsageMinutesCounter++;
        lampUsageSecondsCounter = (lampUsageSecondsCounter + seconds) - 60;
    } else {
        lampUsageSecondsCounter += seconds;
    }
    if (lampUsageMinutesCounter + minutes >= 60 && lampUsageHoursCounter < 999) {
        lampUsageHoursCounter++;
        lampUsageMinutesCounter = (lampUsageMinutesCounter + minutes) - 60;
    } else {
        lampUsageMinutesCounter += minutes;
    }
    preferences.putUInt("LUHours", lampUsageHoursCounter);
    preferences.putUInt("LUMinutes", lampUsageMinutesCounter);
    preferences.putUInt("LUSeconds", lampUsageSecondsCounter);
    preferences.putUInt("LUTenth", lampUsageTenthsCounter);
    preferences.end();
}

void Storage::storeLampUsage(uint8_t l) {
    preferences.begin("data", false);
    preferences.putInt("lampUsage", l);
    preferences.end();
}

void Storage::storePrecision(uint8_t p) {
    preferences.begin("data", false);
    preferences.putInt("precision", p);
    preferences.end();
}

void Storage::storeTestStripMode(uint8_t t) {
    preferences.begin("data", false);
    preferences.putInt("teststrip", t);
    preferences.end();
}

void Storage::storeSafelight(uint8_t s) {
    preferences.begin("data", false);
    preferences.putInt("safelight", s);
    preferences.end();
}

void Storage::storeStartTime(uint8_t s) {
    preferences.begin("data", false);
    preferences.putInt("startTime", s);
    preferences.end();
}

void Storage::storeBuzzer(uint8_t b) {
    preferences.begin("data", false);
    preferences.putInt("buzzer", b);
    preferences.end();
}

void Storage::storePrepare(uint8_t p) {
    preferences.begin("data", false);
    preferences.putInt("prepare", p);
    preferences.end();
}

void Storage::storeBrightness(uint8_t b) {
    preferences.begin("data", false);
    preferences.putInt("brightness", b);
    preferences.end();
}

void Storage::resetLampUsage() {
    preferences.begin("data", false);
    lampUsageHoursCounter = 0;
    lampUsageMinutesCounter = 0;
    lampUsageSecondsCounter = 0;
    lampUsageTenthsCounter = 0;
    preferences.putUInt("LUHours", lampUsageHoursCounter);
    preferences.putUInt("LUMinutes", lampUsageMinutesCounter);
    preferences.putUInt("LUSeconds", lampUsageSecondsCounter);
    preferences.putUInt("LUTenth", lampUsageTenthsCounter);
    preferences.end();
}