/*
 * File:   timer.c
 * Authors:
 *
 * Created on December 30, 2014, 8:07 PM
 */

#include <xc.h>
#include <math.h>
#include "constants.h"

//Uses timer 2
void delayUs(unsigned int delay){
    //TODO: Create a delay using timer 2 for "delay" microseconds.
    unsigned int time = floor(delay * 1.25);
    T2CONbits.ON = OFF;
    IFS0bits.T2IF = FLAG_DOWN;
    TMR2 = 0;
    PR2 = time;
    T2CONbits.ON = ENABLED;
    while(IFS0bits.T2IF != 1);
    T2CONbits.ON = OFF;
    IFS0bits.T2IF = FLAG_DOWN;
    TMR2 = 0;
}

void initTimers() {
     //TODO: Initialize Timer 1 to have a period of
    T1CONbits.TCKPS = PS_64_A;
    T2CONbits.TCKPS = PS_8_B;
    T1CONbits.TCS = 0;  //default clock
    T2CONbits.TCS = 0;
    T1CONbits.SIDL = DISABLED;
    T2CONbits.SIDL = DISABLED;
    IFS0bits.T1IF = FLAG_DOWN;  //puts the flag down
    IFS0bits.T2IF = FLAG_DOWN;
    IPC1bits.T1IP = 7;   //sets timer priority level to 7
//    IPC2bits.T2IP = 7;
    TMR2 = 0;
    TMR1 = 0;
    PR2 = 195;
    PR1 = 1560;
//    IEC0bits.T2IE = ENABLED;    //originally ENABLED
    IEC0bits.T1IE = ENABLED;     //enables the interrupt flag   SAME AS ABOVE
    T2CONbits.ON = 0;
    T1CONbits.ON = ENABLED;
    // 1 second. Enable its interrupt
    
    //-----------------------------------------------
    T3CONbits.TCKPS = PS_8_B;
    T3CONbits.TCS = 0;  //default clock
    T3CONbits.SIDL = DISABLED;
    IFS0bits.T3IF = FLAG_DOWN;  //puts the flag down
//    IPC3bits.T3IP = 7;   //sets timer priority level to 7
    TMR3 = 0;
    PR3 = 195;
    //IEC0bits.T3IE = ENABLED;     //enables the interrupt flag   SAME AS ABOVE
    T3CONbits.ON = 0;
}