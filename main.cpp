#include "mbed.h"
#include "TextLCD.h"

I2C i2c_lcd(D14, D15);                          // SDA, SCL
TextLCD_I2C lcd(&i2c_lcd, 0x4E, TextLCD::LCD16x2);
int main() {
    lcd.setBacklight(TextLCD::LightOn);
    lcd.cls();
    lcd.printf("Hello");
    lcd.locate(0, 1);                           // col 0, row 1
    lcd.printf("bye:)");

    while (true) {
        wait(0.5);
    }
}