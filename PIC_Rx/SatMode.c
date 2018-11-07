#include <xc.h>
#include "typeDefine.h"
#include "pinDefine.h"
#include "time.h"
#include "EEPROM.h"
#include "I2C.h"
#include "MPU.h"
#include "UART.h"
#include "FMCW.h"
#include "ADC.h"
#include "SatMode.h"

UBYTE ReserveBeforeSatMode = SATMODE_NOMINAL;//spare BeforeSatMode (when can't read BeforeSatMode from EEPROM)

UBYTE MeasureBatVoltageAndChangeSatMode(){
          //------battery voltage measure-------------
//        debug : error handling is not determined
            putChar(0xA1);
            putChar(0xA1);
            putChar(0xA1);
            putChar(0xA1);
            putChar(0xA1);
            putChar(0xA1);
            putChar(0xA1);
            putChar(0xA1);
            putChar(0xA1);
            putChar(0xA1);
            putChar(0xA1);
            putChar(0xA1);
            putChar(0xA1);
            putChar(0xA1);
            putChar('\r');
            putChar('\n');
                    
            UBYTE bat_voltage[2];
            UWORD Voltage;//Voltage is 10 bit           
            UBYTE error_status = 0;
            
            
            ReadBatVoltageWithPointer(bat_voltage);
            Voltage = (UWORD)bat_voltage[0] << 8 | (UWORD)bat_voltage[1]; 
            
            //if BatVol_nominal_saving_high is very large,read one more time. Then it is still very large,thereshold BatVol is Initial Value.
            UWORD BatVol_nominal_saving_high = (UWORD)ReadEEPROM(MAIN_EEPROM_ADDRESS, BatVol_nominal_saving_datahigh_addresshigh, BatVol_nominal_saving_datahigh_addressLow);
            UWORD BatVol_nominal_saving_low = (UWORD)ReadEEPROM(MAIN_EEPROM_ADDRESS, BatVol_nominal_saving_datalow_addresshigh, BatVol_nominal_saving_datalow_addressLow);    
            if((BatVol_nominal_saving_high & 0b11111100) != 0){
                BatVol_nominal_saving_high = (UWORD)ReadEEPROM(MAIN_EEPROM_ADDRESS, BatVol_nominal_saving_datahigh_addresshigh, BatVol_nominal_saving_datahigh_addressLow);
                BatVol_nominal_saving_low = (UWORD)ReadEEPROM(MAIN_EEPROM_ADDRESS, BatVol_nominal_saving_datalow_addresshigh, BatVol_nominal_saving_datalow_addressLow);
                if((BatVol_nominal_saving_high & 0b11111100) != 0){
                    BatVol_nominal_saving_high = Init_BatVol_nominal_saving_high;
                    BatVol_nominal_saving_low = Init_BatVol_nominal_saving_low;
                    error_status += 3;//0b00000011
                }              
            }
            
            UBYTE BeforeSatMode = ReadEEPROM(MAIN_EEPROM_ADDRESS,SatelliteMode_addressHigh,SatelliteMode_addressLow);
            UBYTE ChoicedSatMode = BeforeSatMode;//For change BeforeSatMode/ReserveBeforeSatMode when EEPROM broken;
            BeforeSatMode = BeforeSatMode & 0xF0;
            ReserveBeforeSatMode = ReserveBeforeSatMode & 0xF0;
            
            putChar(0xB1);
            putChar(BeforeSatMode);
            putChar(ReserveBeforeSatMode);
            putChar('\r');
            putChar('\n');
            
            switch(BeforeSatMode){
                case SATMODE_NOMINAL:
                case SATMODE_SAVING:
                case SATMODE_SURVIVAL: 
                    ChoicedSatMode =  BeforeSatMode;
                    break;
                default:
                    BeforeSatMode = ReadEEPROM(MAIN_EEPROM_ADDRESS,SatelliteMode_addressHigh,SatelliteMode_addressLow);
                    BeforeSatMode = BeforeSatMode & 0xF0;
                    switch(BeforeSatMode){
                        case SATMODE_NOMINAL:
                        case SATMODE_SAVING:
                        case SATMODE_SURVIVAL: 
                            ChoicedSatMode =  BeforeSatMode;
                            break;
                        default:
                            ChoicedSatMode = ReserveBeforeSatMode;
                            error_status += 12;//0b00001100
                            break;                       
                    }
                    break;
            }
                            putChar(0xA2);
                            putChar(BeforeSatMode);
                            putChar(ReserveBeforeSatMode);
                            putChar(ChoicedSatMode);
                            putChar(0xA2);
                            putChar('\r');
                            putChar('\n');
            switch(ChoicedSatMode){
                case SATMODE_NOMINAL:
                    putChar(0x0A);
                    if(Voltage >= (BatVol_nominal_saving_high << 8 | BatVol_nominal_saving_low)) {// >=7.5V
                        putChar(0x01);
                        //write SatMode nominal(SEP -> ON, RBF -> ON)
                        WriteOneByteToEEPROM(MAIN_EEPROM_ADDRESS, SatelliteMode_addressHigh, SatelliteMode_addressLow, SATMODE_NOMINAL_SEPON_RBFON);
                        ReserveBeforeSatMode = SATMODE_NOMINAL_SEPON_RBFON;
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
                        putChar(0x02);
                        //write SatMode survival(SEP -> OFF, RBF -> ON)
                        WriteOneByteToEEPROM(MAIN_EEPROM_ADDRESS, SatelliteMode_addressHigh, SatelliteMode_addressLow, SATMODE_SURVIVAL_SEPOFF_RBFON);
                        ReserveBeforeSatMode = SATMODE_SURVIVAL_SEPOFF_RBFON;
                        //EPS OFF
                        killEPS();
                    }else{
                        putChar(0x03);
                        //Write SatMode saving(SEP -> OFF, RBF -> ON)
                        WriteOneByteToEEPROM(MAIN_EEPROM_ADDRESS, SatelliteMode_addressHigh, SatelliteMode_addressLow, SATMODE_SAVING_SEPOFF_RBFON);
                        ReserveBeforeSatMode = SATMODE_SAVING_SEPOFF_RBFON;
                        //EPS OFF
                        killEPS();
                        //Turn on NTRX(from CIB)
                        onNtrxPowerSupplyCIB(0,0);                   
                    }
                    break;
                case SATMODE_SAVING:
                    putChar(0x0B);
                    if(Voltage >= ((UWORD)BatVol_OBCrevival_high <<8 |(UWORD)BatVol_OBCrevival_Low)){// >= 7.8V
                        putChar(0x01);
                        //write SatMode nominal(SEP -> ON, RBF -> ON)
                        WriteOneByteToEEPROM(MAIN_EEPROM_ADDRESS, SatelliteMode_addressHigh, SatelliteMode_addressLow, SATMODE_NOMINAL_SEPON_RBFON);
                        ReserveBeforeSatMode = SATMODE_NOMINAL_SEPON_RBFON;
                        //turn off NTRX(CIB power supply)
                        offNtrxPowerSupplyCIB();
                        //EPS ON
                        onEPS();
                        FMTX(FMTX_Nref, FMTX_Nprg);
                        CWTX(CWTX_Nref, CWTX_Nprg);
                        FMRX(FMRX_Nref, FMRX_Nprg);
                    }else if (Voltage <= ((UWORD)BatVol_saving_survival_high << 8 | (UWORD)BatVol_saving_survival_Low)){// <=6.114V
                        putChar(0x02);
                        //write SatMode survival(SEP -> OFF, RBF -> ON)
                        WriteOneByteToEEPROM(MAIN_EEPROM_ADDRESS, SatelliteMode_addressHigh, SatelliteMode_addressLow, SATMODE_SURVIVAL_SEPOFF_RBFON);
                        ReserveBeforeSatMode = SATMODE_SURVIVAL_SEPOFF_RBFON;
                        //turn off NTRX(CIB power supply)
                        offNtrxPowerSupplyCIB();
                    }else{
                        putChar(0x03);
                        //Write SatMode saving(SEP -> OFF, RBF -> ON)
                        WriteOneByteToEEPROM(MAIN_EEPROM_ADDRESS, SatelliteMode_addressHigh, SatelliteMode_addressLow, SATMODE_SAVING_SEPOFF_RBFON);
                        ReserveBeforeSatMode = SATMODE_SAVING_SEPOFF_RBFON;
                    }
                    break;
                case SATMODE_SURVIVAL:
                    putChar(0x0c);
                    if(Voltage >= ((UWORD)BatVol_OBCrevival_high <<8 | (UWORD)BatVol_OBCrevival_Low)){// >= 7.8V
                       putChar(0x01);
                        //write SatMode nominal(SEP -> ON, RBF -> ON)
                        WriteOneByteToEEPROM(MAIN_EEPROM_ADDRESS, SatelliteMode_addressHigh, SatelliteMode_addressLow, SATMODE_NOMINAL_SEPON_RBFON);
                        ReserveBeforeSatMode = SATMODE_NOMINAL_SEPON_RBFON;
                        //EPS ON
                        onEPS();
                        FMTX(FMTX_Nref, FMTX_Nprg);
                        CWTX(CWTX_Nref, CWTX_Nprg);
                        FMRX(FMRX_Nref, FMRX_Nprg);
                    }else if (Voltage <= ((UWORD)BatVol_saving_survival_high << 8 | (UWORD)BatVol_saving_survival_Low)){// <=6.114V
                        putChar(0x02);
                        //write SatMode survival(SEP -> OFF, RBF -> ON)
                        WriteOneByteToEEPROM(MAIN_EEPROM_ADDRESS, SatelliteMode_addressHigh, SatelliteMode_addressLow, SATMODE_SURVIVAL_SEPOFF_RBFON);
                        ReserveBeforeSatMode = SATMODE_SURVIVAL_SEPOFF_RBFON;
                    }else{
                        putChar(0x03);
                        //Write SatMode saving(SEP -> OFF, RBF -> ON)
                        WriteOneByteToEEPROM(MAIN_EEPROM_ADDRESS, SatelliteMode_addressHigh, SatelliteMode_addressLow, SATMODE_SAVING_SEPOFF_RBFON);
                        ReserveBeforeSatMode = SATMODE_SAVING_SEPOFF_RBFON;
                        //Turn on NTRX(from CIB)
                        onNtrxPowerSupplyCIB(0,0);
                    }                  
                    break;
                default:       
                    putChar(0x0D);
                    error_status += 60;//0b00110000
                    break;
        }
            putChar('\r');
            putChar('\n');
            putChar(0xA3);
            putChar('\r');
            putChar('\n');
            putChar(0xA4);
            putChar(ReadEEPROM(MAIN_EEPROM_ADDRESS, SatelliteMode_addressHigh, SatelliteMode_addressLow));
            putChar('\r');
            putChar('\n');
            return error_status;
}
