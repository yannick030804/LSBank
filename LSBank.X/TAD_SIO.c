/*
 * File:   TAD_SIO.c
 * Author: yanni
 *
 * Created on 24 de febrero de 2026, 23:38
 */


#include <xc.h>
#include "TAD_SIO.h"

//TODO
void SIO_Init(void) {

}

int SIO_CharAvail(void) {
    return PIR1bits.RCIF;
}

char SIO_TxAvail(void) {
    return TXSTAbits.TRMT;
}

char SIO_GetChar(void) {
    return RCREG;
}

void SIO_SendChar(char c) {
    TXREG = c;
    TXSTAbits.TXEN = 1;
}

void SIO_SendString(char *str) {
    while(*str) {
        SIO_SendChar(*str++)
    }
}