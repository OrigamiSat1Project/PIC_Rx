//#include <stdio.h>
//#include <stdlib.h>
#include <xc.h>
#include <PIC16F886.h>
#include "UART.h"
#include "Init_MPU.h"
#include "Type_define.h"
#include "time.h"
#include "encode_AX25.h"
#include "I2C.h"
#include "EEPROM.h"
#include "FMCW.h"
#include "WDT.h"
#include "CW.h"
#include "pinDefine.h"

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

void interrupt InterReceiver( void ){
    volatile static int intr_counter;
    if (RCIF == 1) {
        UBYTE RXDATA[];
        RXDATA[0] = getChar();
        if (RXDATA[0] == 0x74){
            RXDATA[1] = getChar();
            RXDATA[2] = getChar();
            RCIF = 0 ;
            __delay_ms(200);
            UBYTE EEPROMCmdData[40];
            UINT EEPROMCmdDataLength;
            EEPROMCmdDataLength = 32;
            ReadDataFromEEPROM(EEPROM_address,RXDATA[1],RXDATA[2], EEPROMCmdData,EEPROMCmdDataLength);
            __delay_ms(200);
            FMPTT = 1;
            CWKEY = 0;
            for(int i = 0; i<5;i++){
                SendPacket(EEPROMCmdData);
                __delay_ms(300);
            }
            FMPTT = 0;
        }else if (RXDATA[0] == 0xCC && UHFstart==1){
            led_yellow = 1;
            RXDATA[1] = getChar();
            RXDATA[2] = getChar();
            RCIF = 0 ;

            __delay_ms(200);
            UBYTE EEPROMCmdData[];
            UINT EEPROMCmdDataLength;
            EEPROMCmdDataLength = 1;
            ReadDataFromEEPROM(EEPROM_address,RXDATA[1],RXDATA[2], EEPROMCmdData,EEPROMCmdDataLength);
            __delay_ms(200);
            FMPTT = 1;
            CWKEY = 0;
            for(int i = 0; i<5;i++){
                SendPacket(EEPROMCmdData);
                __delay_ms(300);
            }
            FMPTT = 0;
            led_yellow = 0;
        }else if (RXDATA[0] == 0xDD && UHFstart==1){
            led_yellow = 1;
            RXDATA[1] = getChar();
            RCIF = 0 ;

            __delay_ms(200);
            if (RXDATA[1] == 0xDD && UHFstart==1){
                __delay_ms(200);
                FMPTT = 0;
                for (int i = 0; i< 5;i++){
                    CWKEY = 1;
                    __delay_ms(Morse_Short);
                    CWKEY = 0;
                    __delay_ms(Morse_Short);

                    CWKEY = 1;
                    __delay_ms(Morse_Short);
                    CWKEY = 0;
                    __delay_ms(Morse_Short);

                    CWKEY = 1;
                    __delay_ms(Morse_Short);
                    CWKEY = 0;
                    __delay_ms(Morse_Short);

                    CWKEY = 1;
                    __delay_ms(Morse_Long);
                    CWKEY = 0;
                    __delay_ms(Morse_Short);
                    __delay_ms(200);
                } 
                
            }
            
            led_yellow = 0;
        
        //FM_photo_downlink
        }else if (RXDATA[0] == 0xEE && UHFstart==1){
            led_yellow = 1;
            RXDATA[1] = getChar();
            RXDATA[2] = getChar();
            RXDATA[3] = getChar();//No. of data
            RXDATA[4] = getChar();//No. of data
            RCIF = 0 ;
            
            short int PhotoAdd;
            PhotoAdd = RXDATA[1] * 256 + RXDATA[2];
            short int RestPhotoSize;
            RestPhotoSize = RXDATA[3] * 256 + RXDATA[4];
            
            while (RestPhotoSize > 0) {
                __delay_ms(50);
                UBYTE Packet[32];
                UINT PacketSize;
                PacketSize = 32;
                UBYTE PhotoHAdd;
                UBYTE PhotoLAdd;
                PhotoHAdd = PhotoAdd / 256;
                PhotoLAdd = PhotoAdd % 256;
                ReadDataFromEEPROM(EEPROM_Maddress,PhotoHAdd,PhotoLAdd, Packet,PacketSize);
                __delay_ms(200);
                FMPTT = 1;
                CWKEY = 0;
//                for(int i = 0; i<20;i++){
//                    SendPacket(Packet);
//                    __delay_ms(250);
//                }
                for(int i = 0; i<32;i++){
                    putChar(Packet[i]);
                    __delay_ms(10);
                }
                PhotoAdd = PhotoAdd + PacketSize;
                RestPhotoSize = RestPhotoSize - PacketSize;
            }
            
            FMPTT = 0;
            led_yellow = 0;
        }else{
            RCIF = 0 ;
//            led_yellow = 0;
        }
    }else if(PIR1bits.TMR1IF == 1){
        TMR1 = TIMER_INTERVAL;  // ?????????
 
        intr_counter++;
        if (intr_counter >= 2) {
            CLRWDT();
            intr_counter = 0;
        }
 
        PIR1bits.TMR1IF = 0;    // ???????????
    }
}


void main(void) {
    __delay_ms(1000);
    /*èâä˙âª*/
    Init_SERIAL();
    Init_MPU();
    InitI2CMaster(I2Cbps);
//    Init_WDT();
    for (int i = 0; i<10; i++){
        __delay_s(TURN_ON_WAIT_TIME);   //wait for PLL satting by RXCOBC and start CW downlink
    }
    
    
    while(1){
        /*
        while(1){
            //getbit();
            //debug_ledy();   //6us
            putch('O');
            __delay_ms(500);
        }*/
        
        //TODO check AD value
        //TODO send CW command
        
    }
    //return;
}
