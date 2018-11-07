#include <xc.h>
#include "timer.h"
#include "UART.h"
#include "typeDefine.h"
#include "pinDefine.h"
#include "EPS.h"
#include "time.h"
#include "FMCW.h"
#include "ADC.h"
#include "I2C.h"
#include "MPU.h"
#include "EEPROM.h"
#include "okError.h"
#include "WDT.h"

void initTimer(void){
    INTCON = 0b11100000;    //GIE = 1, PEIE = 1, TMR0IE = 1
    OPTION_REG = 0b01000111;    //prescaler is assigned, TMR0 rate 1:256
    TMR0 = 0x00;    //Initializing Timer0 Module Register 
}

UBYTE EPS_reset_time = EPS_RSET_INTERVAL_SHORT;
UWORD time = 0;

static UINT timer_counter   = 0;
static UINT second_counter  = 0;
static UINT minute_counter  = 0;
static UINT hour_counter    = 0;
static UINT day_counter     = 0;
static UINT week_counter    = 0;

static UINT receive_command_counter_sec = 0;
static UINT receive_command_counter_min = 0;
static UINT bat_meas_counter_sec        = 0;
static UINT bat_meas_counter_min        = 0;
//static UINT eps_rest_counter_sec        = 0;
static UINT init_ope_counter_sec        = 0;
static UINT init_ope_counter_min        = 0;
UBYTE WDT_flag = 0x00;

//for debug
static UINT eps_reset_counter_sec = 0;
static UINT eps_reset_counter_min = 0;

//for debug function
void interrupt TimerCheck(void){
    if(INTCONbits.TMR0IF){
        INTCONbits.TMR0IF = 0;
        TMR0 = 0x00;
        timer_counter ++;
    }
           
    if(timer_counter >= one_second){
        timer_counter = 0;
        second_counter += 1;
        
        eps_reset_counter_sec ++; //for debug        
        init_ope_counter_sec ++;
        bat_meas_counter_sec ++;
        receive_command_counter_sec ++;
        LED_WHITE = 1 - LED_WHITE;  //for debug
        WDT_flag = 0x01;
    }
    if(second_counter >= one_minute){
        second_counter = 0;
        minute_counter ++;        
        putChar('G');
    }
    //for debug
    if(eps_reset_counter_sec >= one_minute){
        eps_reset_counter_sec = 0;
        eps_reset_counter_min ++;
    }
    if(receive_command_counter_sec >= one_minute){
        receive_command_counter_sec = 0;
        receive_command_counter_min ++;
    }
    if(init_ope_counter_sec >= one_minute){
        init_ope_counter_sec = 0;
        init_ope_counter_min ++;
    }
    if(bat_meas_counter_sec >= one_minute){
        bat_meas_counter_sec = 0;
        bat_meas_counter_min ++;
    }
    if(minute_counter >= one_hour){
        minute_counter = 0;
        hour_counter ++;
    }
    if(hour_counter >= one_day){
        hour_counter = 0;
        day_counter ++;
    }
    if(day_counter >= one_week){
        day_counter = 0;
        week_counter ++;
    }
    if(week_counter >= 2){
        week_counter = 0;
    }
    
    /*---WDT send pulse (4s)---*/
//    if((get_timer_counter('s') % WDT_INTERVAL) == 1 ){
//        if(WDT_flag == 0x01){
//        sendPulseWDT();
//            WDT_flag = 0x00;
//        }
//    }
}

//for debug
void set_eps_reset_counter(UINT time_sec, UINT time_min){
    eps_reset_counter_sec = time_sec;
    eps_reset_counter_min = time_min;
}

//FIXME:for debug
UINT get_eps_reset_counter_sec(void){
    return eps_reset_counter_sec;
}

//for debug
UINT get_eps_reset_counter_min(void){
    return eps_reset_counter_min;
}


void set_receive_command_counter(UINT time_sec, UINT time_min){
    receive_command_counter_sec = time_sec;
    receive_command_counter_min = time_min;
}

//FIXME:for debug
UINT get_receive_command_counter_sec(void){
    return receive_command_counter_sec;
}

UINT get_receive_command_counter_min(void){
    return receive_command_counter_min;
}

void set_init_ope_counter(UINT time_sec, UINT time_min){
    init_ope_counter_sec = time_sec;
    init_ope_counter_min = time_min;
}

//for debug
UINT get_init_ope_counter_sec(void){
    return init_ope_counter_sec;
}

UINT get_init_ope_counter_min(void){
    return init_ope_counter_min;
}

void set_bat_meas_counter(UINT time_sec, UINT time_min){
    bat_meas_counter_sec = time_sec;
    bat_meas_counter_min = time_min;
}

//for debug
UINT get_bat_meas_counter_sec(void){
    return bat_meas_counter_sec;
}

UINT get_bat_meas_counter_min(void){
    return bat_meas_counter_min;
}

UINT get_timer_counter(UBYTE unit){
    switch(unit){
        case 's':
            return second_counter;
        case 'm':
            return minute_counter;
        case 'h':
            return hour_counter;
        case 'd':
            return day_counter;
        case 'w':
            return week_counter;
        default:
            return 0;
    }
}

void reset_timer(void){
    timer_counter   = 0;
    second_counter  = 0;
    minute_counter  = 0;
    hour_counter    = 0;
    day_counter     = 0;
    week_counter    = 0;
}

        
// -----------------------------------------------------------------------------
        //XXX No.3 func 
        //battery voltage measure
        //treadhold is not determined
        //sampling rate is not determined
        //debug : 6v -> send 's' 7v -> send 'n'
//        if(bat_meas_counter >= 3){
//            bat_meas_counter = 0;
////            ReadBatVoltage();
////            if(adcL <= 0xD0) putChar('s');
//            UBYTE bat_voltage[2];
//            ReadBatVoltageWithPointer(bat_voltage);
//            if(bat_voltage[1] <= 0xD0) putChar(0xaa);
//            else putChar(0xbb);
//        }
// -----------------------------------------------------------------------------        
        
          
        
// -----------------------------------------------------------------------------        
        /*---EPS reset for debug (for debug 5/10s)---*/
//        time = second_counter % EPS_reset_time;
//        if(time == 0){
//            Reset_EPS();
//            
//            putChar(0xd1);
//            UBYTE array_2byte[2];
//            array_2byte[0] = checkMeltingStatus(MAIN_EEPROM_ADDRESS);
//            array_2byte[1] = checkMeltingStatus(SUB_EEPROM_ADDRESS);
////            putChar(array_2byte[0]);
////            putChar(array_2byte[1]);
////            array_2byte[0] = 2;
////            array_2byte[1] = 2;
//            
//            if((array_2byte[0] < MELTING_FINISH)&&(array_2byte[1] < MELTING_FINISH)){
//                putChar(0xd2);
//            } else {
//                EPS_reset_time = EPS_RSET_INTERVAL_LONG;
//                putChar('E');
//                putChar(0xd3);
//            }
//        }
// -----------------------------------------------------------------------------        
//        
//       if(second_counter >= one_minute){
//           second_counter = 0;
//           minute_counter += 1;
//           putChar(0xd4);
//           
//           //for debug
//           //EPS reset every 3 minitues
//           if(minute_counter >= 3){
//               putChar(0xd5);
//               Reset_EPS();
//               delay_ms(5000);
//               //resubstitution Nprg
////                FMTX_Nprg[0] = 8; FMTX_Nprg[1] = 7; FMTX_Nprg[2] = 5; FMTX_Nprg[3] = 0; FMTX_Nprg[4] = 1;
////                CWTX_Nprg[0] = 0; CWTX_Nprg[1] = 1; CWTX_Nprg[2] = 4; CWTX_Nprg[3] = 0; CWTX_Nprg[4] = 0;
////                FMTX_Nprg[0] = 2; FMTX_Nprg[1] = 4; FMTX_Nprg[2] = 9; FMTX_Nprg[3] = 1; FMTX_Nprg[4] = 6;
//               //reset PLL setting (because it gets lost during shutdown)
//               FMTX(FMTX_Nref, FMTX_Nprg);
//               CWTX(CWTX_Nref, CWTX_Nprg);
//               FMRX(FMRX_Nref, FMRX_Nprg);
//               __delay_ms(500);
//           }

//            if(minute_counter >= one_hour){
//                minute_counter = 0;
//                hour_counter += 1;
//
//                if(hour_counter >= one_day){
//                    hour_counter = 0;
//                    day_counter += 1;
//
//                    UBYTE array_2byte[2];
//                    array_2byte[0] = checkMeltingStatus(MAIN_EEPROM_ADDRESS);
//                    array_2byte[1] = checkMeltingStatus(SUB_EEPROM_ADDRESS);
//
//                    UBYTE EPS_reset_time;
//                    if((array_2byte[0] < MELTING_FINISH)&&(array_2byte[1] < MELTING_FINISH)){
//                        EPS_reset_time = EPS_RSET_INTERVAL_SHORT;
//                    } else {
//                        EPS_reset_time = EPS_RSET_INTERVAL_LONG;
//                    }
//
//                    if(second_counter >= EPS_reset_time){ 
////                        ResetEPSandSetPLL();
//                    }
//
//                }
//            }    
//       }

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

