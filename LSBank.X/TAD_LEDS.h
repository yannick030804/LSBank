#ifndef TAD_LEDS_H
#define	TAD_LEDS_H

#define CONFIG_OK TRISDbits.TRISD0 = 0
#define CONFIG_ALARM TRISDbits.TRISD1 = 0
#define CONFIG_INTENSITY TRISCbits.TRISC2 = 0

#define LED_OK LATDbits.LATD0
#define LED_ALARM LATDbits.LATD1
#define LED_INTENSITY LATCbits.LATC2

#define PWM_PERIOD_STEPS   10
#define PWM_TIC_STEP       1
#define RAMP_TICS          20

void LED_Init (void);

void setIntensity (unsigned char aux);

void setOK (unsigned char aux);

void setAlarm (unsigned char aux);

void motorLEDs (void);

#endif