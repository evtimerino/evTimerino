#include <Arduino.h>
#include <Preferences.h>
#include <timer.h>
#include <keypad.h>
#include <buzzer.h>
#include <display.h>
#include <menu.h>
#include <enlarger.h>
#include <storage.h>
#include <U8g2lib.h>
#include <MUIU8g2.h>
#include <Wire.h>
#include <exposure.h>

U8G2_SSD1309_128X64_NONAME0_F_HW_I2C u8g2(U8G2_R0);
MUIU8G2 mui;
Preferences preferences;

Display oled = Display(u8g2);
Keypad keypad = Keypad();
Buzzer buzzer = Buzzer();
Exposure exposure = Exposure(buzzer);
Enlarger enlarger = Enlarger(oled, buzzer, exposure);
Storage storage = Storage(oled, preferences, exposure, buzzer, enlarger);
TimerMenu::Menu menu(keypad, exposure, u8g2, enlarger, buzzer, oled);
Timer timer = Timer(oled, keypad, buzzer, exposure, enlarger, menu, storage);

void setup() {
  Serial.begin(115200);
  delay(100);
  u8g2.begin();
  oled.drawLogo();
  delay(LOGO_DURATION);
  storage.load();
  menu.setup();
  menu.goToForm(1, 0);
  timer.init();
}

void loop() {
  timer.Run();
}

