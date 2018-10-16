#include <xc.h>
#include	<stdio.h>
//#include <PIC16F887.h>
#include "UART.h"
//#include "InitMPU.h"
#include "typeDefine.h"
#include "time.h"
#include "decodeAX25.h"
#include "pinDefine.h"

//Macro
#define BIT_HIGH 0x01               
#define BIT_LOW 0x00                
#define BIT_D_H 0x80                
#define BIT_D_L 0x00                
#define UCALL "JQ1YCZ"             //call sign of Tokyo Tech
#define MYCALL  "JS1YAX"           //call sign of OrigamiSat-1

//Global Data   
static UBYTE rcvState = 0;           //TODO: improve readability, recieve state 0= wait for flag; 1= my call correct; 2= ucall correct and get data; 3 = end flag has been found
UBYTE dPacket[PACKET_SIZE];         //whole uplink command
UBYTE dData[DATA_SIZE];             //only information byte of uplink command
UBYTE  dPacketCounter = 0;
UBYTE dfcsHighByte, dfcsLowByte;


//Methods
void waitFlag(void);
void getData(void);
void putAX25(void);
UINT fcsCheck(void);
//UBYTE readByte(void);

  
// reads bit using NRZ (Non-return-to-zero space)
// bit synchronization program using the change in status so that the sampling timing does not shift by adding half the bit time interval
// Number of GET_BIT_WAIT_LOOP and HALF_INTERVA are VERY sensitive (change with care and if changed check runtime again!!!)
UBYTE getBit(void){
    static UBYTE oldBit;
    oldBit = FX614_RXD;
    for(UBYTE i=0;i<GET_BIT_WAIT_LOOP;i++){     //Loop iteration number defines waiting interval for signal to change
        if(FX614_RXD != oldBit){
            __delay_us(HALF_INTERVAL);
//            LED_YELLOW= 1- LED_YELLOW;       //for debugging  
            return 0;
        }
    }
    return 1;
}

//function reads in the next 8 bit (using right shift) and returns that new byte
UBYTE readByte(void){
    UINT readBit;
    UBYTE buf = 0xff;
    for(UINT i=0;i<8;i++){
        readBit = getBit();
        buf = buf >> 1;
        if(readBit == 0){
            buf = buf | BIT_D_L;
        }else{
            buf = buf | BIT_D_H;
        }
    }
    return buf;
}

// function synchronizes start of uplink signal and running programming by locating the last flag
// 1. wait for first flag
// 2. wait for last flag
// 3. read mycall and ucall
void waitFlag(void){
    UBYTE readBit;
    UBYTE buf = 0xff;
    UBYTE callCounter = 0;
    rcvState = 0;
    while(rcvState < 2){
        while(buf != FLAG_AX25){    //Wait for the flag to come
            readBit = getBit();
            buf = buf << 1;         // TODO: Changed bit shift direction, bit_H, bit_L according to LSB, MSB.
            if(readBit == 0){
                buf = buf | BIT_LOW;
            }else{
                buf = buf | BIT_HIGH;
            }
        }
        
        /*Search for extra flags and skip them until different byte is read in*/
        while(buf == FLAG_AX25){
            buf = readByte();
        }
        /*check for call sign of Tokyo Tech (MYCALL) and store it if correct*/
        callCounter = 0;       //should not exceed length of Call array
        while((callCounter<6)){
            if(buf == (MYCALL[callCounter] << 1)){
                if(callCounter < 5){
                    dPacket[dPacketCounter] = buf;
                    dPacketCounter ++;
                    buf = readByte();
                    callCounter++;
                }else{      //if last MYCALL byte is correct as well change receive state to 1 and don't read in new byte
                    dPacket[dPacketCounter] = buf;
                    dPacketCounter ++;
                    callCounter++;
                    rcvState ++;                
                }  
            }else{                                                  //if byte is incorrect reset counter and return to waiting for flag
                dPacketCounter = 0;
                callCounter = 6;
            }
        }
        
        /*What byte is this?*/
        buf = readByte();
        dPacket[dPacketCounter] = buf;
        dPacketCounter ++;
        
        /*if MyCall was correct: check for call sign of OrigamiSat-1 (UCALL) and store it if correct*/
        buf = readByte();
        callCounter = 0;
        while((callCounter<6)){
            if(buf == (UCALL[callCounter] << 1)){
                if(callCounter < 5){
                    dPacket[dPacketCounter] = buf;
                    dPacketCounter ++;
                    buf = readByte();
                    callCounter++;
                }else{      //if last UCALL byte is correct as well change receive state to 2 and don't read in new byte
                    dPacket[dPacketCounter] = buf;
                    dPacketCounter ++;
                    callCounter++;
                    rcvState ++;                
                } 
            }else{                                                  //if byte is incorrect reset counter and return to waiting for flag
                dPacketCounter = 0;
                callCounter = 6;
                rcvState = 0;
            }
        }
    }
}

//function for storing data considering the dummy bit X after 11111X 
void getData(void){
    UBYTE buf = 0x00;
    UINT  dstuff = 0;
    UINT  bufstore_counter = 0;
    UINT  dbitstatus = 0;
    
    while(rcvState == 2){
        dbitstatus = getBit();
        if(dstuff == 5){
            if(dbitstatus == 1){        //end flag leads to exit of function, without storing the flag
                rcvState ++;
                break;
            }else{                      //dummy bit found and not stored
                dstuff = 0;
            }
        }else{
            buf = buf >> 1;
            if(dbitstatus == 0){
                buf = buf | BIT_D_L;
                dstuff = 0;
            }else{
                buf = buf | BIT_D_H;
                dstuff ++;
            }
            bufstore_counter ++;
            if(bufstore_counter == 8){ 
                bufstore_counter = 0;
                dPacket[dPacketCounter] = buf;
                dPacketCounter ++;
            }
        }
    }
}

// determine if the received data is valid
UINT fcsCheck(void){
    UBYTE bt, byte;//, dfcsHighByte, dfcsLowByte;
    dfcsLowByte = dfcsHighByte = 0xff;
    while(rcvState == 3){
        for(UBYTE i=0;i<dPacketCounter-2;i++){      //calculate the FCS for all except the last two bytes
            byte = dPacket[i];
            for(UBYTE j=0;j<8;j++){
                bt = byte & BIT_HIGH;
                #asm                                //embedded assembly language route to do a 16 bit rotate
                    BCF 03,0
                    RRF _dfcsHighByte,F
                    RRF _dfcsLowByte,F
                #endasm
                if(((STATUS & BIT_HIGH)^bt) == BIT_HIGH){
                    dfcsHighByte = dfcsHighByte ^ 0x84;
                    dfcsLowByte = dfcsLowByte ^ 0x08;
                }
                byte = byte >> 1;
            }
        }
        dfcsLowByte = dfcsLowByte ^ 0xff;
        dfcsHighByte = dfcsHighByte ^ 0xff;
        
        
        if((dPacket[dPacketCounter-2] == dfcsLowByte) && (dPacket[dPacketCounter-1] == dfcsHighByte)){     // The dPacketCounter runs one value to far, therefore dPacket[dPacketCounter] is emppty
            return 1;
        }else{
            return 0;
        }
    }
}

//UBYTE *receiveDataPacket(void){
void receiveDataPacket(UBYTE *cdData){
    UINT fcschecker;
    
    waitFlag();
    //putChar('w');
    getData();
    //putChar('d');
    fcschecker = fcsCheck();
    //putChar('f');
    
    if(fcschecker == 1){    //valid data is stored in dData
        for(UBYTE i=0; i<DATA_SIZE; i++){
            cdData[i] = dPacket[i+20];     //[dPacket]0-5:UCALL / 6:SSID / 7-12:MYCALL / 13:SSID / 14:control / 15:PID / 16-19:'ori1' / 20-52:command data(=cdData) / 53,54:FCS 
        }
        dPacketCounter = 0;
        rcvState = 0;
        
//        return cdData;
    }else{                  //the data is invalid everything gets reset and data ignored //TODO check this function by test
        dPacketCounter = 0;
        rcvState = 0;
//        return 0x00;
    }
}

/*---for debug---*/
// function for debugging, it prints out the receive signal
// void putAX25(void){
//     for(UINT i=0;i<6;i++){
//         dPacket[i] = dPacket[i] >> 1;
//         dPacket[i+7] = dPacket[i+7] >> 1;
//     }
//     for(UINT i=0;i<6;i++){
//         putChar(dPacket[i]);
//     }
//     putChar('>');
//     for(UINT i=0;i<6;i++){
//         putChar(dPacket[i+7]);
//     }
//     putChar(':');
//     for(UINT i=0;i<dPacketCounter-18;i++){
//         putChar(dPacket[i+16]);
//     }
//     return;
// }
