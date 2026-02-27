/*
 * File:   TAD_Hall.c
 * Author: yanni
 *
 * Created on 24 de febrero de 2026, 23:38
 */

 #include <xc.h>
 #include "TAD_Hall.h"

 static unsigned char hallEvent = 0;

 void Hall_Init (void) {
    CONFIG_HALL;
 }

 unsigned char getHall (void) {
    unsigned char aux = hallEvent;
    hallEvent = 0;
    return aux;
 }

 void motorHall (void) {
    static unsigned char state = 0;

    switch (state) {
        case 0:
            if (HALL == 1) {
                hallEvent = 1;
                state = 1;
            }
            break;
        case 1:
            if (HALL == 0) {
                state = 0;
            }
            break;
    }
 }