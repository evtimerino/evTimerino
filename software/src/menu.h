#include <U8g2lib.h>
#include <MUIU8g2.h>
#include <keypad.h>
#include <common.h>
#include <exposure.h>
#include <enlarger.h>
#include <display.h>

#ifndef MENU_H
#define MENU_H


namespace TimerMenu {

    class Menu
    {
    private:
        Keypad& keypad;
        Exposure& exposure;
        U8G2_SSD1309_128X64_NONAME0_F_HW_I2C& oled;
        MUIU8G2 mui;
        Enlarger& enlarger;
        Buzzer& buzzer;
        Display& display;

        bool isReDraw = true;
        Event event = Event::NO_EVENT;
        
        void refreshBrightness();
        void update();

    public:
        Menu(Keypad& k, Exposure& e, U8G2_SSD1309_128X64_NONAME0_F_HW_I2C& o, Enlarger& l, Buzzer& b, Display& d);
        ~Menu();

        void setup();
        void goToForm(uint8_t id, uint8_t pos);

        void loop();
    };

}
#endif