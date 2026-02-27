#ifndef TAD_BUTTON_H
#define	TAD_BUTTON_H

#define CONFIG_BTN TRISBbits.TRISB5 = 1

#define BUTTON PORTBbits.RB5

void Button_Init (void);

unsigned char getButton (void);

void motorButton (void);

#endif