#include <xc.h>
#include "Type_define.h"
#include "ITG3701.h"
//#include "I2Clib.h"
#include "EEPROM.h"
//#include "init.h"
//#include "CAN.h"

const UBYTE ITG_ADDR            = 0x69;
const UBYTE ITG_WHO_AM_I        = 0x75;
const UBYTE ITG_WHO_VALUE       = 0x68;
const UBYTE ITG_SMPLRT_DIV      = 0x19;
const UBYTE ITG_CONFIG          = 0x1A;
const UBYTE ITG_GYRO_CONFIG     = 0x1B;
const UBYTE ITG_FIFO_EN         = 0x23;
const UBYTE ITG_INT_PIN_CFG     = 0x37;
const UBYTE ITG_INT_ENABLE      = 0x38;
const UBYTE ITG_INT_STATUS      = 0x3A;
const UBYTE ITG_DATA            = 0x41;
const UBYTE ITG_PWR_MGMT_1      = 0x6B;
const UBYTE ITG_PWR_MGMT_2      = 0x6C;

//int readITGAddr(char address)
//{
//    int ans;
//    ans = startI2C(ITG_ADDR,RW_0);
//    if(ans == 0){
//        sendI2CData(address);
//        restartI2C(ITG_ADDR,RW_1);
//        ans = readI2CData(NOACK);
//    }else ans = -1;
//    stopI2C();
//    return ans;
//}

//int writeITGAddr(char address, char val)
//{
//    int ans;
//    ans = startI2C(ITG_ADDR,RW_0);
//    if(ans == 0){
//        sendI2CData(address);
//        sendI2CData(val);
//    }else ans = -1;
//    stopI2C();
//    return ans;
//}

//int initITG()
//{
//    int ans;
//    __delay_us(2000);
//    ans = readITGAddr(ITG_WHO_AM_I);
//    
//    if(ans == ITG_WHO_VALUE){
//        writeITGAddr(ITG_PWR_MGMT_1,0x01);     //oscirator : PLL
//        writeITGAddr(ITG_PWR_MGMT_2,0x00);     //gyro : not standby mode
//        writeITGAddr(ITG_FIFO_EN,0x00);        //FIFO disabled
//        writeITGAddr(ITG_CONFIG,0x00);         //FIFO disabled , DLPF 250Hz
//        writeITGAddr(ITG_GYRO_CONFIG,0x18);    //FS:4000deg/sec
//        //writeITGAddr(ITG_INT_PIN_CFG,0x00);    //FSYNC pin : activ low
//        writeITGAddr(ITG_INT_ENABLE,0x01);     //DARA Ready Interupt : Enable
//        __delay_us(2000);
//    }else ans = -1;
//    return ans;
//}
//
//int readITG(UBYTE *data, int offset)
//{
//    int ans , i , ack ;
//
//    while(ans != 0x01){
//        ans = readITGAddr(ITG_INT_STATUS);
//        ans = ans & 0x01;               //Data Ready Interupt generated -> break
//    }
//
//    ans = startI2C(ITG_ADDR,RW_0);
//    if(ans == 0){
//        sendI2CData(ITG_DATA);
//        restartI2C(ITG_ADDR,RW_1);
//        ack = ACK;
//        for(i=0;i<8;i++){
//            if(i == 7) ack = NOACK;
//            data[offset+i] = readI2CData(ack);
//        }
//    }else ans = -1;
//    stopI2C();
//    return ans;
//}