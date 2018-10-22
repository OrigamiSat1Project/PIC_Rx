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

/*---Initial Operation---*/
#define MELTING_FINISH 0x06  //TBD
#define WAIT_TIME_FOR_SETTING 2  //[s] //TBD  200->2
//#define BAT_LIMIT_FOR_MELTING 0x01B3 //[V] //TBD 6.0V
#define BAT_LIMIT_FOR_MELTING 0x0077 //[V] //TBD 6.0V
//#define MELTING_COUNTER_LIMIT 72  //for debug 72->10 
#define MELTING_COUNTER_LIMIT 13  //for debug 72->10 

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
        initial_ope_counter +=1;
        second_counter += 1;
        LED_WHITE = 1 - LED_WHITE;  //for debug
        
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
        
        if(second_counter >= 5){
            initial_ope_counter = 0;
            putChar(0xcc);
            putChar(0xcc);
            putChar(0xcc);
            InitialOperation();
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
//                FMTX_Nprg[0] = 8; FMTX_Nprg[1] = 7; FMTX_Nprg[2] = 5; FMTX_Nprg[3] = 0; FMTX_Nprg[4] = 1;
//                CWTX_Nprg[0] = 0; CWTX_Nprg[1] = 1; CWTX_Nprg[2] = 4; CWTX_Nprg[3] = 0; CWTX_Nprg[4] = 0;
//                FMTX_Nprg[0] = 2; FMTX_Nprg[1] = 4; FMTX_Nprg[2] = 9; FMTX_Nprg[3] = 1; FMTX_Nprg[4] = 6;
                //reset PLL setting (because it gets lost during shutdown)
//                FMTX(FMTX_Nref, FMTX_Nprg);
//                CWTX(CWTX_Nref, CWTX_Nprg);
//                FMRX(FMRX_Nref, FMRX_Nprg);
//                setPLL();
                __delay_ms(500);
            }
        }
    }
}

void InitialOperation(void){
    /*---start checking whether antenna are developed or not---*/
    /*---[antenna are not developed]+[OBC does not work]->[RXCOBC develops antenna]---*/
    /*--------------------------------------------------------------------------------*/
    UBYTE temp;
    UBYTE array_2byte[2];
    UWORD bat_voltage_2byte;

    switch(OBC_STATUS){
        case OBC_ALIVE:
            putChar(0xa1);
//                switchOk(ok_main_forOBCstatus_ALIVE);
            break;
        case OBC_DIED:{
            
            putChar(0xa2);
            //FIXME:write datas to EEPROM for debug
            
            temp = 0b00000111;
            WriteOneByteToMainAnadSubB0EEPROM(MeltingStatus_addressHigh, MeltingStatus_addressLow, temp);
            putChar(0xb1);

//            //check melting status
//            array_2byte[0] = ReadEEPROM(MAIN_EEPROM_ADDRESS, MeltingStatus_addressHigh, MeltingStatus_addressLow);
//            array_2byte[1] = ReadEEPROM(SUB_EEPROM_ADDRESS, MeltingStatus_addressHigh, MeltingStatus_addressLow);
//
//            //bit operation
//            //ex: 0b01101011 -> 0+1+1+0+1+0+1+1=5
//            array_2byte[0] = bitCalResult(array_2byte[0]);
//            array_2byte[1] = bitCalResult(array_2byte[1]);
            
            array_2byte[0] = checkMeltingStatus(MAIN_EEPROM_ADDRESS);
            array_2byte[1] = checkMeltingStatus(SUB_EEPROM_ADDRESS);

            //cal_result>TBD: melting already finish   / cal_result=<TBD: not yet
            if((array_2byte[0] < MELTING_FINISH)&&(array_2byte[1] < MELTING_FINISH)){

                putChar(0xa3);
                //check the battery voltage
                ReadBatVoltageWithPointer(array_2byte);
                WriteToMainAndSubB0EEPROM(BatteryVoltage_addressHigh,BatteryVoltage_addressHigh,array_2byte);

//                    putChar(0xb1);
//                    putChar(array_2byte[0]);
//                    putChar(array_2byte[1]);


                bat_voltage_2byte = (array_2byte[0]<<8)| array_2byte[1];

                if(bat_voltage_2byte<BAT_LIMIT_FOR_MELTING){
                    putChar(0xa4);
                } else {
                    putChar(0xa5);
                    //check melting counter
//                    UWORD melting_counter;
//                    //FIXME:for debug
//                    melting_counter = 0;
//                    WriteOneByteToMainAnadSubB0EEPROM(MeltingCounter_addressHigh, MeltingCounter_addressHigh, melting_counter);      

                    UBYTE temp;
                    temp = ReadEEPROM(MAIN_EEPROM_ADDRESS, MeltingCounter_addressHigh, MeltingCounter_addressHigh);
                    putChar(temp);
                    putChar(temp);
                    putChar(temp);
                    //TODO:read data from sub EEPROM (main EEPROM error)
                    
//                    UWORD melting_counter_amari;
//                    melting_counter_amari = melting_counter % MELTING_COUNTER_LIMIT;
//                    
//                    if((7 < melting_counter_amari)&&(melting_counter_amari<MELTING_COUNTER_LIMIT)){
//                        melting_counter++;
//                    } else {
//                        putChar(0xa8);
//                        delay_s (WAIT_TIME_FOR_SETTING); //TBD[s] for debug 200s->2s
//
//                        if(melting_counter<4){
//                            putChar(0xa9);
//                            sendCommand('t','p','t', OnOff_forCutWIRE, CutWIRE_SHORT_highTime, CutWIRE_SHORT_lowTime, 0x03, 0x00);
//                        } else {
//                            putChar(0xa0);
//                            sendCommand('t','p','t', OnOff_forCutWIRE, CutWIRE_LONG_highTime, CutWIRE_LONG_lowTime, 0x03, 0x00);
//                        }
//                        melting_counter++;
////                            switchOk(ok_main_forOBCstatus_DIED);
//                    }                    

                    if(temp==MELTING_COUNTER_LIMIT){
                        putChar(0xa6);
                        temp = 0;
                    } else if ((7 < temp)&&(temp<MELTING_COUNTER_LIMIT)){
                        putChar(0xa7);
                        temp++;
                    } else {
                        putChar(0xa8);
                        delay_s (WAIT_TIME_FOR_SETTING); //TBD[s] for debug 200s->2s

                        if(temp<4){
                            putChar(0xa9);
                            sendCommand('t','p','t', OnOff_forCutWIRE, CutWIRE_SHORT_highTime, CutWIRE_SHORT_lowTime, 0x03, 0x00);
                        } else {
                            putChar(0xa0);
                            sendCommand('t','p','t', OnOff_forCutWIRE, CutWIRE_LONG_highTime, CutWIRE_LONG_lowTime, 0x03, 0x00);
                        }
                        temp++;
                        putChar(0xb1);
                        putChar(temp);
//                            switchOk(ok_main_forOBCstatus_DIED);
                    }
                    putChar(0xaa);
                    WriteOneByteToMainAnadSubB0EEPROM(MeltingCounter_addressHigh, MeltingCounter_addressHigh, temp);

                    temp = ReadEEPROM(MAIN_EEPROM_ADDRESS, MeltingCounter_addressHigh, MeltingCounter_addressHigh);
                    putChar(temp);
                }
            }
            putChar(0xab);
            break;}
        default:
            putChar(0xac);
//            switchError(error_main_forOBCstatus);
            break;    
    }
}

UBYTE checkMeltingStatus(UBYTE e_address){
    UBYTE temp;
    temp = ReadEEPROM(e_address, MeltingStatus_addressHigh, MeltingStatus_addressLow);
    temp = bitCalResult(temp);
    return temp;
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