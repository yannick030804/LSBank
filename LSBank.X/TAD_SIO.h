#ifndef TAD_SIO_H
#define	TAD_SIO_H

void SIO_Init (void);

int SIO_CharAvail (void);

char SIO_TxAvail (void);

char SIO_GetChar (void);

void SIO_SendChar (char c);

void SIO_SendString (char *str);

#endif