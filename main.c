// ******************************************************************************************* //
//
// File:         lab1p1.c
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


typedef enum state_enum {
  RESET, RUNNING, STOPPED  
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
    timer_flag = 0;
    SYSTEMConfigPerformance(10000000);
    enableInterrupts();
    myState = RESET;
    initLEDs();
    initTimers();
    initSW2();
    initButton();
    initLCD();
    timer_flag = 0;
    
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
            case RESET:
                turnOnLED(RED);
                timer_flag = 0;
                totalTime = 0;
                //clearLCD();
                printTime(0, STOPPED_CONST);
                delayUs(30000);
                break;
            case STOPPED:
                turnOnLED(RED);
                timer_flag = 0;
                totalTime += 10*timer_flag;
                printTime(totalTime, STOPPED_CONST);
                delayUs(30000);
                break;
            case RUNNING:
                turnOnLED(GREEN);
                moveCursorLCD(0, 4);
                if(timer_flag >= 6) {
                    totalTime += 10*timer_flag;
                    timer_flag = 0;
                    printTime(totalTime, RUNNING_CONST);
                }
                break;
        }
    }
    
    return 0;
}

void __ISR(_TIMER_1_VECTOR, IPL7SRS) timer1Handler(void){
    IFS0bits.T1IF = FLAG_DOWN;
    timer_flag = timer_flag + 1;    //Increase the 10ms timer count
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
    PORTD;
    PORTA;
    IFS1bits.CNAIF = FLAG_DOWN;
    IFS1bits.CNDIF = FLAG_DOWN;
    IFS0bits.T3IF = 0;      //lower timer 2 flag for delay
    TMR3 = 0;  
    PR3 = 10000;//manually clear timer 2 register
    T3CONbits.ON = 1;       //turn timer 2 on.
    while(IFS0bits.T3IF != 1){};    //5ms delay
    T3CONbits.ON = 0;       //turn off timer 3
    IFS0bits.T3IF = 0;
    read = PORTAbits.RA7;
    read_reset = PORTDbits.RD6;
    if(read == 0 && read_reset != 0){
        switch(myState) {
            case RESET:
                myState = RUNNING;
                break;
            case RUNNING:
                myState = STOPPED;
                break;
            case STOPPED:
                myState = RUNNING;
                break;
            default:
                myState = myState;
                break;
        }
    } else {
        myState = myState;
    }
    if(read_reset == 0) {
        switch(myState) {
            case STOPPED:
                myState = RESET;
                break;
            default:
                myState = myState;
                break;
        }
    }
    else {
        myState = myState;
    }
}
