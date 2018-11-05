#include <xc.h>
#include "UART.h"
#include "typeDefine.h"
#include "pinDefine.h"
#include "time.h"
//#include "FMCW.h"
#include "ADC.h"
#include "I2C.h"
#include "MPU.h"
#include "EEPROM.h"
#include "initial_operation.h"

/*---Initial Operation---*/
#define MELTING_FINISH 0x06  //TBD
#define WAIT_TIME_FOR_SETTING 2  //[s] //TBD  200->2
//#define BAT_LIMIT_FOR_MELTING 0x01B3 //[V] //TBD 6.0V
#define BAT_LIMIT_FOR_MELTING 0x0077 //[V] //TBD 6.0V
//#define MELTING_COUNTER_LIMIT 72  //for debug 72->10 
#define MELTING_COUNTER_LIMIT 13  //for debug 72->10 
#define SAT_MODE_NORMAL 0x01 //FIX ME: need change

void InitialOperation(void){
    /*---start checking whether antenna are developed or not---*/
    /*---[antenna are not developed]+[OBC does not work]->[RXCOBC develops antenna]---*/
    /*--------------------------------------------------------------------------------*/
    UBYTE temp = 0;
    UBYTE array_2byte[2] = {0};
//    UWORD bat_voltage_2byte = 0;

    /*---check OBC status---*/
    switch(OBC_STATUS){
        case OBC_ALIVE:
            putChar(0xa1);
//                switchOk(ok_main_forOBCstatus_ALIVE);
            break;
        case OBC_DIED:
            
            putChar(0xa2);
            
            /*---read melting status & bit cal---*/
            array_2byte[0] = checkMeltingStatus(MAIN_EEPROM_ADDRESS);
            array_2byte[1] = checkMeltingStatus(SUB_EEPROM_ADDRESS);

            //cal_result>TBD: melting already finish   / cal_result=<TBD: not yet
            if((array_2byte[0] < MELTING_FINISH)&&(array_2byte[1] < MELTING_FINISH)){

//                putChar(0xa3);
//                putChar(array_2byte[0]);
//                putChar(array_2byte[1]);
                
                /*---check satellite mode---*/
                temp = ReadEEPROM(MAIN_EEPROM_ADDRESS , SatelliteMode_addressHigh, SatelliteMode_addressLow);

                //sat mode: NORMAL->melting / SAVING or SURVIVAL ->break
//                if(bat_voltage_2byte<BAT_LIMIT_FOR_MELTING){
                if(temp!=SAT_MODE_NORMAL){
                    putChar(0xa4);
                } else {
//                    putChar(0xa5);
                    /*---check melting counter---*/
//                    temp = ReadEEPROM(MAIN_EEPROM_ADDRESS, MeltingCounter_addressHigh, MeltingCounter_addressLow);
                    temp = ReadEEPROMmainAndSub(MeltingCounter_addressHigh, MeltingCounter_addressLow);
//                    putChar(temp);
                    
                    
                    // melting counter
                    //1.temp>=MELTING_COUNTER_LIMIT         -> riset counter
                    //2.7 < temp <MELTING_COUNTER_LIMIT  -> ciunter++
                    //3.0 <= temp <=7                    -> melting + counter++
                    if(temp>=MELTING_COUNTER_LIMIT){
                        putChar(0xa6);
                        temp = 0;
                    } else if ((7 < temp)&&(temp<MELTING_COUNTER_LIMIT)){
                        putChar(0xa7);
                        temp++;
                    } else {
//                        putChar(0xa8);
                        delay_s (WAIT_TIME_FOR_SETTING); //TBD[s] for debug 200s->2s

                        if(temp<4){
                            putChar(0xa9);
//                            //***FIXME*** wire melting!! be careful!!
//                            //sendCommand('t','p','t', OnOff_forCutWIRE, CutWIRE_SHORT_highTime, CutWIRE_SHORT_lowTime, 0x03, 0x00);
                        } else {
                            putChar(0xa0);
//                            //***FIXME***  wire melting!! be careful!!
//                            //sendCommand('t','p','t', OnOff_forCutWIRE, CutWIRE_LONG_highTime, CutWIRE_LONG_lowTime, 0x03, 0x00);
                        }
                        temp++;
//                        putChar(0xb1);
//                        putChar(temp);
//                            switchOk(ok_main_forOBCstatus_DIED);
                    }
                    
//                    putChar(0xaa);
                    
                    /*---write melting counter to main and sub EEPROM---*/
                    WriteOneByteToMainAndSubB0EEPROM(MeltingCounter_addressHigh, MeltingCounter_addressLow, temp);

//                    temp = ReadEEPROM(MAIN_EEPROM_ADDRESS, MeltingCounter_addressHigh, MeltingCounter_addressHigh);
//                    putChar(temp);
                }
            } else {
                putChar(0xab);
            }
            break;
        default:
//            putChar(0xac);
//            switchError(error_main_forOBCstatus);
            break;    
    }
}

//TODO:need debug
//ex: 0b01101011 -> 0+1+1+0+1+0+1+1=5
UBYTE bitCalResult(UBYTE bit_input){
    UBYTE bit_cal_result = 0; 
    for(UBYTE cal_counter=0; cal_counter<8; cal_counter++){
        if((bit_input & 1)==1){
            bit_cal_result = bit_cal_result + 1;
        } else {
            bit_cal_result = bit_cal_result + 0;
        }
        bit_input = bit_input >>1;
    }
    return bit_cal_result;
}


UBYTE checkMeltingStatus(UBYTE e_address){
    /*---read melting status---*/
    UBYTE temp;
    temp = ReadEEPROM(e_address, MeltingStatus_addressHigh, MeltingStatus_addressLow);
      
    /*---bit operation---*/
    //ex: 0b01101011 -> 0+1+1+0+1+0+1+1=5
    temp = bitCalResult(temp);
    return temp;
}