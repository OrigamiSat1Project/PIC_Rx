#include <xc.h>
#include "WDT.h"
#include "typeDefine.h"
#include "pinDefine.h"
#include "UART.h"
#include "time.h"
 
//EXTERNAL Watchdog Timer is used!!
// for reset of PIC the MCLR pin is used

void InitWDT(void){
    POWER_WDT = LOW;
    sendPulseWDT();
}

//process command data if the command type is 'WDT' 
void commandWDT(UBYTE command){   //times are given in ms
    if(command == 's'){
        //Command leads to a reset of PIC (instead of "stop send pulse" use "resetWDT", 
        //this should have same result, but safes the time since it is not necessary
        //to wait until WDT realises that phase is missing (would take about 11 sec))
        resetWDT();
        //WDTwait();//alternative function to forces WDT to reset PIC, if WDT is turned on (POWER_WDT==LOW)
    }else{
        //TODO: return error message
    }
}

void WDTwait(void){
    __delay_ms(12000);
}

//Resets the WDT and as consequence also resets the PIC LOW==ON HIGH==OFF
void resetWDT(){
    __delay_ms(20);
    if(POWER_WDT== LOW){
        POWER_WDT= HIGH;
        __delay_ms(50); // if lower delay no reset of PIC
        POWER_WDT= LOW;
    }else{
        POWER_WDT= LOW;
        __delay_ms(200); // if lower delay no reset of PIC
        POWER_WDT= HIGH;
    }
}

//TODO enter sendPulseWDT function at necessary points in main function, considering pulse max distance of 10sec
//to keep WDT from resetting the PIC 
void sendPulseWDT(void){
//    putChar('W');
    if(POWER_WDT== LOW){
        PULSE_WDT = LOW; //no delay necessary
        delay_ms(300);
        PULSE_WDT = HIGH;
    }
}
