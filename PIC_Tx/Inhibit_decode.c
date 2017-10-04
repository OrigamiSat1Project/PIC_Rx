/*
 * File:   main.c
 * Author: Kosuke
 *
 * Created on 2016/11/03, 14:04
 */

#include <stdio.h>
#include <stdlib.h>
#include <xc.h>
#include <PIC16F887.h>
#include "UART.h"
#include "Init_MPU.h"
#include "Type_define.h"
#include "time.h"


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
#pragma config BOREN    = ON            // Brown Out Reset Selection bits (BOR enabled)
#pragma config IESO     = OFF            // Internal External Switchover bit (Internal/External Switchover mode is enabled)
#pragma config FCMEN    = OFF            // Fail-Safe Clock Monitor Enabled bit (Fail-Safe Clock Monitor is enabled)
#pragma config LVP      = OFF           // Low Voltage Programming Enable bit (RB3 pin has digital I/O, HV on MCLR must be used for programming)
#pragma config BOR4V    = BOR40V        // Brown-out Reset Selection bit (Brown-out Reset set to 4.0V)
#pragma config WRT      = OFF           // Flash Program Memory Self Write Enable bits (Write protection off)

void main(void) {
    /*初期化*/
    //OSCCON = 0x40;
    
    static UBYTE AX25_A[] = "0100000101";
    
    Init_SERIAL();
    Init_MPU();
    
    /*メインループ*/
    while(1){
        /*
        RD1 = 1;
        delay_us(125);
        RD1 = 0;
        delay_us(244);*/
        /*
        for(UINT i=0;i<10;i++){
        if(FX614_RXD == 0){
            putch('0');
        }else{
            putch('1');
        }
        }
        delay_us(span);*/
        //  UART_RXをデジタルinputに変換
        /**/
        UBYTE buf[10];
        while(FX614_RXD == 0){  //後々は割り込み処理
            //led_onoff(10);
            delay_us(span/2);
            for(UINT i=0;i<10;i++){
                if(FX614_RXD == 0){
                    buf[i] = '0';
                }else{
                    buf[i] = '1';
                }
                delay_us(span);
            }
            while(FX614_RXD == 1);
            /*
            for(UINT i=0;i<10;i++){
            putch(buf[i]);
            }*/
            //delay_ms(1000);
            UBYTE check_bit = 0;
            for(UINT i=0;i<10;i++){
                if(buf[i] == AX25_A[i]){
                    check_bit ++;
                    if(check_bit == 10){
                        putch('A');
                        check_bit = 0;
                    }
                }else{
                    putch(check_bit);
                    check_bit = 0;
                    break;
                }
            }
        //delay_ms(1000);
        }
        //  デジタルinputが'A'ならUARTで'A'送信
        /**/
    //}
        
        //  ledチカ
        /*
        ledy_on();
        delay_ms(1000);
        ledy_off();
        delay_ms(1000);*/
    }
    return;
}
