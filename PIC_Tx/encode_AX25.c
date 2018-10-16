#include "UART.h"
#include "Type_define.h"
#include "time.h"
#include "encode_AX25.h"
#include "pinDefine.h"

#define bit_H 0x01
#define bit_L 0x00
#define ucall "JQ1YCZ"   //call sign of Tokyo Tech
#define mycall "JS1YAX"  //call sign of OrigamiSar-1
#define low  0
#define high 1

//void SendPacket(void);
void SendByte(UBYTE);
void flipout(void);
void fcsbit(UBYTE);
UBYTE Packetmaker(UBYTE *);

/*--for debug--*/
//void test_Packetmaker(UBYTE *);

UBYTE eflag = 0;
UBYTE efcsflag = 0;
UBYTE estuff = 0;
UBYTE efcslo, efcshi;
UBYTE ePacket[52];
BIT ebitstatus = low;

/*--for debug--*/
// void test_Packetmaker(UBYTE *eDataField){
//     UBYTE num_ = Packetmaker(eDataField);
//     for(UBYTE i=0;i<num_;i++){
//         putch(ePacket[i]);
//     }
//     putcrlf();
// }

UBYTE Packetmaker(UBYTE *eDataField){
    for(UBYTE i=0;i<6;i++){
        ePacket[i] = ucall[i] << 1;
    }
    ePacket[6] = 0x60;  //SSID
    for(UBYTE i=0;i<6;i++){
        ePacket[i+7] = mycall[i] << 1;
    }
    ePacket[13] = 0xe1; //SSID.e1?
    ePacket[14] = 0x03; //Control.30?
    ePacket[15] = 0xf0; //PID
    const UBYTE Datanum = 36;
//    for(Datanum=0;eDataField[Datanum] != '\0';Datanum++);
    //Datanum -= 1;
    for(UBYTE i=0;i<Datanum;i++){
        ePacket[16+i] = eDataField[i];
    }
    
    //  XXX : for debug
    for(UBYTE i=0;i<16+Datanum;i++){
        putch(ePacket[i]);
    }
    return 16+Datanum;
}

void SendPacket(UBYTE *eDataField){
//void SendPacket(void)
    UBYTE Packetnum;
    Packetnum = 0;
    Packetnum = Packetmaker(eDataField);
    ebitstatus = 1;
    efcslo = efcshi = 0xff;
    estuff = 0;
    //  FlagField
    eflag = 1;
    efcsflag = 0;
    for(UBYTE i=0;i<27;i++){
        SendByte(0x7e);
    }
    eflag = 0;
    //  eDataField
    for(UBYTE i=0;i<Packetnum;i++){
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
    for(UBYTE i=0;i<6;i++){
        SendByte(0x7e);
    }
}


void SendByte(UBYTE byte){
    UBYTE bt;
    for(UBYTE i=0;i<8;i++){
        bt = byte & bit_H;
        //  eDataField -- FCSCalculate
        if(efcsflag == 0 && eflag == 0){
            fcsbit(bt);
        }
        //  eDataField, FCSField -- bitestuffing
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
        txd = high;
        ebitstatus = high;
    }else{
        txd = low;
        ebitstatus = low;
    }
}


//  FCSCalculation
void fcsbit(UBYTE tbyte){
    //FIXME:fix assembry
//    #asm
//        BCF 03,0
//        RRF _efcshi,F
//        RRF _efcslo,F
//    #endasm
    if(((STATUS & bit_H)^(tbyte)) == bit_H){
        efcshi = efcshi ^ 0x84;
        efcslo = efcslo ^ 0x08;
    }
}