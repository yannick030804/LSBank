/*
 * File:   TAD_Button.c
 * Author: yanni
 *
 * Created on 24 de febrero de 2026, 23:38
 */

#include <xc.h>
#include "TAD_Button.h"
#include "TAD_Timer.h"

static unsigned char timerHandle;
static unsigned char isPressed = 0;

void Button_Init (void) {
    CONFIG_BTN;
    TI_NewTimer(&timerHandle);
}

unsigned char getButton (void) {
    unsigned char aux = isPressed;
    isPressed = 0;
    return aux;
}

void motorButton (void) {
    static unsigned char state = 0;

    switch (state) {
        case 0:
            if (BUTTON == 1) {
                state = 0;
            } else if (BUTTON == 0) {
                state = 1;
                TI_ResetTics(timerHandle);
            }
            break;
        case 1:
            if (BUTTON == 0 && TI_GetTics(timerHandle) >= 8) {
                isPressed = 1;
                state = 2;
                TI_ResetTics(timerHandle);
            }
            break;
        case 2:
            if (BUTTON == 1 && TI_GetTics(timerHandle) >= 8) {
                state = 0;
            }
            break;
    }
}