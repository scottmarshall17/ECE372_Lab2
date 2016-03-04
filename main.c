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
  ENTER_PHASE, SCANNING_R0, SCANNING_R1, SCANNING_R2, SCANNING_R3, READ_INPUT, WRITELCD, PASS_CHECK,
          VALID, INVALID, SET_PASS, STORE_PASS
} state_t;


volatile state_t myState;
volatile int read;
volatile int read_reset;    /*integer to store value of the reset button*/
volatile char timer_flag;   /*The timer flag increments every 10ms*/
volatile char charNum;
/* Please note that the configuration file has changed from lab 0.
 * the oscillator is now of a different frequency.
 */
int main(void)
{
    int totalTime = 0;
    char validChars = 1;
    char passIndex = 0;
    char setPassVar = 0;
    char i = 0;
    int j = 0;
    int k = 0;
    char match = 0;
    char charToPrint = '1';
    char row = 0;
    char charsWritten = 0;
    char buttonPress[12] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    char password[4] = {0, 0, 0, 0};
    char dataBase[16] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 6, 3, 0, 8};   //8529 code
    timer_flag = 0;
    SYSTEMConfigPerformance(10000000);
    enableInterrupts();
    myState = ENTER_PHASE;
    initLEDs();
    initTimers();
    initLCD();
    initKeypad();
    timer_flag = 0;
    moveCursorLCD(0, 0);
    charNum = 0;
    
    while(1)
    {
        switch(myState) {
            case ENTER_PHASE:
                clearLCD();
                moveCursorLCD(0, 5);
                printStringLCD("ENTER:");
                moveCursorLCD(1, 5);
                myState = SCANNING_R0;
                break;
            case SCANNING_R0:
                LATDbits.LATD12 = LOW_Z;
                LATDbits.LATD6 = LOW_Z;
                LATDbits.LATD3 = LOW_Z;
                LATDbits.LATD1 = LOW_Z;
                
                TRISDbits.TRISD6 = INPUT;
                TRISDbits.TRISD3 = INPUT;
                TRISDbits.TRISD1 = INPUT;
                TRISDbits.TRISD12 = OUTPUT;
                
                row = 0;
                //delayUs(3000);
                myState = SCANNING_R1;
                delayUs(SCAN_DELAY);
                break;
            case SCANNING_R1:
                TRISDbits.TRISD12 = INPUT;
                TRISDbits.TRISD6 = INPUT;
                TRISDbits.TRISD3 = INPUT;
                TRISDbits.TRISD1 = OUTPUT;
                
                row = 1;
                //delayUs(3000);
                myState = SCANNING_R2;
                delayUs(SCAN_DELAY);
                break;
            case SCANNING_R2:
                TRISDbits.TRISD12 = INPUT;
                TRISDbits.TRISD6 = INPUT;
                TRISDbits.TRISD1 = INPUT;
                TRISDbits.TRISD3 = OUTPUT;
                
                row = 2;
                //delayUs(3000);
                myState = SCANNING_R3;
                delayUs(SCAN_DELAY);
                break;
            case SCANNING_R3:
                
                TRISDbits.TRISD12 = INPUT;
                TRISDbits.TRISD3 = INPUT;
                TRISDbits.TRISD1 = INPUT;
                TRISDbits.TRISD6 = OUTPUT;
                
                row = 3;
                //delayUs(3000);
                myState = SCANNING_R0;
                delayUs(SCAN_DELAY);
                break;
            case READ_INPUT:
                delayUs(3000);
                if(PORTBbits.RB12 == 0) {   //'1', '4', '7', '*'
                    buttonPress[1 + (row*3)] = 1;
                }
                
                if(PORTBbits.RB10 == 0) {   //'2', '5', '8', '0'
                    buttonPress[0 + (row*3)] = 1;
                }
                
                if(PORTBbits.RB14 == 0) {   //'3', '6', '9', '#'
                    buttonPress[2 + (row*3)] = 1;
                }
                //delayUs(500);
                myState = WRITELCD;
                delayUs(1000);
                break;
            case WRITELCD:
                i = 0;
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
                                if(charNum == 0) {
                                    charToPrint = '*';
                                }
                                else if(charNum < 0 && password[0] != 10) {
                                    myState = INVALID;
                                    charToPrint = ' ';
                                }
                                break;
                            case 11:
                                //charToPrint = '#';
                                charToPrint = ' ';
                                myState = INVALID;
                                break;
                        }
                        buttonPress[i] = 0;
                        password[charNum] = i;
                        charNum++;
                        printCharLCD(charToPrint);
                        charsWritten++;
                    }
                }
                
                
                while(PORTBbits.RB10 == 0 || PORTBbits.RB12 == 0 || PORTBbits.RB14 == 0);
                if(myState == INVALID) {
                    charNum = 0;
                    break;
                }
                if(charNum == 2 && password[0] == 10 && password[1] == 10) {
                    myState = SET_PASS;
                    charNum = 0;
                    break;
                }
                if(charNum < 4) {
                    myState = SCANNING_R0;
                }
                else {
                    myState = PASS_CHECK;
                }
                break;
            case PASS_CHECK:
                charNum = 0;
                charsWritten = 0;
                j = 0;
                k = 0;
                             
                for(j = 0; j < 4; ++j) {
                    match = 0;
                    for(k = 0; k < 4; ++k) {
                        if(dataBase[(4*j) + k] == password[k]) {
                            match = match + 1;
                        }
                    }
                    if(password[j] == 10 || password[j] == 11) {
                        myState = INVALID;
                        validChars = 0;
                        break;
                    }
                    if(match == 4) {
                        myState = VALID;
                        break;
                    }
                }
                if(setPassVar == 1 && validChars == 1) {
                    myState = VALID;
                    break;
                }
                if(match != 4) {
                    validChars = 1;
                   myState = INVALID; 
                }
                
                break;
                
            case VALID:
                clearLCD();
                moveCursorLCD(0, 5);
                if(setPassVar == 1) {
                    printStringLCD("VALID");
                } else {
                    moveCursorLCD(0, 6);
                    printStringLCD("GOOD");
                }
                for(j = 0; j < 500; j++) {
                    delayUs(4000);
                }
                if(setPassVar == 1) {
                    myState = STORE_PASS;
                }
                else {
                    myState = ENTER_PHASE;
                }
                
                break;
                
            case INVALID:
                clearLCD();
                moveCursorLCD(0, 4);
                if(setPassVar == 1) {
                    printStringLCD("INVALID");
                } else {
                    moveCursorLCD(0, 7);
                    printStringLCD("BAD");
                }
                
                for(j = 0; j < 500; j++) {
                    delayUs(4000);
                }
                if(setPassVar == 1) {
                    setPassVar = 0;
                    myState = ENTER_PHASE;
                }
                else {
                    myState = ENTER_PHASE;
                }
                break;
                
            case SET_PASS:
                clearLCD();
                moveCursorLCD(0, 2);
                printStringLCD("SET PASSWORD");
                moveCursorLCD(1, 5);
                setPassVar = 1;
                myState = SCANNING_R0;
                break;
                
            case STORE_PASS:
                setPassVar = 0;
                for(k = 0; k < 4; ++k) {
                    dataBase[passIndex*4 + k] = password[k];
                }
                passIndex++;
                passIndex = passIndex == 4 ? 0:passIndex;
                charNum = 0;
                myState = ENTER_PHASE;
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
