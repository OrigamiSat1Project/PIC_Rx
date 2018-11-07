#include <xc.h>
#include "typeDefine.h"
#include "time.h"
#include "EEPROM.h"
#include "I2C.h"
#include "OkError.h"
#include "UART.h"

//UBYTE EEPROMData[32];                  

#define MELTING_FINISH_FLAG 0b11111111

int AckCheck;
int CollisionCheck;

/*******************************************************************************
*setting
******************************************************************************/
void InitI2CMaster(const UDWORD c){//Init Master Synchronous Serial Port(MSSP)
  SSPCON = 0b00101000;          //MSSP Control Register: Synchronous Serial Port Enabled;I2C Master mode, clock = FOSC/(4 * (SSPADD + 1))
  SSPCON2 = 0;                  //MSSP Control Register 2:
  SSPADD = (_XTAL_FREQ/(4*c))-1; //MSSP Address Register: the lower seven bits of SSPADD act as the baud rate generator reload value
  SSPSTAT = 0;                  //MSSP Status Register
  PIE1bits.SSPIE  = 1 ;               // enable MSSP interrupt
  PIE2bits.BCLIE  = 1 ;               // enable bus collision interrupt
  PIR1bits.SSPIF  = 0 ;               // clear MSSP interrupt flag
  PIR2bits.BCLIF  = 0 ;               // clear bus collision flag
}

void interruptI2C(void)
{
     if (PIR1bits.SSPIF == 1) {
          if (AckCheck == 1) {
              AckCheck = 0;
          }
          PIR1bits.SSPIF = 0;
     }
     if (PIR2bits.BCLIF == 1) {
          CollisionCheck = 1;
          PIR2bits.BCLIF = 0;
     }
}

void I2CMasterWait(char mask){
  while ((SSPSTAT & mask) || (SSPCON2 & 0x1F));
}


int I2CMasterStart(UBYTE slave_address,UBYTE rw){
//  I2CMasterWait();
//  SEN = 1;                      //SEN Start Condition Enable; bit 0 of SSPCON2
     CollisionCheck = 0 ;
     I2CMasterWait(0x5) ;
     SSPCON2bits.SEN = 1 ;
     I2CMasterWait(0x5) ;
     if (CollisionCheck == 1) return -1 ;
     AckCheck = 1 ;
     SSPBUF = (char)((slave_address<<1)+rw);
     while (AckCheck);
     if (CollisionCheck == 1) return -1 ;
     return SSPCON2bits.ACKSTAT;
}

int I2CMasterRepeatedStart(UBYTE slave_address,UBYTE rw){
//  I2CMasterWait();
//  RSEN = 1;                     //Repeated Start Condition Enabled bit (Master mode only); bit 1 of SSPCON2
     CollisionCheck = 0 ;
     I2CMasterWait(0x5) ;
     SSPCON2bits.RSEN = 1 ;
     I2CMasterWait(0x5) ;
     if (CollisionCheck == 1) return -1 ;
     AckCheck = 1;
     SSPBUF = (char)((slave_address<<1)+rw);
     while (AckCheck);
     if (CollisionCheck == 1) return -1;
     return SSPCON2bits.ACKSTAT;
}

int I2CMasterStop(void){
//  I2CMasterWait();
//  PEN = 1;                      //Stop Condition Enable bit (Master mode only); bit 2 of SSPCON2
     CollisionCheck = 0 ;
     I2CMasterWait(0x5) ;
     SSPCON2bits.PEN = 1 ;
     if (CollisionCheck == 1) return -1 ;
     else                     return  0 ;
}

int I2CMasterWrite(UBYTE dataByte){                               
//  I2CMasterWait();
//  SSPBUF = dataByte;                   //Serial Receive/Transmit Buffer Register
     CollisionCheck = 0 ;
     I2CMasterWait(0x5) ;
     if (CollisionCheck == 1) return -1;
     AckCheck = 1;
     SSPBUF = dataByte;
     while (AckCheck);
     if (CollisionCheck == 1) return -1;
     return SSPCON2bits.ACKSTAT;
}


int I2CMasterRead(UBYTE address){
     int data_from_slave ;

     CollisionCheck = 0 ;
     I2CMasterWait(0x5) ;
     SSPCON2bits.RCEN = 1;      //  enable receive from slave
     I2CMasterWait(0x4) ;
     if (CollisionCheck == 1) return -1 ;
     data_from_slave = SSPBUF;
     I2CMasterWait(0x5) ;
     if (CollisionCheck == 1) return -1 ;
     SSPCON2bits.ACKDT = address;
     SSPCON2bits.ACKEN = 1;
     return data_from_slave;
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
char WriteToEEPROM(UBYTE addressEEPROM,UBYTE addressHigh,UBYTE addressLow,UBYTE *data){
    char ans;
    
    ans = I2CMasterStart(addressEEPROM,0);
    if (ans == 0){
        I2CMasterWrite(addressHigh);
        I2CMasterWrite(addressLow);
        while(*data){
            I2CMasterWrite(*data);
            ++data;
        }
    } else return -1;
    ans = I2CMasterStop();
    if(ans != 0) return -1;
    return 0;
}

void WriteToMainAndSubB0EEPROM(UBYTE addressHigh,UBYTE addressLow,UBYTE *data){
    WriteToEEPROM(MAIN_EEPROM_ADDRESS,addressHigh,addressLow,data);
    WriteToEEPROM(SUB_EEPROM_ADDRESS,addressHigh,addressLow,data);
}


int WriteOneByteToEEPROM(UBYTE addressEEPROM,UBYTE addressHigh,UBYTE addressLow,UBYTE data){
    int ans = 0;
    ans = I2CMasterStart(addressEEPROM,0);               //Start condition
    if(ans != 0) return -1;
    ans = I2CMasterWrite(addressHigh);    //Adress High Byte
    if(ans != 0) return -1;
    ans = I2CMasterWrite(addressLow);     //Adress Low Byte
    if(ans != 0) return -1;
    ans = I2CMasterWrite(data);           //Data
    if(ans != 0) return -1;
    ans = I2CMasterStop();                //Stop condition
    if(ans != 0) return -1;
    __delay_ms(5);
    return 0;
}

void WriteOneByteToMainAndSubB0EEPROM(UBYTE addressHigh,UBYTE addressLow,UBYTE data){
    WriteOneByteToEEPROM(MAIN_EEPROM_ADDRESS,addressHigh,addressLow,data);
    WriteOneByteToEEPROM(SUB_EEPROM_ADDRESS,addressHigh,addressLow,data);
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
int ReadEEPROM(UBYTE address,UBYTE high_address,UBYTE low_address){
    UBYTE dat;
    int ans;
   
    ans = I2CMasterStart(address,0);         //Start condition
    if(ans != 0) return -1;
    ans = I2CMasterWrite(high_address);    //Adress High Byte
    if(ans != 0) return -1;
    ans = I2CMasterWrite(low_address);    //Adress Low Byte
    if(ans != 0) return -1;
    ans = I2CMasterRepeatedStart(address,1);         //Restart condition
    if(ans != 0) return -1;
    dat = I2CMasterRead(1); //Read + Acknowledge
    if(dat == -1) return -1;
    ans = I2CMasterStop();          //Stop condition
    if(ans != 0) return -1;
    __delay_ms(5);
    return dat;
}

UBYTE ReadEEPROMmainAndSub(UBYTE high_address,UBYTE low_address){
    UBYTE ReadData;
    ReadData = ReadEEPROM(MAIN_EEPROM_ADDRESS,high_address,low_address);
    if (ReadData==0xFF){
        ReadData = ReadEEPROM(SUB_EEPROM_ADDRESS, MeltingCounter_addressHigh, MeltingCounter_addressLow);
    }
    return ReadData;
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
    int ans;
    
    ans = I2CMasterStart(EEPROM_address,0);                       //Start condition
    if(ans == 0){
        I2CMasterWrite(high_address);           //Adress High Byte
        I2CMasterWrite(low_address);            //Adress Low Byte
        
        I2CMasterRepeatedStart(EEPROM_address,1);               //Restart condition
        for(UBYTE i = 0; i < EEPROMDataLength - 1; i++){
            ReadData[i] = I2CMasterRead(0);     //Read + Acknowledge
        }
        ReadData[EEPROMDataLength - 1] = I2CMasterRead(1);
    } else ans = -1;
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
    UBYTE data_length;
    UBYTE *read_data;
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
        case 'm': //write melting status to EEPROM --> stop melting
            WriteCheckByteToEEPROMs(MeltingStatus_B0select, MeltingStatus_addressHigh, MeltingStatus_addressLow, MELTING_FINISH_FLAG);
            //TODO:change task taget r->o
            break;
        default:
            switchError(error_I2C_commandSwitchEEPROM);
            break;
    }
}
