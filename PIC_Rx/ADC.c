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
#include "EEPROM.h"
#include "pinDefine.h"
#include "MPU.h"
#include "FMCW.h"
#include "I2C.h"
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

//void ReadBatVoltage() {
//    initADC();
//
//    ADCON0bits.CHS0 = 0; ADCON0bits.CHS1 = 0; ADCON0bits.CHS2 = 0;
//
//    //Turn ADC on
//    ADCON0bits.ADON = 1;
//    __delay_us(100);
//    ADCON0bits.GO = 1;
//    while (ADCON0bits.GO_nDONE);
//    UBYTE adcH = 0x00;
//    UBYTE adcL = 0x00;
//    adcH = ADRESH; adcL = ADRESL;
//    return;
//}

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

void MeasureBatVoltageAnChangeSatMode(){
          //------battery voltage measure-------------
//        treadhold is not determined
//        sampling rate is not determined
//        debug : 6v -> send 's' 7v -> send 'n'
            putChar('F');
            UBYTE bat_voltage[2];
            UWORD Voltage;
            ReadBatVoltageWithPointer(bat_voltage);
            Voltage = (UWORD)bat_voltage[0] << 8 | (UWORD)bat_voltage[1];      
            putChar('X');
            putChar(bat_voltage[0]);
            putChar(bat_voltage[1]);
            putChar('X');
            UWORD BatVol_nominal_saving_high = (UWORD)ReadEEPROM(MAIN_EEPROM_ADDRESS, BatVol_nominal_saving_datahigh_addresshigh, BatVol_nominal_saving_datahigh_addressLow);
            UWORD BatVol_nominal_saving_low = (UWORD)ReadEEPROM(MAIN_EEPROM_ADDRESS, BatVol_nominal_saving_datalow_addresshigh, BatVol_nominal_saving_datalow_addressLow);
            UBYTE BeforeSatMode = ReadEEPROM(MAIN_EEPROM_ADDRESS,SatelliteMode_addressHigh,SatelliteMode_addressLow);
//            putChar('Y');
//            putChar(BeforeSatMode);
//            putChar('Y');
            BeforeSatMode = BeforeSatMode & 0xF0;

//            UWORD test_vol = BatVol_nominal_saving_high <<8 | BatVol_nominal_saving_low;
//            putChar('Y');
//            putChar((UBYTE)(Voltage>>8));
//            putChar((UBYTE)Voltage);
//            putChar((UBYTE)(test_vol>>8));
//            putChar((UBYTE)test_vol);
//            putChar('Y');
//            if(Voltage >= test_vol){
//               putChar('O'); 
//            }else{
//                putChar('N'); 
//            }
            
            switch(BeforeSatMode){
                case 0x50://nominal mode
                    putChar('A');
                    if(Voltage >= (BatVol_nominal_saving_high << 8 | BatVol_nominal_saving_low)) {// >=7.5V
                        putChar('1');
                        //write SatMode nominal(SEP -> ON, RBF -> ON)
                        WriteOneByteToEEPROM(MAIN_EEPROM_ADDRESS, SatelliteMode_addressHigh, SatelliteMode_addressLow, 0x5A);
                        //obc check
                        switch(OBC_STATUS){
                            case OBC_ALIVE:                               
                                break;
                            case OBC_DIED:
                                killEPS();
                                onEPS();
                                FMTX(FMTX_Nref, FMTX_Nprg);
                                CWTX(CWTX_Nref, CWTX_Nprg);
                                FMRX(FMRX_Nref, FMRX_Nprg);
                                break;
                            default:    
                                break;
                        }
                        //EPS ON
                        onEPS();
                    }else if(Voltage <= ((UWORD)BatVol_saving_survival_high<<8 | (UWORD)BatVol_saving_survival_Low)){// <=6.114V
                        putChar('2');
                        //write SatMode survival(SEP -> OFF, RBF -> ON)
                        WriteOneByteToEEPROM(MAIN_EEPROM_ADDRESS, SatelliteMode_addressHigh, SatelliteMode_addressLow, 0xA6);
                        //EPS OFF
                        killEPS();
                    }else{
                        putChar('3');
                        //Write SatMode saving(SEP -> OFF, RBF -> ON)
                        WriteOneByteToEEPROM(MAIN_EEPROM_ADDRESS, SatelliteMode_addressHigh, SatelliteMode_addressLow, 0x66);
                        //EPS OFF
                        killEPS();
                        //Turn on NTRX(from CIB)
                        onNtrxPowerSupplyCIB(0,0);                   
                    }
                    break;
                case 0x60://saving mode
                    putChar('B');
                    if(Voltage >= ((UWORD)BatVol_OBCrevival_high <<8 |(UWORD)BatVol_OBCrevival_Low)){// >= 7.8V
                        putChar('1');
                        //write SatMode nominal(SEP -> ON, RBF -> ON)
                        WriteOneByteToEEPROM(MAIN_EEPROM_ADDRESS, SatelliteMode_addressHigh, SatelliteMode_addressLow, 0x5A);
                        //turn off NTRX(CIB power supply)
                        offNtrxPowerSupplyCIB();
                        //EPS ON
                        onEPS();
                        FMTX(FMTX_Nref, FMTX_Nprg);
                        CWTX(CWTX_Nref, CWTX_Nprg);
                        FMRX(FMRX_Nref, FMRX_Nprg);
                    }else if (Voltage <= ((UWORD)BatVol_saving_survival_high << 8 | (UWORD)BatVol_saving_survival_Low)){// <=6.114V
                        putChar('2');
                        //write SatMode survival(SEP -> OFF, RBF -> ON)
                        WriteOneByteToEEPROM(MAIN_EEPROM_ADDRESS, SatelliteMode_addressHigh, SatelliteMode_addressLow, 0xA6);
                        //turn off NTRX(CIB power supply)
                        offNtrxPowerSupplyCIB();
                    }else{
                        putChar('3');
                        //Write SatMode saving(SEP -> OFF, RBF -> ON)
                        WriteOneByteToEEPROM(MAIN_EEPROM_ADDRESS, SatelliteMode_addressHigh, SatelliteMode_addressLow, 0x66);
                    }
                    break;
                case 0xA0://survival mode
                    putChar('C');
                    if(Voltage >= ((UWORD)BatVol_OBCrevival_high <<8 | (UWORD)BatVol_OBCrevival_Low)){// >= 7.8V
                       putChar('1');
                        //write SatMode nominal(SEP -> ON, RBF -> ON)
                        WriteOneByteToEEPROM(MAIN_EEPROM_ADDRESS, SatelliteMode_addressHigh, SatelliteMode_addressLow, 0x5A);
                        //EPS ON
                        onEPS();
                        FMTX(FMTX_Nref, FMTX_Nprg);
                        CWTX(CWTX_Nref, CWTX_Nprg);
                        FMRX(FMRX_Nref, FMRX_Nprg);
                    }else if (Voltage <= ((UWORD)BatVol_saving_survival_high << 8 | (UWORD)BatVol_saving_survival_Low)){// <=6.114V
                        putChar('2');
                        //write SatMode survival(SEP -> OFF, RBF -> ON)
                        WriteOneByteToEEPROM(MAIN_EEPROM_ADDRESS, SatelliteMode_addressHigh, SatelliteMode_addressLow, 0xA6);
                    }else{
                        putChar('3');
                        //Write SatMode saving(SEP -> OFF, RBF -> ON)
                        WriteOneByteToEEPROM(MAIN_EEPROM_ADDRESS, SatelliteMode_addressHigh, SatelliteMode_addressLow, 0x66);
                        //Turn on NTRX(from CIB)
                        onNtrxPowerSupplyCIB(0,0);
                    }                  
                    break;
                default:
                    //error
                    putChar('D');
                    break;      
            putChar('E');
        }
}