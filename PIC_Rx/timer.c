#include <xc.h>
#include "timer.h"
#include "UART.h"
#include "typeDefine.h"
#include "pinDefine.h"
#include "EPS.h"
#include "time.h"
#include "FMCW.h"

void initTimer(void){
    INTCON = 0b10100000;    //GIE = 1, TMR0IE = 1
    OPTION_REG = 0b01000111;    //prescaler is assigned, TMR0 rate 1:256
    TMR0 = 0x00;    //Initializing Timer0 Module Register 
}

//for debug function
void interrupt TimerCheck(void){
    if(INTCONbits.TMR0IF){
        INTCONbits.TMR0IF = 0;
        TMR0 = 0x00;
        timer_counter += 1;
    }
    if(timer_counter >= one_second){
        timer_counter = 0;
        second_counter += 1;
        LED_WHITE = 1 - LED_WHITE;  //for debug
        if(second_counter >= one_minute){
            second_counter = 0;
            minute_counter += 1;
            if(minute_counter >= 3){
                Reset_EPS();
                delay_ms(5000);


                //reset PLL setting (because it gets lost during shutdown)
//                FMTX(FMTX_Nref, FMTX_Nprg);
//                CWTX(CWTX_Nref, CWTX_Nprg);
//                FMRX(FMRX_Nref, FMRX_Nprg);
                __delay_ms(500);
            }
        }
    }
}

//EPS reset every week
//void interrupt TimerReset(void){
//    if(INTCONbits.TMR0IF){
//        INTCONbits.TMR0IF = 0;
//        TMR0 = 0x00;
//        timer_counter += 1;
//    }
//    if(timer_counter >= one_second){
//        timer_counter = 0;
//        second_counter += 1;
//        LED_WHITE = 1 - LED_WHITE;  //for debug
//        if(second_counter >= one_minute){
//            second_counter = 0;
//            minute_counter += 1;
//            if(minute_counter >= one_hour){
//                minute_counter = 0;
//                hour_counter += 1;
//                if(hour_counter >= one_day){
//                    hour_counter = 0;
//                    day_counter += 1;
//                    if(day_counter >= one_week){
//                        day_counter = 0;
//                        Reset_ERP();
//                        delay_ms(5000);
//
//                        int FMTX_Nprg[5]     =   {8,7,5,0,1};   // Nprg = 87300 = Ftx / 0.05 [436.500MHz]
//                        int CWTX_Nprg[5]     =   {0,1,4,0,0};   // Nprg = 1747(* see 301ACWPLL-20080520.pdf *) [436.750MHz]
//                        int FMRX_Nprg[5]     =   {2,4,9,1,6};   // Nprg = 24887 = (Frx - 21.4) / 0.05 [145.835MHz]
//
//                        //reset PLL setting (because it gets lost during shutdown)
//                        FMTX(FMTX_Nref, FMTX_Nprg);
//                        CWTX(CWTX_Nref, CWTX_Nprg);
//                        FMRX(FMRX_Nref, FMRX_Nprg);
//                        __delay_ms(500);
//                    }
//                }
//            }
//        }
//    }
//}