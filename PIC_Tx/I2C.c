#include <xc.h>
#include "UART.h"
#include "I2C.h"
#include "Type_define.h"


#define _XTAL_FREQ 10000000
//UBYTE EEPROMData[16];
//UINT EEPROMDataLength;

void Init_I2C_M(const UDWORD c){
  SSPCON = 0b00101000;
  SSPCON2 = 0;
  SSPADD = (_XTAL_FREQ/(4*c))-1;
  SSPSTAT = 0;
}

void I2C_Master_Wait(){
  while ((SSPSTAT & 0x04) || (SSPCON2 & 0x1F));
}

void I2C_Master_Start(){
  I2C_Master_Wait();
  SEN = 1;
}

void I2C_Master_RepeatedStart(){
  I2C_Master_Wait();
  RSEN = 1;
}

void I2C_Master_Stop(){
  I2C_Master_Wait();
  PEN = 1;
}

void I2C_Master_Write(unsigned d){
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

void EEPROM_Write(UBYTE EEPROM_address,UBYTE high_address,UBYTE low_address,UBYTE *data,UINT *DataSize){
    UBYTE Address = EEPROM_address << 1;
    //for (*DataSize = 0; data[*DataSize]!= 0x00; *DataSize++);
   
    
    //UINT Datasize = sizeof(data);
    /**/
    I2C_Master_Start();         //Start condition
    I2C_Master_Write(Address);     //7 bit address + Write
    I2C_Master_Write(high_address);    //Adress High Byte
    I2C_Master_Write(low_address);    //Adress Low Byte
    for(UINT i=0;i<=*DataSize;i++){
        I2C_Master_Write(data[i]);    //Data
    }
    for(UINT j=*DataSize+1;j<=63;j++){
        I2C_Master_Write(0x00);    //Data
    }
    I2C_Master_Stop();          //Stop condition
    __delay_ms(200);
}

void EEPROM_Read_b(UBYTE EEPROM_address,UBYTE high_address,UBYTE low_address,UBYTE *ReadData, UINT EEPROMDataLength){
    UBYTE Address = EEPROM_address << 1;
    UBYTE ReadAddress = Address | 0x01;
    
    
    //UINT Datasize = sizeof(data);
    I2C_Master_Start();         //Start condition
    I2C_Master_Write(Address);     //7 bit address + Write
    I2C_Master_Write(high_address);    //Adress High Byte
    I2C_Master_Write(low_address);    //Adress Low Byte
    I2C_Master_RepeatedStart();         //Restart condition
    
    
    I2C_Master_Write(ReadAddress);     //7 bit address + Read
    
//    for (*EEPROMDataLength = 0; ReadData[*EEPROMDataLength]!= I2Cnull; *EEPROMDataLength++);
    
    //for (*EEPROMDataLength = 0; ReadData[*EEPROMDataLength]!= I2Cnull; *EEPROMDataLength++){
    for(UINT i = 0; i < EEPROMDataLength - 1; i++){
    /*
    //for(UINT i = 0; i < 5; i++){*/    
        
        ReadData[i] = I2C_Master_Read(1); //Read + Acknowledge
        //putch(ReadData[i]);
    }
    ReadData[EEPROMDataLength - 1] = I2C_Master_Read(0);
    //ReadData[0] = I2C_Master_Read(0);
    I2C_Master_Stop();          //Stop condition
    /*
    for(UINT j = 0; j < *EEPROMDataLength; j++){
    //putch(ReadData[0]);
    //for(UINT j = 0; j < 5; j++){
        putch(ReadData[j]);
    }
    putcrlf();*/
    //return EEPROMData;
    __delay_ms(200);
}

void EEPROM_Read(UBYTE EEPROM_address,UBYTE high_address,UBYTE low_address,UBYTE *ReadData, UINT *EEPROMDataLength){
    UBYTE Address = EEPROM_address << 1;
    UBYTE ReadAddress = Address | 0x01;
    
    
    //UINT Datasize = sizeof(data);
    I2C_Master_Start();         //Start condition
    I2C_Master_Write(Address);     //7 bit address + Write
    I2C_Master_Write(high_address);    //Adress High Byte
    I2C_Master_Write(low_address);    //Adress Low Byte
    I2C_Master_RepeatedStart();         //Restart condition
    
    
    I2C_Master_Write(ReadAddress);     //7 bit address + Read
    
    for (*EEPROMDataLength = 0; ReadData[*EEPROMDataLength]!= I2Cnull; *EEPROMDataLength++);
    
    //for (*EEPROMDataLength = 0; ReadData[*EEPROMDataLength]!= I2Cnull; *EEPROMDataLength++){
    for(UINT i = 0; i < *EEPROMDataLength; i++){
    /*
    //for(UINT i = 0; i < 5; i++){*/    
        
        ReadData[i] = I2C_Master_Read(1); //Read + Acknowledge
        //putch(ReadData[i]);
    }
    ReadData[*EEPROMDataLength] = I2C_Master_Read(0);
    //ReadData[0] = I2C_Master_Read(0);
    I2C_Master_Stop();          //Stop condition
    /*
    for(UINT j = 0; j < *EEPROMDataLength; j++){
    //putch(ReadData[0]);
    //for(UINT j = 0; j < 5; j++){
        putch(ReadData[j]);
    }
    putcrlf();*/
    //return EEPROMData;
    __delay_ms(200);
}