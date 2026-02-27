/*
 * File:   TAD_LEDS.c
 * Author: yanni
 *
 * Created on 24 de febrero de 2026, 23:38
 */


#include <xc.h>
#include "TAD_LEDS.h"
#include "TAD_Timer.h"

static unsigned char timerPWM;
static unsigned char timerRamp;
static unsigned char flag = 0;

void LED_Init (void) {
    TI_NewTimer(&timerPWM);
    TI_NewTimer(&timerRamp);

    CONFIG_OK;
    CONFIG_ALARM;
    CONFIG_INTENSITY;
}

void setIntensity (unsigned char aux) {
    flag = aux;
}

void setOK (unsigned char aux) {
    LED_OK = (aux ? 1 : 0);
}

void setAlarm (unsigned char aux) {
    LED_ALARM = (aux ? 1 : 0);
}

void motorLEDs (void) {
    static unsigned char state = 0;
    static unsigned char pwmStep = 0;
    static unsigned char duty = 0;

    switch (state) {
        case 0:
            LED_INTENSITY = 0;
            if (flag == 1) {
                pwmStep = 0;
                duty = 0;
                TI_ResetTics(timerPWM);
                TI_ResetTics(timerRamp);
                state = 1;
            }
            break;
        case 1:
            if (flag == 0) {
                state = 0;
                LED_INTENSITY = 0;
                break;
            }
            if (TI_GetTics(timerPWM) >= PWM_TIC_STEP) {
                TI_ResetTics(timerPWM);
                pwmStep++;
                if (pwmStep >= PWM_PERIOD_STEPS) {
                    pwmStep = 0;
                }
                LED_INTENSITY = (pwmStep < duty) ? 1 : 0;
            }
            if (TI_GetTics(timerRamp) >= RAMP_TICS) {
                TI_ResetTics(timerRamp);
                if (duty < PWM_PERIOD_STEPS) {
                    duty++;
                } else {
                    LED_INTENSITY = 0;
                    duty = 0;
                    flag = 0;
                    state = 0;
                }
            }
        break;
    }
}