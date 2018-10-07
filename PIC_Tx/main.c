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


//void interrupt InterReceiver(void){
//    putChar('I');
//    
//    int commandSize;
//    commandSize = 10;
//    //commandSize = 1;
//    
//    UBYTE RXDATA[10];//array size = commandSize
//    //UBYTE RXDATA[1];//array size = commandSize
//
//    //    UBYTE RXDATA[COMMAND_SIZE];
////    volatile static int intr_counter;
//
//    if (RCIF == 1) {
//        for (int i = 0; i < 1; i++){
//            RXDATA[i] = getChar();
////            putChar(RXDATA[i]);
//        }
//        for (int i = 0; i < 1; i++){
//            putChar(RXDATA[i]);
//            NOP();
//        }
//       //TODO add case RXDATA[0]!=t or g
//        UWORD crcResult, crcValue;
//        UBYTE crcResultHigh,crcResultLow,crcValueHigh,crcValueLow;
//        crcResult = crc16(0,RXDATA,8);
//        crcValue =  CRC_check(RXDATA,8);
//        crcResultHigh = crcResult>>8;
//        crcResultLow = crcResult & 0x00FF;
//        crcValueHigh = crcValue>>8;
//        crcValueLow = crcValue & 0x00FF;
//        
//        putChar(crcResultHigh);
//        putChar(crcResultLow);
//        putChar(crcValueHigh);
//        putChar(crcValueLow);
//        
//        if(crcResult == crcValue){
//            putChar('C');
//            
//            if (RXDATA[0]!='t' && RXDATA[0]!='g' ){
//            } else {
//                switch(RXDATA[1]){
//                    case 0x75:  //'u'
//                        putChar('R');                        
//                        downlinkReceivedCommand(RXDATA[2],RXDATA[3],RXDATA[4],RXDATA[5]);
//                        break;
//                    case 0x63: //'c'
//    //                    CwDownLink(RXDATA);
//                        putChar('C');
//                        putChar('W');
//                        CWKEY = 1;
//                        __delay_ms(50);
//                        CWKEY = 0;
//                        __delay_ms(50);
//
//                        CWKEY = 1;
//                        __delay_ms(50);
//                        CWKEY = 0;
//                        __delay_ms(50);
//
//                        CWKEY = 1;
//                        __delay_ms(50);
//                        CWKEY = 0;
//                        __delay_ms(50);
//
//                        CWKEY = 1;
//                        __delay_ms(150);
//                        CWKEY = 0;
//                        __delay_ms(50);
//                        putChar('C');
//                        putChar('W');
//
//                        break;
//                    case 0x66:  //'f'
//                        putChar('F');
//                        putChar('M');
//                        __delay_ms(2000);
//                        FMPTT = 1;
//                        __delay_ms(2000);
//                        FMPTT = 0;
//                        __delay_ms(2000);
//                        FMPTT = 1;
//                        __delay_ms(2000);
//                        FMPTT = 0;
//                        putChar('F');
//                        putChar('M');
//                        downlinkFMSignal(RXDATA[2],RXDATA[3],RXDATA[4],RXDATA[5],RXDATA[6], RXDATA[7]);
//                        break;
//                    case 0x61:  //'a'
//                        cutWire(RXDATA[2],RXDATA[3]);
//                        break;
//                }
//            }
//        }else{
//            
//            putChar('D');
//            //??¿½?¿½R??¿½?¿½}??¿½?¿½??¿½?¿½??¿½?¿½hCRC??¿½?¿½_??¿½?¿½??¿½?¿½??¿½?¿½??¿½?¿½??¿½?¿½??¿½?¿½??¿½?¿½??¿½?¿½??¿½?¿½??¿½?¿½??¿½?¿½Ìï¿½??¿½?¿½??¿½?¿½
//            //add error messege
//        }
//        RCIF = 0;
//    }
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
//}



////test for interrupt
//void interrupt interReceiverTest( void ){
//    UBYTE RXDATA;
//    if (RCIF == 1) {
//        RXDATA = getChar();
//        //RXDATA++;
//        //putChar('G');
//        putChar(RXDATA);
//        
//        switch (RXDATA){
//        case 'c':
//            putChar('C');
//            putChar('W');
//            
//            while(1){
//            CWKEY = 1;
//            __delay_ms(50);
//            CWKEY = 0;
//            __delay_ms(50);
//
//            CWKEY = 1;
//            __delay_ms(50);
//            CWKEY = 0;
//            __delay_ms(50);
//
//            CWKEY = 1;
//            __delay_ms(50);
//            CWKEY = 0;
//            __delay_ms(50);
//
//            CWKEY = 1;
//            __delay_ms(150);
//            CWKEY = 0;
//            __delay_ms(50);
//            }
//
//            putChar('C');
//            putChar('W');
//            putChar('2');
//            break;
//        case 'f':
//            putChar('F');
//            putChar('M');
//            CWKEY = 0;
////            __delay_ms(2000);
//            FMPTT = 1;
//            UBYTE EEPROMTestData[5];
//            EEPROMTestData[0] = 'H';
//            EEPROMTestData[1] = 'e';
//            EEPROMTestData[2] = 'l';
//            EEPROMTestData[3] = 'l';
//            EEPROMTestData[4] = 'o';
//            for (UINT i = 0; i< 10; i++){
//                SendPacket(EEPROMTestData);
//                __delay_ms(300);
//            }
//            
//            
//            
//            FMPTT = 0;
//            putChar('F');
//            putChar('M');  
//            putChar('2');      
//
//            break;
//        }
//        RCIF = 0;
//    }
//}
    
    
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
    /*??¿½?¿½??¿½?¿½??¿½?¿½??¿½?¿½??¿½?¿½??¿½?¿½*/
    Init_SERIAL();
    Init_MPU();
    InitI2CMaster(I2Cbps);
//    Init_WDT();
    delay_s(TURN_ON_WAIT_TIME);   //wait for PLL satting by RXCOBC and start CW downlink
    putChar('S');
    
    
    while(1){
        /*
        while(1){
            //getbit();
            //debug_ledy();   //6us
            putch('O');
            __delay_ms(500);
//        }*/
//        putChar('m');
//       __delay_ms(1000);
//        FMPTT = 1;
//        UBYTE eDataField[36];
//        for(UBYTE i=0; i<36; i++){
//            eDataField[i] = i;
//        }
//        SendPacket(eDataField);
//        __delay_ms(2000);
        FMPTT = 0;
        
//       __delay_ms(1000);
//        CWKEY = 1;
//        __delay_ms(2000);
//        CWKEY =0;
        //TODO check AD value
        
        //TODO send CW command
        /*---------------------------------------------------------------*/
        //FIXME:[start]debug for downlink CW signal
//        FMPTT = 0; 
//        CWKEY = 1;

        //debug:send morse 'V' 5 times
        for(UBYTE i=0; i<5; i++){
////            Morse_V();
            CWKEY = 1;
            __delay_us(197400);
            CWKEY = 0;
            __delay_us(65800);

            CWKEY = 1;
            __delay_us(65800);
            CWKEY = 0;
            __delay_us(65800);

            CWKEY = 1;
            __delay_us(197400);
            CWKEY = 0;
            __delay_us(65800);

            CWKEY = 1;
            __delay_us(197400);
            CWKEY = 0;
            __delay_us(460600);
            
            
            
            CWKEY = 1;
            __delay_us(65800);
            CWKEY = 0;
            __delay_us(65800);

            CWKEY = 1;
            __delay_us(197400);
            CWKEY = 0;
            __delay_us(65800);

            CWKEY = 1;
            __delay_us(197400);
            CWKEY = 0;
            __delay_us(65800);

            CWKEY = 1;
            __delay_us(197400);
            CWKEY = 0;
            __delay_us(460600);            
        }
        
        //debug send morse char
        UBYTE test_morse[4];
        test_morse[0] = 'O';
        test_morse[1] = 'r';
        test_morse[2] = 'i';
        test_morse[3] = '1';
        sendMorsenew(test_morse,sizeof(test_morse)/sizeof(test_morse[0]));
        
        putChar(0xDD);
        putChar(sizeof(test_morse));
        putChar(sizeof(test_morse[0]));
        int data_size = sizeof(test_morse)/sizeof(test_morse[0]);
        putChar(data_size);
//        
//        //debug:translate binary to char
////        putChar(0xbb);
//        putChar(0xbb);
//        putChar(0xbb);
//        UBYTE _binary;
//        _binary = 0x05;
//        putChar(changeBinaryToChar(_binary));
//        _binary = 0xFF;
//        putChar(changeBinaryToChar(_binary));  //for check to defalt / X -> success
//        __delay_ms(1000);
//
//        //debug:DevideDataAndChangeBinaryToChar
////        putChar(0xcc);
////        putChar(0xcc);
////        putChar(0xcc);
        UBYTE binary_data = 0xF3;
        UBYTE char_data_highLow[2]; 
        DevideDataAndChangeBinaryToChar (binary_data, char_data_highLow);
//        putChar(char_data_highLow[0]);  //'5'->success
//        putChar(char_data_highLow[1]);  //'A'->success
////        __delay_ms(1000);
////
////        //debug:send morse
//////        putChar(0xdd);
//////        putChar(0xdd);
//////        putChar(0xdd);
        for(UBYTE i=0; i<5; i++){
            sendMorsenew(char_data_highLow,sizeof(char_data_highLow)/sizeof(char_data_highLow[0])); //morse signal '5'->delay(150ms)->'A'->success
//            __delay_ms(1000);
        } 
////
////        //debug:send ReadOneByteDataFromEEPROMandSendMorse
////        putChar(0xee);
////        putChar(0xee);
////        putChar(0xee);
//        UBYTE TEST_DATA[3] = {'T', 0x5F, 0b10100111};  //'T'=0x54 / 0b10100111 = 0xA7
//        WriteToEEPROM(EEPROM_address, whigh_address, wlow_address, TEST_DATA);
//        ReadOneByteDataFromEEPROMandSendMorse(EEPROM_address, whigh_address, wlow_address); //morse signal 'T'->success
////       __delay_ms(1000);
////        
////        //debug:ReadDatasFromEEPROMWithDataSizeAndSendMorse
////        putChar(0xff);
////        putChar(0xff);
////        putChar(0xff);
//        UBYTE ReadData[];
//        ReadDatasFromEEPROMWithDataSizeAndSendMorse(EEPROM_address, whigh_address, wlow_address, ReadData, 3); //morse signal 'T'-> 0x5F -> 0b10100111 -> success
//        __delay_ms(1000);
//        
//        //debug:ReadDatasFromEEPROMWithDataSizeAndSendMorseWithDownlinkTimes
//        putChar(0x11);
//        putChar(0x11);
//        putChar(0x11);
//        ReadDatasFromEEPROMWithDataSizeAndSendMorseWithDownlinkTimes(EEPROM_address, whigh_address, wlow_address, ReadData, 3, 5); //morse signal 'T'-> 0x5F -> 0b10100111 -> 5times->success
//        __delay_ms(1000);
        
        //FIXME:[finish]debug for downlink CW signal
        /*---------------------------------------------------------------*/

        //TODO send pulse to WDT
        
    }
    //return;
}
