#include <xc.h>
#include "typeDefine.h"
#include "time.h"

//UBYTE EEPROMData[32];

//Methods
void I2CMasterWait(void);                                               
void I2CMasterStart(void);                                              
void I2CMasterRepeatedStart(void);                                   
void I2CMasterStop(void);                                               
void I2CMasterWrite(unsigned d);                                        
UBYTE I2CMasterRead(UBYTE a);                                           

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

//process command data if the command type is 'I2C'
void commandSwitchI2C(UBYTE command, UBYTE slaveAdress, UBYTE dataHigh, UBYTE dataLow){ 
    switch(command){    
        case 'w': //I2C write
            //TODO: write method for I2C write
            break;
        case 'r': //I2C read
            //TODO: write method for I2C read
            break;
        case 't': //I2C test
            //TODO: write method for I2C test
            break;
        case 'c': //I2C buffer clear
            //TODO: write method for I2C buffer clear
            break;
        case 'b': //change I2C baud rate
            //TODO: write method for change I2C baud rate
            break;
        default:
            //TODO: error message
            break;
    }
}