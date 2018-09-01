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

UBYTE COMMAND_SIZE = 10;


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

void interrupt InterReceiver(UBYTE *RXDATA, UBYTE COMMAND_SIZE){
    volatile static int intr_counter;
    if (RCIF == 1) {
        for (int i = 0; i < COMMAND_SIZE; i++){
            RXDATA[i] = getch();
            putch(RXDATA[i]);
            NOP();
        }
       //TODO add case RXDATA[0]!=t
        if(crc16(0,RXDATA,6) == CRC_check(RXDATA,6)){
            switch(RXDATA[1]){
                case 0x75:
                    downlinkReceivedCommand(RXDATA[2],RXDATA[3],RXDATA[4],RXDATA[5]);
                    break;
                case 0x63:
//                    CwDownLink(RXDATA);
                    break;
                case 0x66:
                    downlinkFMSignal(RXDATA[2],RXDATA[3],RXDATA[4],RXDATA[5],RXDATA[6]);
                    break;
                case 0x61:
                    cutWire(RXDATA[2],RXDATA[3]);
                    break;
            }
        }else{
            ///コマンドCRCダメだった時の処理
        }
    }
}

void main(void) {
    __delay_ms(1000);
    /*初期化*/
    Init_SERIAL();
    Init_MPU();
    InitI2CMaster(I2Cbps);
//    Init_WDT();
    delay_s(TURN_ON_WAIT_TIME);   //wait for PLL satting by RXCOBC and start CW downlink
    putch('A');
    
//    FMPTT = 1;
    
    while(1){
        /*
        while(1){
            //getbit();
            //debug_ledy();   //6us
            putch('O');
            __delay_ms(500);
        }*/
        __delay_ms(1000);
        //TODO check AD value
        //TODO send CW command
        //TODO send pulse to WDT
        
    }
    //return;
}
