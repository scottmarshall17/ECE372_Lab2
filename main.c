// ******************************************************************************************* //
//  This comment is a test for the github repository. test again.
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
  SCANNING_R0, SCANNING_R1, SCANNING_R2, SCANNING_R3, READ_INPUT, WRITELCD  
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
    char i = 0;
    char charToPrint = '1';
    char row = 0;
    char charsWritten = 0;
    char buttonPress[12] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    timer_flag = 0;
    SYSTEMConfigPerformance(10000000);
    enableInterrupts();
    myState = SCANNING_R0;
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
            case SCANNING_R0:
                LATDbits.LATD12 = LOW_Z;
                LATDbits.LATD6 = HI_Z;
                LATDbits.LATD3 = HI_Z;
                LATDbits.LATD1 = HI_Z;
                row = 0;
                delayUs(3000);
                myState = SCANNING_R1;
                break;
            case SCANNING_R1:
                LATDbits.LATD12 = HI_Z;
                LATDbits.LATD6 = LOW_Z;
                LATDbits.LATD3 = HI_Z;
                LATDbits.LATD1 = HI_Z;
                row = 1;
                delayUs(3000);
                myState = SCANNING_R2;
                break;
            case SCANNING_R2:
                LATDbits.LATD12 = HI_Z;
                LATDbits.LATD6 = HI_Z;
                LATDbits.LATD3 = LOW_Z;
                LATDbits.LATD1 = HI_Z;
                row = 2;
                delayUs(3000);
                myState = SCANNING_R3;
                break;
            case SCANNING_R3:
                LATDbits.LATD12 = HI_Z;
                LATDbits.LATD6 = HI_Z;
                LATDbits.LATD3 = HI_Z;
                LATDbits.LATD1 = LOW_Z;
                row = 3;
                delayUs(3000);
                myState = SCANNING_R0;
                break;
            case READ_INPUT:
                delayUs(3000);
                if(PORTBbits.RB12 == 0 && (buttonPress[1 + (row*3)] != 2)) {   //'1', '4', '7', '*'
                    buttonPress[1 + row*3] = 1;
                }
                
                if(PORTBbits.RB10 == 0 && buttonPress[0 + row*3] != 2) {   //'2', '5', '8', '0'
                    buttonPress[0 + row*3] = 1;
                }
                
                if(PORTBbits.RB14 == 0 && buttonPress[2 + row*3] != 2) {   //'3', '6', '9', '#'
                    buttonPress[2 + row*3] = 1;
                }
                //delayUs(500);
                myState = WRITELCD;
                break;
            case WRITELCD:
                
                for(i = 0; i < 12; ++i) {
                    if(buttonPress[i] == 1) {
                        switch(i) {
                            case 0:
                                charToPrint = '2';
                                break;
                            case 1:
                                charToPrint = '1';
                                break;
                            case 2:
                                charToPrint = '3';
                                break;
                            case 3:
                                charToPrint = '5';
                                break;
                            case 4:
                                charToPrint = '4';
                                break;
                            case 5:
                                charToPrint = '6';
                                break;
                            case 6:
                                charToPrint = '8';
                                break;
                            case 7:
                                charToPrint = '7';
                                break;
                            case 8:
                                charToPrint = '9';
                                break;
                            case 9:
                                charToPrint = '0';
                                break;
                            case 10:
                                charToPrint = '*';
                                break;
                            case 11:
                                charToPrint = '#';
                                break;
                        }
                        buttonPress[i] = 0;
                        printCharLCD(charToPrint);
                        charsWritten++;
                        if(charsWritten == 16) {
                            moveCursorLCD(1,0);
                        }
                        if(charsWritten == 32) {
                            moveCursorLCD(0,0);
                            charsWritten = 0;
                        }
                    }
                }
                /*
                if(buttonPress[0] == 1) {
                    buttonPress[0] = 0;     //'2' on Keypad
                    printCharLCD('2');
                    charsWritten++;
                    if(charsWritten == 16) {
                        moveCursorLCD(1,0);
                    }
                    if(charsWritten == 32) {
                        moveCursorLCD(0,0);
                        charsWritten = 0;
                    }
                }
                if(buttonPress[1] == 1) {
                    buttonPress[1] = 0;     //'1' on Keypad
                    printCharLCD('1');
                    charsWritten++;
                    if(charsWritten == 16) {
                        moveCursorLCD(1,0);
                    }
                    if(charsWritten == 32) {
                        moveCursorLCD(0,0);
                        charsWritten = 0;
                    }
                }
                if(buttonPress[2] == 1) {
                    buttonPress[2] = 0;
                    printCharLCD('3');
                    charsWritten++;
                    if(charsWritten == 16) {
                        moveCursorLCD(1,0);
                    }
                    if(charsWritten == 32) {
                        moveCursorLCD(0,0);
                        charsWritten = 0;
                    }
                }
                //delayUs(1000);
                 */
                
                while(PORTBbits.RB10 == 0 || PORTBbits.RB12 == 0 || PORTBbits.RB14 == 0);
                myState = SCANNING_R0;
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
            case SCANNING_R0:
                myState = READ_INPUT;
                break;
            case SCANNING_R1:
                myState = READ_INPUT;
                break;
            case SCANNING_R2:
                myState = READ_INPUT;
                break;
            case SCANNING_R3:
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
