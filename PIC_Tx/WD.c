#include <xc.h>
#include "WDT.h"
#include "Type_define.h"

void Init_WDT(void){
    INTCONbits.GIE  = 1;    // Grobal interrupt??????
    INTCONbits.PEIE = 1;    // ???????????
 
    PIR1bits.TMR1IF = 0;    // Timer1??????????????
    PIE1bits.TMR1IE = 1;    // Timer1???????
 
 
    T1CONbits.T1CKPS = 2;   // ???????1/1???
    TMR1 = TIMER_INTERVAL;  // ????????????
 
    T1CONbits.TMR1ON = 1;   // Timer1??????
    
//    PORTAbits.RA0 = 1;
}



//void interrupt intr(void){
//    volatile static int intr_counter;
//    if ( PIR1bits.TMR1IF == 1 ) {
//        TMR1 = TIMER_INTERVAL;  // ?????????
// 
//        intr_counter++;
//        if ( intr_counter > 20 ) {
//            intr_counter = 0;
//        }
// 
//        // 0.5sec???RB0???????    
//        if ( intr_counter == 20) {
//            PORTAbits.RA0 = 0;
//            PORTBbits.RB2 = 0;
//        } else {
//            PORTAbits.RA0 = 1;
//            PORTBbits.RB2 = 1;
//        }
// 
//        PIR1bits.TMR1IF = 0;    // ???????????
//    }   
// 
//    return;
//}