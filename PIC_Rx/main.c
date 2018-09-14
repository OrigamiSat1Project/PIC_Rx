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
    
    __delay_ms(500);
    RB3 = 1;
//    RC5 = 1;
//    printf("start\r\n");
//    putch(0x00);
//    RA1 = 0;
    UBYTE DATAfromTXPIC = 0x00;
    while(1){
        DATAfromTXPIC = getChar();
        putChar(DATAfromTXPIC);
    }
    return;
}
