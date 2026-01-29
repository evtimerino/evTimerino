#include <storage.h>

Storage::Storage(Display& o, Preferences& p, Exposure& e, Buzzer& b, Enlarger& l) : display(o), preferences(p), exposure(e), buzzer(b), enlarger(l) {
}

Storage::~Storage() {}


void Storage::load() {
    preferences.begin("data", false);
    enlarger.setPrepare(preferences.getBool("prepare", false));
    enlarger.setLampUsage(preferences.getBool("lampUsage", false));
    display.updateBrightness(preferences.getInt("brightness", 0));
    uint8_t safelightState = preferences.getInt("safelight", 0);
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
        buzzer.setState(Tone::OFF);
        break;
    case 1:
        buzzer.setState(Tone::ON);
        break;
    default:
        break;
    }
    exposure.setPrecision(preferences.getInt("precision", 6));
    uint8_t testStripState = preferences.getInt("teststrip", 0);
    switch (testStripState)
    {
    case 0:
        exposure.setTestStripMode(Teststrip::SEPARATE);
        break;
    case 1:
        exposure.setTestStripMode(Teststrip::SPLIT_GRADE);
        break;
    case 2:
        exposure.setTestStripMode(Teststrip::INCREMENTAL);
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


void Storage::save() {
    preferences.begin("data", false);
    preferences.putInt("precision", exposure.getPrecisionIdx());
    preferences.putBool("prepare", enlarger.getPrepare());
    preferences.putBool("lampUsage", enlarger.getLampUsage());
    preferences.putInt("brightness", display.getBrightnessLevel());
    switch (enlarger.getSafelight())
    {
    case Safelight::OFF:
        preferences.putInt("safelight", 0);
        break;
    case Safelight::OFF_ON_EXPOSURE:
        preferences.putInt("safelight", 1);
        break;
    case Safelight::ALWAYS_ON:
        preferences.putInt("safelight", 2);
        break;
    default:
        break;
    }
    switch (buzzer.getState())
    {
    case Tone::OFF:
        preferences.putInt("buzzer", 0);
        break;
    case Tone::ON:
        preferences.putInt("buzzer", 1);
        break;
    default:
        break;
    }
    switch (exposure.getTestStripMode())
    {
    case Teststrip::SEPARATE:
        preferences.putInt("teststrip", 0);
        break;
    case Teststrip::SPLIT_GRADE:
        preferences.putInt("teststrip", 1);
        break;
    case Teststrip::INCREMENTAL:
        preferences.putInt("teststrip", 2);
        break;
    default:
        break;
    }
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
    if (lampUsageMinutesCounter + minutes >= 60) {
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