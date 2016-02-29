// ******************************************************************************************* //
//  This comment is a test for the github repository.
// File:         lab2p1.c
// Date:         
// Authors:      
//
// Description: Part 1 for lab 1
// ******************************************************************************************* //

#include <xc.h>
#include <sys/attribs.h>
#include "config.h"
#include "constants.h"
#include "leds.h"
#include "interrupt.h"
#include "switch.h"
#include "button.h"
#include "timer.h"
#include "lcd.h"
#include "keypad.h"


typedef enum state_enum {
  SCANNING, READ_INPUT, WRITELCD  
} state_t;


volatile state_t myState;
volatile int read;
volatile int read_reset;    /*integer to store value of the reset button*/
volatile char timer_flag;   /*The timer flag increments every 10ms*/
/* Please note that the configuration file has changed from lab 0.
 * the oscillator is now of a different frequency.
 */
int main(void)
{
    int totalTime = 0;
    char buttonPress[] = {0, 0, 0};
    timer_flag = 0;
    SYSTEMConfigPerformance(10000000);
    enableInterrupts();
    myState = SCANNING;
    initLEDs();
    initTimers();
    initLCD();
    initKeypad();
    timer_flag = 0;
    moveCursorLCD(0, 0);
    
    while(1)
    {
        /* 
        ** This stopwatch functions using a timer set with a period of 10ms. Every 10ms the 
        ** interrupt triggers and the timer_flag variable increments. After a certain amount of time
        ** based on the state, the total_time is increased by 10ms*timer_flag value. This value is
        ** Then printed to the LCD. FAQ: Why the delays? A: The screen takes a non-zero amount of time
        ** to print to the screen, so we delay to allow the LCD to finish updating the screen. 
        */
        switch(myState) {
            case SCANNING:
                LATDbits.LATD12 = LOW_Z;
                //delayUs(3000);
                break;
            case READ_INPUT:
                delayUs(3000);
                if(PORTBbits.RB10 == 0 && buttonPress[0] != 2) {
                    buttonPress[0] = 1;
                }
                if(PORTBbits.RB10 == 0 && buttonPress[0] == 2) {
                    buttonPress[0] = 0;
                }
                if(PORTBbits.RB12 == 0 && buttonPress[1] != 2) {
                    buttonPress[1] = 1;
                }
                if(PORTBbits.RB12 == 0 && buttonPress[1] == 2) {
                    buttonPress[1] = 0;
                }
                if(PORTBbits.RB14 == 0 && buttonPress[2] != 2) {
                    buttonPress[2] = 1;
                }
                if(PORTBbits.RB14 == 0 && buttonPress[2] == 2) {
                    buttonPress[2] = 0;
                }
                //delayUs(500);
                myState = WRITELCD;
                break;
            case WRITELCD:
                if(buttonPress[0] == 1) {
                    buttonPress[0] = 2;
                    printCharLCD('2');
                }
                if(buttonPress[1] == 1) {
                    buttonPress[1] = 2;
                    printCharLCD('1');
                }
                if(buttonPress[2] == 1) {
                    buttonPress[2] = 2;
                    printCharLCD('3');
                }
                //delayUs(1000);
                myState = SCANNING;
                break;
        }
    }
    
    return 0;
}

void __ISR(_TIMER_1_VECTOR, IPL7SRS) timer1Handler(void){
    IFS0bits.T1IF = FLAG_DOWN;
    TMR1 = 0;
}

/*
** The change notification interrupt handles the start/stop button and the 
** reset button logic, hence its length. There are really only two switch statements
** in the ISR, so it really doesn't have to make many decisions that would 
** bloat the ISR.
*/

void __ISR(_CHANGE_NOTICE_VECTOR, IPL7SRS) _CNInterrupt(void){
    //TODO: Implement the interrupt to capture the press of the button
    PORTB;
    IFS1bits.CNBIF = FLAG_DOWN;
    IFS0bits.T3IF = 0;      //lower timer 2 flag for delay
    TMR3 = 0;  
    PR3 = 10000;//manually clear timer 2 register
    T3CONbits.ON = 1;       //turn timer 2 on.
    while(IFS0bits.T3IF != 1){};    //5ms delay
    T3CONbits.ON = 0;       //turn off timer 3
    IFS0bits.T3IF = 0;
    read = PORTBbits.RB12;
    if(1){
        switch(myState) {
            case SCANNING:
                myState = READ_INPUT;
                break;
            default:
                myState = myState;
                break;
        }
    } else {
        myState = myState;
    }
}
