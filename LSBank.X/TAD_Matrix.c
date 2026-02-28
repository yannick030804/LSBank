/*
 * File:   TAD_Matrix.c
 * Author: yanni
 *
 * Created on 24 de febrero de 2026, 23:38
 */


#include <xc.h>
#include "TAD_Matrix.h"
#include "TAD_SIO.h"

static unsigned char rows;
static unsigned char columns;
static unsigned char newRow;
static unsigned char newColumn;
static unsigned char lastRow;
static unsigned char lastColumn;

static unsigned char timerHandle;
static unsigned char timer1s;

static const char* lista;
static char key;
static unsigned char idx;
static unsigned char editing;
static const char* const teclas[3][4] = {
    {"1", "GHI4", "PQRS7", "*"},
    {"ABC2", "JKL5", "TUV8", " 0"},
    {"DEF3", "MNO6", "WXYZ9", "#"}
};

static unsigned char start = 0;

static unsigned char password[8] = "DDMM---";
static unsigned char count = 0;
static unsigned char flag = 0;

void Matrix_Init (void) {
    TI_NewTimer(&timerHandle);
    TI_NewTimer(&timer1s);
    CONFIG_ROWS;
    CONFIG_COLS;
    INTCON2bits.NOT_RBPU = 0;
}

char keyPressed (void) {
    return (F0 == 0 || F1 == 0 || F2 == 0 || F3 == 0);
}

void getKey (void) {
    if (PORTBbits.RB0 == 0) {
        rows = 0;
    } else if (PORTBbits.RB1 == 0) {
        rows = 1;
    } else if (PORTBbits.RB2 == 0) {
        rows = 2;
    } else if (PORTBbits.RB3 == 0) {
        rows = 3;
    }
    lista = teclas[columns][rows];
    idx = 0;
    key = lista[idx];
}

void nextChar (void) {
    if (lista == 0) {
        return;
    }
    idx++;
    if (lista[idx] == '\0') {
        idx = 0;
    }
    key = lista[idx];
}

unsigned char passwordSet (void) {
    unsigned char aux = flag;
    flag = 0;
    return aux;
}

char* getPassword (void) {
    return password;
}

void setMatrix (unsigned char st) {
    start = st;
}

void motorMatrix (void) {
    static char state = 0;

    if (start == 0) {
        return;
    }

    if (editing == 1 && !keyPressed() && (TI_GetTics(timer1s) >= 500)) {
        if (count < 7) {
            password[count] = key;
            SIO_SendChar(password[count]);
            count++;
        }
        
        if (count == 7) {
            password[7] = '\0';
            flag = 1;
            count = 0;
        }
        editing = 0;
    }

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
                columns = 1;
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
                columns = 2;
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
            if (!keyPressed()) {
                state = 0;
            } else if(TI_GetTics(timerHandle) > 8) {

                if (F0 == 0) {
                    newRow = 0;
                } else  if (F1 == 0) {
                    newRow = 1;
                } else if (F2 == 0) {
                    newRow = 2;
                } else if (F3 == 0) {
                    newRow = 3;
                }
                newColumn = columns;

                if (newRow == 3 && ((newColumn == 0) || (newColumn == 2))) {
                    state = 6;
                    break;
                }

                TI_ResetTics(timer1s);

                if (editing == 0) {
                    state = 4;
                } else {
                    if (newColumn == lastColumn && newRow == lastRow) {
                        nextChar();
                        state = 6;
                    } else {
                        state = 5;
                    }
                }
            }
            break;
        case 4:
            lastRow = newRow;
            lastColumn = newColumn;
            rows = newRow;
            columns = newColumn;
            getKey();
            editing = 1;
            state = 6;
            break;
        case 5:
            if (count < 7) {
                password[count] = key;
                SIO_SendChar(password[count]);
                count++;
            }
            if (count == 7) {
                password[7] = '\0';
                flag = 1;
                count = 0;
            }

            lastRow = newRow;
            lastColumn = newColumn;
            rows = newRow;
            columns = newColumn;

            getKey();

            editing = 1;
            state = 6;
            break;
        case 6:
            if (!keyPressed()) {
                state = 0;
            }
            break;
    }
}