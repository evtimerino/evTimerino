#include <menu.h>


namespace TimerMenu {

    bool isUpdate = false;

    uint8_t precisionPreviousSelection = 3;
    uint16_t precisionCurrentSelection = 3;
    uint8_t teststripPreviousSelection = 0;
    uint16_t teststripCurrentSelection = 0;
    uint8_t safelightPreviousSelection = 0;
    uint16_t safelightCurrentSelection = 0;
    uint8_t buzzerPreviousSelection = 0;
    uint16_t buzzerCurrentSelection = 0;
    uint8_t preparePreviousSelection = 0;
    uint16_t prepareCurrentSelection = 0;
    uint8_t lampUsagePreviousSelection = 0;
    uint16_t lampUsageCurrentSelection = 0;

    uint8_t previousBrightness = 0;
    uint8_t currentBrightness = 0;

    // FSTOP

    static const char *fstops[] = { 
    "1/2", "1/3", "1/4", "1/6", "1/8", "1/12", "1/16" };
  
    uint16_t fstop_get_cnt(void *data) {
        return sizeof(fstops)/sizeof(*fstops);
    }
    
    const char *fstop_get_str(void *data, uint16_t index) {
        return fstops[index];
    }

    // TESTSTRIP

    static const char *teststrip[] = { 
    "separate", "split grade", "incremental"};
  
    uint16_t teststrip_get_cnt(void *data) {
        return sizeof(teststrip)/sizeof(*teststrip);
    }
    
    const char *teststrip_get_str(void *data, uint16_t index) {
        return teststrip[index];
    }

    // SAFELIGHT

    static const char *safelight[] = { 
    "off", "off on exposure", "always on"};
  
    uint16_t safelight_get_cnt(void *data) {
        return sizeof(safelight)/sizeof(*safelight);
    }
    
    const char *safelight_get_str(void *data, uint16_t index) {
        return safelight[index];
    }

    // BUZZER

    static const char *buzzer[] = { 
    "on", "off"};
  
    uint16_t buzzer_get_cnt(void *data) {
        return sizeof(buzzer)/sizeof(*buzzer);
    }
    
    const char *buzzer_get_str(void *data, uint16_t index) {
        return buzzer[index];
    }

    // PREPARE

    static const char *prepare[] = { 
    "off", "on"};
  
    uint16_t prepare_get_cnt(void *data) {
        return sizeof(prepare)/sizeof(*prepare);
    }
    
    const char *prepare_get_str(void *data, uint16_t index) {
        return prepare[index];
    }

    // LAMP USAGE

    static const char *lampUsage[] = { 
    "off", "on"};
  
    uint16_t lampUsage_get_cnt(void *data) {
        return sizeof(lampUsage)/sizeof(*lampUsage);
    }

    const char *lampUsage_get_str(void *data, uint16_t index) {
        return lampUsage[index];
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

        MUIF_BUTTON("G1", mui_u8g2_btn_goto_wm_fi),

        MUIF_U8G2_U16_LIST("A1", &precisionCurrentSelection, NULL, fstop_get_str, fstop_get_cnt, mui_u8g2_u16_list_line_wa_mse_pi),
        MUIF_U8G2_U16_LIST("A2", &teststripCurrentSelection, NULL, teststrip_get_str, teststrip_get_cnt, mui_u8g2_u16_list_line_wa_mse_pi),
        MUIF_U8G2_U8_MIN_MAX("NR", &currentBrightness, 1, 4, mui_u8g2_u8_min_max_wm_mse_pi),
        MUIF_U8G2_U16_LIST("A3", &safelightCurrentSelection, NULL, safelight_get_str, safelight_get_cnt, mui_u8g2_u16_list_line_wa_mse_pi),
        MUIF_U8G2_U16_LIST("A4", &buzzerCurrentSelection, NULL, buzzer_get_str, buzzer_get_cnt, mui_u8g2_u16_list_line_wa_mse_pi),
        MUIF_U8G2_U16_LIST("A5", &prepareCurrentSelection, NULL, prepare_get_str, prepare_get_cnt, mui_u8g2_u16_list_line_wa_mse_pi),
        MUIF_U8G2_U16_LIST("A6", &lampUsageCurrentSelection, NULL, lampUsage_get_str, lampUsage_get_cnt, mui_u8g2_u16_list_line_wa_mse_pi),
    };

    fds_t fds_data[] = 
        MUI_FORM(1)
        MUI_STYLE(1)
        MUI_LABEL(5,10, "Menu")
        MUI_XY("HR", 0,13)
        MUI_STYLE(0)

        MUI_DATA("GP", 
            MUI_10 "Precision|"
            MUI_11 "Test Strip|"
            MUI_12 "Brightness|"
            MUI_13 "Safelight|"
            MUI_14 "Buzzer|"
            MUI_15 "Prepare|"
            MUI_16 "Lamp Usage|"
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
        MUI_XYA("A1",45, 40, 50)
    
        MUI_FORM(11)
        MUI_STYLE(1)
        MUI_LABEL(5, 8, "Test Strip")
        MUI_XY("HR", 0,11)
        MUI_STYLE(0)
        MUI_XYA("A2",45, 40, 50)


        MUI_FORM(12)
        MUI_STYLE(1)
        MUI_LABEL(5, 8, "Brightness")
        MUI_XY("HR", 0,11)
        MUI_STYLE(0)
        MUI_XY("NR",70, 40)

        MUI_FORM(13)
        MUI_STYLE(1)
        MUI_LABEL(5, 8, "Safelight")
        MUI_XY("HR", 0,11)
        MUI_STYLE(0)
        MUI_XYA("A3",45, 40, 50)

        MUI_FORM(14)
        MUI_STYLE(1)
        MUI_LABEL(5, 8, "Buzzer")
        MUI_XY("HR", 0,11)
        MUI_STYLE(0)
        MUI_XYA("A4",45, 40, 50)

        MUI_FORM(15)
        MUI_STYLE(1)
        MUI_LABEL(5, 8, "Prepare")
        MUI_XY("HR", 0,11)
        MUI_STYLE(0)
        MUI_XYA("A5",45, 40, 50)
        
        MUI_FORM(16)
        MUI_STYLE(1)
        MUI_LABEL(5, 8, "Lamp Usage")
        MUI_XY("HR", 0,11)
        MUI_STYLE(0)
        MUI_XYA("A6",45, 40, 50)
    ;


    Menu::Menu(Keypad& k, Exposure& e, U8G2_SSD1309_128X64_NONAME0_F_HW_I2C& o, Enlarger& l, Buzzer& b, Display& d) : keypad(k), exposure(e), oled(o), enlarger(l), buzzer(b), display(d) {}

    Menu::~Menu() {}

    void Menu::setup() {
        mui.begin(oled, fds_data, muif_list, sizeof(muif_list)/sizeof(muif_t));

        precisionPreviousSelection = exposure.getPrecisionIdx();
        precisionCurrentSelection = precisionPreviousSelection;

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
        switch (exposure.getTestStripMode())
        {
        case Teststrip::SEPARATE:
            teststripPreviousSelection = 0;
            teststripCurrentSelection = 0;
            break;
        case Teststrip::SPLIT_GRADE:
            teststripPreviousSelection = 1;
            teststripCurrentSelection = 1;
            break;        
        case Teststrip::INCREMENTAL:
            teststripPreviousSelection = 2;
            teststripCurrentSelection = 2;
            break;
        default:
            break;
        }
        switch (buzzer.getState())
        {
        case Tone::ON:
            buzzerPreviousSelection = 0;
            buzzerCurrentSelection = 0;
            break;
        case Tone::OFF:
            buzzerPreviousSelection = 1;
            buzzerCurrentSelection = 1;
            break;
        default:
            break;
        }

        previousBrightness = display.getBrightnessLevel();
        currentBrightness = previousBrightness;
    }

    void Menu::goToForm(uint8_t id, uint8_t pos) {
        mui.gotoForm(id, pos);
        isReDraw = true;
    }

    void Menu::loop() {
        while (event != Event::RELEASED_EXIT) {        
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
            exposure.reset();
            precisionPreviousSelection = precisionCurrentSelection;
            isUpdate = true;
        }
        if (teststripCurrentSelection != teststripPreviousSelection) {
            switch (teststripCurrentSelection)
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
            safelightPreviousSelection = safelightPreviousSelection;
            isUpdate = true;
        }
        if (buzzerCurrentSelection != buzzerPreviousSelection) {
            switch (buzzerCurrentSelection)
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
            lampUsagePreviousSelection = lampUsageCurrentSelection;
            isUpdate = true;
        }
    }

    void Menu::refreshBrightness() {
        if (currentBrightness != previousBrightness) {
            display.updateBrightness(currentBrightness);
            previousBrightness = currentBrightness;
        }
    }
}