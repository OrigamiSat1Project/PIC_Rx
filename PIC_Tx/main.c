//#include <stdio.h>
//#include <stdlib.h>
#include <xc.h>
#include <PIC16F886.h>
#include "UART.h"
#include "MPU.h"
#include "Type_define.h"
#include "time.h"
#include "encode_AX25.h"
#include "I2C.h"
#include "EEPROM.h"
#include "FMCW.h"
#include "WDT.h"
#include "CW.h"
#include "pinDefine.h"
#include "CRC16.h"
#include "ADC.h"
#include "OkError.h"

void interrupt InterReceiver(void);

#define COMMAND_SIZE 10;


// PIC16F887 Configuration Bit Settings

// 'C' source line config statements

/* PIC16F887 Configuration Bit Settings */
#pragma config FOSC     = HS            // Oscillator Selection bits (HS oscillator: High-speed crystal/resonator on RA6/OSC2/CLKOUT and RA7/OSC1/CLKIN)
#pragma config WDTE     = OFF           // Watchdog Timer Enable bit (WDT disabled and can be enabled by SWDTEN bit of the WDTCON register)
#pragma config PWRTE    = ON            // Power-up Timer Enable bit (PWRT disabled)
#pragma config MCLRE    = ON            // RE3/MCLR pin function select bit (RE3/MCLR pin function is MCLR)
#pragma config CP       = OFF           // Code Protection bit (Program memory code protection is disabled)
#pragma config CPD      = OFF           // Data Code Protection bit (Data memory code protection is disabled)
#pragma config BOREN    = OFF            // Brown Out Reset Selection bits (BOR enabled)
#pragma config IESO     = OFF            // Internal External Switchover bit (Internal/External Switchover mode is enabled)
#pragma config FCMEN    = OFF            // Fail-Safe Clock Monitor Enabled bit (Fail-Safe Clock Monitor is enabled)
#pragma config LVP      = OFF           // Low Voltage Programming Enable bit (RB3 pin has digital I/O, HV on MCLR must be used for programming)
#pragma config BOR4V    = BOR40V        // Brown-out Reset Selection bit (Brown-out Reset set to 4.0V)
#pragma config WRT      = OFF           // Flash Program Memory Self Write Enable bits (Write protection off)

#define commandSize 10

//test_interrupt
//pc-->pic-->pc 
//void interrupt interReceiverTest( void ){
//    UBYTE RXDATA;
//    if (RCIF == 1) {
//        RXDATA = getChar();
//        RXDATA++;
//        putChar('S');
//        RCIF = 0;
//    }
//}

/**/
//test_get EEPROM address
//pc-->pic-->pc 
//void interrupt InterReceiver( void ){
//    UBYTE RXDATA[3];
//    UINT COMMAND_SIZE;
//    COMMAND_SIZE =3;
//    if (RCIF == 1) {
//        for(UINT i=0; i<COMMAND_SIZE; i++){
//            RXDATA[i] =getChar();
//            putChar(RXDATA[i]);
//            NOP();
//        }
//        putChar('s');
//        RCIF = 0;
//    }
//}


void interrupt InterReceiver(void){
    putChar('I');
    putChar('I');
    
//    UBYTE commandSize;
//    commandSize = 10;

//    UBYTE RXDATA[10];//array size = commandSize
    UBYTE RXDATA[commandSize];
//    volatile static int intr_counter;

    if (RCIF == 1) {
        /*---for debug---*/
//         for (UBYTE i = 0; i < commandSize; i++){
//             RXDATA[i] = getChar();
//         }
        UBYTE get_char_state = 0;
        while(get_char_state < commandSize){
            RXDATA[0] = getChar();
            putChar('X');
//            putChar('e');
//            putChar('t');
//            putChar(RXDATA[0]);
            get_char_state++;
            if(RXDATA[0] != 't' && RXDATA[0] != 'g'){
                get_char_state = 0;
            } else {    
                for (UBYTE i = 1; i < commandSize; i++){
                RXDATA[i] = getChar();
                get_char_state++;
                }
            }
        }
        
        //TODO:need?
        /*---Send command using UARTto RXCOBC---*/
        for (UBYTE i = 0; i < commandSize; i++){
            putChar(RXDATA[i]);
            NOP();
        }
       
        /*---CRC check for command from RXCOBC or OBC---*/ 
       //TODO add case RXDATA[0]!=t or g
        UWORD crcResult, crcValue;
        UBYTE crcResultHigh,crcResultLow,crcValueHigh,crcValueLow;
        crcResult = crc16(0,RXDATA,8);
        crcValue =  CRC_check(RXDATA,8);
        crcResultHigh = crcResult>>8;
        crcResultLow = crcResult & 0x00FF;
        crcValueHigh = crcValue>>8;
        crcValueLow = crcValue & 0x00FF;
        
        /*----------------------------------------------*/
        //FIXME:[start]debug for test to CRCcheck
//        putChar(0xcc);
//        putChar(0xcc);
//        putChar(crcResultHigh);
//        putChar(crcResultLow);
//        putChar(crcValueHigh);
//        putChar(crcValueLow);
        //FIXME:[finish]debug for test to CRCcheck
        /*----------------------------------------------*/
        
        /*---read command ID---*/
        UBYTE commandID;
        commandID = ReadEEPROM(EEPROM_address, HighAddress_for_commandID, LowAddress_for_commandID);
        //TODO:read datas from sub EEPROM

        /*---read CRC check from EEPROM---*/
        UBYTE CRC_check_result;
        //FIXME:for debug
        CRC_check_result = 0b1000000;
        WriteOneByteToMainAndSubB0EEPROM(crcResult_addressHigh, crcResult_addressLow,CRC_check_result);
        CRC_check_result = ReadEEPROM(EEPROM_address, crcResult_addressHigh, crcResult_addressLow);
        
        if(crcResult != crcValue){  //crc error
            
            /*---write CRC error result (6bit 0) ---*/
            CRC_check_result = CRC_check_result & 0b1011111;
            WriteOneByteToMainAndSubB0EEPROM(crcResult_addressHigh, crcResult_addressLow,CRC_check_result);
            
            putChar(0xa1);
            putChar(CRC_check_result);
            putErrorNoDownlink(error_main_crcCheck);    
            
        } else {  //crc  OK
            
            /*---write CRC ok result (6bit 1) ---*/
            CRC_check_result = CRC_check_result | 0b01000000;
            WriteOneByteToMainAndSubB0EEPROM(crcResult_addressHigh, crcResult_addressLow,CRC_check_result);
            
            putChar(0xa2);
            putChar(CRC_check_result);
            
            /*---Define if command target is 't' or 'g' and read in task target ---*/
            /*------------------------------------------------------------------*/
            if (RXDATA[0]!='t' && RXDATA[0]!='g' ){
                //TODO:add error messege
                putChar(0xa3);
            } else {
                switch(RXDATA[1]){
                    /*---Command from RXCOBC---*/
                    /*------------------------------------------------------------------*/
                    case 0x75:  //'u'
                        putChar('R'); 
                        putChar(0xa4);
                        downlinkReceivedCommand(RXDATA[2],RXDATA[3],RXDATA[4],RXDATA[5]);
                        break;

                    /*---Command from OBC---*/
                    /*------------------------------------------------------------------*/
                    case 0x63: /*'c':CW Downlink*/
                        putChar(0xa5);
                        commandSwitchCWDownlink(RXDATA[2], RXDATA[3], RXDATA[4], RXDATA[5], RXDATA[6], RXDATA[7], RXDATA[8]);
                        WriteLastCommandIdToEEPROM(commandID);
                        break;
                    case 0x66:  /*'f':FM Downlink*/
                        putChar(0xa6);
                        downlinkFMSignal(RXDATA[2],RXDATA[3],RXDATA[4],RXDATA[5],RXDATA[6]);
                        WriteLastCommandIdToEEPROM(commandID);
                        break;
                    case 'p':/*'p':power*/
                        putChar(0xa7); 
                        commandSwitchPowerSupply(RXDATA[2],RXDATA[3],RXDATA[4],RXDATA[5],RXDATA[6]);
                        WriteLastCommandIdToEEPROM(commandID);
                        break;
                    case 0x68: /*'h':update HK data (DC-DC voltage) (HK = house keeping)*/
                        measureDcDcTemperature();
                        WriteLastCommandIdToEEPROM(commandID);
                        break;
                    case 0x72: /*'r':send command to RXCOBC*/
                        sendCommand(RXDATA[2], RXDATA[3], RXDATA[4], RXDATA[5], RXDATA[6], RXDATA[7], 0x00, 0x00);
                        break;
                    default:
                        switchError(error_main_commandfromOBCorRXCOBC);   
                        break;                             
                }
            }
        /*---write CRC result 6bit 1 ---*/
        CRC_check_result = CRC_check_result | 0b0100000;
        WriteOneByteToMainAndSubB0EEPROM(crcResult_addressHigh, crcResult_addressLow,CRC_check_result);
        switchOk(error_main_crcCheck);   
        }
        RCIF = 0;
    }
//    if ( PIR1bits.TMR1IF == 1 ) {
//        TMR1 = TIMER_INTERVAL;  // ?????????
// 
//        intr_counter++;
//        if ( intr_counter >= 100 ) {
//            intr_counter = 0;
//        }
//         //0.5sec???RB0???????    
//        if ( intr_counter <= 50 || intr_counter > 51) {
////            PORTAbits.RA0 = 1;
//        } else {
////            PORTAbits.RA0 = 0;
//        }
// 
//        PIR1bits.TMR1IF = 0;    // ???????????
//    }   
// 
//    return;
}

/*******************************************************************************
*test for interrupt
******************************************************************************/
//FIXME:[start]test for interrupt
// void interrupt interReceiverTest( void ){
//    UBYTE RXDATA;
//    if (RCIF == 1) {
//        RXDATA = getChar();
//        //RXDATA++;
//        //putChar('G');
//        putChar(RXDATA);
       
//        switch (RXDATA){
//             case 'h':
//                 putChar('H');
//                 HEATER = 1;
//                 for(int i=0;i<300;i++) delay_ms(1000);
//                 HEATER = 0;
//                 break;
//            case 'i':
//                HEATER = 1;
//                for(int i=0;i<60;i++) delay_ms(1000);
//                HEATER = 0;
//                break;
//             case 'c':
//                 putChar('C');
//                 putChar('W');
                
//                 for(int i=0;i<120;i++){
//                 CWKEY = 1;
//                 __delay_ms(50);
//                 CWKEY = 0;
//                 __delay_ms(50);

//                 CWKEY = 1;
//                 __delay_ms(50);
//                 CWKEY = 0;
//                 __delay_ms(50);

//                 CWKEY = 1;
//                 __delay_ms(50);
//                 CWKEY = 0;
//                 __delay_ms(50);

//                 CWKEY = 1;
//                 __delay_ms(150);
//                 CWKEY = 0;
//                 __delay_ms(50);
//                 }

//                 putChar('C');
//                 putChar('W');
//                 putChar('2');
//                 break;
//             case 'f':
//                 putChar('F');
//                 putChar('m');
//                 CWKEY = 0;
//         //            __delay_ms(2000);
//                 __delay_ms(300);
//                 UBYTE EEPROMTestData[36];
//                 EEPROMTestData[0] = 'o';
//                 EEPROMTestData[1] = 'r';
//                 EEPROMTestData[2] = 'i';
//                 EEPROMTestData[3] = '1';
//                 for(int i=4;i<36;i++) EEPROMTestData[i] = 'A';
//                 __delay_ms(300);
//                 FMPTT = 1;
//                 SendPacket(EEPROMTestData);
//                 FMPTT = 0;
//                 __delay_ms(300);
//                 break;
                

//        }
//        RCIF = 0;
//    }
// }

/*******************************************************************************
*test for timer
******************************************************************************/  
//void interrupt timer(void){
    
//    if(INTCONbits.TMR0IF){
//        INTCONbits.TMR0IF = 0;
//        TMR0L = 0x00;
//        timer_counter++;
//        constant_timer_counter++;
//    }
//    if(timer_counter >= 62){
//        //  past 1 second
//        increment_globalClock();
//        timer_counter = 0;
//        //sendCanData(&globalClock);
//    }
//    interruptI2C();
//}

//void interrupt intr(void){
//    volatile static int intr_counter;
//    if ( PIR1bits.TMR1IF == 1 ) {
//        TMR1 = TIMER_INTERVAL;  // ?????????
// 
//        intr_counter++;
//        if ( intr_counter >= 100 ) {
//            intr_counter = 0;
//        }
// 
//        // 0.5sec???RB0???????    
//        if ( intr_counter <= 50 || intr_counter > 51) {
//            PORTAbits.RA0 = 1;
//        } else {
//            PORTAbits.RA0 = 0;
//        }
// 
//        PIR1bits.TMR1IF = 0;    // ???????????
//    }   
// 
//    return;
//}


void main(void) {
    
    __delay_ms(1000);
    Init_SERIAL();
    Init_MPU();
    InitI2CMaster(I2Cbps);
    Init_WDT();
//    delay_s(TURN_ON_WAIT_TIME);   //wait for PLL satting by RXCOBC
//    delay_s(CW_START_WAIT_TIME);  //wait for 200sec --> start CW downlink

    putChar('S');
    putChar('S');
    putChar('S');
    putChar('y');
    
    //FIXME:write melting status for debug
//    UBYTE main_test_melting_status = 0b00000011;
//    UBYTE sub_test_melting_status = 0b01111111;
//    WriteOneByteToEEPROM(EEPROM_address,MeltingStatus_addressHigh, MeltingStatus_addressLow, main_test_melting_status);
//    WriteOneByteToEEPROM(EEPROM_subaddress,MeltingStatus_addressHigh, MeltingStatus_addressLow, sub_test_melting_status);
//    putChar(0xa1);
////    
//    UBYTE main_melting_status;
//    UBYTE sub_melting_status;
//    main_melting_status = ReadEEPROM(EEPROM_address, MeltingStatus_addressHigh, MeltingStatus_addressLow);
//    sub_melting_status = ReadEEPROM(EEPROM_subaddress, MeltingStatus_addressHigh, MeltingStatus_addressLow);
//    putChar(0xa2);
//    putChar(main_melting_status);
//    putChar(sub_melting_status);
    
    while(1){
        
        //FIXME
        for(UBYTE i=0; i<30; i++){
        led_yellow = high;
        __delay_ms(1000);
        led_yellow = low;
        __delay_ms(1000);
        }
        
        putChar('m');
        putChar('X');
        __delay_ms(1000);

        //TODO send pulse to WDT
        
        //switchSatMode
        UBYTE SatMode = ReadEEPROM( EEPROM_address,satelliteMode_addressHigh,satelliteMode_addressLow) & 0xF0;
        //TODO check AD value
        switch(SatMode){
            case 0x50:/*------------Nominal Mode------------*/
                break;
            case 0x60:/*------------Saving Mode------------*/
                //Todo write HK Data
                break;
            case 0xA0:/*------------Survival Mode------------*/
                break;
            default:
                break;           
    }
        //ADC();
             
        //TODO send HK 
        HKDownlink(SatMode);
        
        
        /*---------------------------------------------------------------*/
        //FIXME:[start]debug for downlink FM signal        
        // FMPTT = 1;
        // UBYTE eDataField[36];
        // for(UBYTE i=0; i<36; i++){
        //     eDataField[i] = i;
        // }
        // SendPacket(eDataField);
        // __delay_ms(2000);
        //  FMPTT = 0;
        //FIXME:[finish]debug for downlink CW signal
        /*---------------------------------------------------------------*/

        /*---------------------------------------------------------------*/
        //FIXME:[start]check for CW signal       
//       __delay_ms(1000);
//        CWKEY = 1;
//        __delay_ms(2000);
//        CWKEY =0;
        //FIXME:[finish]check for  CW signal
        /*---------------------------------------------------------------*/
        
        /*---------------------------------------------------------------*/
        //FIXME:[start]debug for downlink CW signal
        // testForCwFunctions();
        //FIXME:[finish]debug for downlink CW signal
        /*---------------------------------------------------------------*/
        
       /*----------------------------------------------------------*/
       //FIXME : method for test to measure ADC start
//       putChar(0xaa);
//       putChar(0xaa);
//       putChar(0xaa);
//       measureAllChanelADC();
//       putChar(0xbb);
//       putChar(0xbb);
//       putChar(0xbb);
       //method for test to measure ADC finish
       /*----------------------------------------------------------*/
        
    }
    //return;
}
