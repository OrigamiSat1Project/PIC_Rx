#include <xc.h>
//#include <PIC16F887.h>
#include "UART.h"
#include "Init_MPU.h"
#include "Type_define.h"
#include "time.h"
#include "decode_AX25.h"
#include "encode_AX25.h"
#include "PIN_define.h"

#define bit_H 0x01
#define bit_L 0x00

//reverse_bit8(),MSB,LSB


void SendPacket(void);
void SendByte(UBYTE);
void flipout(void);
void fcsbit(UBYTE);
UINT Packetmaker(void);
void test_Packetmaker(void);

UINT eflag = 0;
UINT efcsflag = 0;
UINT estuff = 0;
UBYTE efcslo, efcshi;
UBYTE eDataField[] = "Hello! I'm OrigamiSat1!!";
UBYTE ePacket[5];
UINT ebitstatus = low;


void test_Packetmaker(void){
    UINT num_ = Packetmaker();
    for(UINT i=0;i<num_;i++){
        putch(ePacket[i]);
    }
}

UINT Packetmaker(void){
    for(UINT i=0;i<6;i++){
        ePacket[i] = mycall[i] << 1;
    }
    ePacket[6] = 0x60;  //SSID
    for(UINT i=0;i<6;i++){
        ePacket[i+7] = ucall[i] << 1;
    }
    ePacket[13] = 0xe1; //SSID.e1?
    ePacket[14] = 0x03; //Control.30?
    ePacket[15] = 0xf0; //PID
    UINT Datanum = 0;
    for(Datanum=0;eDataField[Datanum] != '\0';Datanum++);
    for(UINT i=0;i<Datanum;i++){
        ePacket[16+i] = eDataField[i];
    }
    return 16+Datanum;
}

void SendPacket(void){
    UINT Packetnum;
    Packetnum = 0;
    Packetnum = Packetmaker();
    ebitstatus = 1;
    efcslo = efcshi = 0xff;
    estuff = 0;
    //  FlagField
    eflag = 1;
    efcsflag = 0;
    for(UINT i=0;i<27;i++){
        SendByte(0x7e);
    }
    eflag = 0;
    //  eDataField
    for(UINT i=0;i<Packetnum;i++){
        SendByte(ePacket[i]);
    }
    
    
    //  FCSField
    efcsflag = 1;
    efcslo = efcslo ^ 0xff;
    efcshi = efcshi ^ 0xff;
    SendByte(efcslo);
    SendByte(efcshi);
    efcsflag = 0;
    
    //  FlagField
    eflag = 1;
    for(UINT i=0;i<6;i++){
        SendByte(0x7e);
    }
}


void SendByte(UBYTE byte){
    UBYTE bt;
    for(UINT i=0;i<8;i++){
        bt = byte & bit_H;
        //  eDataField ?申?申 FCSCalculate
        if(efcsflag == 0 && eflag == 0){
            fcsbit(bt);
        }
        //  eDataField, FCSField ?申?申 bitestuffing
        if(bt == bit_L){
            flipout();
        }else{
            estuff ++;
            
            if(eflag == 0 && estuff == 5){
                __delay_us(espan);
                flipout();
            }
        }
        __delay_us(espan);
        byte = byte >> 1;
    }
}
//NRZI
void flipout(void){
    estuff = 0;
    if(ebitstatus == low){
        TXD = high;
        ebitstatus = high;
    }else{
        TXD = low;
        ebitstatus = low;
    }
}


//  FCSCalculation
void fcsbit(UBYTE tbyte){
    #asm
        BCF 03,0
        RRF _efcshi,F
        RRF _efcslo,F
    #endasm
    if(((STATUS & bit_H)^(tbyte)) == bit_H){
        efcshi = efcshi ^ 0x84;
        efcslo = efcslo ^ 0x08;
    }
}