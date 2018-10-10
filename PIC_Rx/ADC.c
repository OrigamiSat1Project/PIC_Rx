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
//    ANSEL = 0x00;
//    ANSELH = 0x00;
    
    //----------------------
    // Run at 8 MHz //if this value is changed, ADC clock frequency must be adjusted accordingly
    //Clock determined by FOSC in configuration bits
//    SCS = 0;
    //Frequency select bits
//    IRCF0 = 1;
//    IRCF1 = 1;
//    IRCF2 = 1;
    //-----------------------
    

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
UWORD adc_read(){
    //Turn ADC on
    ADCON0bits.ADON = 1;
    //Sample Channel
    __delay_us(100);
    ADCON0bits.GO = 1;
    while (ADCON0bits.GO_nDONE);
    //give out ADC result
    UWORD value;
    value = (ADRESH<<8) | ADRESL;
    return(value);              
}
// void adc_led_Test(UBYTE voltage){
//     if(voltage<1000){//Blink two times
//         for(UBYTE i=0;i<2;i++){
//             PORTBbits.RB1 = 1;
//             __delay_ms(300);
//             PORTBbits.RB1 = 0;
//             __delay_ms(300);
//         }
//     } 
//     else if(voltage<3300){//Blink three times
//         for(UBYTE i=0;i<3;i++){
//             PORTBbits.RB1 = 1;
//             __delay_ms(200);
//             PORTBbits.RB1 = 0;
//             __delay_ms(200);
//         }
//     }
//     else {
//         PORTBbits.RB1 = 1;
//         __delay_ms(1000);
//     }
// }
/*******************************************************************************
* Function: Main
*
* Returns: Nothing
*
 * Description: Program entry point
******************************************************************************/
void ADC() {
    
    putChar(0x01);
    initADC();
    UBYTE chanel = 1;
    UWORD adcValue[];
   
    ADCON0bits.CHS = 0b00;
    adcValue[0] = adc_read();
        
        //Further instructions on what should be done with result
//        float adcVoltage_1;
//        float adcVoltage_2;
//        float adcVoltage_3;
//        float adcVoltage_4;
        
    //    adcVoltage = adcValue;
//        adcVoltage_1 = ((float)adcValue_1*3300) / 1024 ;
//        adcVoltage_2 = ((float)adcValue_2*3300) / 1024 ;
//        adcVoltage_3 = ((float)adcValue_3*3300) / 1024 ;
//        adcVoltage_4 = ((float)adcValue_4*3300) / 1024 ;
        
    //    printf("%f %f %f %f \r\n",adcVoltage_1,adcVoltage_2,adcVoltage_3,adcVoltage_4);
    //    printf("%d %d %d %d \r\n",adcValue_1,adcValue_2,adcValue_3,adcValue_4);
        
        for (UBYTE i=0; i<chanel; i++){
            putChar('A');
            putChar('N');
            putChar((UBYTE)(adcValue[i] >> 8));
            putChar((UBYTE)(adcValue[i] & 0xff));
        }
        //Test functionality of ADC
    //    adc_led_Test(adcVoltage);

        //Update every second
        __delay_ms(1000);
        //Set clears if necessary

return;
}



