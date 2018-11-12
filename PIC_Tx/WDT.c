#include <xc.h>
#include "WDT.h"
#include "Type_define.h"
#include "pinDefine.h"
#include "time.h"
#include "UART.h"
void resetWDT(void);

void Init_WDT(void){
    WDT_POWER = LOW;
    sendPulseWDT();
}

//process command data if the command type is 'WDT' 
void commandWDT(UBYTE command){   //times are given in ms
    if(command = 's'){
        //Command leads to a reset of PIC (instead of "stop send pulse" use "resetWDT", 
        //this should have same result, but safes the time since it is not necessary
        //to wait until WDT realises that phase is missing (would take about 11 sec))
        resetWDT();
    }else{
        //TODO: return error message
    }
}

//Resets the WDT and as consequence also resets the TX_PIC; LOW==ON HIGH==OFF
void resetWDT(void){
    __delay_ms(20);
    if(WDT_POWER== LOW){
       WDT_POWER= HIGH;//TODO: check if a delay is needed
       __delay_ms(50); // if lower delay no reset of PIC
       WDT_POWER= LOW;
    }else{
       WDT_POWER= LOW;//TODO: check if a delay is needed
       __delay_ms(200); // if lower delay no reset of PIC
       WDT_POWER= HIGH;
       __delay_ms(200);
    }
}

//TODO enter sendPulseWDT function at necessary points in main function, considering pulse max distance of 10sec
//to keep WDT from resetting the PIC 
void sendPulseWDT(void){
    if(WDT_POWER== LOW){
//        putChar('W');
        WDT_PULSE = LOW; //no delay necessary
        delay_us(800);
        WDT_PULSE = HIGH;
    }
}
