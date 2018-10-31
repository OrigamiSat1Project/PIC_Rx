//#include <stdio.h>
//#include <stdlib.h>
#include <string.h>
#include <xc.h>
#include <PIC16LF877A.h>
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

//Receive command from TX_PIC
//void interrupt InterReceiver(void);

//void interrupt InterReceiver(void){
//    
//    UBYTE RXDATA[2];
//    
//    //volatile static int intr_counter;
//    if (RCIF == 1) {                              //The USART receive buffer is full
//        putChar(0xab);
//        putChar(0xab);
//        putChar(0xab);
//        
//        for (int i = 0; i<2; i++){
//            RXDATA[i] = getChar();
//            NOP();
//        }        
//        for (int i=0; i<2; i++){
//            putChar(RXDATA[i]);
//            NOP();
//        }
//        RCIF = 0;  //USART Receive Interrupt Flag is reset
//    }
//}

void main(void) {
    
    
    /*---Initialization---*/
    /*----------------------------------------------------------------------*/ 
    InitSerial();
    InitMPU();
    InitWDT();
    InitI2CMaster(I2Cbps);
    
    POWER_5R8G = HIGH;
    putChar('S');
    
    LED_WHITE = 1;              //for debugging of init
    __delay_ms(1000);           //wait until 5V is generated by EPS
    
    //count reset times
//    UBYTE reset_counter;
//    reset_counter = 0;
//    WriteToEEPROM(SUB_EEPROM_ADDRESS_B1,0x00,0x80,reset_counter);
//    reset_counter = ReadEEPROM(SUB_EEPROM_ADDRESS_B1,0x00,0x80);
//    reset_counter++;
//    putChar(0xab);
//    putChar(reset_counter);
//    putChar(0xcd);
//    WriteToEEPROM(SUB_EEPROM_ADDRESS_B1,0x00,0x80,reset_counter);
    
    //PLL setting (common Phase-Locked Loop)
    FMTX(FMTX_Nref, FMTX_Nprg);
    CWTX(CWTX_Nref, CWTX_Nprg);
    FMRX(FMRX_Nref, FMRX_Nprg);
    
    LED_WHITE = 0;              //for debugging of PLL setting
    __delay_ms(1000);           //wait for circuit of PLL
    
//    GIE = 1;
//    PEIE = 1;
    
    while(1){
        //power on 5R8G
        POWER_5R8G = HIGH;
        
        //power on WDT
        POWER_WDT = LOW;
        //power off WDT
//        POWER_WDT = HIGH;
//        
        putChar('m');
        sendPulseWDT();
        delay_ms(3000);
        
        //check for loop
//        while(1){
//            putChar(0xad);
//            delay_ms(1000);
//        }
        
//        UBYTE data[2];
//        switch(OBC_STATUS){
//            case HIGH:
//                putChar('H');
//                data[0]=0xab;
//                data[1]=0xcd;
//                WriteToEEPROM(SUB_EEPROM_ADDRESS_B1,0x00,0x82,data);
//                break;
//            case LOW: 
//                putChar('L');
//                data[0]=0x12;
//                data[1]=0x34;
//                WriteToEEPROM(SUB_EEPROM_ADDRESS_B1,0x00,0x82,data);
//                break;
//        }
    }
}
