#include <xc.h>
#include "timer.h"
#include "UART.h"
#include "typeDefine.h"
#include "pinDefine.h"

void initTimer(void){
    INTCON = 0b10100000;    //GIE = 1, PEIE = 0, TMR0IE = 1
    OPTION_REG = 0b01000111;    //prescaler is assigned, TMR0 rate 1:256
    TMR0 = 0x00;    //Initializing Timer0 Module Register 
}
//
void interrupt TimerCheck(void){
    if(INTCONbits.TMR0IF){
        INTCONbits.TMR0IF = 0;
        TMR0 = 0x00;
        timer_counter += 1;
    }
    if(timer_counter >= 62){
        timer_counter = 0;
        LED_WHITE = 1;
        __delay_ms(200);
        LED_WHITE = 0;
    }
}