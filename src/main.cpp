/* mbed Microcontroller Library
 * Copyright (c) 2019 ARM Limited
 * SPDX-License-Identifier: Apache-2.0
 */

#include "AnalogIn.h"
#include "PwmOut.h"
#include "ThisThread.h"
#include "mbed.h"
#include <chrono>

PwmOut srv(D6);
AnalogIn ldr1(A0);
AnalogIn ldr2(A1);


int main() {
    srv.period_ms(20); // Set the PWM period to 20 ms (50 Hz)
    
    float pulse = 1.75;
    while (true) {


        if(ldr1 - ldr2 > 0.01 && pulse < 2.5){
                pulse += 0.01;
                srv.pulsewidth(pulse/1000);
        }
        if(ldr2 - ldr1 > 0.01 && pulse > 1) {
                pulse -= 0.01;
                srv.pulsewidth(pulse/1000);
        }
        printf("LDR 1: %f, LDR 2: %f\n", ldr1.read(), ldr2.read());
        ThisThread::sleep_for(chrono::milliseconds(10));


    }
}