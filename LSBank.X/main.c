/*
 * File:   main.c
 * Author: yanni
 *
 * Created on 24 de febrero de 2026, 23:35
 */

#include <xc.h>

#include "TAD_Button.h"
#include "TAD_Controller.h"
#include "TAD_Hall.h"
#include "TAD_LEDS.h"
#include "TAD_Matrix.h"
#include "TAD_SIO.h"
#include "TAD_Speaker.h"
#include "TAD_Timer.h"

#pragma config OSC = HS
#pragma config PBADEN = DIG
#pragma config WDT = OFF
#pragma config MCLRE = OFF
#pragma config DEBUG = OFF
#pragma config PWRT = OFF
#pragma config BOR = OFF
#pragma config LVP = OFF

void __interrupt () RSI_HIGH (void) {
    if(INTCONbits.TMR0IF == 1) {
        RSI_Timer0();
    }
}

void main (void) {
    
    SIO_Init();
    TI_Init();
    Matrix_Init();
    Hall_Init();
    Button_Init();

    while (1) {
        motorMatrix();
        motorHall();
        motorButton();
    }
}
