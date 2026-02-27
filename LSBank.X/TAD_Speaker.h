#ifndef TAD_SPEAKER_H
#define	TAD_SPEAKER_H

#define CONFIG_SPEAKER TRISDbits.TRISD2 = 0

#define SPEAKER LATDbits.LATD2

void Speaker_Init (void);

#endif