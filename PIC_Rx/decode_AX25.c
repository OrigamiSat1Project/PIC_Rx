<<<<<<< HEAD:PIC_Rx/decode_AX25.c
#include <xc.h>
//#include <PIC16F887.h>
#include "UART.h"
#include "Init_MPU.h"
#include "Type_define.h"
#include "time.h"
#include "decode_AX25.h"
#include "encode_AX25.h"

void UART_TX(UINT,UINT);
void waitByte(UINT,UINT);
UINT getbit(void);
void waitFlag(void);
void getdata(void);
void conv_DI_UARTRX(void);
UBYTE reverse_bit8(UBYTE x);


#define bit_H 0x01
#define bit_L 0x00
#define bit_dH 0x80
#define bit_dL 0x00

static UINT rcv_state = 0;
UBYTE dPacket[50];
UBYTE dData[32];
UINT  dPacketnum = 0;
UBYTE dfcshi, dfcslo;

UINT getbit(void);
void waitFlag(void);
void getdata(void);
UINT fcscheck(void);
void putAX25(void);
void conv_DI_UARTRX(void);

//  bitÉTÉìÉvÉäÉìÉOÇÃÉ^ÉCÉ~ÉìÉOÇ™Ç∏ÇÍÇ»Ç¢ÇÊÇ§bitîΩì]Ç≤Ç∆Ç…ìØä˙Ç∑ÇÈÉvÉçÉOÉâÉÄ
//  getbitloopÇ™äÑÇ∆ÉVÉrÉAÇ…å¯Ç¢ÇƒÇ≠ÇÈÇ©ÇÁíçà”(hspanÇ‡Ç‹Ç†Ç‹Ç†óòÇ¢ÇƒÇ≠ÇÈ)  
UINT getbit(void){
    static UINT oldbit;
    oldbit = FX614_RXD;
    for(UINT i=0;i<getbitloop;i++){
        if(FX614_RXD != oldbit){
            __delay_us(hspan);
            led_yellow= 1- led_yellow;
            return 0;
        }
    }
    return 1;
}

//  FlagÇ≈É\ÉtÉgÉXÉPÉãÉ`&ìØä˙Ç∑ÇÈÉvÉçÉOÉâÉÄ
void waitFlag(void){
    UINT flag_judge;
    UBYTE buf = 0xff;
    rcv_state = 0;
    while(rcv_state < 2){
        while(buf != Flag_AX25){    //ÉtÉâÉOÇ™óàÇÈÇ‹Ç≈ë“Ç¬
            flag_judge = getbit();
            buf = buf << 1;         //LSB, MSBÇ…âûÇ∂ÇƒÉrÉbÉgÉVÉtÉgÇÃï˚å¸ÅCbit_H, bit_LÇïœçXÅD
            if(flag_judge == 0){
                buf = buf | bit_L;
            }else{
                buf = buf | bit_H;
            }
        }
        
        //whileî≤ÇØÇΩå„ÇÃbufÇÕÉtÉâÉOÇ≈ÇÕÇ»Ç≠ucallÇÃêÊì™1ÉoÉCÉgï™ÅDÇ‚ÇËï˚ÇÕà·Ç§ÇŸÇ§Ç™ó«Ç¢ãCÇ™Ç∑ÇÈÅD
        //ÉrÉbÉgãlÇﬂÇ≥ÇÍÇΩÇÁÇªÇÃï™ä‘à·Ç¶ÇƒÉoÉbÉtÉ@Ç…äiî[ÇµÇƒÇµÇ‹Ç§
        while(buf == Flag_AX25){
            for(UINT i=0;i<8;i++){
                flag_judge = getbit();
                buf = buf >> 1;
                if(flag_judge == 0){
                    buf = buf | bit_dL;
                }else{
                    buf = buf | bit_dH;
                }
            }
        }
        if(buf == (mycall[0] << 1)){
            dPacket[dPacketnum] = buf;
            dPacketnum ++;
            for(UINT j=0;j<8;j++){
                flag_judge = getbit();
                buf = buf >> 1;
                if(flag_judge == 0){
                    buf = buf | bit_dL;
                }else{
                    buf = buf | bit_dH;
                }
            }
            if(buf == (mycall[1] << 1)){
                dPacket[dPacketnum] = buf;
                dPacketnum ++;
                for(UINT j=0;j<8;j++){
                    flag_judge = getbit();
                    buf = buf >> 1;
                    if(flag_judge == 0){
                        buf = buf | bit_dL;
                    }else{
                        buf = buf | bit_dH;
                    }
                }
                if(buf == (mycall[2] << 1)){
                    dPacket[dPacketnum] = buf;
                    dPacketnum ++;
                    for(UINT j=0;j<8;j++){
                        flag_judge = getbit();
                        buf = buf >> 1;
                        if(flag_judge == 0){
                            buf = buf | bit_dL;
                        }else{
                            buf = buf | bit_dH;
                        }
                    }
                    if(buf == (mycall[3] << 1)){
                        dPacket[dPacketnum] = buf;
                        dPacketnum ++;
                        for(UINT j=0;j<8;j++){
                            flag_judge = getbit();
                            buf = buf >> 1;
                            if(flag_judge == 0){
                                buf = buf | bit_dL;
                            }else{
                                buf = buf | bit_dH;
                            }
                        }
                        if(buf == (mycall[4] << 1)){
                            dPacket[dPacketnum] = buf;
                            dPacketnum ++;
                            for(UINT j=0;j<8;j++){
                                flag_judge = getbit();
                                buf = buf >> 1;
                                if(flag_judge == 0){
                                    buf = buf | bit_dL;
                                }else{
                                    buf = buf | bit_dH;
                                }
                            }
                            if(buf == (mycall[5] << 1)){
                                dPacket[dPacketnum] = buf;
                                dPacketnum ++;
                                rcv_state ++;
                            }else{
                                dPacketnum = 0;
                            }
                        }else{
                            dPacketnum = 0;
                        }
                    }else{
                        dPacketnum = 0;
                    }
                }else{
                    dPacketnum = 0;
                }
            }else{
                dPacketnum = 0;
            }
        }else{
            dPacketnum = 0;
        }
        /**/
        for(UINT j=0;j<8;j++){
            flag_judge = getbit();
            buf = buf >> 1;
            if(flag_judge == 0){
                buf = buf | bit_dL;
            }else{
                buf = buf | bit_dH;
            }
        }
        dPacket[dPacketnum] = buf;
        dPacketnum ++;
        for(UINT j=0;j<8;j++){
            flag_judge = getbit();
            buf = buf >> 1;
            if(flag_judge == 0){
                buf = buf | bit_dL;
            }else{
                buf = buf | bit_dH;
            }
        }
        if(buf == (ucall[0] << 1)){
            dPacket[dPacketnum] = buf;
            dPacketnum ++;
            for(UINT j=0;j<8;j++){
                flag_judge = getbit();
                buf = buf >> 1;
                if(flag_judge == 0){
                    buf = buf | bit_dL;
                }else{
                    buf = buf | bit_dH;
                }
            }
            if(buf == (ucall[1] << 1)){
                dPacket[dPacketnum] = buf;
                dPacketnum ++;
                for(UINT j=0;j<8;j++){
                    flag_judge = getbit();
                    buf = buf >> 1;
                    if(flag_judge == 0){
                        buf = buf | bit_dL;
                    }else{
                        buf = buf | bit_dH;
                    }
                }
                if(buf == (ucall[2] << 1)){
                    dPacket[dPacketnum] = buf;
                    dPacketnum ++;
                    for(UINT j=0;j<8;j++){
                        flag_judge = getbit();
                        buf = buf >> 1;
                        if(flag_judge == 0){
                            buf = buf | bit_dL;
                        }else{
                            buf = buf | bit_dH;
                        }
                    }
                    if(buf == (ucall[3] << 1)){
                        dPacket[dPacketnum] = buf;
                        dPacketnum ++;
                        for(UINT j=0;j<8;j++){
                            flag_judge = getbit();
                            buf = buf >> 1;
                            if(flag_judge == 0){
                                buf = buf | bit_dL;
                            }else{
                                buf = buf | bit_dH;
                            }
                        }
                        if(buf == (ucall[4] << 1)){
                            dPacket[dPacketnum] = buf;
                            dPacketnum ++;
                            for(UINT j=0;j<8;j++){
                                flag_judge = getbit();
                                buf = buf >> 1;
                                if(flag_judge == 0){
                                    buf = buf | bit_dL;
                                }else{
                                    buf = buf | bit_dH;
                                }
                            }
                            if(buf == (ucall[5] << 1)){
                                dPacket[dPacketnum] = buf;
                                dPacketnum ++;
                                rcv_state ++;
                            }else{
                                dPacketnum = 0;
                                rcv_state = 0;
                            }
                        }else{
                            dPacketnum = 0;
                            rcv_state = 0;
                        }
                    }else{
                        dPacketnum = 0;
                        rcv_state = 0;
                    }
                }else{
                    dPacketnum = 0;
                    rcv_state = 0;
                }
            }else{
                dPacketnum = 0;
                rcv_state = 0;
            }
        }else{
            dPacketnum = 0;
            rcv_state = 0;
        }
    }
}

//  11111ÇÃéüÉ_É~Å[ÉfÅ[É^îªíËÇ≈Ç´ÇÈÉvÉçÉOÉâÉÄ
/**/
void getdata(void){
    UBYTE buf = 0x00;
    UINT  dstuff = 0;
    UBYTE dammy_data;
    UINT  bufstore_counter = 0;
    UINT  dbitstatus = 0;
    
    while(rcv_state == 2){
        dbitstatus = getbit();
        if(dstuff == 5){
            if(dbitstatus == 1){
                rcv_state ++;
                break;
            }else{
                dstuff = 0;
            }
        }else{
            buf = buf >> 1;
            if(dbitstatus == 0){
                buf = buf | bit_dL;
                dstuff = 0;
            }else{
                buf = buf | bit_dH;
                dstuff ++;
            }
            bufstore_counter ++;
            if(bufstore_counter == 8){  //ÉfÉoÉbÉNópÅDñ{óàÇÕÉoÉbÉtÉ@Ç…ÇΩÇﬂÇƒÇ¢Ç≠ÅD
                bufstore_counter = 0;
                dPacket[dPacketnum] = buf;
                dPacketnum ++;
            }
        }
    }
}

UINT fcscheck(void){
    UBYTE bt,byte;//, dfcslo, dfcshi;
    dfcslo = dfcshi = 0xff;
    while(rcv_state == 3){
        for(UINT i=0;i<dPacketnum-2;i++){      //dPacketnumÇÕmycall,SSID,ucall,Control,PID,Data,FCSÇÃbyteêîÅD-2Ç≈FCSï™ÇèúÇ≠ÅD
            byte = dPacket[i];
            for(UINT i=0;i<8;i++){
                bt = byte & bit_H;
                #asm
                    BCF 03,0
                    RRF _dfcshi,F
                    RRF _dfcslo,F
                #endasm
                if(((STATUS & bit_H)^bt) == bit_H){
                    dfcshi = dfcshi ^ 0x84;
                    dfcslo = dfcslo ^ 0x08;
                }
                byte = byte >> 1;
            }
        }
        dfcslo = dfcslo ^ 0xff;
        dfcshi = dfcshi ^ 0xff;
        
        //  ÉfÉoÉbÉNíÜÇÕÇ±Ç±ÇÕÉRÉÅÉìÉgÉAÉEÉgÇ∑ÇÈÇŸÇ§Ç™ó«Ç¢Ç©Ç‡ÅH
        if((dPacket[dPacketnum-2] == dfcslo) && (dPacket[dPacketnum-1] == dfcshi)){     //dPacketnumî‘ñ⁄ÇÃÉoÉbÉtÉ@ÇÕÉtÉâÉOÇ™äiî[Ç≥ÇÍÇƒÇµÇ‹Ç¡ÇƒÇ¢ÇÈÇÕÇ∏
            //putAX25();
            //putcrlf();
            //dPacketnum = 0;
            //rcv_state = 0;
            return 1;
        }else{
            //put_error();
            //putcrlf();
            //dPacketnum = 0;
            //rcv_state = 0;
            return 0;
        }
    }
}

void ReceivePacket(void){
    waitFlag();
    getdata();
    fcscheck();
}

UBYTE *ReceivePacket_data(void){
    UINT fcschecker;
    //UBYTE dData[];
    
    waitFlag();
    led_white = 1;
    getdata();
    led_white = 0;
    fcschecker = fcscheck();
    
    if(fcschecker == 1){
        for(UINT i=0; i<dPacketnum-18; i++){
            dData[i] = dPacket[i+16];
        }
        dPacketnum = 0;
        rcv_state = 0;
        return dData;
    }else{
        dPacketnum = 0;
        rcv_state = 0;
        return 0x00;
    }
}


void putAX25(void){
    for(UINT i=0;i<6;i++){
        dPacket[i] = dPacket[i] >> 1;
        dPacket[i+7] = dPacket[i+7] >> 1;
    }
    for(UINT i=0;i<6;i++){
        putch(dPacket[i]);
    }
    putch('>');
    for(UINT i=0;i<6;i++){
        putch(dPacket[i+7]);
    }
    putch(':');
    for(UINT i=0;i<dPacketnum-18;i++){
        putch(dPacket[i+16]);
    }
    return;
}

//  ÉfÉWÉ^ÉãÉCÉìÉvÉbÉgÇUARTRXÇ∆ÇµÇƒégÇ¶ÇÈÉvÉçÉOÉâÉÄ
void conv_DI_UARTRX(void){
    UBYTE buf_dammy = 0x00;
    UBYTE buf = 0x00;
    
    getbit();
    while(FX614_RXD == 0){  //å„ÅXÇÕäÑÇËçûÇ›èàóùÅDSTARTbitë“ã@
        for(UINT i=0;i<8;i++){
            getbit();
            buf_dammy = buf_dammy << 1;
            if(FX614_RXD == 0){
                buf_dammy = buf_dammy | bit_L;
            }else{
                buf_dammy = buf_dammy | bit_H;
            }
        }
        getbit();   //STOPbitóp
        buf = reverse_bit8(buf_dammy);
        putch(buf);
    }
}

//  MNB,LNBÇÃãtì]èàóùÇ∑ÇÈÉvÉçÉOÉâÉÄ
UBYTE reverse_bit8(UBYTE x){
	x = ((x & 0x55) << 1) | ((x & 0xAA) >> 1);
	x = ((x & 0x33) << 2) | ((x & 0xCC) >> 2);
	return (x << 4) | (x >> 4);
=======
#include <xc.h>
//#include <PIC16F887.h>
#include "UART.h"
#include "Init_MPU.h"
#include "Type_define.h"
#include "time.h"
#include "decode_AX25.h"
#include "encode_AX25.h"

void UART_TX(UINT,UINT);
void waitByte(UINT,UINT);
UINT getbit(void);
void waitFlag(void);
void getdata(void);
void conv_DI_UARTRX(void);
UBYTE reverse_bit8(UBYTE x);


#define bit_H 0x01
#define bit_L 0x00
#define bit_dH 0x80
#define bit_dL 0x00

static UINT rcv_state = 0;
UBYTE dPacket[50];
UBYTE dData[32];
UINT  dPacketnum = 0;
UBYTE dfcshi, dfcslo;

UINT getbit(void);
void waitFlag(void);
void getdata(void);
UINT fcscheck(void);
void putAX25(void);
void conv_DI_UARTRX(void);

//  bit„Çµ„É≥„Éó„É™„É≥„Ç∞„ÅÆ„Çø„Ç§„Éü„É≥„Ç∞„Åå„Åö„Çå„Å™„ÅÑ„Çà„ÅÜbitÂèçËª¢„Åî„Å®„Å´ÂêåÊúü„Åô„Çã„Éó„É≠„Ç∞„É©„É†
//  getbitloop„ÅåÂâ≤„Å®„Ç∑„Éì„Ç¢„Å´Âäπ„ÅÑ„Å¶„Åè„Çã„Åã„ÇâÊ≥®ÊÑè(hspan„ÇÇ„Åæ„ÅÇ„Åæ„ÅÇÂà©„ÅÑ„Å¶„Åè„Çã)  
UINT getbit(void){
    static UINT oldbit;
    oldbit = FX614_RXD;
    for(UINT i=0;i<getbitloop;i++){
        if(FX614_RXD != oldbit){
            __delay_us(hspan);
            return 0;
        }
    }
    return 1;
}

//  Flag„Åß„ÇΩ„Éï„Éà„Çπ„Ç±„É´„ÉÅ&ÂêåÊúü„Åô„Çã„Éó„É≠„Ç∞„É©„É†
void waitFlag(void){
    UINT flag_judge;
    UBYTE buf = 0xff;
    rcv_state = 0;
    while(rcv_state < 2){
        while(buf != Flag_AX25){    //wait for flag
            flag_judge = getbit();
            buf = buf << 1;         //LSB, MSB„Å´Âøú„Åò„Å¶„Éì„ÉÉ„Éà„Ç∑„Éï„Éà„ÅÆÊñπÂêëÔºåbit_H, bit_L„ÇíÂ§âÊõ¥Ôºé
            if(flag_judge == 0){
                buf = buf | bit_L;
            }else{
                buf = buf | bit_H;
            }
        }
        
        //whileÊäú„Åë„ÅüÂæå„ÅÆbuf„ÅØ„Éï„É©„Ç∞„Åß„ÅØ„Å™„Åèucall„ÅÆÂÖàÈ†≠1„Éê„Ç§„ÉàÂàÜÔºé„ÇÑ„ÇäÊñπ„ÅØÈÅï„ÅÜ„Åª„ÅÜ„ÅåËâØ„ÅÑÊ∞ó„Åå„Åô„ÇãÔºé
        //„Éì„ÉÉ„ÉàË©∞„ÇÅ„Åï„Çå„Åü„Çâ„Åù„ÅÆÂàÜÈñìÈÅï„Åà„Å¶„Éê„ÉÉ„Éï„Ç°„Å´Ê†ºÁ¥ç„Åó„Å¶„Åó„Åæ„ÅÜ
        while(buf == Flag_AX25){
            for(UINT i=0;i<8;i++){
                flag_judge = getbit();
                buf = buf >> 1;
                if(flag_judge == 0){
                    buf = buf | bit_dL;
                }else{
                    buf = buf | bit_dH;
                }
            }
        }
        if(buf == (mycall[0] << 1)){
            dPacket[dPacketnum] = buf;
            dPacketnum ++;
            for(UINT j=0;j<8;j++){
                flag_judge = getbit();
                buf = buf >> 1;
                if(flag_judge == 0){
                    buf = buf | bit_dL;
                }else{
                    buf = buf | bit_dH;
                }
            }
            if(buf == (mycall[1] << 1)){
                dPacket[dPacketnum] = buf;
                dPacketnum ++;
                for(UINT j=0;j<8;j++){
                    flag_judge = getbit();
                    buf = buf >> 1;
                    if(flag_judge == 0){
                        buf = buf | bit_dL;
                    }else{
                        buf = buf | bit_dH;
                    }
                }
                if(buf == (mycall[2] << 1)){
                    dPacket[dPacketnum] = buf;
                    dPacketnum ++;
                    for(UINT j=0;j<8;j++){
                        flag_judge = getbit();
                        buf = buf >> 1;
                        if(flag_judge == 0){
                            buf = buf | bit_dL;
                        }else{
                            buf = buf | bit_dH;
                        }
                    }
                    if(buf == (mycall[3] << 1)){
                        dPacket[dPacketnum] = buf;
                        dPacketnum ++;
                        for(UINT j=0;j<8;j++){
                            flag_judge = getbit();
                            buf = buf >> 1;
                            if(flag_judge == 0){
                                buf = buf | bit_dL;
                            }else{
                                buf = buf | bit_dH;
                            }
                        }
                        if(buf == (mycall[4] << 1)){
                            dPacket[dPacketnum] = buf;
                            dPacketnum ++;
                            for(UINT j=0;j<8;j++){
                                flag_judge = getbit();
                                buf = buf >> 1;
                                if(flag_judge == 0){
                                    buf = buf | bit_dL;
                                }else{
                                    buf = buf | bit_dH;
                                }
                            }
                            if(buf == (mycall[5] << 1)){
                                dPacket[dPacketnum] = buf;
                                dPacketnum ++;
                                rcv_state ++;
                            }else{
                                dPacketnum = 0;
                            }
                        }else{
                            dPacketnum = 0;
                        }
                    }else{
                        dPacketnum = 0;
                    }
                }else{
                    dPacketnum = 0;
                }
            }else{
                dPacketnum = 0;
            }
        }else{
            dPacketnum = 0;
        }
        /**/
        for(UINT j=0;j<8;j++){
            flag_judge = getbit();
            buf = buf >> 1;
            if(flag_judge == 0){
                buf = buf | bit_dL;
            }else{
                buf = buf | bit_dH;
            }
        }
        dPacket[dPacketnum] = buf;
        dPacketnum ++;
        for(UINT j=0;j<8;j++){
            flag_judge = getbit();
            buf = buf >> 1;
            if(flag_judge == 0){
                buf = buf | bit_dL;
            }else{
                buf = buf | bit_dH;
            }
        }
        if(buf == (ucall[0] << 1)){
            dPacket[dPacketnum] = buf;
            dPacketnum ++;
            for(UINT j=0;j<8;j++){
                flag_judge = getbit();
                buf = buf >> 1;
                if(flag_judge == 0){
                    buf = buf | bit_dL;
                }else{
                    buf = buf | bit_dH;
                }
            }
            if(buf == (ucall[1] << 1)){
                dPacket[dPacketnum] = buf;
                dPacketnum ++;
                for(UINT j=0;j<8;j++){
                    flag_judge = getbit();
                    buf = buf >> 1;
                    if(flag_judge == 0){
                        buf = buf | bit_dL;
                    }else{
                        buf = buf | bit_dH;
                    }
                }
                if(buf == (ucall[2] << 1)){
                    dPacket[dPacketnum] = buf;
                    dPacketnum ++;
                    for(UINT j=0;j<8;j++){
                        flag_judge = getbit();
                        buf = buf >> 1;
                        if(flag_judge == 0){
                            buf = buf | bit_dL;
                        }else{
                            buf = buf | bit_dH;
                        }
                    }
                    if(buf == (ucall[3] << 1)){
                        dPacket[dPacketnum] = buf;
                        dPacketnum ++;
                        for(UINT j=0;j<8;j++){
                            flag_judge = getbit();
                            buf = buf >> 1;
                            if(flag_judge == 0){
                                buf = buf | bit_dL;
                            }else{
                                buf = buf | bit_dH;
                            }
                        }
                        if(buf == (ucall[4] << 1)){
                            dPacket[dPacketnum] = buf;
                            dPacketnum ++;
                            for(UINT j=0;j<8;j++){
                                flag_judge = getbit();
                                buf = buf >> 1;
                                if(flag_judge == 0){
                                    buf = buf | bit_dL;
                                }else{
                                    buf = buf | bit_dH;
                                }
                            }
                            if(buf == (ucall[5] << 1)){
                                dPacket[dPacketnum] = buf;
                                dPacketnum ++;
                                rcv_state ++;
                            }else{
                                dPacketnum = 0;
                                rcv_state = 0;
                            }
                        }else{
                            dPacketnum = 0;
                            rcv_state = 0;
                        }
                    }else{
                        dPacketnum = 0;
                        rcv_state = 0;
                    }
                }else{
                    dPacketnum = 0;
                    rcv_state = 0;
                }
            }else{
                dPacketnum = 0;
                rcv_state = 0;
            }
        }else{
            dPacketnum = 0;
            rcv_state = 0;
        }
    }
}

//  11111„ÅÆÊ¨°„ÉÄ„Éü„Éº„Éá„Éº„ÇøÂà§ÂÆö„Åß„Åç„Çã„Éó„É≠„Ç∞„É©„É†
/**/
void getdata(void){
    UBYTE buf = 0x00;
    UINT  dstuff = 0;
    UBYTE dammy_data;
    UINT  bufstore_counter = 0;
    UINT  dbitstatus = 0;
    
    while(rcv_state == 2){
        dbitstatus = getbit();
        if(dstuff == 5){
            if(dbitstatus == 1){
                rcv_state ++;
                break;
            }else{
                dstuff = 0;
            }
        }else{
            buf = buf >> 1;
            if(dbitstatus == 0){
                buf = buf | bit_dL;
                dstuff = 0;
            }else{
                buf = buf | bit_dH;
                dstuff ++;
            }
            bufstore_counter ++;
            if(bufstore_counter == 8){  //„Éá„Éê„ÉÉ„ÇØÁî®ÔºéÊú¨Êù•„ÅØ„Éê„ÉÉ„Éï„Ç°„Å´„Åü„ÇÅ„Å¶„ÅÑ„ÅèÔºé
                bufstore_counter = 0;
                dPacket[dPacketnum] = buf;
                dPacketnum ++;
            }
        }
    }
}

UINT fcscheck(void){
    UBYTE bt,byte;//, dfcslo, dfcshi;
    dfcslo = dfcshi = 0xff;
    while(rcv_state == 3){
        for(UINT i=0;i<dPacketnum-2;i++){      //dPacketnum„ÅØmycall,SSID,ucall,Control,PID,Data,FCS„ÅÆbyteÊï∞Ôºé-2„ÅßFCSÂàÜ„ÇíÈô§„ÅèÔºé
            byte = dPacket[i];
            for(UINT i=0;i<8;i++){
                bt = byte & bit_H;
                #asm
                    BCF 03,0
                    RRF _dfcshi,F
                    RRF _dfcslo,F
                #endasm
                if(((STATUS & bit_H)^bt) == bit_H){
                    dfcshi = dfcshi ^ 0x84;
                    dfcslo = dfcslo ^ 0x08;
                }
                byte = byte >> 1;
            }
        }
        dfcslo = dfcslo ^ 0xff;
        dfcshi = dfcshi ^ 0xff;
        
        //  „Éá„Éê„ÉÉ„ÇØ‰∏≠„ÅØ„Åì„Åì„ÅØ„Ç≥„É°„É≥„Éà„Ç¢„Ç¶„Éà„Åô„Çã„Åª„ÅÜ„ÅåËâØ„ÅÑ„Åã„ÇÇÔºü
        if((dPacket[dPacketnum-2] == dfcslo) && (dPacket[dPacketnum-1] == dfcshi)){     //dPacketnumÁï™ÁõÆ„ÅÆ„Éê„ÉÉ„Éï„Ç°„ÅØ„Éï„É©„Ç∞„ÅåÊ†ºÁ¥ç„Åï„Çå„Å¶„Åó„Åæ„Å£„Å¶„ÅÑ„Çã„ÅØ„Åö
            //putAX25();
            //putcrlf();
            //dPacketnum = 0;
            //rcv_state = 0;
            return 1;
        }else{
            //put_error();
            //putcrlf();
            //dPacketnum = 0;
            //rcv_state = 0;
            return 0;
        }
    }
}

void ReceivePacket(void){
    waitFlag();
    getdata();
    fcscheck();
}

UBYTE *ReceivePacket_data(void){
    UINT fcschecker;
    //UBYTE dData[];
    
    waitFlag();
    getdata();
    fcschecker = fcscheck();
    
    if(fcschecker == 1){
        for(UINT i=4; i<dPacketnum-18; i++){    // i=0 -> i=4 ("ori1"„ÅÆÊ¨°„Åã„Çâ)
            dData[i] = dPacket[i+16];
        }
        dPacketnum = 0;
        rcv_state = 0;
        return dData;
    }else{
        dPacketnum = 0;
        rcv_state = 0;
        return 0x00;
    }
}


void putAX25(void){
    for(UINT i=0;i<6;i++){
        dPacket[i] = dPacket[i] >> 1;
        dPacket[i+7] = dPacket[i+7] >> 1;
    }
    for(UINT i=0;i<6;i++){
        putch(dPacket[i]);
    }
    putch('>');
    for(UINT i=0;i<6;i++){
        putch(dPacket[i+7]);
    }
    putch(':');
    for(UINT i=0;i<dPacketnum-18;i++){
        putch(dPacket[i+16]);
    }
    return;
}

//  „Éá„Ç∏„Çø„É´„Ç§„É≥„Éó„ÉÉ„Éà„ÇíUARTRX„Å®„Åó„Å¶‰Ωø„Åà„Çã„Éó„É≠„Ç∞„É©„É†
void conv_DI_UARTRX(void){
    UBYTE buf_dammy = 0x00;
    UBYTE buf = 0x00;
    
    getbit();
    while(FX614_RXD == 0){  //Âæå„ÄÖ„ÅØÂâ≤„ÇäËæº„ÅøÂá¶ÁêÜÔºéSTARTbitÂæÖÊ©ü
        for(UINT i=0;i<8;i++){
            getbit();
            buf_dammy = buf_dammy << 1;
            if(FX614_RXD == 0){
                buf_dammy = buf_dammy | bit_L;
            }else{
                buf_dammy = buf_dammy | bit_H;
            }
        }
        getbit();   //STOPbitÔøΩp
        buf = reverse_bit8(buf_dammy);
        putch(buf);
    }
}

//  MNB,LNB„ÅÆÈÄÜËª¢Âá¶ÁêÜ„Åô„Çã„Éó„É≠„Ç∞„É©„É†
UBYTE reverse_bit8(UBYTE x){
	x = ((x & 0x55) << 1) | ((x & 0xAA) >> 1);
	x = ((x & 0x33) << 2) | ((x & 0xCC) >> 2);
	return (x << 4) | (x >> 4);
>>>>>>> DH_dev:decode_AX25.c
}