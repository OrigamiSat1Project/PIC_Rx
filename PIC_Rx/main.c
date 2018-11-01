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
//#include "OkError.h"
//#include "timer.h"



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
    putChar('A');
    GIE = 1;
    putChar('B');
    PEIE = 1;
    __delay_ms(300);
    putChar('C');
//    initTimer();
    
    

    
    
//    FMTX(FMTX_Nref, FMTX_Nprg);
//    CWTX(CWTX_Nref, CWTX_Nprg);
//    FMRX(FMRX_Nref, FMRX_Nprg);
    
    
//    LED_WHITE = 0;              //for debugging of PLL setting
    __delay_ms(500);           //wait for circuit of PLL
    
    //define command ID
    //UBYTE lastCommandID;        //ID of last uplink command
    
    //WDT = 1; // kick watchdog

    // RC5 = 1;    //5R8G on
    
    UBYTE rxdata = 0xAA;
    //FIXME:for TXPIC
    putChar('S');
    putChar('Y');
    putChar('S');

    putChar('A');
    
    putChar('B');
    
    while(1){
        while(1){
            rxdata = 0xAA;
            __delay_ms(300);
            rxdata = getChar();
            __delay_ms(300);
            putChar(rxdata);
            __delay_ms(300);
            sendPulseWDT();
            __delay_ms(300);
        }
       __delay_ms(500);
    }
    //return;
}
