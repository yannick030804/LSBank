/*
 * File:   TAD_SIO.c
 * Author: yanni
 *
 * Created on 24 de febrero de 2026, 23:38
 */


#include <xc.h>
#include "TAD_SIO.h"

void SIO_Init (void) {
    TRISCbits.TRISC6 = 0;
    TRISCbits.TRISC7 = 1;
    TXSTAbits.SYNC = 0;
    TXSTAbits.BRGH = 1;
    TXSTAbits.TXEN = 1;
    RCSTAbits.SPEN = 1;
    RCSTAbits.CREN = 1;
    BAUDCONbits.BRG16 = 1;
    /* 9600 baud at 10 MHz: n = Fosc/(4*9600)-1 = 260 */
    SPBRGH = 0x01;
    SPBRG = 4;
}

int SIO_CharAvail (void) {
    return PIR1bits.RCIF;
}

char SIO_TxAvail (void) {
    return TXSTAbits.TRMT;
}

char SIO_GetChar (void) {
    return RCREG;
}

unsigned char SIO_PutChar (unsigned char c) {
    if (!SIO_TxAvail()) {
        return 0;
    } else {
        TXREG = c;
        return 1;
    }
}

void SIO_SendChar (char c) {
    TXREG = c;
    TXSTAbits.TXEN = 1;
}