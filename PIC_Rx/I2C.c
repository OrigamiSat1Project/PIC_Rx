#include <xc.h>
#include "UART.h"
#include "Type_define.h"

#define _XTAL_FREQ 10000000
//UBYTE EEPROMData[32];

void InitI2CMaster(const UDWORD c){
  SSPCON = 0b00101000;
  SSPCON2 = 0;
  SSPADD = (_XTAL_FREQ/(4*c))-1;
  SSPSTAT = 0;
}

void I2CMasterWait(){
  while ((SSPSTAT & 0x04) || (SSPCON2 & 0x1F));
}

void I2CMasterStart(){
  I2CMasterWait();
  SEN = 1;
}

void I2CMasterRepeatedStart(){
  I2CMasterWait();
  RSEN = 1;
}

void I2CMasterStop(){
  I2CMasterWait();
  PEN = 1;
}

void I2CMasterWrite(unsigned d){
  I2CMasterWait();
  SSPBUF = d;
}

UBYTE I2CMasterRead(UBYTE a){
  unsigned char temp;
  I2CMasterWait();
  RCEN = 1;
  I2CMasterWait();
  temp = SSPBUF;
  I2CMasterWait();
  ACKDT = (a)?0:1;
  ACKEN = 1;
  return temp;
}

void WriteToEEPROM(UBYTE EEPROM_address,UBYTE high_address,UBYTE low_address,UBYTE *data){
    UBYTE Address = EEPROM_address << 1;
    //UINT Datasize = sizeof(data);
    /**/
    I2CMasterStart();         //Start condition
    I2CMasterWrite(Address);     //7 bit address + Write
    I2CMasterWrite(high_address);    //Adress High Byte
    I2CMasterWrite(low_address);    //Adress Low Byte
    while(*data){
        I2CMasterWrite(*data);    //Data
        ++data;
    }
    I2CMasterStop();          //Stop condition
    __delay_ms(300);
}

void WriteToEEPROMWithDataSize(UBYTE addressEEPROM,UBYTE addressHigh,UBYTE addressLow,UBYTE *data, UBYTE dataSize){
    UBYTE address = addressEEPROM << 1;
    //UINT Datasize = sizeof(data);
    /**/
    I2CMasterStart();               //Start condition
    I2CMasterWrite(address);        //7 bit address + Write
    I2CMasterWrite(addressHigh);    //Adress High Byte
    I2CMasterWrite(addressLow);     //Adress Low Byte
    for (UINT i = 0; i< dataSize; i++){
        I2CMasterWrite(*data);      //Data
        ++data;
    }
    I2CMasterStop();                //Stop condition
    __delay_ms(300);
}
//UBYTE *EEPROM_Read(UBYTE EEPROM_address,UBYTE high_address,UBYTE low_address, UINT DataSize){
//    UBYTE Address = EEPROM_address << 1;
//    UBYTE ReadAddress = Address | 0x01;
//    
//    
//    //UINT Datasize = sizeof(data);
//    I2C_Master_Start();         //Start condition
//    I2C_Master_Write(Address);     //7 bit address + Write
//    I2C_Master_Write(high_address);    //Adress High Byte
//    I2C_Master_Write(low_address);    //Adress Low Byte
//    I2C_Master_RepeatedStart();         //Restart condition
//    
//    
//    I2C_Master_Write(ReadAddress);     //7 bit address + Read
//    for(UINT i = 0; i < DataSize-1; i++){
//        EEPROMData[i] = I2C_Master_Read(1); //Read + Acknowledge
//    }
//    EEPROMData[DataSize-1] = I2C_Master_Read(0);
//    I2C_Master_Stop();          //Stop condition
//    
//    for(UINT j = 0; j < DataSize; j++){
//        putch(EEPROMData[j]);
//    }
//    return EEPROMData;
//    __delay_ms(200);
//}


void ReadDataFromEEPROM(UBYTE Address7Bytes,UBYTE high_address,UBYTE low_address,UBYTE *ReadData, UINT EEPROMDataLength){
    UBYTE Address = Address7Bytes << 1;
    UBYTE ReadAddress = Address | 0x01;
    I2CMasterStart();                       //Start condition
    I2CMasterWrite(Address);                //7 bit address + Write
    I2CMasterWrite(high_address);           //Adress High Byte
    I2CMasterWrite(low_address);            //Adress Low Byte
    I2CMasterRepeatedStart();               //Restart condition
    
    I2CMasterWrite(ReadAddress);            //7 bit address + Read
    for(UINT i = 0; i < EEPROMDataLength - 1; i++){
        ReadData[i] = I2CMasterRead(1);     //Read + Acknowledge
    }
    ReadData[EEPROMDataLength - 1] = I2CMasterRead(0);
    I2CMasterStop();                        //Stop condition
    
    //for debugging
    /*
    for(UINT j = 0; j < *EEPROMDataLength; j++){
    //putch(ReadData[0]);
    //for(UINT j = 0; j < 5; j++){
        putch(ReadData[j]);
    }
    putcrlf();*/
    __delay_ms(200);
}