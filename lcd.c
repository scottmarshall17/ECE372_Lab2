/*
 * File:   lcd.c
 * Authors:
 *
 * Created on December 31, 2014, 1:39 PM
 */

/* TODO: Make define statements for each pin used in the LCD
 */
#include <xc.h>
#include <string.h>
#include "constants.h"
#include "lcd.h"
#include "timer.h"

/* The pins that you will use for the lcd control will be
 * 
 * LCD_RS      RC4
 * LCD_E       RC2
 * LCD_D4      RE7
 * LCD_D5      RE5
 * LCD_D6      RE3
 * LCD_D7      RE1
 * Use these so that we will be able to test your code for grading and to
 * help you debug your implementation!
 */

#define LCD_DATA  
#define LCD_RS  
#define LCD_E   

#define TRIS_D7 TRISE1
#define TRIS_D6 TRISE3
#define TRIS_D5 TRISE5
#define TRIS_D4 TRISE7
#define TRIS_RS TRISC4
#define TRIS_E  TRISC2
#define TRIS_RW TRISC3

#define LAT_D7 LATE1
#define LAT_D6 LATE3
#define LAT_D5 LATE5
#define LAT_D4 LATE7
#define LAT_RS LATC4
#define LAT_E  LATC2
#define LAT_RW LATC3
#define E_DELAY 500
#define DATA 1
#define COMMAND 0

/* The toggleE() function turns the E signal on for E_DELAY microseconds and off for E_DELAY us
 */

void toggleE() {
    delayUs(5);
    LATCbits.LAT_E = ENABLED;
    delayUs(E_DELAY);
    LATCbits.LAT_E = DISABLED;
    delayUs(E_DELAY);
    return;
}

/*The writeFourBits() function takes the lower four bits of the word and writes to the LCD*/
void writeFourBits(char word, unsigned int commandType, unsigned int delayAfter, unsigned int lower){
    //TODO:
    if(commandType == DATA){
        LATCbits.LAT_RS = DATA;
    }
    else {
        LATCbits.LAT_RS = COMMAND;
    }
    
    // set the commandType (RS value)
    LATEbits.LAT_D7 = (word&(0b1000)) >> 3;
    LATEbits.LAT_D6 = (word&(0b0100)) >> 2;
    LATEbits.LAT_D5 = (word&(0b0010)) >> 1;
    LATEbits.LAT_D4 = word&(0b0001);
    
    //enable
    toggleE();
    if(lower == 1) {
        delayUs(delayAfter);
    }
    //delay
    //disable
    return;
}

/* 
 * The writeLCD() function uses the writeFourBits() function to send an 8 bit command/data to the
 * LCD.
 */
void writeLCD(unsigned char word, unsigned int commandType, unsigned int delayAfter){
    //TODO:
    char first = (word&0xF0)>>4;    //USED E INSTEAD OF F
    char second = word&0x0F;
    writeFourBits(first, commandType, 0, 0);
    writeFourBits(second, commandType, delayAfter, 1);
}

/*
 * The printCharLCD() function takes a character and prints it to the LCD screen
 */
void printCharLCD(unsigned char c) {
    //TODO:
    writeLCD(c, DATA, 100);
}
/*Initialize the LCD
 */

/*
** The initLCD() function initializes the LCD to be configured for the 4-bit interface.
 * This function COULD be shorter if the writeLCD() function was used, but this 
 * function was written before the writeLCD() function. IF IT ISN'T BROKEN, DON'T FIX IT!!!!
 * IF IT ISN'T BROKEN, DON'T FIX IT!!!! IF IT ISN'T BROKEN, DON'T FIX IT!!!!
*/
void initLCD(void) {
	//reference: http://web.alfredstate.edu/weimandn/lcd/lcd_initialization/lcd_initialization_index.html
    // Setup D, RS, and E to be outputs (0).
	TRISCbits.TRIS_RS = OUTPUT;
	TRISCbits.TRIS_E = OUTPUT;
	TRISEbits.TRIS_D7 = OUTPUT;
	TRISEbits.TRIS_D6 = OUTPUT;
	TRISEbits.TRIS_D5 = OUTPUT;
	TRISEbits.TRIS_D4 = OUTPUT;
	TRISCbits.TRIS_RW = OUTPUT;
	
    LATCbits.LAT_E = DISABLED;
	LATCbits.LAT_RS = DISABLED;
	LATCbits.LAT_RW = DISABLED;
	LATEbits.LAT_D7 = DISABLED;
	LATEbits.LAT_D6 = DISABLED;
	LATEbits.LAT_D5 = DISABLED;
	LATEbits.LAT_D4 = DISABLED;
	LATCbits.LAT_E = DISABLED;
    
	delayUs(16000);
	
	LATCbits.LAT_E = DISABLED;
	LATCbits.LAT_RS = DISABLED;
	LATCbits.LAT_RW = DISABLED;
	LATEbits.LAT_D7 = DISABLED;
	LATEbits.LAT_D6 = DISABLED;
	LATEbits.LAT_D5 = ENABLED;
	LATEbits.LAT_D4 = ENABLED;
	LATCbits.LAT_E = ENABLED;
    
    delayUs(500);
    LATCbits.LAT_E = DISABLED;
    delayUs(500);
	
	delayUs(4150); 	//more than 4.1ms
	
	LATCbits.LAT_E = DISABLED;
	delayUs(10);
	LATCbits.LAT_RS = DISABLED;
	LATCbits.LAT_RW = DISABLED;
	LATEbits.LAT_D7 = DISABLED;
	LATEbits.LAT_D6 = DISABLED;
	LATEbits.LAT_D5 = ENABLED;
	LATEbits.LAT_D4 = ENABLED;
	delayUs(500);
	LATCbits.LAT_E = ENABLED;
    
    delayUs(500);
    LATCbits.LAT_E = DISABLED;
    delayUs(500);
	
	delayUs(110); 	//more than 100 Us
	
	LATCbits.LAT_E = DISABLED;
	delayUs(10);
	LATCbits.LAT_RS = DISABLED;
	LATCbits.LAT_RW = DISABLED;
	LATEbits.LAT_D7 = DISABLED;
	LATEbits.LAT_D6 = DISABLED;
	LATEbits.LAT_D5 = ENABLED;
	LATEbits.LAT_D4 = ENABLED;
	delayUs(500);
	LATCbits.LAT_E = ENABLED;
    
    delayUs(500);
    LATCbits.LAT_E = DISABLED;
	
	delayUs(10); 	//for good measure
	
	LATCbits.LAT_E = DISABLED;
	delayUs(10);
	LATCbits.LAT_RS = DISABLED;
	LATCbits.LAT_RW = DISABLED;
	LATEbits.LAT_D7 = DISABLED;
	LATEbits.LAT_D6 = DISABLED;
	LATEbits.LAT_D5 = ENABLED;
	LATEbits.LAT_D4 = DISABLED; 	//first unset bit in last block
	delayUs(500);
	LATCbits.LAT_E = ENABLED;
    
    delayUs(500);
    LATCbits.LAT_E = DISABLED;
	
	delayUs(10); 	//for good measure
	
	LATCbits.LAT_E = DISABLED;
	delayUs(5);
	LATCbits.LAT_RS = DISABLED;
	LATCbits.LAT_RW = DISABLED;
	LATEbits.LAT_D7 = DISABLED;
	LATEbits.LAT_D6 = DISABLED;
	LATEbits.LAT_D5 = ENABLED;
	LATEbits.LAT_D4 = DISABLED; 	//second line
	delayUs(500);
	LATCbits.LAT_E = ENABLED;
    
    delayUs(500);
    LATCbits.LAT_E = DISABLED;
	
	delayUs(10); 	//for good measure
	
	LATCbits.LAT_E = DISABLED;
	delayUs(5);
	LATCbits.LAT_RS = DISABLED;
	LATCbits.LAT_RW = DISABLED;
	LATEbits.LAT_D7 = ENABLED; 		//N = 1
	LATEbits.LAT_D6 = DISABLED; 	//F = 0
	LATEbits.LAT_D5 = DISABLED;
	LATEbits.LAT_D4 = DISABLED; 	//third line
	delayUs(500);
	LATCbits.LAT_E = ENABLED;
    
    delayUs(500);
    LATCbits.LAT_E = DISABLED;
	
	delayUs(500); 	//for good measure
	
	LATCbits.LAT_E = DISABLED;
	delayUs(5);
	LATCbits.LAT_RS = DISABLED;
	LATCbits.LAT_RW = DISABLED;
	LATEbits.LAT_D7 = DISABLED;
	LATEbits.LAT_D6 = DISABLED;
	LATEbits.LAT_D5 = DISABLED;
	LATEbits.LAT_D4 = DISABLED; 	//fourth line
	delayUs(5);
	LATCbits.LAT_E = ENABLED;
	
    delayUs(500);
    LATCbits.LAT_E = DISABLED;
    
	delayUs(500); 	//for good measure
	
	LATCbits.LAT_E = DISABLED;
	delayUs(5);
	LATCbits.LAT_RS = DISABLED;
	LATCbits.LAT_RW = DISABLED;
	LATEbits.LAT_D7 = ENABLED;
	LATEbits.LAT_D6 = DISABLED;
	LATEbits.LAT_D5 = DISABLED;
	LATEbits.LAT_D4 = DISABLED; 	//fifth line
	delayUs(500);
	LATCbits.LAT_E = ENABLED;
    
    delayUs(500);
    LATCbits.LAT_E = DISABLED;
	
	delayUs(500); 	//for good measure
	
	LATCbits.LAT_E = DISABLED;
	delayUs(5);
	LATCbits.LAT_RS = DISABLED;
	LATCbits.LAT_RW = DISABLED;
	LATEbits.LAT_D7 = DISABLED;
	LATEbits.LAT_D6 = DISABLED;
	LATEbits.LAT_D5 = DISABLED;
	LATEbits.LAT_D4 = DISABLED; 	//sixth line
	delayUs(500);
	LATCbits.LAT_E = ENABLED;
    
    delayUs(500);
    LATCbits.LAT_E = DISABLED;
	
	delayUs(500); 	//for good measure
	
	LATCbits.LAT_E = DISABLED;
	delayUs(5);
	LATCbits.LAT_RS = DISABLED;
	LATCbits.LAT_RW = DISABLED;
	LATEbits.LAT_D7 = DISABLED;
	LATEbits.LAT_D6 = DISABLED;
	LATEbits.LAT_D5 = DISABLED; 	//Display Clear
	LATEbits.LAT_D4 = ENABLED; 		//seventh line
	delayUs(500);
	LATCbits.LAT_E = ENABLED;
    
    delayUs(500);
    LATCbits.LAT_E = DISABLED;
	
	delayUs(4000); 	//for good measure to clear display (takes longer than usual)
	
	LATCbits.LAT_E = DISABLED;
	delayUs(5);
	LATCbits.LAT_RS = DISABLED;
	LATCbits.LAT_RW = DISABLED;
	LATEbits.LAT_D7 = DISABLED;
	LATEbits.LAT_D6 = DISABLED;
	LATEbits.LAT_D5 = DISABLED;
	LATEbits.LAT_D4 = DISABLED; 	//Eighth line
	delayUs(5);
	LATCbits.LAT_E = ENABLED;
    
    delayUs(500);
    LATCbits.LAT_E = DISABLED;
	
	delayUs(500); 	//for good measure
	
	LATCbits.LAT_E = DISABLED;
	delayUs(5);
	LATCbits.LAT_RS = DISABLED;
	LATCbits.LAT_RW = DISABLED;
	LATEbits.LAT_D7 = DISABLED;
	LATEbits.LAT_D6 = ENABLED; 		
	LATEbits.LAT_D5 = ENABLED; 		//increment I/D
	LATEbits.LAT_D4 = DISABLED; 		//Ninth and Last line 	--cursor shift(datasheet)
	delayUs(500);
	LATCbits.LAT_E = ENABLED;
    
    delayUs(500);
    LATCbits.LAT_E = DISABLED;
	
	delayUs(500);
	//initialization has ended, turning screen on now.------------------------
    
    //first nibble
    LATCbits.LAT_E = DISABLED;
	delayUs(5);
	LATCbits.LAT_RS = DISABLED;
	LATCbits.LAT_RW = DISABLED;
	LATEbits.LAT_D7 = DISABLED;
	LATEbits.LAT_D6 = DISABLED; 	//Turn screen on
	LATEbits.LAT_D5 = DISABLED; 	//Turn cursor off
	LATEbits.LAT_D4 = DISABLED; 	//Turn Blink off
	delayUs(5);
	LATCbits.LAT_E = ENABLED;
    delayUs(500);
	//second nibble
	LATCbits.LAT_E = DISABLED;
	delayUs(500);
	LATCbits.LAT_RS = DISABLED;
	LATCbits.LAT_RW = DISABLED;
	LATEbits.LAT_D7 = ENABLED;
	LATEbits.LAT_D6 = ENABLED; 	//Turn screen on
	LATEbits.LAT_D5 = DISABLED; 	//Turn cursor off
	LATEbits.LAT_D4 = DISABLED; 	//Turn Blink off
	delayUs(5);
	LATCbits.LAT_E = ENABLED;
    
    delayUs(500);
    LATCbits.LAT_E = DISABLED;
	
    
    //------------------Set DD RAM address------------
    //first nibble
    LATCbits.LAT_E = DISABLED;
	delayUs(500);
	LATCbits.LAT_RS = DISABLED;
	LATCbits.LAT_RW = DISABLED;
	LATEbits.LAT_D7 = ENABLED;
	LATEbits.LAT_D6 = DISABLED; 	//Turn screen on
	LATEbits.LAT_D5 = DISABLED; 	//Turn cursor off
	LATEbits.LAT_D4 = DISABLED; 	//Turn Blink off
	delayUs(5);
	LATCbits.LAT_E = ENABLED;
    delayUs(500);
	//second nibble
	LATCbits.LAT_E = DISABLED;
	delayUs(500);
	LATCbits.LAT_RS = DISABLED;
	LATCbits.LAT_RW = DISABLED;
	LATEbits.LAT_D7 = DISABLED;
	LATEbits.LAT_D6 = DISABLED; 	//Turn screen on
	LATEbits.LAT_D5 = DISABLED; 	//Turn cursor off
	LATEbits.LAT_D4 = DISABLED; 	//Turn Blink off
	delayUs(5);
	LATCbits.LAT_E = ENABLED;
    
    delayUs(500);
    LATCbits.LAT_E = DISABLED;
    
   /*
    //---------------Write some character---------------------------
    //first nibble
    LATCbits.LAT_E = DISABLED;
	delayUs(500);
	LATCbits.LAT_RS = ENABLED;
	LATCbits.LAT_RW = DISABLED;
	LATEbits.LAT_D7 = ENABLED;
	LATEbits.LAT_D6 = DISABLED; 	//Turn screen on
	LATEbits.LAT_D5 = ENABLED; 	//Turn cursor off
	LATEbits.LAT_D4 = DISABLED; 	//Turn Blink off
	delayUs(5);
	LATCbits.LAT_E = ENABLED;
    delayUs(500);
	//second nibble
	LATCbits.LAT_E = DISABLED;
	delayUs(500);
	LATCbits.LAT_RS = ENABLED;
	LATCbits.LAT_RW = DISABLED;
	LATEbits.LAT_D7 = ENABLED;
	LATEbits.LAT_D6 = DISABLED; 	//Turn screen on
	LATEbits.LAT_D5 = ENABLED; 	//Turn cursor off
	LATEbits.LAT_D4 = DISABLED; 	//Turn Blink off
	delayUs(5);
	LATCbits.LAT_E = ENABLED;
    
    delayUs(500);
    LATCbits.LAT_E = DISABLED;
    delayUs(500);
    * */
    
     

    
    //writeLCD(0b00010100, COMMAND, 100);  //Sets the cursor to increment on writes
    
    // Initilization sequence utilizes specific LCD commands before the general configuration
    // commands can be utilized. The first few initilition commands cannot be done using the
    // WriteLCD function. Additionally, the specific sequence and timing is very important.

    // Enable 4-bit interface

    // Function Set (specifies data width, lines, and font.

    // 4-bit mode initialization is complete. We can now configure the various LCD
    // options to control how the LCD will function.

    // TODO: Display On/Off Control
        // Turn Display (D) Off
    // TODO: Clear Display (The delay is not specified in the data sheet at this point. You really need to have the clear display delay here.
    // TODO: Entry Mode Set
        // Set Increment Display, No Shift (i.e. cursor move)
    // TODO: Display On/Off Control
        // Turn Display (D) On, Cursor (C) Off, and Blink(B) Off
}

/*
 * The printStringLCD() function prints a string of characters to the LCD
 */
void printStringLCD(const char* s) {
    //TODO: BOGUS
    unsigned char i = 0;
    while(s[i] != '\0') {
        printCharLCD(s[i]);
        i = i + 1;
    }
}

/*
 * Clears the display.
 */
void clearLCD(){
    writeLCD(0x01, COMMAND, 1700);
}

/*
 * Changes the DD RAM address to put the cursor somewhere.
 */
void moveCursorLCD(unsigned char x, unsigned char y){
    unsigned char addr = 0x80;
    if(x == 1){
        addr = addr + 0x40;
    }
    if(y >= 0x00 && y < 0x10) {
        addr = addr + y;
    }
    writeLCD(addr, COMMAND, 50);
}

/*
 * This function is called in lab1p2.c for testing purposes.
 * If everything is working properly, you should get this to look like the video on D2L
 * However, it is suggested that you test more than just this one function.
 */
void testLCD(){
    initLCD();
    int i = 0;
    while(1){
        printCharLCD('c');
        for(i = 0; i < 1000; i++) delayUs(1000);
        clearLCD();
        printStringLCD("Hello!");
        moveCursorLCD(1, 2);
        for(i = 0; i < 1000; i++) delayUs(1000);
        printStringLCD("Hello!");
        for(i = 0; i < 1000; i++) delayUs(1000);
        clearLCD();
    }
}

/*
 * The myTestLCD() function was originally created for preliminary testing *DEPRECATED*
 */
void myTestLCD() {
    char test = 0b01100110;
    initLCD();
    
    /*
    LATCbits.LAT_RS = DATA;
    LATEbits.LAT_D7 = ((test>>4)&0b1000) >> 3;
    LATEbits.LAT_D6 = (test>>4)&0b0100;
    LATEbits.LAT_D5 = (test>>4)&0b0010;
    LATEbits.LAT_D4 = (test>>4)&0b0001;
    
    toggleE();
    
    LATCbits.LAT_RS = DATA;
    LATEbits.LAT_D7 = DISABLED;
    LATEbits.LAT_D6 = ENABLED;
    LATEbits.LAT_D5 = ENABLED;
    LATEbits.LAT_D4 = DISABLED;
    
    toggleE();
     * */
    writeFourBits(0b0110, DATA, 500, 1);
    writeFourBits(0b0110, DATA, 500, 1);
   // moveCursorLCD(0, 4);
   //printCharLCD('S');
}


/*
 * The printTime() function takes a time value in milliseconds and a header value 
 * and prints the time and the header to the LCD.
 */
void printTime(int totalTime, char header){
    int mins = totalTime/60000;
    int mins_tens = mins/10;
    int secs = 0;
    int secs_tens = 0;
    int ms = 0;
    int ms_tens = 0;
    totalTime = totalTime - mins*60000;
    secs = totalTime/1000;
    secs_tens = secs/10;
    totalTime = totalTime - secs*1000;
    ms = totalTime/10;
    ms_tens = ms/10;
    
    if(header == RUNNING_CONST) {
        moveCursorLCD(0, 4);
        printStringLCD("Running:");
    }
    else {
        moveCursorLCD(0, 4);
        printStringLCD("Stopped:");
    }
    
    moveCursorLCD(1,4);
    printCharLCD(mins_tens|(0b00110000));
    printCharLCD((mins%10)|(0b00110000));
    printCharLCD(':');
    printCharLCD(secs_tens|(0b00110000));
    printCharLCD((secs%10)|(0b00110000));
    printCharLCD(':');
    printCharLCD(ms_tens|(0b00110000));
    printCharLCD((ms%10)|(0b00110000));
    
}