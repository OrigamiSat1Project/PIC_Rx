//#include <stdio.h>
//#include <stdlib.h>
#include <string.h>
#include <xc.h>
#include <PIC16LF877A.h>
#include "ADC.h"
#include "typeDefine.h"
#include "pinDefine.h"
#include "MPU.h"
#include "time.h"
#include "UART.h"
#include "decodeAX25.h"
#include "I2C.h"
#include "EEPROM.h"
#include "FMCW.h"
#include "EPS.h"
#include "WDT.h"
#include "CRC16.h"
#include "OkError.h"
#include "timer.h"



// PIC16F887 Configuration Bit Settings
#pragma config FOSC     = HS        // Oscillator Selection bits (HS oscillator)
#pragma config WDTE     = OFF       // Watchdog Timer Enable bit (WDT disabled)
#pragma config PWRTE    = ON        // Power-up Timer Enable bit (PWRT enabled)
#pragma config BOREN    = OFF       // Brown-out Reset Enable bit (BOR enabled)
#pragma config LVP      = OFF       // Low-Voltage (Single-Supply) In-Circuit Serial Programming Enable bit (RB3 is digital I/O, HV on MCLR must be used for programming)
#pragma config CPD      = OFF       // Data EEPROM Memory Code Protection bit (Data EEPROM code protection off)
#pragma config WRT      = OFF       // Flash Program Memory Write Enable bits (Write protection off; all program memory may be written to by EECON control)
#pragma config CP       = OFF       // Flash Program Memory Code Protection bit (Code protection off)
// #pragma config statements should precede project file includes.
// Use project enums instead of #define for ON and OFF.


//TODO:add interrupt finction?
void main(void) {
    
    /*---Initialization---*/
    /*----------------------------------------------------------------------*/ 
    InitSerial();
    InitMPU();
    InitWDT();
    InitI2CMaster(I2C_baud_rate_def);
    initTimer();
    
//    LED_WHITE = 1;              //for debugging of init
    __delay_ms(2000);           //wait until 5V is generated by EPS


    
    FMTX(FMTX_Nref, FMTX_Nprg);
    CWTX(CWTX_Nref, CWTX_Nprg);
    FMRX(FMRX_Nref, FMRX_Nprg);
    
    
//    LED_WHITE = 0;              //for debugging of PLL setting
    __delay_ms(500);           //wait for circuit of PLL
    
    //define command ID
    //UBYTE lastCommandID;        //ID of last uplink command
    
    //WDT = 1; // kick watchdog

    // RC5 = 1;    //5R8G on
    
    //FIXME:for TXPIC
//    putChar('S');
//    putChar('Y');
//    putChar('S');
    
    UBYTE *bat;
    int flag = -1;
    
    while(1){
        while(1){
//            while(flag == -1){
                flag = ReadEEPROM(0x50,0x00,0x00);
//            if(flag != -1){
//            }else putChar(0xEE);
                __delay_ms(5);
//            }
            putChar((UBYTE)flag);
            flag = -1;
        }
            
//        SendBatVoltage();
//        delay_ms(300);
//        WriteOneByteToEEPROM(0x50,0x00,0x00,0xAE);
//        eep[0] = ReadEEPROM(0x50,0x00,0x00);
//        putChar(eep[0]);
//        WriteOneByteToEEPROM(0x52,0x00,0x00,0xAF);
//        eep[1] = ReadEEPROM(0x52,0x00,0x00);
//        putChar(eep[1]);
        
        
        /*---Receive command data---*/ 
        /*------------------------------------------------------------------*/
        UBYTE commandData[DATA_SIZE];         //data of uplink command
//         UBYTE *commandData;         //data of uplink command
//        UBYTE commandID;            //ID of uplink command

        //for information on EEPROM see data sheet: 24LC1025        
        UBYTE B0select;             //control byte B0 of EEPROM
        UBYTE wHighAddress;         //address high byte of EEPROM
        UBYTE wLowAddress;          //address low byte of EEPROM
        UBYTE mainControlByte;      //control byte of main EEPROM
        UBYTE subControlByte;       //control byte of sub EEPROM       
        UBYTE downlinkTimes;       //downlink times of received command

        receiveDataPacket(commandData);

        
//        putString(commandData);
//        putChar('C');
        
//        commandID = commandData[1];
//        
//        putChar(commandID);
//        putCrLf();
//        putChar(lastCommandID);
//        putCrLf();
////        if (commandID == lastCommandID) continue;       //same uplink command
//        lastCommandID = commandID;                      //update command ID
        
//        for (int i = 0; i<5;i++){
//            LED_WHITE = 1;
//            __delay_ms(2000);
//            LED_WHITE = 0;
//            __delay_ms(1000);
//        }
        // putChar('D');
        // LED_WHITE = 1;
        // __delay_ms(500);
        // LED_WHITE = 0;
        
        B0select = commandData[19];
        wHighAddress = commandData[20];
        wLowAddress = commandData[21];
        downlinkTimes = commandData[22];
        mainControlByte = MAIN_EEPROM_ADDRESS | B0select;
        subControlByte = SUB_EEPROM_ADDRESS | B0select;
        
        //printf("%s\r\n", commandData); //used for debugging with computer
                
//        LED_WHITE = 1; //debugging receive command from ground station
//        __delay_ms(100);
//        LED_WHITE = 0;

        
        /*---CRC check for command from Grand Station---*/ 
        /*------------------------------------------------------------------*/
        UWORD crcResult, crcValue;
        crcResult = crc16(0,commandData,29);
        crcValue =  checkCRC(commandData,29);
        
        /*------------------------------------*/
        //for debug (check CRC)
//        UBYTE crcResultHigh,crcResultLow,crcValueHigh,crcValueLow;
//        crcResultHigh = crcResult>>8;
//        crcResultLow = crcResult & 0x00FF;
//        crcValueHigh = crcValue>>8;
//        crcValueLow = crcValue & 0x00FF;
        /*------------------------------------*/
        
        if(crcResult != crcValue){
            commandData[31] = 0x00; 
            switchError(error_main_crcCheck);
        }else{
            commandData[31] = 0b1000000;
//            switchOk(ok_main_crcCheck);           
        }  
        
        /*---Write uplink command in EEPROM---*/
        /*------------------------------------------------------------------*/
        WriteToEEPROM(mainControlByte,wHighAddress,wLowAddress,commandData);
        WriteToEEPROM(subControlByte,wHighAddress,wLowAddress,commandData);
        WriteLastCommandIdToEEPROM(commandData[1]);
        putChar('S');
        
        /*---Send address using UART to OBC and TXCOBC---*/
        /*------------------------------------------------------------------*/
//        UBYTE send_command[8];
//        send_command[0] = 'g';
//        send_command[1] = 'u';
//        send_command[2] = B0select;
//        send_command[3] = wHighAddress;
//        send_command[4] = wLowAddress;
//        send_command[5] = downlinkTimes;
//        send_command[6] = 0x00;
//        send_command[7] = 0x00;
//        sendCommandByPointer(send_command);
        sendCommand('g','u',B0select, wHighAddress, wLowAddress, downlinkTimes, 0x00, 0x00);
        putChar('G');
        
        for(int i=0; i<DATA_SIZE; i++){
            putChar(commandData[i]);
        }
        putChar('H');
        /*---Define if command target is RXCOBC 'R' and read in task target ---*/
        /*------------------------------------------------------------------*/
        if(commandData[0]=='R'){                //command target = PIC_RX
            //Task target
            if(commandData[2] == 'r'){          //task target =  PIC_RX
                // Command type
                switch(commandData[3]){         //Process command type
                case 'm': /*change sattelite mode*/
                    commandSwitchSatMode(commandData[4], commandData[5], commandData[6]);
                    break;
                case 'p': /*power supply*/
                    commandSwitchPowerSupply(commandData[4], commandData[5], commandData[6], commandData[7]);
                    break;
                case 'n': /*radio unit*/
                    commandSwitchFMCW(commandData[4], commandData[5], commandData[6], commandData[7], commandData[8], commandData[9]);
                    break;
                case 'i':/*I2C*/
                    commandSwitchI2C(commandData[4], commandData[5], commandData[6], commandData[7], commandData[8]);
                    break;
                case 'e': /*EEPROM*/
                    commandSwitchEEPROM(commandData[4], commandData[5], commandData[6], commandData[7], commandData[8], commandData[9]);
                    break;
                case 'u':/*UART*/
                    commandSwitchUART(commandData[4], commandData[5], commandData[6], commandData[7], commandData[8], commandData[9]);
                    break;
                case 'w':/*WDT (watch dog timer)*/
                    commandWDT(commandData[4]);
                    break;
                case 'h':/*update HK data (BAT_POS V) (HK = house keeping)*/
                    //TODO: write function directly here or in MPU.c                   
                    break;
                case 'r':/*internal processing*/
                    commandSwitchIntProcess(commandData[4], commandData[5], commandData[6]);                   
                    break;
                default:
                    switchError(error_main_reveiveCommand);
                    break;
                }

            // }else if(commandData[2] == 't'){      //task target =  PIC_TX       

            // }else if(commandData[2] == 'o'){      //task target =  OBC       

            // }else if(commandData[2] == '5'){      //task target =  5R8G       

            }
        }else{
            //debugging if coomand target is not RXCOBC
            LED_WHITE = 1;  
            __delay_ms(1000);
            LED_WHITE = 0;
        }
        
        /*---Old command switch case---*/ //kept for reference can be deleted once new switch code is finished and tested
//        if(commandData[0]=='R'){                //command target = PIC_RX
//            //Task target
//            if(commandData[2] == 'r'){          //task target =  PIC_RX
//                // Command type
//                switch(commandData[3]){
//                case 'E': /*EPS kill*/
//                    Reset_EPS();
//                    __delay_ms(5000);
//                    // values for Nprg are changed in setNprg function so they have to be reset
//                    //TODO: make seperate function for set-up
//                    int FMTX_Nprg[5]     =   {8,7,5,0,1};   // Nprg = 87300 = Ftx / 0.05 [436.500MHz]
//                    int CWTX_Nprg[5]     =   {0,1,4,0,0};   // Nprg = 1747(* see 301ACWPLL-20080520.pdf *) [436.750MHz]
//                    int FMRX_Nprg[5]     =   {2,4,9,1,6};   // Nprg = 24887 = (Frx - 21.4) / 0.05 [145.835MHz]
//                    //reset PLL setting (because it gets lost during shutdown)
//                    FMTX(FMTX_Nref, FMTX_Nprg);
//                    CWTX(CWTX_Nref, CWTX_Nprg);
//                    FMRX(FMRX_Nref, FMRX_Nprg);
//                    __delay_ms(500);
//                    break;
//                case 'I':
//                    // I2C mode
//                    break;
//                case '3':
//                    // 
//                    break;
//                case 'N':
//                    // NanoMind
//                    break;
//                case 'T':
//                    // send TXPIC by I2C
//                    break;
//                default:
//                    // error
//                    break;
//                }
//
//            }else if(commandData[2] == 't'){      //task target =  PIC_TX       
//
//            }else if(commandData[2] == 'o'){      //task target =  OBC       
//
//            }else if(commandData[2] == '5'){      //task target =  5R8G       
//
//            }
//        }else{
//            //debugging if coomand target is not RXCOBC
//            LED_WHITE = 1;  
//            __delay_ms(1000);
//            LED_WHITE = 0;
//        }
        // __delay_ms(500);

        /*-------------------------------------------------------------------*/
        //FIME:debug for test to change UART baud rate start 
//        UBYTE c;
//        c = getChar();
//        c++;
//        putChar(c);
        // for(UBYTE i=1; i<20; i++){
        // putChar(i);
        // }
        //FIME:debug for test to change UART baud rate finish 
        /*-------------------------------------------------------------------*/

        /*-------------------------------------------------------------------*/
        //FIXME:[start]for debug to test to change h/l and i/o
//        UBYTE m;
//        UBYTE s; 
//        putChar(0xaa);
//         changeInOut(0x0a, 0b00011001);
//         putChar(TRISA);
//         m = ReadEEPROM(MAIN_EEPROM_ADDRESS, TRISA_addressHigh, TRISA_addressLow);
//         m++;
//         putChar(m);
//         s = ReadEEPROM(SUB_EEPROM_ADDRESS, TRISA_addressHigh, TRISA_addressLow);
//         s++;
//         putChar(s);
//         __delay_ms(1000);
//         
//         putChar(0xbb);
//         changeInOut(0x0b, 0b00100001);
//         putChar(TRISB);
//         __delay_ms(1000); 
//         
//         putChar(0xcc);
//         changeInOut(0x0c, 0b10011001);
//         putChar(TRISC);
//         __delay_ms(1000);   
//         
//         putChar(0xdd);
//         changeInOut(0x0d, 0b00001101);
//         putChar(TRISD);
//         __delay_ms(1000);   
//         
//         putChar(0xee);
//         changeInOut(0x0e, 0b00000111);
//         putChar(TRISE);
//         __delay_ms(1000); 
        //FIXME:[finish]for debug to test to change h/l and i/o
        /*-------------------------------------------------------------------*/
         
         /*-------------------------------------------------------------------*/
        //FIXME:[start]for debug to test to change h/l and i/o
//         putChar(0xaa);
//         changeHighLow(0x0a, 0b00000001);
//         //changeHighLow(0x0a, 0b00000010);
//         putChar(RA0);      
//         __delay_ms(1000);
//         
//         putChar(0xbb);
//        // changeHighLow(0x0b, 0b00010000);
//         changeHighLow(0x0b, 0b00000001);
//         putChar(RB0);      
//         __delay_ms(1000);
//         
//         putChar(0xcc);
//         changeHighLow(0x0c, 0x01);
//         //putChar(PORTC);
//         putChar(RC0);
//         __delay_ms(1000);
//         
//         putChar(0xdd);
//         changeHighLow(0x0d, 0x01);
//         //putChar(PORTD); 
//         putChar(RD0);
//         __delay_ms(1000);   
//         
//         putChar(0xee);
//         changeHighLow(0x0e, 0x01);
//         //putChar(PORTE);
//         putChar(RE0);
//         __delay_ms(1000); 
        //FIXME:[finish]for debug to test to change h/l and i/o
        /*-------------------------------------------------------------------*/
        
        /*-------------------------------------------------------------------*/
        //FIXME:for debug to test switch power supply start
        // UBYTE onOff = 0x01;
        // UBYTE timeHigh = 0x00;
        // UBYTE timeLow = 0x00;
 
        // putChar(0xaa);
        // putChar(0xaa);
        // putChar(0xaa);
//        for(UBYTE i=1; i<10; i++){
//            SEP_SW = HIGH;
//            RBF_SW = HIGH;
//            putChar(0x11);
//            __delay_ms(2000); 
//            SEP_SW = LOW;
//            RBF_SW = LOW;
//            putChar(0x22);
//            __delay_ms(2000); 
//        }
//        for(UBYTE i=1; i<5; i++){
//            putChar(i);
//            onOff = 0x01;
//            switchPowerEPS(onOff, timeHigh, timeLow);
//            __delay_ms(1000);
//            onOff = 0x00;
//            switchPowerEPS(onOff, timeHigh, timeLow);
//            __delay_ms(1000);            
//        }   
        // delay_ms(100);
        // for(UBYTE i=0; i<4; i++){
        //     putChar(0xbb);
        //     putChar(0xbb);
        //     putChar(i);
        //     putChar(0xbb);
        //     putChar(0xbb);
       
        //     onOff = 0x01;
        //     timeHigh = 0x00;
        //     timeLow = 0x03;
        //     delay_ms(100);
            
        //     putChar(0xc1);
        //     putChar(0xc1);
        //     delay_ms(100);
        //     switchPowerEPS(onOff, timeHigh, timeLow);
        //     delay_ms(100);
        //     putChar(0xc2);
        //     putChar(0xc2);
        //     delay_s(1);
        // }   
        // putChar(0xf2);
        // putChar(0xf2);
        // putChar(0xf2);
        // delay_ms(100);
        //FIXME:for debug to test switch power supply finish
        /*-------------------------------------------------------------------*/
    
       __delay_ms(500);
    }
    //return;
}
