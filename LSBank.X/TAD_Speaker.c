/*
 * File:   TAD_Speaker.c
 * Author: yanni
 *
 * Created on 24 de febrero de 2026, 23:38
 */

#include <xc.h>
#include "TAD_Speaker.h"
#include "TAD_Timer.h"

static unsigned char timerHandle;
static unsigned char count = 0;
static unsigned char start = 0;
static unsigned char stop = 0;

static unsigned char soundActive = 0;
static unsigned long soundDuration = 0;
static unsigned long soundPeriod = 0;
static unsigned char soundTimerHandle;

void Speaker_Init (void) {
    TI_NewTimer(&timerHandle);
    TI_NewTimer(&soundTimerHandle);
    CONFIG_SPEAKER;
    SPEAKER = 0;
}

void setStart (unsigned char st) {
    start = st;
}

void setStop (unsigned char st) {
    stop = st;
}

/*
Cuando se usa el speaker:
1. Cuando se ha abierto la puerta interior y suena agudo
2. Cuando se introduce la contraseña tiene que ir subiendo la cantidad de pitidos, 0:00 - 1:45 pitido cada 1s y de 1:45 - 2:00 pitido cada 500ms y suena grave
3. Si se pasa el tiempo o número de intentos, pitido durante 10 segundos (no especifica grave o agudo)
4. Cuando se ha abierto la puerta exterior y suena agudo
5. Cuando se presiona el botón de exit y se responde con Yes, suena de forma aguda
*/

void speaker_sound (unsigned char tipo, unsigned long duracion_ms) {
    if (soundActive) {
        return;
    }

    soundActive = 1;
    SPEAKER = 0;
    soundDuration = (duracion_ms + 1) / 2;

    soundPeriod = (tipo == SONIDO_GRAVE) ? 2 : 1;

    TI_ResetTics(soundTimerHandle);
}

void processSpeakerSound (void) {
    static unsigned long lastToggleTics = 0;

    if (soundActive == 0) {
        SPEAKER = 0;
        return;
    }

    unsigned long t = TI_GetTics(soundTimerHandle);

    if ((t - lastToggleTics) >= soundPeriod) {
        SPEAKER = !SPEAKER;
        lastToggleTics = t;
    }

    if (t >= soundDuration) {
        soundActive = 0;
        SPEAKER = 0;
        lastToggleTics = 0;
    }
}

void motorSpeaker (void) {
    static unsigned char state = 0;

    processSpeakerSound();

    switch (state) {
        case 0:
            if (start == 1) {
                state = 1;
                TI_ResetTics(timerHandle);
                count = 0;
            }
            break;
        case 1:
            if (stop == 1) {
                state = 0;
                start = 0;
                stop = 0;
                count = 0;
                break;
            }
            if (TI_GetTics(timerHandle) >= 500) {
                speaker_sound(SONIDO_GRAVE, 100);
                TI_ResetTics(timerHandle);
                count++;
                if (count >= 105) {
                    state = 2;
                    TI_ResetTics(timerHandle);
                    count = 0;
                }
            }
            break;
        case 2:
            if (stop == 1) {
                state = 0;
                start = 0;
                stop = 0;
                count = 0;
                break;
            }
            if (TI_GetTics(timerHandle) >= 250) {
                speaker_sound(SONIDO_GRAVE, 100);
                TI_ResetTics(timerHandle);
                count++;
                if (count >= 30) {
                    state = 0;
                    start = 0;
                    count = 0;
                }
            }
            break;
    }
}