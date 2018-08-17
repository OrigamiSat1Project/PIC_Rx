#include <xc.h>
#include "WDT.h"
//#include "typeDefine.h"

void InitWDT(void){
    INTCONbits.GIE  = 1;    // Grobal Interrupt Enable bit
    INTCONbits.PEIE = 1;    // Peripheral Interrupt Enable bit
 
    PIR1bits.TMR1IF = 0;    // TIMER1 Overflow Interrupt Flag bit
    PIE1bits.TMR1IE = 1;    // TIMER1 Overflow Interrupt Enable bit
 
 
    T1CONbits.T1CKPS = 2;   // Timer1 Input Clock Prescale Select bits (10 = 1:4 prescale value)
    TMR1 = TIMER_INTERVAL;  // TODO: comment
 
    T1CONbits.TMR1ON = 1;   // Timer1 On bit
    
//    PORTAbits.RA0 = 1;
}

//process command data if the command type is 'WDT' 
void commandWDT(UBYTE command, UBYTE timeHigh, UBYTE timeLow){   //times are given in ms
    if(command = 's'){
        //TODO: method to: "stop send pulse"
    }else{
        //TODO: return error message
    }
}

