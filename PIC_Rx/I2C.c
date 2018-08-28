#include <xc.h>
#include "UART.h"
#include "Type_define.h"

#define _XTAL_FREQ 10000000
//UBYTE EEPROMData[32];

void Init_I2C_M(const UDWORD c){
  SSPCON = 0b00101000;
  SSPCON2 = 0;
  SSPADD = (_XTAL_FREQ/(4*c))-1;
  SSPSTAT = 0;
}

void I2C_Master_Wait(){
  while ((SSPSTAT & 0x04) || (SSPCON2 & 0x1F));
}

void I2CMasterStart(){
  I2C_Master_Wait();
  SEN = 1;
}

void I2C_Master_RepeatedStart(){
  I2C_Master_Wait();
  RSEN = 1;
}

void I2CMasterStop(){
  I2C_Master_Wait();
  PEN = 1;
}

void I2CMasterWrite(unsigned d){
  I2C_Master_Wait();
  SSPBUF = d;
}

UBYTE I2C_Master_Read(UBYTE a){
  unsigned char temp;
  I2C_Master_Wait();
  RCEN = 1;
  I2C_Master_Wait();
  temp = SSPBUF;
  I2C_Master_Wait();
  ACKDT = (a)?0:1;
  ACKEN = 1;
  return temp;
}

void EEPROM_Write(UBYTE EEPROM_address,UBYTE high_address,UBYTE low_address,UBYTE *data){
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