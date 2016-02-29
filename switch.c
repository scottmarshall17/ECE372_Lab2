/*
 * File:   switch.c
 * Author: Garrett
 *
 * Created on September 19, 2015, 10:46 AM
 */
#include <xc.h>
#include "constants.h"
#include "switch.h"

/*
 * The initSW2() function initializes the start/stop button for the stopwatch
 */
void initSW2(){
    //TODO: Initialize the appropriate pins to work with the external switch using a
    //change notification interrupt.
    TRISAbits.TRISA7 = INPUT;   //enable the button pin as an input
    CNCONAbits.ON = 1;
    CNENAbits.CNIEA7 = 1;       // Enable interrupt for button
    CNPUAbits.CNPUA7 = 1;        // Enable Internal Pull-up resistor
    IPC8bits.CNIP = 7; //CHANGED THIS!!!!
    IFS1bits.CNAIF = 0;         // Put Interrupt flag down
    IEC1bits.CNAIE = 1;         // Enable overall CN Interrupt
    
}
