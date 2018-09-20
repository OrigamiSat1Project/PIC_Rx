#include <xc.h>
#include "WDT.h"
#include "typeDefine.h"
#include "pinDefine.h"
 
//EXTERNAL Watchdog Timer is used!!
// for reset of PIC the MCLR pin is used

//process command data if the command type is 'WDT' 
void commandWDT(UBYTE command, UBYTE timeHigh, UBYTE timeLow){   //times are given in ms
    if(command = 's'){
        //Command leads to a reset of PIC (instead of "stop send pulse" use "resetWDT", 
        //this should have same result, but safes the time since it is not necessary
        //to wait until WDT realises that phase is missing (would take about 11 sec))
        resetWDT();
    }else{
        //TODO: return error message
    }
}

//Resets the WDT and as consequence also resets the PIC LOW==ON HIGH==OFF
void resetWDT(void){
    if(POWER_WDT== LOW){
        POWER_WDT== HIGH;//TODO: check if a delay is needed
        POWER_WDT== LOW;
    }else{
        POWER_WDT== LOW;//TODO: check if a delay is needed
        POWER_WDT== HIGH;
    }
}

//TODO enter sendPulseWDT function at necessary points in main function, considering pulse max distance of 10sec
//to keep WDT from resetting the PIC 
void sendPulseWDT(void){
    if(POWER_WDT== LOW){
        PULSE_WDT = LOW; //TODO: check if a delay is needed for the pulse
        PULSE_WDT = HIGH;
    }
}
