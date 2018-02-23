#include <xc.h>
#include "WDT.h"
#include "Type_define.h"
#include "FMCW.h"
#include "time.h"

void Init_WDT(void){
    INTCONbits.GIE  = 1;    // Grobal interrupt??????
    INTCONbits.PEIE = 1;    // ???????????
    
    OPTION_REGbits.PSA = 1;
    OPTION_REGbits.PS = 7;
    
//    WDTCONbits.WDTPS = 11;
 
    PIR1bits.TMR1IF = 0;    // Timer1??????????????
    PIE1bits.TMR1IE = 1;    // Timer1???????
 
 
    T1CONbits.T1CKPS = 3;   // ???????1/1???
    TMR1 = TIMER_INTERVAL;  // ????????????
 
    T1CONbits.TMR1ON = 1;   // Timer1??????
}


void interrupt InterReceiver( void ){
    volatile static int intr_counter;
    volatile static int intr_counter_H;
    if(PIR1bits.TMR1IF == 1){
        TMR1 = TIMER_INTERVAL;  // ?????????
 
        intr_counter++;
        if (intr_counter >= 2) {
            CLRWDT();
            intr_counter = 0;
            intr_counter_H ++;
            if (intr_counter_H >= 60) {
//                int FMTX_Nprg[5]     =   {8,7,3,0,0};   // Nprg = 87300 = Ftx / 0.05 [436.500MHz]
//                int CWTX_Nprg[5]     =   {0,1,7,4,7};   // Nprg = 1747(* see 301ACWPLL-20080520.pdf *) [436.750MHz]
//                int FMRX_Nprg[5]     =   {2,4,8,8,7};   // Nprg = 24887 = (Frx - 21.4) / 0.05 [145.835MHz]
//
//                FMTX(FMTX_Nref, FMTX_Nprg);
//                CWTX(CWTX_Nref, CWTX_Nprg);
//                FMRX(FMRX_Nref, FMRX_Nprg);
//                ResetFreq();
                __delay_ms(500);
            }
        }
 
        PIR1bits.TMR1IF = 0;    // ???????????
    }
}