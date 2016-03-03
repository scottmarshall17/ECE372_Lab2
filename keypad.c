
#include <xc.h>
#include "leds.h"
#include "interrupt.h"
#include "switch.h"
#include "button.h"
#include "timer.h"
#include "lcd.h"
#include "constants.h"
#include "keypad.h"

void initKeypad(void) {
    
    //Set the analog pins to read and output digital values
    ANSELDbits.ANSD3 = DISABLED;
    ANSELDbits.ANSD1 = DISABLED;
    ANSELBbits.ANSB10 = DISABLED;
    ANSELBbits.ANSB12 = DISABLED;
    ANSELBbits.ANSB14 = DISABLED;
    
    //Configure ODC as outputs
    TRISDbits.TRISD12 = OUTPUT;
    TRISDbits.TRISD6 = OUTPUT;
    TRISDbits.TRISD3 = OUTPUT;
    TRISDbits.TRISD1 = OUTPUT;
    
    //Configure input pins for columns of keypad
    TRISBbits.TRISB10 = INPUT;
    TRISBbits.TRISB12 = INPUT;
    TRISBbits.TRISB14 = INPUT;
    
    //Enable the ODC for the row pins
    ODCDbits.ODCD12 = ENABLED;
    ODCDbits.ODCD6 = ENABLED;
    ODCDbits.ODCD3 = ENABLED;
    ODCDbits.ODCD1 = ENABLED;
    
    //Set ODC output as high for all rows
    LATDbits.LATD12 = HI_Z;
    LATDbits.LATD6 = HI_Z;
    LATDbits.LATD3 = HI_Z;
    LATDbits.LATD1 = HI_Z;
    
    //Configure inputs for change notification interrupts
    
    CNENBbits.CNIEB10 = ENABLED;       // Enable interrupt for button
    CNPUBbits.CNPUB10 = ENABLED;        // Enable Internal Pull-up resistor
    CNENBbits.CNIEB12 = ENABLED;       // Enable interrupt for button
    CNPUBbits.CNPUB12 = ENABLED;        // Enable Internal Pull-up resistor
    CNENBbits.CNIEB14 = ENABLED;       // Enable interrupt for button
    CNPUBbits.CNPUB14 = ENABLED;        // Enable Internal Pull-up resistor
    
    
    IPC8bits.CNIP = 7;          //CHANGED THIS!!!!
    IFS1bits.CNBIF = 0;         // Put Interrupt flag down
    IEC1bits.CNBIE = 1;         // Enable overall CN Interrupt
    CNCONBbits.ON = ENABLED;
    
    
    
    return;
}