/*
 * File:   TAD_Controller.c
 * Author: yanni
 *
 * Created on 24 de febrero de 2026, 23:38
 */


#include <xc.h>
#include "TAD_Controller.h"
#include "TAD_SIO.h"
#include "TAD_Timer.h"
#include "TAD_Hall.h"
#include "TAD_Speaker.h"
#include "TAD_LEDS.h"
#include "TAD_Button.h"

static char i;
static char tries;
static unsigned char count;

static unsigned char message;
static unsigned char *uid_ptr;
static unsigned char psswrd[] = PASSWORD;
static unsigned char *password;
static char yes_aux[] = YES;
static char yesBuf[4];
static unsigned char yesIdx;

static unsigned char timerHandle;

void Controller_Init (void) {
    TI_NewTimer(&timerHandle);
}

void motorController (void) {
    static unsigned char state = 0;

    switch (state) {
        case 0:
            uid_ptr = NEW_DAY;
            i = 0;
            state = 1;
            setOK(1);
            setAlarm(0);
            break;
        case 1:
            if (SIO_TxAvail()) {
                if (uid_ptr[i] != '\0') {
                    SIO_SendChar(uid_ptr[i]);
                    i++;
                } else {
                    i = 0;
                    state = 2;
                }
            }
            break;
        case 2:
            if (getHall() == 1) {
                speaker_sound(SONIDO_AGUDO, 200);
                uid_ptr = OPEN_EXTERIOR_DOOR;
                i = 0;
                state = 3;
            }
            break;
        case 3:
            if (SIO_TxAvail()) {
                if (uid_ptr[i] != '\0') {
                    SIO_SendChar(uid_ptr[i]);
                    i++;
                } else {
                    i = 0;
                    TI_ResetTics(timerHandle);
                    state = 4;
                }
            }
            break;
        case 4:
            if (TI_GetTics(timerHandle) >= 1000) {
                uid_ptr = CLOSE_EXTERIOR_DOOR;
                i = 0;
                state = 5;
            }
            break;
        case 5:
            if (SIO_TxAvail()) {
                if (uid_ptr[i] != '\0') {
                    SIO_SendChar(uid_ptr[i]);
                    i++;
                } else {
                    i = 0;
                    state = 6;
                    uid_ptr = ENTER_PIN;
                }
            }
            break;
        case 6:
            if (SIO_TxAvail()) {
                if (uid_ptr[i] != '\0') {
                    SIO_SendChar(uid_ptr[i]);
                    i++;
                } else {
                    i = 0;
                    state = 7;
                    TI_ResetTics(timerHandle);
                    setMatrix(1); //para habilitar la matriz
                    setSpeaker(1); //para habilitar el alrtavoz
                    setIntensity(1); //para que el led intensity empiece a iluminarse
                    tries = 0;
                }
            }
            break;
        case 7:
            if (TI_GetTics(timerHandle) < 60000 && tries < 3) {
                if (passwordSet() == 1) {
                    password = getPassword();
                    state = 8;
                    i = 0;
                    count = 0;
                }
            } else {
                setMatrix(0); //parar la matriz
                setStop(1); //parar el altavoz
                setIntensity(0); //parar el LED de intensidad
                setAlarm(1); //activar el LED de Alarma
                setOK(0);
                uid_ptr = THIEF_INTERCEPTED;
                i = 0;
                state = 11;
            }
            break;
        case 8:
            if (i  < 7) {
                if (password[i] == psswrd[i]) {
                    count++;
                }
                i++;
            } else {
                if (count == 7) {
                    setMatrix(0);
                    setStop(1);
                    setIntensity(0);
                    uid_ptr = OPEN_INTERIOR_DOOR;
                    state = 15;
                } else {
                    SIO_SendChar('\n');
                    uid_ptr = PERMISSION_DENIED;
                    state = 9;
                    i = 0;
                    tries++;
                }
            }
            break;
        case 9:
            if (SIO_TxAvail()) {
                if (uid_ptr[i] != '\0') {
                    SIO_SendChar(uid_ptr[i]);
                    i++;
                } else {
                    if (TI_GetTics(timerHandle) < 60000 && tries < 3) {
                        i = 0;
                        state = 10;
                        uid_ptr = ENTER_PIN;
                    } else {
                        setMatrix(0); //parar la matriz
                        setStop(1); //parar el altavoz
                        setIntensity(0); //parar el LED de intensidad
                        setAlarm(1); //activar el LED de Alarma
                        setOK(0);
                        uid_ptr = THIEF_INTERCEPTED;
                        i = 0;
                        state = 11;
                    }
                }
            }
            break;
        case 10:
            if (SIO_TxAvail()) {
                if (uid_ptr[i] != '\0') {
                    SIO_SendChar(uid_ptr[i]);
                    i++;
                } else {
                    i = 0;
                    state = 7;
                }
            }
            break;
        case 11:
            if (SIO_TxAvail()) {
                if (uid_ptr[i] != '\0') {
                    SIO_SendChar(uid_ptr[i]);
                    i++;
                } else {
                    speaker_sound(SONIDO_GRAVE, 10000); //hace sonar el speaker durante 10 segundos
                    i = 0;
                    state = 12;
                    uid_ptr = RESET_SYSTEM;
                }
            }
            break;
        case 12:
            if (SIO_TxAvail()) {
                if (uid_ptr[i] != '\0') {
                    SIO_SendChar(uid_ptr[i]);
                    i++;
                } else {
                    i = 0;
                    state = 13;
                    yesIdx = 0;
                }
            }
            break;
        case 13:
            if (SIO_CharAvail()) {
                char c = SIO_GetChar();
                if (!(c == '\r' || c == '\n')) {
                    if (yesIdx < 3) {
                        yesBuf[yesIdx++] = c;
                    }
                } else {
                    yesBuf[yesIdx] = '\0';
                    i = 0;
                    count = 0;
                    state = 14;
                }
            }
            break;
        case 14:
            if (i < 3) {
                if (yesBuf[i] == yes_aux[i]) {
                    count++;
                }
                i++;
            } else {
                if (count == 3) {
                    state = 0;
                    i = 0;
                } else {
                    state = 12;
                    yesIdx = 0;
                    uid_ptr = RESET_SYSTEM;
                    i = 0;
                }
            }
            break;
        case 15:
            if (SIO_TxAvail()) {
                if (uid_ptr[i] != '\0') {
                    SIO_SendChar(uid_ptr[i]);
                    i++;
                } else {
                    TI_ResetTics(timerHandle);
                    state = 16;
                }
            }
            break;
        case 16:
            if (TI_GetTics(timerHandle) >= 1000) {
                uid_ptr = CLOSE_INTERIOR_DOOR;
                i = 0;
                state = 17;
            }
            break;
        case 17:
            if (SIO_TxAvail()) {
                if (uid_ptr[i] != '\0') {
                    SIO_SendChar(uid_ptr[i]);
                    i++;
                } else {
                    speaker_sound(SONIDO_AGUDO, 200);
                    state = 18;
                }
            }
            break;
        case 18:
            if (getButton()) {
                uid_ptr = EXIT_REQUEST;
                state = 19;
                i = 0;
            }
            break;
        case 19:
            if (SIO_TxAvail()) {
                if (uid_ptr[i] != '\0') {
                    SIO_SendChar(uid_ptr[i]);
                    i++;
                } else {
                    i = 0;
                    state = 20;
                    yesIdx = 0;
                }
            }
            break;
        case 20:
            if (SIO_CharAvail()) {
                char c = SIO_GetChar();
                if (!(c == '\r' || c == '\n')) {
                    if (yesIdx < 3) {
                        yesBuf[yesIdx++] = c;
                    }
                } else {
                    yesBuf[yesIdx] = '\0';
                    i = 0;
                    count = 0;
                    state = 21;
                }
            }
            break;
        case 21:
            if (yesIdx == 3 && yesBuf[0]=='Y' && yesBuf[1]=='e' && yesBuf[2]=='s') {
                uid_ptr = OPEN_BOTH_DOORS;
                i = 0;
                state = 23;
            } 
            else if (yesIdx == 2 && yesBuf[0]=='N' && yesBuf[1]=='o') {
                i = 0;
                state = 22;
            } 
            else {
                uid_ptr = EXIT_REQUEST;
                i = 0;
                state = 19;
            }
            break;
        case 22:
            setMatrix(0); //parar la matriz
            setStop(1); //parar el altavoz
            setIntensity(0); //parar el LED de intensidad
            setAlarm(1); //activar el LED de Alarma
            setOK(0);
            uid_ptr = THIEF_INTERCEPTED;
            i = 0;
            state = 11;
            break;
        case 23:
            if (SIO_TxAvail()) {
                if (uid_ptr[i] != '\0') {
                    SIO_SendChar(uid_ptr[i]);
                    i++;
                } else {
                    TI_ResetTics(timerHandle);
                    speaker_sound(SONIDO_AGUDO, 200);
                    state = 24;
                }
            }
            break;
        case 24:
            if (TI_GetTics(timerHandle) >= 1000) {
                i = 0;
                uid_ptr = CLOSE_BOTH_DOORS;
                state = 25;
            }
            break;
        case 25:
            if (SIO_TxAvail()) {
                if (uid_ptr[i] != '\0') {
                    SIO_SendChar(uid_ptr[i]);
                    i++;
                } else {
                    i = 0;
                    state = 26;
                }
            }
            break;
        case 26:
            break;
    }
}