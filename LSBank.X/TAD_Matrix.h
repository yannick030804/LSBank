#ifndef TAD_MATRIX_H
#define	TAD_MATRIX_H

#define CONFIG_ROWS TRISBbits.TRISB0 = TRISBbits.TRISB1 = TRISBbits.TRISB2 = TRISBbits.TRISB3 = 1
#define CONFIG_COLS TRISDbits.TRISD5 = TRISDbits.TRISD6 = TRISDbits.TRISD7 = 0

#define F0 PORTBbits.RB0
#define F1 PORTBbits.RB1
#define F2 PORTBbits.RB2
#define F3 PORTBbits.RB3

#define C0 LATDbits.LATD5
#define C1 LATDbits.LATD6
#define C2 LATDbits.LATD7

void motorMatrix (void);

void Matrix_Init (void);

unsigned char passwordSet (void);

char* getPassword (void);

void setMatrix (unsigned char st);

#endif