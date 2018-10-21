/*
 * File:   main.c
 * Author: Hohmann Britta
 * Revision history: v1.0
 * 
 * Program Description: This Program Allows
 * implements ADC for PIN RA3 of PIC16F886
 * function is checked using the onboard LED
 * 
 * Created on 2018/05/08, 16:54
 */

/*******************************************************************************
*Includes and defines
******************************************************************************/
//#include "PIC16F886_Internal.h"
#include "typeDefine.h"
#include "UART.h"
#include "time.h"
#include "ADC.h"
#include <xc.h>
#include <stdio.h>
/*******************************************************************************
* Function: void initMain()
*
* Returns: Nothing
*
* Description: Contains initializations for main
*
* Usage: initMain()
******************************************************************************/
void initADC(){
    /*ADCON0: A/N CONTROL REGISTER 0*/
    // Set ADC conversion clock source, conversion time is 3.2us (Fosc/32)
    ADCON0bits.ADCS1 = 1;
    ADCON0bits.ADCS0 = 0;

    /*ADCON1: A/N CONTROL REGISTER 1*/
    // Set result formatting to right justified
    ADCON1bits.ADFM = 1;

    // Set ADC conversion clock source, conversion time is 3.2us (Fosc/32)
    ADCON1bits.ADCS2 = 0;

    // Set Port Configuration and ADC reference Voltage
    //AN7-AN1 Digital I/O / Vref- = Vss / Vref+ = Vdd
    ADCON1bits.PCFG3 = 1;
    ADCON1bits.PCFG2 = 1;
    ADCON1bits.PCFG1 = 1;
    ADCON1bits.PCFG0 = 0;

    // Set PIN A0 as analog input
    TRISAbits.TRISA0 = 1;

    //Set interrupt control 
    // PIE1bits.ADIE = 0;  //disable ADC interrupt
    // PIR1bits.ADIF = 0;  //ADC has not completed or has not been started
    
    // Zero ADRESL and ADRESH
    ADRESL = 0;
    ADRESH = 0;

}

// void ReadBatVoltage() {
//     initADC();
   
//     ADCON0bits.CHS0 = 0; ADCON0bits.CHS1 = 0; ADCON0bits.CHS2 = 0;
    
//     //Turn ADC on
//     ADCON0bits.ADON = 1;
//     __delay_us(100);
//     ADCON0bits.GO = 1;
//     while (ADCON0bits.GO_nDONE);
//     UBYTE adcH = 0x00;
//     UBYTE adcL = 0x00;
//     adcH = ADRESH; adcL = ADRESL;
//     return;
// }

// void SendBatVoltage() {
//     initADC();
   
//     ADCON0bits.CHS0 = 0; ADCON0bits.CHS1 = 0; ADCON0bits.CHS2 = 0;
    
//     //Turn ADC on
//     ADCON0bits.ADON = 1;
//     __delay_us(100);
//     ADCON0bits.GO = 1;
//     while (ADCON0bits.GO_nDONE);
//     UBYTE adcH = 0x00;
//     UBYTE adcL = 0x00;
//     adcH = ADRESH; adcL = ADRESL;
        
//     putChar('A');
//     putChar('N');
//     putChar(adcH); putChar(adcL);
//     return;
// }

void ReadBatVoltageWithPointer(UBYTE *bat_voltage){
    initADC();
   
    ADCON0bits.CHS0 = 0; ADCON0bits.CHS1 = 0; ADCON0bits.CHS2 = 0;
    
    //Turn ADC on
    ADCON0bits.ADON = 1;
    __delay_us(100);
    ADCON0bits.GO = 1;
    while (ADCON0bits.GO_nDONE);
    bat_voltage[0] = ADRESH; bat_voltage[1] = ADRESL;
}