#include <xc.h>
#include "Type_define.h"
//#include "CommonDefine.h"
#include "I2C.h"
#include "EEPROM.h"
//#include "init.h"
//#include "CAN.h"
#include "adxl345.h"
#include "ITG3701.h"
#include "ICM20601.h"
#include "time.h"
#include "IMU.h"


void readIMUsequence_ICM(UBYTE ee_p, UBYTE EEPROMH, UBYTE EEPROML, int measuring_time){
    
   
   UBYTE zeros[10] = {};
   UBYTE IMUdata[16] = {};
   UBYTE IMUdatas[64] = {};//change array size  TODO: check array size
   UBYTE delay_time = ee_p && 0x3F;
   UBYTE selEEP = (ee_p && 0xC0) >> 6;
   
   //syncWithOBC(OBCClock);
   //sendCanData(&globalClock);
   
//   WriteToEEPROM(selEEP, EEPROMH, EEPROML, &globalClock,6);
   __delay_us(5000);
   EEPROML += 0x06;
//   writeEEPROM(selEEP, EEPROMH, EEPROML, zeros,10);
   __delay_us(5000);
   EEPROML += 0x0A;
   
   sampling_counter[0] = 0;
   sampling_counter[1] = 0;
   int constant_timer_counter = 0;
    while(constant_timer_counter <= measuring_time){
        
        while(EEPROML != 0x00){
            sampling_counter[0] += 1;
            IMUdata[0] = sampling_counter[1];
            IMUdata[1] = sampling_counter[0];
            readICM(IMUdata,2);
            __delay_us(20);

//            writeEEPROM(selEEP, EEPROMH, EEPROML, IMUdata, 16);
            __delay_us(3000);
            if(EEPROML == 0xF0){
                EEPROMH = EEPROMH + 0x01;
                EEPROML = 0x00;
            }else{
                EEPROML += 0x10;
            }
            if(EEPROMH >= 0xF0){
                selEEP += 1;
                EEPROML = 0;
                EEPROMH = 0;
                if(selEEP == 4){
                    break;  //EEPROM full
                }
            }
//           wait1ms(delay_time);
        }
        
        if(sampling_counter[0] == 0xFF){
            sampling_counter[0] = 0x00;
            sampling_counter[1] += 0x01;
        }
        for(int k=0;k<2;k++){
            for(int i=0;i<8;i++){
                if(sampling_counter[0] == 0xFF){
                    sampling_counter[0] = 0x00;
                    sampling_counter[1] += 0x01;
                }else{
                    sampling_counter[0] += 1;
                }
                IMUdatas[16*i] = sampling_counter[1];
                IMUdatas[1+16*i] = sampling_counter[0];
                readICM(IMUdatas,2+16*i);
                __delay_us(20);
//                wait1ms(delay_time);
            }
        
//            writeEEPROM(selEEP, EEPROMH, EEPROML, IMUdatas, 128);
            __delay_us(3000);
            if(EEPROML == 0x80){
                EEPROMH += 0x01;
                EEPROML = 0x00;
            }else{
                EEPROML = 0x80;
            }
            if(EEPROMH >= 0xF0){
                selEEP += 1;
                EEPROML = 0;
                EEPROMH = 0;
                if(selEEP == 4){
                    break;  //EEPROM full
                }
            }
        }
        
    }
   // ending 
   __delay_us(3000);
//   writeEEPROM(selEEP, EEPROMH, EEPROML, &globalClock,6);
   __delay_us(5000);
   EEPROML += 0x06;
//   writeEEPROM(selEEP, EEPROMH, EEPROML, zeros,10);
   __delay_us(5000);
   EEPROML += 0x0A;

    return;
}
//
//void readIMUsequence_adxl_ITG(UBYTE ee_p, UBYTE EEPROMH, UBYTE EEPROML, int measuring_time){
//    
//   
//   UBYTE zeros[10] = {};
//   UBYTE IMUdata[16] = {};
//   UBYTE IMUdatas[128] = {};
//   UBYTE delay_time = ee_p && 0x3F;
//   UBYTE selEEP = (ee_p && 0xC0) >> 6;
//   
//   //syncWithOBC(OBCClock);
//   //sendCanData(&globalClock);
//   
//   writeEEPROM(selEEP, EEPROMH, EEPROML, &globalClock,6);
//   __delay_us(5000);
//   EEPROML += 0x06;
//   writeEEPROM(selEEP, EEPROMH, EEPROML, zeros,10);
//   __delay_us(5000);
//   EEPROML += 0x0A;
//   
//   sampling_counter[0] = 0;
//   sampling_counter[1] = 0;
//   constant_timer_counter = 0;
//    while(constant_timer_counter <= measuring_time){
//        
//        while(EEPROML != 0x00){
//            sampling_counter[0] += 1;
//            IMUdata[0] = sampling_counter[1];
//            IMUdata[1] = sampling_counter[0];
//            readADXL(IMUdata,2);
//            __delay_us(20);
//            readITG(IMUdata,8);
//            __delay_us(20);
//
//            writeEEPROM(selEEP, EEPROMH, EEPROML, IMUdata, 16);
//            __delay_us(3000);
//            if(EEPROML == 0xF0){
//                EEPROMH = EEPROMH + 0x01;
//                EEPROML = 0x00;
//            }else{
//                EEPROML += 0x10;
//            }
//            if(EEPROMH >= 0xF0){
//                selEEP += 1;
//                EEPROML = 0;
//                EEPROMH = 0;
//                if(selEEP == 4){
//                    break;  //EEPROM full
//                }
//            }
//            wait1ms(delay_time);
//        }
//        
//        if(sampling_counter[0] == 0xFF){
//            sampling_counter[0] = 0x00;
//            sampling_counter[1] += 0x01;
//        }
//        for(int k=0;k<2;k++){
//            for(int i=0;i<8;i++){
//                if(sampling_counter[0] == 0xFF){
//                    sampling_counter[0] = 0x00;
//                    sampling_counter[1] += 0x01;
//                }else{
//                    sampling_counter[0] += 1;
//                }
//                IMUdatas[16*i] = sampling_counter[1];
//                IMUdatas[1+16*i] = sampling_counter[0];
//                readADXL(IMUdatas,2+16*i);
//                __delay_us(20);
//                readITG(IMUdatas,2+8+16*i);
//                __delay_us(20);
//                wait1ms(delay_time);
//            }
//        
//            writeEEPROM(selEEP, EEPROMH, EEPROML, IMUdatas, 128);
//            __delay_us(3000);
//            if(EEPROML == 0x80){
//                EEPROMH += 0x01;
//                EEPROML = 0x00;
//            }else{
//                EEPROML = 0x80;
//            }
//            if(EEPROMH >= 0xF0){
//                selEEP += 1;
//                EEPROML = 0;
//                EEPROMH = 0;
//                if(selEEP == 4){
//                    break;  //EEPROM full
//                }
//            }
//        }
//        
//    }
//   // ending 
//   __delay_us(3000);
//   writeEEPROM(selEEP, EEPROMH, EEPROML, &globalClock,6);
//   __delay_us(5000);
//   EEPROML += 0x06;
//   writeEEPROM(selEEP, EEPROMH, EEPROML, zeros,10);
//   __delay_us(5000);
//   EEPROML += 0x0A;
//
//    return;
//}
