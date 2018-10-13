#include <xc.h>
#include "typeDefine.h"
#include "time.h"
#include "EEPROM.h"
#include "I2C.h"
#include "OkError.h"

//UBYTE EEPROMData[32];                                         

UBYTE data_length;
UBYTE *read_data;

/*******************************************************************************
*setting
******************************************************************************/
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

/*******************************************************************************
*Method for EEPROM Write 
******************************************************************************/
/*
 *  Write To EEPROM
 *	arg      :   addressEEPROM, addressHigh, addressLow, *data
 *	return   :   EEPROM_address, high_address, low_address -> *data
 *	TODO     :   debug --->finish
 *	FIXME    :   not yet
 *	XXX      :   not yet
 */
void WriteToEEPROM(UBYTE addressEEPROM,UBYTE addressHigh,UBYTE addressLow,UBYTE *data){
    UBYTE address = addressEEPROM << 1;
 
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

/*
 *  Write To EEPROM
 *	arg      :   addressEEPROM, addressHigh, addressLow, *data, dataSize
 *	return   :   EEPROM_address, high_address, low_address, dataSize -> *data
 *	TODO     :   debug --->finish
 *	FIXME    :   not yet
 *	XXX      :   not yet
 */
void WriteToEEPROMWithDataSize(UBYTE addressEEPROM,UBYTE addressHigh,UBYTE addressLow,UBYTE *data, UBYTE dataSize){
    UBYTE address = addressEEPROM << 1;

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
    mainAddress = MAIN_EEPROM_ADDRESS | B0Select;
    subAddress = SUB_EEPROM_ADDRESS | B0Select;
    WriteOneByteToEEPROM(mainAddress,addressHigh,addressLow,data);
    WriteOneByteToEEPROM(subAddress,addressHigh,addressLow,data);
}

void WriteLastCommandIdToEEPROM(UBYTE last_command_ID){
    WriteCheckByteToEEPROMs(B0select_for_RXCOBCLastCommand, HighAddress_for_RXCOBCLastCommand, LowAddress_for_RXCOBCLastCommand, last_command_ID);
}

/*******************************************************************************
*Method for EEPROM Read
******************************************************************************/
/*
 *  Read Data From EEPROM (the size od read data is only 1byte)
 *	arg      :   EEPROM_address, high_address, low_address
 *	return   :   EEPROM_address, high_address, low_address -> ReadData
 *	TODO     :   debug  ---> finish
 *	FIXME    :   not yet
 *	XXX      :   not yet
 */
UBYTE ReadEEPROM(UBYTE EEPROM_address,UBYTE high_address,UBYTE low_address){
    UBYTE Address = EEPROM_address << 1;
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

/*
 *  Read Data From EEPROM With DataSize
 *	arg      :   EEPROM_address, high_address, low_address, *ReadData, EEPROMDataLength
 *	return   :   EEPROM_address, high_address, low_address, EEPROMDataLength -> *ReadData
 *	TODO     :   debug ---> finish
 *	FIXME    :   not yet
 *	XXX      :   not yet
 */
void ReadDataFromEEPROMWithDataSize(UBYTE EEPROM_address,UBYTE high_address,UBYTE low_address,UBYTE *ReadData, UINT EEPROMDataLength){
    UBYTE Address = EEPROM_address << 1;
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
    
    __delay_ms(200);
}

/*******************************************************************************
*Method for various function
******************************************************************************/

//TODO:check
void I2CBufferClear(void){
   SSPBUF = 0;   //Serial Receive/Transmit Buffer Register
}

/*
 *  Change I2C Baud Rate
 *	arg      :   I2C_baud_rate_type (0x00/0x01)
 *	return   :   0x00->high-speed mode (400 kHz) / 0x01->standard speed mode (100 kHz)
 *	TODO     :   debug ---> finish
 *	FIXME    :   not yet
 *	XXX      :   not yet
 *  default 100000bps / datasheet p81-p82
 */
void ChangeI2CBaudRate( UBYTE I2C_baud_rate_type ){
    switch (I2C_baud_rate_type){
        case 0x00:     //high-speed mode (400 kHz)
            SMP = 0;  //Slew Rate Control bit
            SSPADD = (_XTAL_FREQ/(4*I2C_baud_rate_high))-1; //MSSP Address Register: the lower seven bits of SSPADD act as the baud rate generator reload value
            break;
        case 0x01:     //standard speed mode (100 kHz)
            SMP = 1;  //Slew Rate Control bit
            SSPADD = (_XTAL_FREQ/(4*I2C_baud_rate_low))-1; //MSSP Address Register: the lower seven bits of SSPADD act as the baud rate generator reload value
            break;
    }
}

/* metthod for test EEPROM to read and write
 * 1. write test data to EEPROM
 * 2. read EEPROM
 * 3. send EEPROM address to TXCOBC
 */
void TestEEPROM(UBYTE slaveaddress){
        UBYTE commandData[datalength_for_test];
        UBYTE ControlByte;                       //control byte of EEPROM
        UBYTE ReadData[datalength_for_test];
        
        commandData[0] = 0x01;
        commandData[1] = 0x02;
        commandData[2] = 0x03;
        commandData[3] = 0x04;

        ControlByte = slaveaddress | B0select_for_testEEPROM;
        
        for (UBYTE i=0; i<datalength_for_test; i++){
            WriteToEEPROM(ControlByte, HighAddress_for_testEEPROM, LowAddress_for_testEEPROM, commandData[i]);
        }
        
        ReadDataFromEEPROMWithDataSize(ControlByte, HighAddress_for_testEEPROM, LowAddress_for_testEEPROM, ReadData, datalength_for_test);
        
        //TODO:where send to read data
}

/*******************************************************************************
*process command data if the command type is 'I2C'
******************************************************************************/
void commandSwitchI2C(UBYTE command, UBYTE slaveAdress, UBYTE dataHigh, UBYTE dataLow, UBYTE data){ 
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
            //TODO: write method for I2C test (OBC and TXCOBC)
            //TODO: write test data to EEPROM
            //TODO: read EEPROM 
            //TODO: send EEPROM address to TXCOBC
            break;
        case 'c': //I2C buffer clear
            //TODO: write method for I2C buffer clear---finish?
            //TODO: clear: SSPBUF---finish?
            I2CBufferClear();
            break;
        case 'b': //change I2C baud rate
            //TODO: write method for change I2C baud rate---finish
            ChangeI2CBaudRate( slaveAdress );
            break;
        default:
            switchError(error_I2C_commandSwitchI2C);
            break;
    }
}

/*******************************************************************************
*process command data if the command type is 'EEPROM'
******************************************************************************/
void commandSwitchEEPROM(UBYTE command, UBYTE slaveAdress, UBYTE dataHigh, UBYTE dataLow, UBYTE data1, UBYTE data2){ 
    switch(command){    
        case 'w': //write data to EEPROM
            //TODO:now send data is only 1byte. change data size
            WriteToEEPROM(slaveAdress, dataHigh, dataLow, data1);  //data1 is the data to send
            break;
        case 'r': //read data from EEPROM
            data_length = data1;
            ReadDataFromEEPROMWithDataSize(slaveAdress, dataHigh, dataLow, read_data, data1);
            //TODO: send data to TXCOBC or/and OBC by I2C or UART
            break;
        case 'e': //read any size of data from EEPROM 
            /* this function for read any size of data from EEPROM
             * 1.read data from EEPROM
             * 2.get data size
             * 3.read data from EEPROM (RX pic gets data size at step2)
             * 4.get any size of data 
             */
            data_length = ReadEEPROM(slaveAdress, dataHigh, dataLow);
            ReadDataFromEEPROMWithDataSize(slaveAdress, dataHigh, dataLow, read_data, data_length);
            //TODO: send data to TXCOBC or/and OBC by I2C or UART
            // sendCommand('t', 'f', 0xaa, UBYTE Parameter2, UBYTE Parameter3, UBYTE Parameter4, UBYTE Parameter5, UBYTE Parameter6);
        case 't': //EEPROM test
            TestEEPROM(slaveAdress);
            break;
        default:
            switchError(error_I2C_commandSwitchEEPROM);
            break;
    }
}