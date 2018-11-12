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
UBYTE RXDATA[commandSize];
UBYTE ReceiveFlag = NOT_RECEIVE;


void interrupt InterReceiver(void){
//    putChar(0xee);
    
    if (RCIF == 1) {
        putChar('U');
        RXDATA[0] = 0x21;
        for(UINT i=0;i<commandSize;i++){
            RXDATA[i] = getChar();
        }
        for(UINT i=0;i<commandSize;i++){
            putChar(RXDATA[i]);
        }
//            /*for debug
//        putChar(0xcc);
//        putChar((UBYTE)(crc16(0,RXDATA,8) >> 8));
//        putChar((UBYTE)(crc16(0,RXDATA,8) & 0xff));
//             end*/
        ReceiveFlag = UNCORRECT_RECEIVE;
        if(crc16(0,RXDATA,8) == CRC_check(RXDATA, 8)){
            ReceiveFlag = CORRECT_RECEIVE;
        }
        if(RXDATA[0] == 't'){
            NOP();
        }else if(RXDATA[0] == 'g'){
            NOP();
             //Write to EEPROM for CW Downlink
//            UBYTE commandID = 0x00;
//            WriteOneByteToEEPROM(EEPROM_address, HighAddress_for_commandID, LowAddress_for_commandID, commandID);
            //Write to EEPROM for CW Downlink
//            WriteOneByteToEEPROM(EEPROM_address, crcResult_addressHigh, crcResult_addressLow, ReceiveFlag);
        }else{
            ReceiveFlag = UNCORRECT_RECEIVE;
        }
        RCIF = 0;
        putChar(0xff);
        put_lf();
    }
}

void main(void) {
    
    __delay_ms(1000);
    Init_MPU();
    InitI2CMaster(I2Cbps);
    Init_SERIAL();     
//    Init_WDT();    
//    sendPulseWDT();
    delay_s(TURN_ON_WAIT_TIME);   //wait for PLL satting by RXCOBC and start CW downlink
    putChar('S');


//    delay_s(TURN_ON_WAIT_TIME);   //wait for PLL satting by RXCOBC
//    delay_s(CW_START_WAIT_TIME);  //wait for 200sec --> start CW downlink
   
    while(1){
        
        putChar('m');
        delay_ms(3000);

        sendPulseWDT();
        __delay_ms(5000);
         
        measureDcDcTemperature();
        if(read5VBusAndSwitchNtrxPower() != 0){
            if(read5VBusAndSwitchNtrxPower() != 0){
                onOffNTRX(0x01,0,0);//subPower ON
            }
        }  
        //TODO debug send HK 
        HKDownlink();
       
        //======================================================================
        //UART receive process

        if(ReceiveFlag == CORRECT_RECEIVE){
            
            /*for debug
            putChar(0x33);
            put_lf();
            for(UINT i=0;i<commandSize;i++){
                putChar(RXDATA[i]);
            }
             end*/
            

            /*---read command ID---*/
            UBYTE commandID;
//            commandID = ReadEEPROM(EEPROM_address, HighAddress_for_commandID, LowAddress_for_commandID);
            

            /*---Define if command target is 't' or 'g' and read in task target ---*/
            /*------------------------------------------------------------------*/
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
                    break;
                case 0x66:  /*'f':FM Downlink*/
                    putChar(0xa6);
                    downlinkFMSignal(RXDATA[2],RXDATA[3],RXDATA[4],RXDATA[5],RXDATA[6]);
                    break;
                case 'p':/*'p':power*/
                    putChar(0xa7);
                    commandSwitchPowerSupply(RXDATA[2],RXDATA[3],RXDATA[4],RXDATA[5],RXDATA[6]);
                    break;
                case 0x68: /*'h':update HK data (DC-DC voltage) (HK = house keeping)*/
                    measureDcDcTemperature();
                    break;
                case 0x72: /*'r':send command to RXCOBC*/
                    sendCommand(RXDATA[2], RXDATA[3], RXDATA[4], RXDATA[5], RXDATA[6], RXDATA[7], 0x00, 0x00);
                    break;
                    
                    //for debug putChar only
                case 0x80:
                    putChar(0x80);
                    put_ok();
                    break;
                default:
//                    switchError(error_main_commandfromOBCorRXCOBC);
                    put_error();
                    break;
            }
            WriteLastCommandIdToEEPROM(commandID);
            ReceiveFlag = NOT_RECEIVE;
            putChar(0x39);
        }
        /*---write CRC result 6bit 1 ---*/
//        switchOk(error_main_crcCheck);
        
    //======================================================================
    }
}