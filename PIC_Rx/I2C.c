#include <xc.h>
#include "typeDefine.h"
#include "time.h"
#include "EEPROM.h"
#include "I2C.h"

//UBYTE EEPROMData[32];                                         

//Global Data
UBYTE I2C_baud_rate = I2Cbps_def;

void InitI2CMaster(const UDWORD c){//Init Master Synchronous Serial Port(MSSP)
  SSPCON = 0b00101000;          //MSSP Control Register: Synchronous Serial Port Enabled;I2C Master mode, clock = FOSC/(4 * (SSPADD + 1))
  SSPCON2 = 0;                  //MSSP Control Register 2:
  SSPADD = (_XTAL_FREQ/(4*c))-1; //MSSP Address Register: the lower seven bits of SSPADD act as the baud rate generator reload value
  SSPSTAT = 0;                  //MSSP Status Register
}

void I2CMasterWait(){
  while ((SSPSTAT & 0x04) || (SSPCON2 & 0x1F));
}


void I2CMasterStart(){
  I2CMasterWait();
  SEN = 1;                      //SEN Start Condition Enable; bit 0 of SSPCON2
}

void I2CMasterRepeatedStart(){
  I2CMasterWait();
  RSEN = 1;                     //Repeated Start Condition Enabled bit (Master mode only); bit 1 of SSPCON2
}

void I2CMasterStop(){
  I2CMasterWait();
  PEN = 1;                      //Stop Condition Enable bit (Master mode only); bit 2 of SSPCON2
}

void I2CMasterWrite(unsigned dataByte){                               
  I2CMasterWait();
  SSPBUF = dataByte;                   //Serial Receive/Transmit Buffer Register
}


UBYTE I2CMasterRead(UBYTE address){                                         
  unsigned char temp;
  I2CMasterWait();
  RCEN = 1;                     //Receive Enable bit; bit 3 of SSPCON2
  I2CMasterWait();
  temp = SSPBUF;
  I2CMasterWait();
  ACKDT = (address)?0:1;              //Acknowledge Data bit (Master Receive mode only); bit 3 of SSPCON2 
  ACKEN = 1;                    //Acknowledge Sequence Enable bit (Master Receive mode only); bit 3 of SSPCON2 
  return temp;
}

void WriteToEEPROM(UBYTE addressEEPROM,UBYTE addressHigh,UBYTE addressLow,UBYTE *data){
    UBYTE address = addressEEPROM << 1;
    //UINT Datasize = sizeof(data);
    /**/
    I2CMasterStart();               //Start condition
    I2CMasterWrite(address);        //7 bit address + Write
    I2CMasterWrite(addressHigh);    //Adress High Byte
    I2CMasterWrite(addressLow);     //Adress Low Byte
    while(*data){
        I2CMasterWrite(*data);      //Data
        ++data;
    }
    I2CMasterStop();                //Stop condition
    __delay_ms(200);
}

//TODO:check
//TODO:任意の字数に対応
UBYTE ReadEEPROM(UBYTE EEPROM_address,UBYTE high_address,UBYTE low_address){
    UBYTE Address = EEPROM_address << 1;
    UBYTE ReadAddress = Address | 0x01;
    UBYTE ReadData;
   
    //UINT Datasize = sizeof(data);
    I2CMasterStart();         //Start condition
    I2CMasterWrite(Address);     //7 bit address + Write
    I2CMasterWrite(high_address);    //Adress High Byte
    I2CMasterWrite(low_address);    //Adress Low Byte
    I2CMasterRepeatedStart();         //Restart condition
    
    I2CMasterWrite(ReadAddress);     //7 bit address + Read
    
    ReadData = I2CMasterRead(0); //Read + Acknowledge
    
    I2CMasterStop();          //Stop condition
    return ReadData;
    __delay_ms(200);  //TODO:delay時間は大丈夫か？
}


void TestI2C(void){
    UBYTE I2C_test[4];
    I2C_test[0]='T';
    I2C_test[1]='E';
    I2C_test[2]='S';
    I2C_test[3]='T';
}

//TODO:check
void I2CBufferClear(void){
   SSPBUF = 0;   //Serial Receive/Transmit Buffer Register
}

//TODO:check
//TODO:2択でよいのか？
//change I2C baud rate
//default 400000bps
//h -> I2Cbps_high(400000bps) / l -> I2Cbps_low(100000bps)
void ChangeI2CBaudRate( UBYTE I2C_baud_rate_type ){
    switch (I2C_baud_rate_type){
        case 'h':
            I2C_baud_rate = I2Cbps_high;
            break;
        case 'l':
            I2C_baud_rate = I2Cbps_low;
            break;
    }
}

//process command data if the command type is 'I2C'
void commandSwitchI2C(UBYTE command, UBYTE slaveAdress, UBYTE *dataHigh, UBYTE *dataLow){ 
    switch(command){    
        case 'w': //I2C write
            I2CMasterWrite(slaveAdress);//TODO: check if method 'I2C write' is correct
            I2CMasterWrite(dataHigh);
            I2CMasterWrite(dataLow);
            break;
        case 'r': //I2C read
            dataHigh = I2CMasterRead(slaveAdress);//TODO: check if method 'I2C read' is correct, especially do the pointers work as intended
            dataLow = I2CMasterRead(slaveAdress); //TODO: what should happen with read in data?
            //TODO: write data to EEPROM
            //TODO: send Address where it is written to TXCOBC
            break;
        case 't': //I2C test
            //TODO: write method for I2C test
            //TODO: write test data to EEPROM
            //TODO: read EEPRON---finish
            //TODO: send EEPROM address to TXCOBC
            TestI2C();
            //TODO:コメントアウト外すとエラー発生
            //WriteToEEPROM(slaveAdress, dataHigh, dataLow, I2C_test);
            UBYTE EEPROMData;
            EEPROMData = ReadEEPROM(slaveAdress, dataHigh, dataLow);
            break;
        case 'c': //I2C buffer clear
            //TODO: write method for I2C buffer clear---finish?
            //TODO: clear: SSPBUF---finish?
            I2CBufferClear();
            break;
        case 'b': //change I2C baud rate
            //TODO: write method for change I2C baud rate
            ChangeI2CBaudRate( slaveAdress );
            break;
        default:
            //TODO: error message
            break;
    }
}