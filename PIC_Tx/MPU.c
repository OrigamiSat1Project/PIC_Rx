#include <xc.h>
#include "MPU.h"
#include "Type_define.h"
#include "time.h"
//#include "decode_AX25.h"
#include "encode_AX25.h"
#include "EEPROM.h"
#include "I2C.h"
#include "pinDefine.h"
#include "OkError.h"

#define MELTING_TIME_MAX     4000  //0x0FA0 -> 4000[ms] TBD[ms]
#define MELTING_TIME_DEFAULT 2500 //[ms] 
#define MELTING_FINISH 0x08  //TBD

UINT invertState(UINT);
UINT invertStateWithTime(UINT,UBYTE,UBYTE);

void Init_MPU(void)
{
	//Initialize Port Status
	PORTA  = 0x00;
	PORTB  = 0x00;
	PORTC  = 0x00;
    //PORTD  = 0x00;
    PORTE  = 0x00;
	
	//AnalogorDigital Setting(All Digital)
	ANSEL  = 0x00;	//AD??¿½?¿½Ý’ï¿½
	ANSELH = 0x00;	//AD??¿½?¿½Ý’ï¿½
	
	//Port I/O Setting 
    //       0b76543210
	TRISA  = 0b10000000;
	TRISB  = 0b00010000;
    TRISC  = 0b01011100;
    //TRISD  = 0b00000000;
    TRISE  = 0b00000000;	

	//Port Initial Value Setting	
	PORTA  = 0x00;
	PORTB  = 0x00;
	PORTC  = 0x00;
    //PORTD  = 0x00;
    PORTE  = 0x00;
}

//void led_onoff(void){
//    ledy_on();
//    __delay_us(espan);
//    ledy_off();
//    __delay_us(espan);
//}
//void debug_ledy(void){
//    if(led_yellow == high){
//        ledy_off();
//    }else{
//        ledy_on();
//    }
//}

//Used to switch PIN to the opposite status(high/low)
//bit invertState(bit pinState){
UINT invertState(UINT pinState){
    if(pinState==high){
        return low;
    }else{
        return high;
    }
}

//Used to switch PIN to the opposite status(high/low)
UINT invertStateWithTIme(UINT pinState,UBYTE timeHigh, UBYTE timeLow){
    if (timeHigh != 0x00 && timeLow != 0x00){
        UWORD operationTime;
        operationTime = (UWORD)timeHigh * 0x100 + timeLow;
        delay_ms(operationTime);
        pinState = 1 - pinState;
    }else{
        return pinState;
    }
}

// void cutWire(UBYTE timeHigh, UBYTE timeLow){
//     UWORD cutTime;
//     cutTime = (UWORD)timeHigh * 0x100 + timeLow;
//     WIRE_CUTTER = 1;
//     delay_ms(cutTime);
//     WIRE_CUTTER = 0;
// }

/*heater*/
void onOffHEATER(UBYTE onOff, UBYTE timeHigh, UBYTE timeLow){ //high->on
    if ( onOff == 0x00 ){        
            HEATER = low;  
    } else {                     
            HEATER = high;
    }

    if(timeHigh == 0x00 && timeLow == 0x00){ 
    }else {        
        UWORD wait_time;
        wait_time = (timeHigh << 8 | timeLow);
        delay_ms(wait_time);
        HEATER =invertState(onOff);
    }
}

/*NTRX*/
void onOffNTRX(UBYTE onOff, UBYTE timeHigh, UBYTE timeLow){ //high->on
    if ( onOff == 0x00 ){        
            NTRX = low;  
    } else {                     
            NTRX = high;
    }

    if(timeHigh == 0x00 && timeLow == 0x00){ 
    }else {        
        UWORD wait_time;
        wait_time = (timeHigh << 8 | timeLow);
        delay_ms(wait_time);
        NTRX =invertState(onOff);
    }
}

/*5R8G 5V Sub Power*/
void onOff5R8GSubPower(UBYTE onOff, UBYTE timeHigh, UBYTE timeLow){ //high->on
    if ( onOff == 0x00 ){        
            SW_5R8G = low;  
    } else {                     
            SW_5R8G = high;
    }

    if(timeHigh == 0x00 && timeLow == 0x00){ 
    }else {        
        UWORD wait_time;
        wait_time = (timeHigh << 8 | timeLow);
        delay_ms(wait_time);
        SW_5R8G =invertState(onOff);
    }
}

/*antenna melting*/
void cutWire(UBYTE onOff, UBYTE timeHigh, UBYTE timeLow){ //high->on
    if ( onOff == 0x00 ){        
            WIRE_CUTTER = low;
            putChar(0xd1);
    } else {                     
            WIRE_CUTTER = high;
            putChar(0xd2);
    }

    if(timeHigh == 0x00 && timeLow == 0x00){
        putChar(0xd3);
    }else {        
        UWORD wait_time;
        wait_time = (timeHigh << 8 | timeLow);
        putChar(0xd4);

        if(wait_time>MELTING_TIME_MAX){
            wait_time = MELTING_TIME_DEFAULT;
            putChar(0xd5);
        } else {
            putChar(0xd6);
        }
        
        putChar(0xd7);
        delay_ms(wait_time);
        WIRE_CUTTER =invertState(onOff);
        putChar(0xd8);
        //TODO:wait time ga over -> error
    }
}

/*antenna melting with meliing times*/
void cutWireWithMeltingtimes(UBYTE onOff, UBYTE timeHigh, UBYTE timeLow, UBYTE meltingTimes){
    UBYTE main_melting_status;
    UBYTE sub_melting_status;
    main_melting_status = ReadEEPROM(EEPROM_address,MeltingStatus_addressHigh, MeltingStatus_addressLow);
    sub_melting_status = ReadEEPROM(EEPROM_subaddress,MeltingStatus_addressHigh, MeltingStatus_addressLow);

//    //FIXME:
//    main_melting_status = 0b01000000;
//    sub_melting_status = 0b01000000;
    
    //bit operation
    //ex: 0b01101011 -> 0+1+1+0+1+0+1+1=5
    UBYTE main_melting_status_cal_result;
    UBYTE sub_melting_status_cal_result;
    main_melting_status_cal_result = bitCalResult(main_melting_status);
    sub_melting_status_cal_result = bitCalResult(sub_melting_status);
  
    //cal_result>TBD: melting already finish   / cal_result=<TBD: not yet
    if((main_melting_status_cal_result < MELTING_FINISH)&&(sub_melting_status_cal_result < MELTING_FINISH)){ 
        putChar(0xb1);
        for(UBYTE i=0; i<meltingTimes; i++){    
            putChar(0xb2);
            cutWire(onOff, timeHigh, timeLow);
            delay_s(WIRE_CUT_INTERVAL);
        }
    } else {
        putChar(0xb3);
        //already melting finishs
    }
}

/*WDT*/
void onOffTXWDT(UBYTE onOff, UBYTE timeHigh, UBYTE timeLow){ //high->on
    if ( onOff == 0x00 ){        
            WDT_POWER = low;  
    } else {                     
            WDT_POWER = high;
    }

    if(timeHigh == 0x00 && timeLow == 0x00){ 
    }else {        
        UWORD wait_time;
        wait_time = (timeHigh << 8 | timeLow);
        delay_ms(wait_time);
        WDT_POWER =invertState(onOff);
    }
}

/*FMPTT*/
void onOffFMPTT(UBYTE onOff, UBYTE timeHigh, UBYTE timeLow){ //high->on
    if ( onOff == 0x00 ){        
            FMPTT = low;  
    } else {                     
            FMPTT = high;
    }

    if(timeHigh == 0x00 && timeLow == 0x00){ 
    }else {        
        UWORD wait_time;
        wait_time = (timeHigh << 8 | timeLow);
        delay_ms(wait_time);
        FMPTT =invertState(onOff);
    }
}

/*CWKEY*/
void onOffCWKEY(UBYTE onOff, UBYTE timeHigh, UBYTE timeLow){ //high->on
    if ( onOff == 0x00 ){        
            CWKEY = low;  
    } else {                     
            CWKEY = high;
    }

    if(timeHigh == 0x00 && timeLow == 0x00){ 
    }else {        
        UWORD wait_time;
        wait_time = (timeHigh << 8 | timeLow);
        delay_ms(wait_time);
        CWKEY =invertState(onOff);
    }
}


//process command data if the command type is 'power supply'
void commandSwitchPowerSupply(UBYTE command, UBYTE onOff, UBYTE timeHigh, UBYTE timeLow, UBYTE melting_times){ //times are given in ms
    switch(command){
        case 'h':   //Heater
            onOffHEATER(onOff, timeHigh, timeLow);
            break;
        case 'n':   //NTRX
            onOffNTRX(onOff, timeHigh, timeLow);
            break;
        case '5': //5R8G 5V Sub Power
            onOff5R8GSubPower(onOff, timeHigh, timeLow);
            break;
        case 'a': //WIRE_CUTTER
            putChar(0xb1);
            cutWire(onOff, timeHigh, timeLow);
            break;
        case 't': //WIRE_CUTTER with times
            putChar(0xb2);
            cutWireWithMeltingtimes(onOff, timeHigh, timeLow, melting_times);
            break;
        case 'w': //WDT
            onOffTXWDT(onOff, timeHigh, timeLow);
            break;
        case 'p': //FMPTT
            onOffFMPTT(onOff, timeHigh, timeLow);
            break;
        case 'k': //CWKEY
            onOffCWKEY(onOff, timeHigh, timeLow);
            break;
        default:
            switchError(error_MPU_commandSwitchPowerSupply);
            break;
    }
}

