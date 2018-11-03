#include <xc.h>
#include "Type_define.h"
//#include "CommonDefine.h"
#include "ICM20601.h"
//#include "I2Clib.h"
#include "EEPROM.h"
#include "I2C.h"
#include "time.h"
//#include "init.h"
//#include "CAN.h"

const UBYTE ICM_ADDR            = 0x68;
const UBYTE ICM_DATA_START      = 0x3B;
const UBYTE ICM_DATA_STOP       = 0x48;
const UBYTE ICM_WHO_AM_I        = 0x75;
const UBYTE ICM_WHO_VALUE       = 0xAC;
//const UBYTE SMPLRT_DIV      = 0x19;
const UBYTE ICM_CONFIG          = 0x1A;
const UBYTE ICM_GYRO_CONFIG     = 0x1B;
const UBYTE ICM_ACCEL_CONFIG    = 0x1C;
const UBYTE ICM_ACCEL_CONFIG2   = 0x1D;
const UBYTE ICM_FIFO_EN         = 0x23;
const UBYTE ICM_INT_PIN_CONFIG  = 0x37;
const UBYTE ICM_INT_ENABLE      = 0x38;
const UBYTE ICM_INT_STATUS      = 0x3A; 
const UBYTE ICM_PWR_MGMT_1      = 0x6B;
const UBYTE ICM_PWR_MGMT_2      = 0x6C;


UBYTE readICMAddr(UBYTE);
void writeICMAddr(UBYTE , UBYTE);

UBYTE readICMAddr(UBYTE address)
{
    UBYTE Address = ICM_ADDR << 1;
    UBYTE ReadAddress = (UBYTE)(Address | 0x01);
    UBYTE ans;
    I2CMasterStart();                   //Start condition
    I2CMasterWrite(ICM_ADDR);           //7 bit address + Write
    I2CMasterWrite(address);            //7 bit address + Write
    I2CMasterRepeatedStart();           //Restart condition
    I2CMasterWrite(ReadAddress);        //7 bit address + Read
    ans = I2CMasterRead(0);
    I2CMasterStop();                    //Stop condition
    return ans;
}

void writeICMAddr(UBYTE address , UBYTE data)
{
    UBYTE Address = ICM_ADDR << 1;
    I2CMasterStart();               //Start condition
    I2CMasterWrite(address);        //7 bit address + Write
    I2CMasterWrite(data);      //Data
    I2CMasterStop();                //Stop condition
}

int initICM()
{
    int ans;
    ans = readICMAddr(ICM_WHO_AM_I);
    if(ans == ICM_WHO_VALUE){
            __delay_us(2000);
            writeICMAddr(ICM_PWR_MGMT_1,0x01);      //clock : PLL
            //  TODO    : need to consider about DLPF settings
            writeICMAddr(ICM_INT_ENABLE,0x01);      //interupt enable
            writeICMAddr(ICM_CONFIG,0x00);          //FSYNC:disabled
            //  XXX     : change the range of gyro sensor to ±4000dps (degree per sec)
            writeICMAddr(ICM_GYRO_CONFIG,0x18);     //FS:4000 deg/sec
            //  TODO    : need to confirm the range of accelemometer
            writeICMAddr(ICM_ACCEL_CONFIG,0x00);    //FS:4g
            //  TODO    : need to consider about DLPF setting
            writeICMAddr(ICM_ACCEL_CONFIG2,0x00);   //averaging 4 samples, DLPF : 218Hz
            __delay_us(2000);
    }else ans == -1;
    return ans;
}

int readICM(UBYTE *data, UINT offset)
{
    UBYTE Address = ICM_ADDR << 1;
    UBYTE ReadAddress = (UBYTE)(Address | 0x01);
    int ans , ack ;
    UBYTE dataNumber;
    dataNumber = ICM_DATA_STOP - ICM_DATA_STOP +1;

    
    while(ans != 0x01){
        ans = readICMAddr(ICM_INT_STATUS);
        ans = ans & 0x01;
    }
    
    I2CMasterStart();                   //Start condition
    I2CMasterWrite(Address);           //7 bit address + Write
    I2CMasterWrite(ICM_DATA_START);            //7 bit address + Write
    I2CMasterRepeatedStart();           //Restart condition
    I2CMasterWrite(ReadAddress);        //7 bit address + Read
    for (UINT i = 0; i < dataNumber-1; i++){
        data[offset+i] = I2CMasterRead(1);
    }
    data[offset + dataNumber-1] = I2CMasterRead(0);
    I2CMasterStop();                //Stop condition
}
