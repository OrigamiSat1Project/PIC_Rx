//#include <stdio.h>
//#include <stdlib.h>
#include <string.h>
#include <xc.h>
//#include <PIC16F887.h>
#include <PIC16LF877A.h>
#include "UART.h"
#include "Init_MPU.h"
#include "Type_define.h"
#include "time.h"
#include "decode_AX25.h"
#include "encode_AX25.h"
#include "I2C.h"
#include "EEPROM.h"
#include "FMCW.h"
#include "EPS.h"
#include "CRC16.h"

// PIC16F887 Configuration Bit Settings

// 'C' source line config statements

#define SLAVEADDRESS     0x50 << 1      // Slave Address 

///* PIC16F887 Configuration Bit Settings */
//#pragma config FOSC     = HS            // Oscillator Selection bits (HS oscillator: High-speed crystal/resonator on RA6/OSC2/CLKOUT and RA7/OSC1/CLKIN)
//#pragma config WDTE     = OFF           // Watchdog Timer Enable bit (WDT disabled and can be enabled by SWDTEN bit of the WDTCON register)
//#pragma config PWRTE    = ON            // Power-up Timer Enable bit (PWRT disabled)
////#pragma config POR      = ON            // Power On Reset is enabled
//#pragma config MCLRE    = ON            // RE3/MCLR pin function select bit (RE3/MCLR pin function is MCLR)
//#pragma config CP       = OFF           // Code Protection bit (Program memory code protection is disabled)
//#pragma config CPD      = OFF           // Data Code Protection bit (Data memory code protection is disabled)
//#pragma config BOREN    = ON            // Brown Out Reset Selection bits (BOR enabled)
//#pragma config IESO     = OFF            // Internal External Switchover bit (Internal/External Switchover mode is enabled)
//#pragma config FCMEN    = OFF            // Fail-Safe Clock Monitor Enabled bit (Fail-Safe Clock Monitor is enabled)
//#pragma config LVP      = OFF           // Low Voltage Programming Enable bit (RB3 pin has digital I/O, HV on MCLR must be used for programming)
//#pragma config BOR4V    = BOR21V        // Brown-out Reset Selection bit (Brown-out Reset set to 4.0V)
//#pragma config WRT      = OFF           // Flash Program Memory Self Write Enable bits (Write protection off)

// CONFIG
#pragma config FOSC = HS        // Oscillator Selection bits (HS oscillator)
#pragma config WDTE = OFF       // Watchdog Timer Enable bit (WDT disabled)
#pragma config PWRTE = ON       // Power-up Timer Enable bit (PWRT enabled)
#pragma config BOREN = OFF       // Brown-out Reset Enable bit (BOR enabled)
#pragma config LVP = OFF        // Low-Voltage (Single-Supply) In-Circuit Serial Programming Enable bit (RB3 is digital I/O, HV on MCLR must be used for programming)
#pragma config CPD = OFF        // Data EEPROM Memory Code Protection bit (Data EEPROM code protection off)
#pragma config WRT = OFF        // Flash Program Memory Write Enable bits (Write protection off; all program memory may be written to by EECON control)
#pragma config CP = OFF         // Flash Program Memory Code Protection bit (Code protection off)

// #pragma config statements should precede project file includes.
// Use project enums instead of #define for ON and OFF.


void main(void) {
    /*Initialize*/
    //OSCCON = 0x40;
    //Init_FMCW();
    
    __delay_ms(200);
    
    Init_SERIAL();
    Init_MPU();
    Init_I2C_M(I2Cbps);
//    Init_WDT();
    
    //PLL setting
    //SetPLL(FMTX_Nref, FMTX_Nprg, CWTX_Nref, CWTX_Nprg, FMRX_Nref, FMRX_Nprg);
    //Set PLL DATA 
    /**/
//    RB1 = 1;
    led_white = 1;
    __delay_ms(1000);
    
    FMTX(FMTX_Nref, FMTX_Nprg);
    CWTX(CWTX_Nref, CWTX_Nprg);
    FMRX(FMRX_Nref, FMRX_Nprg);
    led_white = 0;
    
    __delay_ms(500);
    RB3 = 1;
//    RC5 = 1;
    printf("start\r\n");
//    RA1 = 0;
    while(1){
        /*
        while(1){
            //getbit();
            //debug_ledy();   //6us
            //
        }*/
        //
        //  decoder
        /**/
        //ReceivePacket();
        
        /**/
        //Receive command data
//        RA1 = 0;
        UBYTE *commandData;
        //for information on EEPROM see data sheet: 24LC1025        
        UBYTE B0select;             //control byte B0 of EEPROM
        UBYTE wHighAddress;         //address high byte of EEPROM
        UBYTE wLowAddress;          //address low byte of EEPROM
        UBYTE mainControlByte;      //control byte of main EEPROM
        UBYTE subControlByte;       //control byte of sub EEPROM
        
        
        UBYTE downlinkTimes;       //downlink times of received command
        commandData = ReceivePacket_data();
        led_white = 1;
        __delay_ms(500);
        led_white = 0;
        
        B0select = commandData[19];
        wHighAddress = commandData[20];
        wLowAddress = commandData[21];
        downlinkTimes = commandData[22];
        mainControlByte = MAIN_EEPROM_ADDRESS | B0select;
        subControlByte = SUB_EEPROM_ADDRESS | B0select;
        
        
        //Write uplink command in EEPROM
        EEPROM_Write(mainControlByte,wHighAddress,wLowAddress,commandData);
        EEPROM_Write(subControlByte,wHighAddress,wLowAddress,commandData);
        __delay_ms(100);
        
        //inform TXPIC RXDATA(PIN43 = 1)
        
//        UBYTE TXOBC_wad_header = 0x74;
//        sendCommand(TXOBC_wad_header, whigh_address, wlow_address);
        /*---Send address using UART to OBC and TXCOBC---*/
        /*------------------------------------------------------------------*/
        sendCommand('g', 'u', B0select, wHighAddress, wLowAddress, downlinkTimes);
        
        printf("%s\r\n", commandData);
//        for (UINT i = 0;i < 32;i++){
//            printf("%c", dData[i]);
//            dData[i] = 0x00;
//        }
//        led_white = 1;
        __delay_ms(7000);
//        led_white = 0;
        
        //printf("\r\n");
        // Command target
        if(commandData[0]=='n'){      //NanoMind
            //Send address to NanoMind
            UBYTE NM_wad_header = 0x72;
//            NM_waddress(NM_wad_header, whigh_address, wlow_address);
            
        }else if(commandData[0]=='p'){        //PIC_RX
            //Task target
//            led_white = 1;
//            __delay_ms(500);
//            led_white = 0;
            if(commandData[2] == 'r'){        // PIC_RX
                // Command type
//                led_white = 1;
//        __delay_ms(500);
//        led_white = 0;
                switch(commandData[3]){
                case 'E':
//                    led_white = 1;
//                    __delay_ms(500);
//                    led_white = 0;
                    // EPS kill
//                    
//                    __delay_ms(5000);
                    Reset_EPS();
                    __delay_ms(5000);
                    //以下�?�数字�?�初期設定時と変化して�?るためも�?�?度定義
                    //本来なら変化する�?字�?�を他に用意したほ�?が良�?かもしれな�?
                    int FMTX_Nprg[5]     =   {8,7,3,0,0};   // Nprg = 87300 = Ftx / 0.05 [436.500MHz]
                    int CWTX_Nprg[5]     =   {0,1,7,4,7};   // Nprg = 1747(* see 301ACWPLL-20080520.pdf *) [436.750MHz]
                    int FMRX_Nprg[5]     =   {2,4,8,8,7};   // Nprg = 24887 = (Frx - 21.4) / 0.05 [145.835MHz]
                    
                    FMTX(FMTX_Nref, FMTX_Nprg);
                    CWTX(CWTX_Nref, CWTX_Nprg);
                    FMRX(FMRX_Nref, FMRX_Nprg);
                    __delay_ms(500);
                    break;
                case 'I':
                    // I2C mode
                    break;
                case '3':
                    // TX周波数設�?
                    break;
                case 'N':
                    // NanoMind
                    break;
                case 'T':
                    // send TXPIC by I2C
                    break;
                default:
                    // error
                    break;
                }
                
            }else if(commandData[2] == 't'){      // PIC_TX

            }
        }else{
            led_white = 1;
            __delay_ms(1000);
            led_white = 0;
        }
        
        
        /*
        //Read data in EEPROM
        UBYTE *EEPROMData;
        EEPROMData = EEPROM_Read(EEPROM_address,rhigh_address,rlow_address,4);*/
        
        //Reset_EPS();
          //  __delay_ms(500);
        
        //EPS kill
        /**/
        // UBYTE EPS_kill[];
        // UBYTE EPS_kill_1[] = "EA";
        // UBYTE EPS_kill_2 = 0x0D;
        // sprintf(EPS_kill,"%s%c",EPS_kill_1,EPS_kill_2);
        //strcat(EPS_kill_1,EPS_kill_2);
        //putstr(dData);
        //putstr(EPS_kill);
        /*
        UBYTE EPS_kill[];
        sprintf(EPS_kill,"%s%c",EPS_kill_1,EPS_kill_2);
        putstr(dData);
        putstr(EPS_kill);*/
        // if (strcmp(dData,EPS_kill) == 0){
        //     Reset_EPS();
        //     __delay_ms(5000);
        //     //以下�?�数字�?�初期設定時と変化して�?るためも�?�?度定義
        //     //本来なら変化する�?字�?�を他に用意したほ�?が良�?かもしれな�?
        //     int FMTX_Nprg[5]     =   {8,7,3,0,0};   // Nprg = 87300 = Ftx / 0.05 [436.500MHz]
        //     int CWTX_Nprg[5]     =   {0,1,7,4,7};   // Nprg = 1747(* see 301ACWPLL-20080520.pdf *) [436.750MHz]
        //     int FMRX_Nprg[5]     =   {2,4,8,8,7};   // Nprg = 24887 = (Frx - 21.4) / 0.05 [145.835MHz]
            
        //     FMTX(FMTX_Nref, FMTX_Nprg);
        //     CWTX(CWTX_Nref, CWTX_Nprg);
        //     FMRX(FMRX_Nref, FMRX_Nprg);
        //     __delay_ms(500);
            
        // }
        
        
        //  encoder
        //SendPacket();
        __delay_ms(500);
    }
    return;
}
