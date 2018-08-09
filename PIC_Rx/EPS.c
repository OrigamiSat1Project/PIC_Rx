//#include <string.h>
#include <xc.h>
#include "EPS.h"
#include "pinDefine.h"
#include "time.h"

// turns of the electrical power system for 5 seconds by switching from 0 Å® 1 Å® 0
void Reset_EPS(void){
    SEP_SW = 1;
    RBF_SW = 1;
    __delay_ms(5000);
    SEP_SW = 0;
    RBF_SW = 0;
    __delay_ms(500);
}

