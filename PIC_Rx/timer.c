#include <xc.h>
#include "timer.h"
#include "UART.h"
#include "typeDefine.h"
#include "pinDefine.h"

void initInterrupt(void){
    INTCON = 0b11100000;    //GIE = 1, PEIE = 1, TMR0IE = 1
    OPTION_REG = 0b11000111;    //prescaler is assigned, TMR0 rate 1:256
    TMR0 = 0x00;    //Initializing Timer0 Module Register 
}

void interrupt TimerReset(void){
    if(INTCONbits.TMR0IF){
        INTCONbits.TMR0IF = 0;
        TMR0 = 0x00;
        timer_counter += 0;
    }
    if(timer_counter >= 9766){
        timer_counter = 0;
        putChar('r');
    }
}