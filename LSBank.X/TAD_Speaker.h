#ifndef TAD_SPEAKER_H
#define	TAD_SPEAKER_H

#define CONFIG_SPEAKER TRISCbits.TRISC3 = 0

#define SPEAKER LATCbits.LATC3

#define SONIDO_GRAVE 0
#define SONIDO_AGUDO 1

void Speaker_Init (void);

void setSpeaker (unsigned char st);

void setStop (unsigned char st);

void speaker_sound (unsigned char tipo, unsigned int duracion_ms);

void processSpeakerSound (void);

void motorSpeaker (void);

#endif