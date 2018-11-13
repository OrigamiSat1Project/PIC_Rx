//#include <string.h>
#include <xc.h>
#include "EPS.h"
#include "pinDefine.h"
#include "time.h"
#include "FMCW.h"
#include "MPU.h"
#include "WDT.h"

// turns of the electrical power system for 500 msec by switching from 0 ¨ 1 ¨ 0
void Reset_EPS(void){
    sendPulseWDT();
    SEP_SW = HIGH;     //EPS off -> 5VBUS off
    RBF_SW = LOW;
    __delay_ms(500);  
    SEP_SW = LOW;      //EPS on  -> 800ms INTERVAL ->5VBUS on
    RBF_SW = LOW;
    __delay_ms(2000);
    sendPulseWDT();
}

