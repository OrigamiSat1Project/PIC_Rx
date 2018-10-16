
#include "UART.h"
#include "I2C.h"
#include "Type_define.h"
#include "EEPROM.h"
#include "OkError.h"

#define _XTAL_FREQ 10000000
//UBYTE EEPROMData[16];
//UINT EEPROMDataLength;


/*******************************************************************************
*setting
******************************************************************************/
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

void I2CMasterWrite(UBYTE d){
  I2CMasterWait();
  SSPBUF = d;
}

UBYTE I2CMasterRead(UBYTE a){
  UBYTE temp;
  
  I2CMasterWait();
  RCEN = 1;
  I2CMasterWait();
  temp = SSPBUF;
  
  I2CMasterWait();
  ACKDT = (a)?0:1;
  ACKEN = 1;
  return temp;
}

/*******************************************************************************
*Method for EEPROM Write 
******************************************************************************/
void WriteToEEPROM(UBYTE addressEEPROM,UBYTE addressHigh,UBYTE addressLow,UBYTE *data){
    UBYTE address;
    address= addressEEPROM << 1;
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
/**/
void WriteOneByteToEEPROM(UBYTE addressEEPROM,UBYTE addressHigh,UBYTE addressLow,UBYTE data){
    UBYTE address;
    address= addressEEPROM << 1;
    //UINT Datasize = sizeof(data);
    I2CMasterStart();               //Start condition
    I2CMasterWrite(address);        //7 bit address + Write
    I2CMasterWrite(addressHigh);    //Adress High Byte
    I2CMasterWrite(addressLow);     //Adress Low Byte
    I2CMasterWrite(data);      //Data
    I2CMasterStop();                //Stop condition
    __delay_ms(200);
}

void WriteCheckByteToEEPROMs(UBYTE B0Select,UBYTE addressHigh,UBYTE addressLow,UBYTE data){
    UBYTE mainAddress;
    UBYTE subAddress;
    mainAddress = EEPROM_address | B0Select;
    subAddress = EEPROM_subaddress | B0Select;
    WriteOneByteToEEPROM(mainAddress,addressHigh,addressLow,data);
    WriteOneByteToEEPROM(subAddress,addressHigh,addressLow,data);
}

void WriteLastCommandIdToEEPROM(UBYTE last_command_ID){
    WriteCheckByteToEEPROMs(B0select_for_TXCOBCLastCommandID, HighAddress_for_TXCOBCLastCommandID, LowAddress_for_TXCOBCLastCommandID, last_command_ID);
}


/*******************************************************************************
*Method for EEPROM Read
******************************************************************************/
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

/*
 *  Read Data From EEPROM (the size od read data is only 1byte)
 *	arg      :   EEPROM_address, high_address, low_address
 *	return   :   EEPROM_address, high_address, low_address -> ReadData
 *	TODO     :   debug  ---> finish
 *	FIXME    :   not yet
 *	XXX      :   not yet
 */
UBYTE ReadEEPROM(UBYTE Address7Bytes, UBYTE high_address, UBYTE low_address){
    UBYTE Address = Address7Bytes << 1;
    UBYTE ReadAddress = Address | 0x01;
    UBYTE ReadData;
   
    I2CMasterStart();         //Start condition
    I2CMasterWrite(Address);     //7 bit address + Write
    I2CMasterWrite(high_address);    //Adress High Byte
    I2CMasterWrite(low_address);    //Adress Low Byte
    I2CMasterRepeatedStart();         //Restart condition
    
    I2CMasterWrite(ReadAddress);     //7 bit address + Read
    
    ReadData = I2CMasterRead(0); //Read + Acknowledge
    
    I2CMasterStop();          //Stop condition
    return ReadData;
    __delay_ms(200);  
}

//TODO:need debug
void ReadDataAndDataSizeFromEEPROM(UBYTE Address7Bytes,UBYTE high_address,UBYTE low_address,UBYTE *ReadData, UINT *EEPROMDataLength){
    UBYTE Address = Address7Bytes << 1;
    UBYTE ReadAddress = Address | 0x01;
    I2CMasterStart();                       //Start condition
    I2CMasterWrite(Address);                //7 bit address + Write
    I2CMasterWrite(high_address);           //Adress High Byte
    I2CMasterWrite(low_address);            //Adress Low Byte
    I2CMasterRepeatedStart();               //Restart condition
    
    I2CMasterWrite(ReadAddress);            //7 bit address + Read
    for (*EEPROMDataLength = 0; ReadData[*EEPROMDataLength]!= I2Cnull; *EEPROMDataLength++);
    for(UINT i = 0; i < *EEPROMDataLength; i++){
        ReadData[i] = I2CMasterRead(1);     //Read + Acknowledge
    }
    ReadData[*EEPROMDataLength] = I2CMasterRead(0);
    I2CMasterStop();          //Stop condition
    
    //for denbugging
    /*
    for(UINT j = 0; j < *EEPROMDataLength; j++){
    //putch(ReadData[0]);
    //for(UINT j = 0; j < 5; j++){
        putch(ReadData[j]);
    }
    putcrlf();*/
    __delay_ms(200);
}

//process command data if the command type is 'I2C'
void commandSwitchI2C(UBYTE command, UBYTE slaveAdress, UBYTE *dataHigh, UBYTE *dataLow){ 
    switch(command){    
        case 'w': //I2C write
//            I2CMasterWrite(slaveAdress);//TODO: check if method 'I2C write' is correct
//            I2CMasterWrite(dataHigh);
//            I2CMasterWrite(dataLow);
            break;
        case 'r': //I2C read
//            dataHigh = I2CMasterRead(slaveAdress);//TODO: check if method 'I2C read' is correct, especially do the pointers work as intended
//            dataLow = I2CMasterRead(slaveAdress); //TODO: what should happen with read in data?
            //TODO: write data to EEPROM
            //TODO: send Address where it is written to TXCOBC
            break;
        case 't': //I2C test
            //TODO: write method for I2C test
            //TODO: write test data to EEPROM
            //TODO: read EEPRON
            //TODO: send EEPROM address to TXCOBC
            break;
        case 'c': //I2C buffer clear
            //TODO: write method for I2C buffer clear 
            //TODO: clear: SSPBUF
            break;
        case 'b': //change I2C baud rate
            //TODO: write method for change I2C baud rate
            break;
        case 's': //set as a slave ic
            //TODO: write method for set as a slave ic
            break;
        case 'i': //measure IMU
            //TODO: write method for measure IMU
            break;
        default:
            switchError(error_I2C_commandSwitchI2C);
            break;
    }
}