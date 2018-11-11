//#include <stdio.h>
//#include <stdlib.h>
#include <xc.h>
#include <PIC16F886.h>
#include "UART.h"
#include "Init_MPU.h"
#include "Type_define.h"
#include "time.h"
#include "decode_AX25.h"
#include "encode_AX25.h"
#include "I2C.h"
#include "EEPROM.h"
#include "FMCW.h"

// PIC16F887 Configuration Bit Settings

// 'C' source line config statements

#define SLAVEADDRESS     0x50 << 1      // Slave Address 

/* PIC16F887 Configuration Bit Settings */
#pragma config FOSC     = HS            // Oscillator Selection bits (HS oscillator: High-speed crystal/resonator on RA6/OSC2/CLKOUT and RA7/OSC1/CLKIN)
#pragma config WDTE     = OFF           // Watchdog Timer Enable bit (WDT disabled and can be enabled by SWDTEN bit of the WDTCON register)
#pragma config PWRTE    = ON            // Power-up Timer Enable bit (PWRT disabled)
//#pragma config POR      = ON            // Power On Reset is enabled
#pragma config MCLRE    = ON            // RE3/MCLR pin function select bit (RE3/MCLR pin function is MCLR)
#pragma config CP       = OFF           // Code Protection bit (Program memory code protection is disabled)
#pragma config CPD      = OFF           // Data Code Protection bit (Data memory code protection is disabled)
#pragma config BOREN    = OFF            // Brown Out Reset Selection bits (BOR enabled)
#pragma config IESO     = OFF            // Internal External Switchover bit (Internal/External Switchover mode is enabled)
#pragma config FCMEN    = OFF            // Fail-Safe Clock Monitor Enabled bit (Fail-Safe Clock Monitor is enabled)
#pragma config LVP      = OFF           // Low Voltage Programming Enable bit (RB3 pin has digital I/O, HV on MCLR must be used for programming)
#pragma config BOR4V    = BOR40V        // Brown-out Reset Selection bit (Brown-out Reset set to 4.0V)
#pragma config WRT      = OFF           // Flash Program Memory Self Write Enable bits (Write protection off)

void main(void) {
    /*初期化*/
    //OSCCON = 0x40;
    //Init_FMCW();
    Init_SERIAL();
    Init_MPU();
    Init_I2C_M(I2Cbps);
    //PLL設定
    //SetPLL(FMTX_Nref, FMTX_Nprg, CWTX_Nref, CWTX_Nprg, FMRX_Nref, FMRX_Nprg);
    //Set PLL DATA 
    /*
    FMTX(FMTX_Nref, FMTX_Nprg);
    CWTX(CWTX_Nref, CWTX_Nprg);
    FMRX(FMRX_Nref, FMRX_Nprg);*/
    
    while(1){
        /*
        while(1){
            //getbit();
            //debug_ledy();   //6us
            putch('O');
            __delay_ms(500);
        }*/
        //putch('A');
        //  decoder
        
        /*
        //ReceivePacket();
        //受信
        UBYTE *dData;
        dData = ReceivePacket_data();*/
        UBYTE dData[];
        //ReceivePacket_data(dData);
        
        //UBYTE dData[];
        //UBYTE dData[]="ABXYCDEZF";
        UINT write_data_length;
        for (write_data_length = 0; dData[write_data_length]!= '\0'; ++write_data_length);
        //EEPROM書き込み
        //for(UINT i=0;i<=4;i++){
        __delay_ms(200);
        //EEPROM_Write(EEPROM_address,whigh_address,wlow_address,dData,write_data_length);
        //EEPROM_Write(EEPROM_address,whigh_address,wlow_address,dData,6);
        __delay_ms(200);
        //}
        /*
        //デバッグ用データUART送信
        
        if(dData != 0x00){
            putstr(dData);
            putcrlf();
        }else{
            put_error;
        }*/
        
        //NanoMindにアドレス送る
        /*UBYTE NM_wad_header = 0x72;
        NM_waddress(NM_wad_header, whigh_address, wlow_address);
        */
        //while(1){
        //RB5がHighならEEPROM読み込み
        UBYTE *EEPROMData;
        if (UHFstart==1){
            UBYTE EEPROMData[32];
            UINT EEPROMDataLength;
            //putch('B');
            __delay_ms(200);
            EEPROM_Read(EEPROM_address,rhigh_address,rlow_address, EEPROMData,&EEPROMDataLength);
            __delay_ms(200);
            //putch('G');
            //
            //  encoder
            //UBYTE ePacket[50];
            SendPacket(EEPROMData);
            //test_Packetmaker(EEPROMData);
            //SendPacket();
            __delay_ms(200);
            
            
            //デバッグ用データUART送信
            /*putch(EEPROMData[0]);
            putch(EEPROMData[1]);
            putch(EEPROMData[2]);
            //putch('A');
            putch(EEPROMData[3]);
            putch(EEPROMData[4]);
            putch(EEPROMData[5]);
            putch(EEPROMData[6]);
            putch(EEPROMData[7]);
            putch(EEPROMData[8]);*/
            //
            /* 
            if(EEPROMData != 0x00){
                putstr(EEPROMData);
                //putch('B');
                putcrlf();
            }else{
                //putch('C');
            }*/

        
        }else{
            put_error;
            //putch('B');
        }
        //}
        
        //EPSキル
        //Reset_EPS();
        __delay_ms(200);
        
        
    }
    //return;
}
