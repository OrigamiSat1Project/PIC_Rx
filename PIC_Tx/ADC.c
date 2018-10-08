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

/* 
* ch1 AN2  BAT temperature
* ch2 AN3  5VBUS voltage(EPS 5V)
* ch3 AN4  3V3BUS voltage(EPS 3.3V) 
* ch4 AN10 5V voltage
* 
*/

/*******************************************************************************
*Includes and defines
******************************************************************************/
//#include "PIC16F886_Internal.h"
#include "Type_define.h"
#include "UART.h"
#include "time.h"
#include "ADC.h"
#include "EEPROM.h"
#include "I2C.h"
#include <xc.h>
#include <stdio.h>

/*******************************************************************************
*method
******************************************************************************/
void initADC(void);
UWORD adc_read();
void setAddressEEPROM(void);

UWORD adcValue[CHANEL_SIZE];
UBYTE adcValue_dataHigh_addressHigh[CHANEL_SIZE];
UBYTE adcValue_dataHigh_addressLow[CHANEL_SIZE];
UBYTE adcValue_dataLow_addressHigh[CHANEL_SIZE];
UBYTE adcValue_dataLow_addressLow[CHANEL_SIZE];

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
    ANSEL = 0x00;
    ANSELH = 0x00;
    
    //----------------------
    // Run at 8 MHz //if this value is changed, ADC clock frequency must be adjusted accordingly
    //Clock determined by FOSC in configuration bits
    SCS = 0;
    //Frequency select bits
//    IRCF0 = 1;
//    IRCF1 = 1;
//    IRCF2 = 1;
    //-----------------------
    
    // Set PIN B2 as output
//    TRISBbits.TRISB1 = 0;
    
    
    // Set PIN A2 as analog input
    TRISAbits.TRISA2 = 1;
    ANSELbits.ANS2 = 1;
    // Set PIN A3 as analog input
    TRISAbits.TRISA3 = 1;
    ANSELbits.ANS3 = 1;
    // Set PIN A5 as analog input
    TRISAbits.TRISA5 = 1;
    ANSELbits.ANS4 = 1;
    // Set PIN B1 as analog input
    TRISBbits.TRISB1 = 1;
    ANSELHbits.ANS10 = 1;
    // Set channel select to AN3
//    ADCON0bits.CHS = 0b0011;
//    ADCON0bits.CHS3 = 0;
//    ADCON0bits.CHS2 = 0;
//    ADCON0bits.CHS1 = 1;
//    ADCON0bits.CHS0 = 1;
    // Set ADC reference Voltage
    ADCON1bits.VCFG1 = 0;   //Vref- = Vss
    ADCON1bits.VCFG0 = 0;   //Vref+ = Vdd
    // Set ADC conversion clock source, conversion time is 4us
    ADCON0bits.ADCS1 = 1;
    ADCON0bits.ADCS1 = 0;
    //Set interrupt control /// ? Is this really needed???
    PIE1bits.ADIE = 0;  //disable ADC interrupt
    PIR1bits.ADIF = 0;  //ADC has not completed or has not been started 
    // Set result formatting to right justified
    ADCON1bits.ADFM = 1;
    
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
void adc_led_Test(UBYTE voltage){
    if(voltage<1000){//Blink two times
        for(UBYTE i=0;i<2;i++){
            PORTBbits.RB1 = 1;
            __delay_ms(300);
            PORTBbits.RB1 = 0;
            __delay_ms(300);
        }
    } 
    else if(voltage<3300){//Blink three times
        for(UBYTE i=0;i<3;i++){
            PORTBbits.RB1 = 1;
            __delay_ms(200);
            PORTBbits.RB1 = 0;
            __delay_ms(200);
        }
    }
    else {
        PORTBbits.RB1 = 1;
        __delay_ms(1000);
    }
}

void setAddressEEPROM(){
    
    //set EEPROM address
    adcValue_dataHigh_addressHigh[0] = adcValue_CH1_DATAHIGH_addressHigh;
    adcValue_dataHigh_addressLow[0]  = adcValue_CH1_DATAHIGH_addressLow;
    adcValue_dataLow_addressHigh[0] = adcValue_CH1_DATALOW_addressHigh;
    adcValue_dataLow_addressLow[0]  = adcValue_CH1_DATALOW_addressLow;
    adcValue_dataHigh_addressHigh[1] = adcValue_CH2_DATAHIGH_addressHigh;
    adcValue_dataHigh_addressLow[1]  = adcValue_CH2_DATAHIGH_addressLow;
    adcValue_dataLow_addressHigh[1] = adcValue_CH2_DATALOW_addressHigh;
    adcValue_dataLow_addressLow[1]  = adcValue_CH2_DATALOW_addressLow;
    adcValue_dataHigh_addressHigh[2] = adcValue_CH3_DATAHIGH_addressHigh;
    adcValue_dataHigh_addressLow[2]  = adcValue_CH3_DATAHIGH_addressLow;
    adcValue_dataLow_addressHigh[2] = adcValue_CH3_DATALOW_addressHigh;
    adcValue_dataLow_addressLow[2]  = adcValue_CH3_DATALOW_addressLow;
    adcValue_dataHigh_addressHigh[3] = adcValue_CH4_DATAHIGH_addressHigh;
    adcValue_dataHigh_addressLow[3]  = adcValue_CH4_DATAHIGH_addressLow;
    adcValue_dataLow_addressHigh[3] = adcValue_CH4_DATALOW_addressHigh;
    adcValue_dataLow_addressLow[3]  = adcValue_CH4_DATALOW_addressLow; 
    
}
/*******************************************************************************
* Function: Main
*
* Returns: Nothing
*
 * Description: Program entry point
******************************************************************************/
void measureAllChanelADC(){
    
    initADC();    
    setAddressEEPROM();
        
    //Set LED off
    PORTBbits.RB1 = 0;
    ADCON0bits.CHS = 0b0010;
    adcValue[0] = adc_read();
    ADCON0bits.CHS = 0b0011;
    adcValue[1] = adc_read();
    ADCON0bits.CHS = 0b0100;
    adcValue[2] = adc_read();
    ADCON0bits.CHS = 0b1010;
    adcValue[3] = adc_read();
    
    //write data to main and sub EEPROM
    for (UBYTE i=0; i<CHANEL_SIZE; i++){
        //data High
        WriteOneByteToEEPROM(EEPROM_address, adcValue_dataHigh_addressHigh[i], adcValue_dataHigh_addressLow[i], (UBYTE)(adcValue[i] >> 8));
        WriteOneByteToEEPROM(EEPROM_subaddress, adcValue_dataHigh_addressHigh[i], adcValue_dataHigh_addressLow[i], (UBYTE)(adcValue[i] >> 8));
        //data Low
        WriteOneByteToEEPROM(EEPROM_address, adcValue_dataLow_addressHigh[i], adcValue_dataLow_addressLow[i], (UBYTE)(adcValue[i] & 0xff));
        WriteOneByteToEEPROM(EEPROM_subaddress, adcValue_dataLow_addressHigh[i], adcValue_dataLow_addressLow[i], (UBYTE)(adcValue[i] & 0xff));
    }
    
    /*----------------------------------------------*/
    //FIXME:[start]debug for check the adcValue--->success 
//    for (UBYTE i=0; i<CHANEL_SIZE; i++){
//        putChar(i);
//        putChar(i);
//        putChar(i);
//        putChar((UBYTE)(adcValue[i] >> 8));
//        putChar((UBYTE)(adcValue[i] & 0xff));
//    }
    //FIXME:[finish]debug for check the adcValue 
    /*------------------------------------------------*

    /*-------------------------------------------------*/
    //FIXME:[start]debug for write/read adc value--->success
//    for (UBYTE i=0; i<4; i++){    
//        WriteOneByteToEEPROM(EEPROM_address, adcValue_addressHigh, adcValue_addressLow, (UBYTE)(adcValue[i] >> 8));
//        putChar(ReadEEPROM(EEPROM_address, adcValue_addressHigh, adcValue_addressLow));
//        WriteOneByteToEEPROM(EEPROM_address, adcValue_addressHigh, adcValue_addressLow, (UBYTE)(adcValue[i] & 0xff));
//        putChar(ReadEEPROM(EEPROM_address, adcValue_addressHigh, adcValue_addressLow));
//    }
    //FIXME:[finish]debug for write/read adc value
    /*--------------------------------------------------*/
    
    /*--------------------------------------------------*/
    //FIXME:[start]debug for change address--->success
//    putChar(adcValue_addressLow);
//    UBYTE address;
//    address = adcValue_addressLow+0x08;
//    putChar(address);
    //FIXME:[finish]debug for change address
    /*--------------------------------------------------*/
    
    /*--------------------------------------------------*/
    //FIXME:[start]debug for write adc value--->successs
    //write data to main and sub EEPROM
//    for (UBYTE i=0; i<CHANEL_SIZE; i++){
//        putChar(adcValue_dataHigh_addressHigh[i]);
//        putChar(adcValue_dataHigh_addressLow[i]);
//        putChar(adcValue_dataLow_addressHigh[i]);
//        putChar(adcValue_dataLow_addressLow[i]);
//        //data High
//        WriteOneByteToEEPROM(EEPROM_address, adcValue_dataHigh_addressHigh[i], adcValue_dataHigh_addressLow[i], (UBYTE)(adcValue[i] >> 8));
//        WriteOneByteToEEPROM(EEPROM_subaddress, adcValue_dataHigh_addressHigh[i], adcValue_dataHigh_addressLow[i], (UBYTE)(adcValue[i] >> 8));
//        //data Low
//        WriteOneByteToEEPROM(EEPROM_address, adcValue_dataLow_addressHigh[i], adcValue_dataLow_addressLow[i], (UBYTE)(adcValue[i] & 0xff));
//        WriteOneByteToEEPROM(EEPROM_subaddress, adcValue_dataLow_addressHigh[i], adcValue_dataLow_addressLow[i], (UBYTE)(adcValue[i] & 0xff));
//        //data High
//        putChar(ReadEEPROM(EEPROM_address, adcValue_dataHigh_addressHigh[i], adcValue_dataHigh_addressLow[i]));
//        putChar(ReadEEPROM(EEPROM_subaddress, adcValue_dataHigh_addressHigh[i], adcValue_dataHigh_addressLow[i]));
//        //data Low
//        putChar(ReadEEPROM(EEPROM_address, adcValue_dataLow_addressHigh[i], adcValue_dataLow_addressLow[i]));
//        putChar(ReadEEPROM(EEPROM_subaddress, adcValue_dataLow_addressHigh[i], adcValue_dataLow_addressLow[i]));
//    }
    //FIXME:[finish]debug for write adc value
    /*--------------------------------------------------*/
    
    //Update every second
    // __delay_ms(1000);
    //Set clears if necessary

return;
}

/**
 * measure 1Chanel (DC-DC temperature)
 * 1. select chanel
 * 2. read ADC data (**the size of data is 2byte)
 * 3. write to main and sub EEPROM 
 * arg     : slaveaddress, high_address, low_address
 * return  : ---
 * TODO    : debug 
 * 
*/
void measureDcDcTemperature(UBYTE slaveaddress, UBYTE high_address, UBYTE low_address) {
    
    initADC();    
        
    //Set LED off
    PORTBbits.RB1 = 0;
    ADCON0bits.CHS = 0b0010;
    adcValue[0] = adc_read();
    
    /*write data to EEPROM*/
    //data high
    WriteOneByteToEEPROM(EEPROM_address, high_address, low_address, (UBYTE)(adcValue[0] >> 8));     
    WriteOneByteToEEPROM(EEPROM_subaddress, high_address, low_address, (UBYTE)(adcValue[0] >> 8));     
    
    high_address = high_address + 0x08;
    low_address = low_address + 0x08;

    //data low
    WriteOneByteToEEPROM(EEPROM_address, high_address, low_address, (UBYTE)(adcValue[0] & 0xff));   
    WriteOneByteToEEPROM(EEPROM_subaddress, high_address, low_address, (UBYTE)(adcValue[0] & 0xff));  
    
    /*--------------------------------------------------*/
    //FIXME:[start]debug for write adc value--->successs
    //write data to EEPROM
//    putChar((UBYTE)(adcValue[0] >> 8));
//    putChar((UBYTE)(adcValue[0] & 0xff));
//    
//    WriteOneByteToEEPROM(slaveaddress, high_address, low_address, (UBYTE)(adcValue[0] >> 8));  //data high
//    putChar(ReadEEPROM(slaveaddress, high_address, low_address));
//    
//    high_address = high_address + 0x08;
//    low_address = low_address + 0x08;
//    
//    putChar(high_address);
//    putChar(low_address);
//
//    WriteOneByteToEEPROM(slaveaddress, high_address, low_address, (UBYTE)(adcValue[0] & 0xff)); //data low
//    putChar(ReadEEPROM(slaveaddress, high_address, low_address));
    //FIXME:[finish]debug for write adc value
    /*--------------------------------------------------*/
}

/**
 * measure 1 channle and downlinl the data 
 * 1. select chanel
 * 2. read ADC data (**the size of data is 2byte)
 * 3. write to EEPROM
 * 4. downlink the data 
 * arg     : slaveaddress, high_address, low_address
 * return  : ---
 * TODO    : debug 
 * 
*/
void measureValueAndDownlink(UBYTE channel_select, UBYTE slaveaddress, UBYTE high_address, UBYTE low_address) {
    measureAllChanelADC();
    //TODO:add data downlink
}

//process command data if the command type is 'HKdata'
void commandSwitchHKdata(UBYTE type_sellect, UBYTE data1, UBYTE data2, UBYTE data3){ 
   switch(type_sellect){    
        case 'd': //measure DC-DC temperature
            measureDcDcTemperature(data1, data2, data3);
            break;
        case '5': //5VBUS 
            break;
        case '3': //3VBUS
            break;
        case 'C': //5V CIB
            break;
        case 'u': //update all HK data
            measureAllChanelADC();
            break;
        default:
            //TODO: error message
            break;
   }
}