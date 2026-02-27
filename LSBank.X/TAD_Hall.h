#ifndef TAD_HALL_H
#define	TAD_HALL_H

#define CONFIG_HALL TRISBbits.TRISB4 = 1

#define HALL PORTBbits.RB4

void Hall_Init (void);

void motorHall (void);

unsigned char getHall (void);

#endif