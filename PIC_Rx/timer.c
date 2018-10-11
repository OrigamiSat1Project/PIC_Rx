#include <xc.h>
#include "timer.h"
#include "UART.h"
#include "typeDefine.h"
#include "pinDefine.h"
#include "EPS.h"
#include "time.h"
#include "FMCW.h"
#include "ADC.h"

void initTimer(void){
    INTCON = 0b11100000;    //GIE = 1, PEIE = 1, TMR0IE = 1
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
        bat_meas_counter += 1;
        second_counter += 1;
        LED_WHITE = 1 - LED_WHITE;  //for debug
        
        //battery voltage measure
        //treadhold is not determined
        //sampling rate is not determined
        //debug : 6v -> send 's' 7v -> send 'n'
        if(bat_meas_counter >= 3){
            bat_meas_counter = 0;
            ReadBatVoltage();
            if(adcL <= 0xD0) putChar('s');
            else putChar('n');
        }
        
        if(second_counter >= one_minute){
            second_counter = 0;
            minute_counter += 1;
            
            //for debug
            //EPS reset every 3 minitues
            if(minute_counter >= 3){
                Reset_EPS();
                delay_ms(5000);
                //resubstitution Nprg
                FMTX_Nprg[0] = 8; FMTX_Nprg[1] = 7; FMTX_Nprg[2] = 5; FMTX_Nprg[3] = 0; FMTX_Nprg[4] = 1;
                CWTX_Nprg[0] = 0; CWTX_Nprg[1] = 1; CWTX_Nprg[2] = 4; CWTX_Nprg[3] = 0; CWTX_Nprg[4] = 0;
                FMTX_Nprg[0] = 2; FMTX_Nprg[1] = 4; FMTX_Nprg[2] = 9; FMTX_Nprg[3] = 1; FMTX_Nprg[4] = 6;
                //reset PLL setting (because it gets lost during shutdown)
                FMTX(FMTX_Nref, FMTX_Nprg);
                CWTX(CWTX_Nref, CWTX_Nprg);
                FMRX(FMRX_Nref, FMRX_Nprg);
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