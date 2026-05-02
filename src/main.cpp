/* mbed Microcontroller Library
 * Copyright (c) 2019 ARM Limited
 * SPDX-License-Identifier: Apache-2.0
 */

#include "AnalogIn.h"
#include "PwmOut.h"
#include "ThisThread.h"
#include "mbed.h"
#include <chrono>
#include "INA219.hpp"
#include "TextLCD.h"
#include "mbed_thread.h"

I2C i2c_lcd(D14, D15);                          // SDA, SCL
TextLCD_I2C lcd(&i2c_lcd, 0x4E, TextLCD::LCD16x2);
PwmOut srv(D6);
AnalogIn ldr1(A0);
AnalogIn ldr2(A1);
INA219 ina219(D14, D15, 0x40, 400000, RES_10BITS);

Ticker measure;
Ticker m;

volatile bool flag = false;
volatile bool f2 = false;
float current_ma;
float voltage;

void tick()
{
    flag = true;
}
void t2(){
    f2 = true;
}

int main() {
    srv.period_ms(20); // Set the PWM period to 20 ms (50 Hz)
    
    float pulse = 1.75;
    measure.attach(&tick, 10);
    m.attach(&t2, 1);
    lcd.setBacklight(TextLCD::LightOn);
    lcd.cls();
    while (true) {
        float val1 = ldr1.read();
        float val2 = ldr2.read();

        if (fabs(val1 - val2) > 0.05) {
            if (val1 > val2 && pulse < 2.5) {
                pulse += 0.01;
            } else if (val2 > val1 && pulse > 1.0) {
             pulse -= 0.01;
            }
            srv.pulsewidth(pulse * 1e-3f);

        }

        printf("LDR1: %.3f, LDR2: %.3f\n", val1, val2);

        wait_ms(20);

        if (flag) {
            flag = false;

            current_ma = ina219.read_current_mA();
            voltage = ina219.read_bus_voltage();
            printf("c: %f; v: %f", current_ma, voltage);

            lcd.locate(0, 0);
            lcd.printf("curr: %.2f mA ", current_ma);

            lcd.locate(0, 1);
            lcd.printf("vol: %.2f V  ", voltage);
        }
    }
}