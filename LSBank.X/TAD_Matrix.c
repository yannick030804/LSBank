/*
 * File:   TAD_Matrix.c
 * Author: yanni
 *
 * Created on 24 de febrero de 2026, 23:38
 */


#include <xc.h>
#include "TAD_Matrix.h"

static unsigned char rows;
static unsigned char columns;
static unsigned char key;
static unsigned char timerHandle;
static unsigned char isPressed;

void Matrix_Init () {
    TI_NewTimer(&timerHandle);
    CONFIG_ROWS;
    CONFIG_COLS;
    INTCON2bits.NOT_RBPU = 0;
}

char keyPressed (void) {
    return !(F0 && F1 && F2 && F3);
}

void getKey (void) {
    if (PORTBbits.RB0 == 0) {
        rows = 0;
    }
    if (PORTBbits.RB1 == 0) {
        rows = 1;
    }
    if (PORTBbits.RB2 == 0) {
        rows = 2;
    }
    if (PORTBbits.RB3 == 0) {
        rows = 3;
    }
    key = teclas[columns][rows]
}

void Matrix_motor (void) {
    static char state = 0;

    switch (state) {
        case 0:
            if (!keyPressed()) {
                columns = 0;
                C0 = 0;
                C1 = 1;
                C2 = 1;
                state = 1;
            } else {
                TI_ResetTics(timerHandle);
                state = 3;
            }
            break;
        case 1:
            if (!keyPressed()) {
                colums = 1;
                C0 = 1;
                C1 = 0;
                C2 = 1;
                state = 2;
            } else {
                TI_ResetTics(timerHandle);
                state = 3;
            }
            break;
        case 2:
            if (!keyPressed()) {
                colums = 2;
                C0 = 1;
                C1 = 1;
                C2 = 0;
                state = 0;
            } else {
                TI_ResetTics(timerHandle);
                state = 3;
            }
            break;
        case 3:
            
            break;
    }
}