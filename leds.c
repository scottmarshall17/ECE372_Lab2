/*
* File:   leds.c
* Author: 
*
* Created on December 27, 2014, 1:31 PM
*/

#include <xc.h>
#include "leds.h"
#include "constants.h"

void initLEDs(){
    //TODO: Initialize the appropriate pins to work with the LEDs
    TRISGbits.TRISG12 = OUTPUT;     //set LED pins at outputs --RED
    TRISGbits.TRISG14 = OUTPUT;     //--GREEN
    
    ODCGbits.ODCG12 = ENABLED;      //Enable Open-Drain Configuration
    ODCGbits.ODCG14 = ENABLED;
    
    LATGbits.LATG12 = HI_Z;         //Turn off the Red LED
    LATGbits.LATG14 = LOW_Z;        //Turn on the Green LED
}

void turnOnLED(int led) {
    if(led == 0) {  //turns on the Greed LED
        LATGbits.LATG12 = HI_Z;
        LATGbits.LATG14 = LOW_Z;
    }
    else if(led == 1) {     //turns on the Red LED
        LATGbits.LATG12 = LOW_Z;
        LATGbits.LATG14 = HI_Z;
    }
}