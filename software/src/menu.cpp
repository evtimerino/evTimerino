#include <menu.h>


namespace TimerMenu {

    bool isUpdate = false;

    uint8_t precisionPreviousSelection = 3;
    uint8_t precisionCurrentSelection = 3;
    uint8_t teststripPreviousSelection = 0;
    uint8_t teststripCurrentSelection = 0;
    uint8_t safelightPreviousSelection = 0;
    uint8_t safelightCurrentSelection = 0;
    uint8_t startTimePreviousSelection = 0;
    uint8_t startTimeCurrentSelection = 0;
    uint8_t startTriggerPreviousSelection = 1;
    uint8_t startTriggerCurrentSelection = 1;
    uint8_t buzzerPreviousSelection = 0;
    uint8_t buzzerCurrentSelection = 0;
    uint8_t preparePreviousSelection = 0;
    uint8_t prepareCurrentSelection = 0;
    uint8_t lampUsagePreviousSelection = 0;
    uint8_t lampUsageCurrentSelection = 0;
    uint8_t brightnessPreviousSelection = 0;
    uint8_t brightnessCurrentSelection = 0;
    uint8_t paperEnabledPreviousSelection = 1;
    uint8_t paperEnabledCurrentSelection = 1;
    uint8_t paperDevMinutesPreviousSelection = 0;
    uint8_t paperDevMinutesCurrentSelection = 0;
    uint8_t paperDevSecondsPreviousSelection = 30;
    uint8_t paperDevSecondsCurrentSelection = 30;
    uint8_t paperStopMinutesPreviousSelection = 0;
    uint8_t paperStopMinutesCurrentSelection = 0;
    uint8_t paperStopSecondsPreviousSelection = 30;
    uint8_t paperStopSecondsCurrentSelection = 30;
    uint8_t paperFixerMinutesPreviousSelection = 0;
    uint8_t paperFixerMinutesCurrentSelection = 0;
    uint8_t paperFixerSecondsPreviousSelection = 30;
    uint8_t paperFixerSecondsCurrentSelection = 30;

    static uint16_t paperCounterFromParts(uint8_t minutesSelection, uint8_t secondsSelection) {
        if (minutesSelection > 3) minutesSelection = 3;
        if (secondsSelection > 59) secondsSelection = 59;
        return static_cast<uint16_t>((static_cast<uint16_t>(minutesSelection) * 60) + secondsSelection) * 10;
    }

    static void paperCounterToParts(uint16_t t, uint8_t& minutesSelection, uint8_t& secondsSelection) {
        minutesSelection = static_cast<uint8_t>(t / 600);
        if (minutesSelection > 3) minutesSelection = 3;
        secondsSelection = static_cast<uint8_t>((t % 600) / 10);
        if (secondsSelection > 59) secondsSelection = 59;
    }


    uint8_t muiHrule(mui_t *ui, uint8_t msg)
    {
        u8g2_t *u8g2 = mui_get_U8g2(ui);
        switch (msg)
        {
        case MUIF_MSG_DRAW:
            u8g2_DrawHLine(u8g2, 0, mui_get_y(ui), u8g2_GetDisplayWidth(u8g2));
            break;
        }
        return 0;
    }
    
    muif_t muif_list[] = {  
        MUIF_U8G2_FONT_STYLE(0, u8g2_font_helvR08_tr),        /* regular font */
        MUIF_U8G2_FONT_STYLE(1, u8g2_font_helvB08_tr),        /* bold font */

        MUIF_RO("HR", muiHrule),
        MUIF_U8G2_LABEL(),

        MUIF_RO("GP", mui_u8g2_goto_data),
        MUIF_BUTTON("GC", mui_u8g2_goto_form_w1_pi),

        MUIF_BUTTON("G0", mui_u8g2_btn_goto_wm_fi),

        MUIF_VARIABLE("RS", &precisionCurrentSelection, mui_u8g2_u8_radio_wm_pi),
        MUIF_VARIABLE("RT", &teststripCurrentSelection, mui_u8g2_u8_radio_wm_pi),
        MUIF_VARIABLE("RU", &safelightCurrentSelection, mui_u8g2_u8_radio_wm_pi),
        MUIF_VARIABLE("RP", &startTimeCurrentSelection, mui_u8g2_u8_radio_wm_pi),
        MUIF_VARIABLE("RX", &startTriggerCurrentSelection, mui_u8g2_u8_radio_wm_pi),
        MUIF_VARIABLE("RB", &buzzerCurrentSelection, mui_u8g2_u8_radio_wm_pi),
        MUIF_VARIABLE("RE", &prepareCurrentSelection, mui_u8g2_u8_radio_wm_pi),
        MUIF_VARIABLE("RC", &lampUsageCurrentSelection, mui_u8g2_u8_radio_wm_pi),
        MUIF_VARIABLE("RV", &brightnessCurrentSelection, mui_u8g2_u8_radio_wm_pi),
        MUIF_VARIABLE("RW", &paperEnabledCurrentSelection, mui_u8g2_u8_radio_wm_pi),
        MUIF_U8G2_U8_MIN_MAX("Q1", &paperDevMinutesCurrentSelection, 0, 3, mui_u8g2_u8_min_max_wm_mse_pi),
        MUIF_U8G2_U8_MIN_MAX("Q2", &paperDevSecondsCurrentSelection, 0, 59, mui_u8g2_u8_min_max_wm_mse_pi),
        MUIF_U8G2_U8_MIN_MAX("Q3", &paperStopMinutesCurrentSelection, 0, 3, mui_u8g2_u8_min_max_wm_mse_pi),
        MUIF_U8G2_U8_MIN_MAX("Q4", &paperStopSecondsCurrentSelection, 0, 59, mui_u8g2_u8_min_max_wm_mse_pi),
        MUIF_U8G2_U8_MIN_MAX("Q5", &paperFixerMinutesCurrentSelection, 0, 3, mui_u8g2_u8_min_max_wm_mse_pi),
        MUIF_U8G2_U8_MIN_MAX("Q6", &paperFixerSecondsCurrentSelection, 0, 59, mui_u8g2_u8_min_max_wm_mse_pi),
    };

    fds_t fds_data[] = 
        MUI_FORM(1)
        MUI_STYLE(1)
        MUI_LABEL(5,10, "Settings")
        MUI_XY("HR", 0,13)
        MUI_STYLE(0)

        MUI_DATA("GP", 
            MUI_10 "Precision|"
            MUI_11 "Test Strip|"
            MUI_19 "Paper Dev|"
            MUI_12 "Brightness|"
            MUI_13 "Safelight|"
            MUI_14 "Start Time|"
            MUI_24 "Start Trigger|"
            MUI_15 "Buzzer|"
            MUI_16 "Prepare|"
            MUI_17 "Lamp Usage|"
            MUI_18 "Firmware Version|"
            )
        MUI_XYA("GC", 5, 25, 0) 
        MUI_XYA("GC", 5, 37, 1) 
        MUI_XYA("GC", 5, 49, 2) 
        MUI_XYA("GC", 5, 61, 3) 

        MUI_FORM(10)
        MUI_STYLE(1)
        MUI_LABEL(5, 8, "Precision")
        MUI_XY("HR", 0,11)
        MUI_STYLE(0)
        MUI_XYAT("RS",5, 22, 0 , "1/2")
        MUI_XYAT("RS",5, 32, 1, "1/3")
        MUI_XYAT("RS",5, 42, 2, "1/4")
        MUI_XYAT("RS",5, 52, 3, "1/6")
        MUI_XYAT("G0", 114, 60, 1, " OK ")
    
        MUI_FORM(11)
        MUI_STYLE(1)
        MUI_LABEL(5, 8, "Test Strip")
        MUI_XY("HR", 0,11)
        MUI_STYLE(0)
        MUI_XYAT("RT", 5, 22, 0, "Separate A")
        MUI_XYAT("RT", 5, 32, 1, "Incremental A")
        MUI_XYAT("RT", 5, 42, 2, "Split Grade")
        MUI_XYAT("RT", 5, 52, 3, "Separate B")
        MUI_XYAT("RT", 5, 62, 4, "Incremental B")
        MUI_XYAT("G0", 114, 60, 1, " OK ")

        MUI_FORM(12)
        MUI_STYLE(1)
        MUI_LABEL(5, 8, "Brightness")
        MUI_XY("HR", 0,11)
        MUI_STYLE(0)
        MUI_XYAT("RV", 5, 22, 0, "Low")
        MUI_XYAT("RV", 5, 32, 1, "Medium Low")
        MUI_XYAT("RV", 5, 42, 2, "Medium High")
        MUI_XYAT("RV", 5, 52, 3, "High")
        MUI_XYAT("G0", 114, 60, 1, " OK ")

        MUI_FORM(13)
        MUI_STYLE(1)
        MUI_LABEL(5, 8, "Safelight")
        MUI_XY("HR", 0,11)
        MUI_STYLE(0)
        MUI_XYAT("RU", 5, 22, 0, "Off")
        MUI_XYAT("RU", 5, 32, 1, "Off on Exposure")
        MUI_XYAT("RU", 5, 42, 2, "Always On")
        MUI_XYAT("G0", 114, 60, 1, " OK ")

        MUI_FORM(14)
        MUI_STYLE(1)
        MUI_LABEL(5, 8, "Start Time")
        MUI_XY("HR", 0,11)
        MUI_STYLE(0)
        MUI_XYAT("RP", 5, 22, 0, "4 sec")
        MUI_XYAT("RP", 5, 32, 1, "8 sec")
        MUI_XYAT("RP", 5, 42, 2, "16 sec")
        MUI_XYAT("RP", 5, 52, 3, "32 sec")
        MUI_XYAT("RP", 5, 62, 4, "64 sec")
        MUI_XYAT("G0", 114, 60, 1, " OK ")

        MUI_FORM(24)
        MUI_STYLE(1)
        MUI_LABEL(5, 8, "Start Trigger")
        MUI_XY("HR", 0,11)
        MUI_STYLE(0)
        MUI_XYAT("RX", 5, 22, 0, "On Press")
        MUI_XYAT("RX", 5, 32, 1, "On Release")
        MUI_XYAT("G0", 114, 60, 1, " OK ")

        MUI_FORM(15)
        MUI_STYLE(1)
        MUI_LABEL(5, 8, "Buzzer")
        MUI_XY("HR", 0,11)
        MUI_STYLE(0)
        MUI_XYAT("RB", 5, 22, 0, "Off")
        MUI_XYAT("RB", 5, 32, 1, "On")
        MUI_XYAT("G0", 114, 60, 1, " OK ")

        MUI_FORM(16)
        MUI_STYLE(1)
        MUI_LABEL(5, 8, "Prepare")
        MUI_XY("HR", 0,11)
        MUI_STYLE(0)
        MUI_XYAT("RE", 5, 22, 0, "Off")
        MUI_XYAT("RE", 5, 32, 1, "On")
        MUI_XYAT("G0", 114, 60, 1, " OK ")

        
        MUI_FORM(17)
        MUI_STYLE(1)
        MUI_LABEL(5, 8, "Lamp Usage")
        MUI_XY("HR", 0,11)
        MUI_STYLE(0)
        MUI_XYAT("RC", 5, 22, 0, "Off")
        MUI_XYAT("RC", 5, 32, 1, "On")
        MUI_XYAT("G0", 114, 60, 1, " OK ")

        MUI_FORM(18)
        MUI_STYLE(1)
        MUI_LABEL(5, 8, "Firmware version")
        MUI_XY("HR", 0,11)
        MUI_STYLE(0)
        MUI_LABEL(45, 40, "1.3.0-RC01")
        MUI_XYAT("G0", 114, 60, 1, " OK ")

        MUI_FORM(19)
        MUI_STYLE(1)
        MUI_LABEL(5, 8, "Paper Dev")
        MUI_XY("HR", 0,11)
        MUI_STYLE(0)
        MUI_DATA("GP", 
            MUI_20 "Enabled|"
            MUI_21 "Dev Time|"
            MUI_22 "Stop Time|"
            MUI_23 "Fixer Time|"
            )
        MUI_XYA("GC", 5, 25, 0)
        MUI_XYA("GC", 5, 37, 1)
        MUI_XYA("GC", 5, 49, 2)
        MUI_XYA("GC", 5, 61, 3)

        MUI_FORM(20)
        MUI_STYLE(1)
        MUI_LABEL(5, 8, "Paper Enabled")
        MUI_XY("HR", 0,11)
        MUI_STYLE(0)
        MUI_XYAT("RW", 5, 22, 0, "Off")
        MUI_XYAT("RW", 5, 32, 1, "On")
        MUI_XYAT("G0", 100, 60, 19, " OK ")

        MUI_FORM(21)
        MUI_STYLE(1)
        MUI_LABEL(5, 8, "Dev Time")
        MUI_XY("HR", 0,11)
        MUI_STYLE(0)
        MUI_LABEL(5, 24, "Minutes")
        MUI_XY("Q1", 70, 24)
        MUI_LABEL(5, 40, "Seconds")
        MUI_XY("Q2", 70, 40)
        MUI_LABEL(5, 52, "Range: 0-59")
        MUI_XYAT("G0", 100, 60, 19, " OK ")

        MUI_FORM(22)
        MUI_STYLE(1)
        MUI_LABEL(5, 8, "Stop Time")
        MUI_XY("HR", 0,11)
        MUI_STYLE(0)
        MUI_LABEL(5, 24, "Minutes")
        MUI_XY("Q3", 70, 24)
        MUI_LABEL(5, 40, "Seconds")
        MUI_XY("Q4", 70, 40)
        MUI_LABEL(5, 52, "Range: 0-59")
        MUI_XYAT("G0", 100, 60, 19, " OK ")

        MUI_FORM(23)
        MUI_STYLE(1)
        MUI_LABEL(5, 8, "Fixer Time")
        MUI_XY("HR", 0,11)
        MUI_STYLE(0)
        MUI_LABEL(5, 24, "Minutes")
        MUI_XY("Q5", 70, 24)
        MUI_LABEL(5, 40, "Seconds")
        MUI_XY("Q6", 70, 40)
        MUI_LABEL(5, 52, "Range: 0-59")
        MUI_XYAT("G0", 100, 60, 19, " OK ")
    ;


    Menu::Menu(Keypad& k, Exposure& e, U8G2_SSD1309_128X64_NONAME0_F_HW_I2C& o, Enlarger& l, Buzzer& b, Display& d, Storage& s, Paper& p) : keypad(k), exposure(e), oled(o), enlarger(l), buzzer(b), display(d), storage(s), paper(p) {}
    Menu::~Menu() {}

    void Menu::setup() {
        mui.begin(oled, fds_data, muif_list, sizeof(muif_list)/sizeof(muif_t));

        precisionCurrentSelection = exposure.getPrecisionIdx();
        precisionPreviousSelection = precisionCurrentSelection;
        switch (exposure.getTestStripMode())
        {
        case Teststrip::SEPARATE_A:
            teststripPreviousSelection = 0;
            teststripCurrentSelection = 0;
            break;
        case Teststrip::INCREMENTAL_A:
            teststripPreviousSelection = 1;
            teststripCurrentSelection = 1;
            break;
        case Teststrip::SPLIT_GRADE:
            teststripPreviousSelection = 2;
            teststripCurrentSelection = 2;
            break;        
        case Teststrip::SEPARATE_B:
            teststripPreviousSelection = 3;
            teststripCurrentSelection = 3;
            break;
        case Teststrip::INCREMENTAL_B:
            teststripPreviousSelection = 4;
            teststripCurrentSelection = 4;
            break;
        default:
            break;
        }
        switch (enlarger.getSafelight())
        {
        case Safelight::OFF:
            safelightPreviousSelection = 0;
            safelightCurrentSelection = 0;
            break;
        case Safelight::OFF_ON_EXPOSURE:
            safelightPreviousSelection = 1;
            safelightCurrentSelection = 1;
            break;
        case Safelight::ALWAYS_ON:
            safelightPreviousSelection = 2;
            safelightCurrentSelection = 2;
            break;
        default:
            break;
        }

        if (!enlarger.getPrepare())
        {
            prepareCurrentSelection = 0;
            preparePreviousSelection = 0;
        } else {
            prepareCurrentSelection = 1;
            preparePreviousSelection = 1;
        }

        if (!enlarger.getLampUsage())
        {
            lampUsageCurrentSelection = 0;
            lampUsagePreviousSelection = 0;
        } else {
            lampUsageCurrentSelection = 1;
            lampUsagePreviousSelection = 1;
        }

        switch (buzzer.getState())
        {
        case Tone::OFF:
            buzzerPreviousSelection = 0;
            buzzerCurrentSelection = 0;
            break;
        case Tone::ON:
            buzzerPreviousSelection = 1;
            buzzerCurrentSelection = 1;
            break;
        default:
            break;
        }
        switch (exposure.getPrecisionMultiplier())
        {
        case 2:
            startTimePreviousSelection = 0;
            startTimeCurrentSelection = 0;
            break;
        case 3:
            startTimePreviousSelection = 1;
            startTimeCurrentSelection = 1;
            break;
        case 4:
            startTimePreviousSelection = 2;
            startTimeCurrentSelection = 2;
            break;
        case 5:
            startTimePreviousSelection = 3;
            startTimeCurrentSelection = 3;
            break;
        case 6:
            startTimePreviousSelection = 4;
            startTimeCurrentSelection = 4;
            break;
        default:
            break;
        }

        if (keypad.getStartOnRelease()) {
            startTriggerPreviousSelection = 1;
            startTriggerCurrentSelection = 1;
        } else {
            startTriggerPreviousSelection = 0;
            startTriggerCurrentSelection = 0;
        }

        brightnessCurrentSelection = display.getBrightnessLevel();
        brightnessPreviousSelection = brightnessCurrentSelection;

        paperEnabledCurrentSelection = paper.getEnabled() ? 1 : 0;
        paperEnabledPreviousSelection = paperEnabledCurrentSelection;
        paperCounterToParts(paper.getDevTimeCounter(), paperDevMinutesCurrentSelection, paperDevSecondsCurrentSelection);
        paperDevMinutesPreviousSelection = paperDevMinutesCurrentSelection;
        paperDevSecondsPreviousSelection = paperDevSecondsCurrentSelection;
        paperCounterToParts(paper.getStopTimeCounter(), paperStopMinutesCurrentSelection, paperStopSecondsCurrentSelection);
        paperStopMinutesPreviousSelection = paperStopMinutesCurrentSelection;
        paperStopSecondsPreviousSelection = paperStopSecondsCurrentSelection;
        paperCounterToParts(paper.getFixerTimeCounter(), paperFixerMinutesCurrentSelection, paperFixerSecondsCurrentSelection);
        paperFixerMinutesPreviousSelection = paperFixerMinutesCurrentSelection;
        paperFixerSecondsPreviousSelection = paperFixerSecondsCurrentSelection;
    }

    void Menu::goToForm(uint8_t id, uint8_t pos) {
        mui.gotoForm(id, pos);
        isReDraw = true;
    }

    void Menu::loop() {
        while (event != Event::RELEASED_EXIT && event != Event::LONGPRESS_EXIT) {
            keypad.tick();
            event = keypad.fetchKeypadEvent();
            if (event == Event::LONGPRESS_MENU) tone(BUZZER_PIN, 500, 100);

            refreshBrightness();

            oled.setFont(u8g2_font_helvR08_tr);
            if (mui.isFormActive()) {
                if ( isReDraw ) {
                    oled.firstPage();
                    do {
                        mui.draw();
                    } while ( oled.nextPage() );
                    isReDraw = false;
                }
                switch (event)
                {
                case Event::RELEASED_TESTSTRIP:
                    tone(BUZZER_PIN, 500, 100);
                    break;
                case Event::RELEASED_MENU:
                    mui.sendSelect();
                    isReDraw = true;
                    break;
                case Event::RELEASED_DOWN:
                    mui.nextField();
                    isReDraw = true;
                    break;
                case Event::RELEASED_UP:
                    mui.prevField();
                    isReDraw = true;
                    break;
                default:
                    break;
                }
            } else {
                goToForm(0, 0);
            }
        }
        goToForm(1, 0);
        update();
        event = Event::NO_EVENT;
    }

    void Menu::update() {
        if (precisionCurrentSelection != precisionPreviousSelection) {
            exposure.setPrecision(precisionCurrentSelection);
            exposure.clear();
            precisionPreviousSelection = precisionCurrentSelection;
            storage.storePrecision(precisionCurrentSelection);
            isUpdate = true;
        }
        if (teststripCurrentSelection != teststripPreviousSelection) {
            switch (teststripCurrentSelection)
            {
            case 0:
                exposure.setTestStripMode(Teststrip::SEPARATE_A);
                break;
            case 1:
                exposure.setTestStripMode(Teststrip::INCREMENTAL_A);
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
            storage.storeTestStripMode(teststripCurrentSelection);
            exposure.resetTestStrip();
            teststripPreviousSelection = teststripCurrentSelection;
            isUpdate = true;
        }
        if (safelightCurrentSelection != safelightPreviousSelection) {
            switch (safelightCurrentSelection)
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
            storage.storeSafelight(safelightCurrentSelection);
            safelightPreviousSelection = safelightCurrentSelection;
            isUpdate = true;
        }
        if (startTimeCurrentSelection != startTimePreviousSelection) {
            switch (startTimeCurrentSelection)
            {
            case 0:
                exposure.setPrecisionMultiplier(2);
                break;
            case 1:
                exposure.setPrecisionMultiplier(3);
                break;
            case 2:
                exposure.setPrecisionMultiplier(4);
                break;
            case 3:
                exposure.setPrecisionMultiplier(5);
                break;
            case 4:
                exposure.setPrecisionMultiplier(6);
                break;
            default:
                break;
            }
            storage.storeStartTime(startTimeCurrentSelection);
            startTimePreviousSelection = startTimeCurrentSelection;
            isUpdate = true;
        }
        if (startTriggerCurrentSelection != startTriggerPreviousSelection) {
            bool startOnRelease = (startTriggerCurrentSelection == 1);
            keypad.setStartOnRelease(startOnRelease);
            storage.storeStartTrigger(startTriggerCurrentSelection);
            startTriggerPreviousSelection = startTriggerCurrentSelection;
            isUpdate = true;
        }
        if (buzzerCurrentSelection != buzzerPreviousSelection) {
            switch (buzzerCurrentSelection)
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
            storage.storeBuzzer(buzzerCurrentSelection);
            buzzerPreviousSelection = buzzerCurrentSelection;
            isUpdate = true;
        }
        if (prepareCurrentSelection != preparePreviousSelection) {
            switch (prepareCurrentSelection)
            {
            case 0:
                enlarger.setPrepare(false);
                break;
            case 1:
                enlarger.setPrepare(true);
                break;
            default:
                break;
            }
            storage.storePrepare(prepareCurrentSelection);
            preparePreviousSelection = prepareCurrentSelection;
            isUpdate = true;
        }

        if (lampUsageCurrentSelection != lampUsagePreviousSelection) {
            switch (lampUsageCurrentSelection)
            {
            case 0:
                enlarger.setLampUsage(false);
                break;
            case 1:
                enlarger.setLampUsage(true);          
                break;
            default:
                break;
            }
            storage.storeLampUsage(lampUsageCurrentSelection);
            lampUsagePreviousSelection = lampUsageCurrentSelection;
            isUpdate = true;
        }

        if (paperEnabledCurrentSelection != paperEnabledPreviousSelection) {
            paper.setEnabled(paperEnabledCurrentSelection != 0);
            storage.storePaperEnabled(paperEnabledCurrentSelection);
            paperEnabledPreviousSelection = paperEnabledCurrentSelection;
            isUpdate = true;
        }

        if (paperDevMinutesCurrentSelection != paperDevMinutesPreviousSelection || paperDevSecondsCurrentSelection != paperDevSecondsPreviousSelection) {
            uint16_t value = paperCounterFromParts(paperDevMinutesCurrentSelection, paperDevSecondsCurrentSelection);
            paper.setDevTimeCounter(value);
            storage.storePaperDevTime(value);
            paperDevMinutesPreviousSelection = paperDevMinutesCurrentSelection;
            paperDevSecondsPreviousSelection = paperDevSecondsCurrentSelection;
            isUpdate = true;
        }

        if (paperStopMinutesCurrentSelection != paperStopMinutesPreviousSelection || paperStopSecondsCurrentSelection != paperStopSecondsPreviousSelection) {
            uint16_t value = paperCounterFromParts(paperStopMinutesCurrentSelection, paperStopSecondsCurrentSelection);
            paper.setStopTimeCounter(value);
            storage.storePaperStopTime(value);
            paperStopMinutesPreviousSelection = paperStopMinutesCurrentSelection;
            paperStopSecondsPreviousSelection = paperStopSecondsCurrentSelection;
            isUpdate = true;
        }

        if (paperFixerMinutesCurrentSelection != paperFixerMinutesPreviousSelection || paperFixerSecondsCurrentSelection != paperFixerSecondsPreviousSelection) {
            uint16_t value = paperCounterFromParts(paperFixerMinutesCurrentSelection, paperFixerSecondsCurrentSelection);
            paper.setFixerTimeCounter(value);
            storage.storePaperFixerTime(value);
            paperFixerMinutesPreviousSelection = paperFixerMinutesCurrentSelection;
            paperFixerSecondsPreviousSelection = paperFixerSecondsCurrentSelection;
            isUpdate = true;
        }
    }

    void Menu::refreshBrightness() {
        if (brightnessCurrentSelection != brightnessPreviousSelection) {
            display.updateBrightness(brightnessCurrentSelection);
            brightnessPreviousSelection = brightnessCurrentSelection;
        }
    }
}