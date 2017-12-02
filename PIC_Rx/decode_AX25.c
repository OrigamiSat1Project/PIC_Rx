//<<<<<<< HEAD:PIC_Rx/decode_AX25.c
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
const UINT commandSize = 32;


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

//  bitサンプリングのタイミングがずれないようbit反転ごとに同期するプログラム
//  getbitloopが割とシビアに効いてくるから注意(hspanもまあまあ利いてくる)  
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

//  Flagでソフトスケルチ&同期するプログラム
void waitFlag(void){
    UINT flag_judge;
    UBYTE buf = 0xff;
    rcv_state = 0;
    while(rcv_state < 2){
        while(buf != Flag_AX25){    //フラグが来るまで待つ
            flag_judge = getbit();
            buf = buf << 1;         //LSB, MSBに応じてビットシフトの方向，bit_H, bit_Lを変更．
            if(flag_judge == 0){
                buf = buf | bit_L;
            }else{
                buf = buf | bit_H;
            }
        }
        
        //while抜けた後のbufはフラグではなくucallの先頭1バイト分．やり方は違うほうが良い気がする．
        //ビット詰めされたらその分間違えてバッファに格納してしまう
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

//  11111の次ダミーデータ判定できるプログラム
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
            if(bufstore_counter == 8){  //デバック用．本来はバッファにためていく．
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
        for(UINT i=0;i<dPacketnum-2;i++){      //dPacketnumはmycall,SSID,ucall,Control,PID,Data,FCSのbyte数．-2でFCS分を除く．
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
        
        //  デバック中はここはコメントアウトするほうが良いかも？
        if((dPacket[dPacketnum-2] == dfcslo) && (dPacket[dPacketnum-1] == dfcshi)){     //dPacketnum番目のバッファはフラグが格納されてしまっているはず
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
//    led_white = 1;
    getdata();
//    led_white = 0;
    fcschecker = fcscheck();
    
    if(fcschecker == 1){
        for(UINT i=0; i<commandSize; i++){
            dData[i] = dPacket[i+20];
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

//  デジタルインプットをUARTRXとして使えるプログラム
void conv_DI_UARTRX(void){
    UBYTE buf_dammy = 0x00;
    UBYTE buf = 0x00;
    
    getbit();
    while(FX614_RXD == 0){  //後々は割り込み処理．STARTbit待機
        for(UINT i=0;i<8;i++){
            getbit();
            buf_dammy = buf_dammy << 1;
            if(FX614_RXD == 0){
                buf_dammy = buf_dammy | bit_L;
            }else{
                buf_dammy = buf_dammy | bit_H;
            }
        }
        getbit();   //STOPbit用
        buf = reverse_bit8(buf_dammy);
        putch(buf);
    }
}

//  MNB,LNBの逆転処理するプログラム
UBYTE reverse_bit8(UBYTE x){
	x = ((x & 0x55) << 1) | ((x & 0xAA) >> 1);
	x = ((x & 0x33) << 2) | ((x & 0xCC) >> 2);
	return (x << 4) | (x >> 4);
}
// =======
//#include <xc.h>
////#include <PIC16F887.h>
//#include "UART.h"
//#include "Init_MPU.h"
//#include "Type_define.h"
//#include "time.h"
//#include "decode_AX25.h"
//#include "encode_AX25.h"
//
//void UART_TX(UINT,UINT);
//void waitByte(UINT,UINT);
//UINT getbit(void);
//void waitFlag(void);
//void getdata(void);
//void conv_DI_UARTRX(void);
//UBYTE reverse_bit8(UBYTE x);
//
//
//#define bit_H 0x01
//#define bit_L 0x00
//#define bit_dH 0x80
//#define bit_dL 0x00
//
//static UINT rcv_state = 0;
//UBYTE dPacket[50];
//UBYTE dData[32];
//UINT  dPacketnum = 0;
//UBYTE dfcshi, dfcslo;
//
//UINT getbit(void);
//void waitFlag(void);
//void getdata(void);
//UINT fcscheck(void);
//void putAX25(void);
//void conv_DI_UARTRX(void);
//
////  bit繧ｵ繝ｳ繝励Μ繝ｳ繧ｰ縺ｮ繧ｿ繧､繝溘Φ繧ｰ縺後★繧後↑縺?繧医≧bit蜿崎ｻ｢縺斐→縺ｫ蜷梧悄縺吶ｋ繝励Ο繧ｰ繝ｩ繝?
////  getbitloop縺悟牡縺ｨ繧ｷ繝薙い縺ｫ蜉ｹ縺?縺ｦ縺上ｋ縺九ｉ豕ｨ諢?(hspan繧ゅ∪縺ゅ∪縺ょ茜縺?縺ｦ縺上ｋ)  
//UINT getbit(void){
//    static UINT oldbit;
//    oldbit = FX614_RXD;
//    for(UINT i=0;i<getbitloop;i++){
//        if(FX614_RXD != oldbit){
//            __delay_us(hspan);
//            return 0;
//        }
//    }
//    return 1;
//}
//
////  Flag縺ｧ繧ｽ繝輔ヨ繧ｹ繧ｱ繝ｫ繝?&蜷梧悄縺吶ｋ繝励Ο繧ｰ繝ｩ繝?
//void waitFlag(void){
//    UINT flag_judge;
//    UBYTE buf = 0xff;
//    rcv_state = 0;
//    while(rcv_state < 2){
//        while(buf != Flag_AX25){    //wait for flag
//            flag_judge = getbit();
//            buf = buf << 1;         //LSB, MSB縺ｫ蠢懊§縺ｦ繝薙ャ繝医す繝輔ヨ縺ｮ譁ｹ蜷托ｼ恵it_H, bit_L繧貞､画峩?ｼ?
//            if(flag_judge == 0){
//                buf = buf | bit_L;
//            }else{
//                buf = buf | bit_H;
//            }
//        }
//        
//        //while謚懊￠縺溷ｾ後?ｮbuf縺ｯ繝輔Λ繧ｰ縺ｧ縺ｯ縺ｪ縺瞬call縺ｮ蜈磯?ｭ1繝舌う繝亥??ｼ弱ｄ繧頑婿縺ｯ驕輔≧縺ｻ縺?縺瑚憶縺?豌励′縺吶ｋ?ｼ?
//        //繝薙ャ繝郁ｩｰ繧√＆繧後◆繧峨◎縺ｮ蛻?髢馴＆縺医※繝舌ャ繝輔ぃ縺ｫ譬ｼ邏阪＠縺ｦ縺励∪縺?
//        while(buf == Flag_AX25){
//            for(UINT i=0;i<8;i++){
//                flag_judge = getbit();
//                buf = buf >> 1;
//                if(flag_judge == 0){
//                    buf = buf | bit_dL;
//                }else{
//                    buf = buf | bit_dH;
//                }
//            }
//        }
//        if(buf == (mycall[0] << 1)){
//            dPacket[dPacketnum] = buf;
//            dPacketnum ++;
//            for(UINT j=0;j<8;j++){
//                flag_judge = getbit();
//                buf = buf >> 1;
//                if(flag_judge == 0){
//                    buf = buf | bit_dL;
//                }else{
//                    buf = buf | bit_dH;
//                }
//            }
//            if(buf == (mycall[1] << 1)){
//                dPacket[dPacketnum] = buf;
//                dPacketnum ++;
//                for(UINT j=0;j<8;j++){
//                    flag_judge = getbit();
//                    buf = buf >> 1;
//                    if(flag_judge == 0){
//                        buf = buf | bit_dL;
//                    }else{
//                        buf = buf | bit_dH;
//                    }
//                }
//                if(buf == (mycall[2] << 1)){
//                    dPacket[dPacketnum] = buf;
//                    dPacketnum ++;
//                    for(UINT j=0;j<8;j++){
//                        flag_judge = getbit();
//                        buf = buf >> 1;
//                        if(flag_judge == 0){
//                            buf = buf | bit_dL;
//                        }else{
//                            buf = buf | bit_dH;
//                        }
//                    }
//                    if(buf == (mycall[3] << 1)){
//                        dPacket[dPacketnum] = buf;
//                        dPacketnum ++;
//                        for(UINT j=0;j<8;j++){
//                            flag_judge = getbit();
//                            buf = buf >> 1;
//                            if(flag_judge == 0){
//                                buf = buf | bit_dL;
//                            }else{
//                                buf = buf | bit_dH;
//                            }
//                        }
//                        if(buf == (mycall[4] << 1)){
//                            dPacket[dPacketnum] = buf;
//                            dPacketnum ++;
//                            for(UINT j=0;j<8;j++){
//                                flag_judge = getbit();
//                                buf = buf >> 1;
//                                if(flag_judge == 0){
//                                    buf = buf | bit_dL;
//                                }else{
//                                    buf = buf | bit_dH;
//                                }
//                            }
//                            if(buf == (mycall[5] << 1)){
//                                dPacket[dPacketnum] = buf;
//                                dPacketnum ++;
//                                rcv_state ++;
//                            }else{
//                                dPacketnum = 0;
//                            }
//                        }else{
//                            dPacketnum = 0;
//                        }
//                    }else{
//                        dPacketnum = 0;
//                    }
//                }else{
//                    dPacketnum = 0;
//                }
//            }else{
//                dPacketnum = 0;
//            }
//        }else{
//            dPacketnum = 0;
//        }
//        /**/
//        for(UINT j=0;j<8;j++){
//            flag_judge = getbit();
//            buf = buf >> 1;
//            if(flag_judge == 0){
//                buf = buf | bit_dL;
//            }else{
//                buf = buf | bit_dH;
//            }
//        }
//        dPacket[dPacketnum] = buf;
//        dPacketnum ++;
//        for(UINT j=0;j<8;j++){
//            flag_judge = getbit();
//            buf = buf >> 1;
//            if(flag_judge == 0){
//                buf = buf | bit_dL;
//            }else{
//                buf = buf | bit_dH;
//            }
//        }
//        if(buf == (ucall[0] << 1)){
//            dPacket[dPacketnum] = buf;
//            dPacketnum ++;
//            for(UINT j=0;j<8;j++){
//                flag_judge = getbit();
//                buf = buf >> 1;
//                if(flag_judge == 0){
//                    buf = buf | bit_dL;
//                }else{
//                    buf = buf | bit_dH;
//                }
//            }
//            if(buf == (ucall[1] << 1)){
//                dPacket[dPacketnum] = buf;
//                dPacketnum ++;
//                for(UINT j=0;j<8;j++){
//                    flag_judge = getbit();
//                    buf = buf >> 1;
//                    if(flag_judge == 0){
//                        buf = buf | bit_dL;
//                    }else{
//                        buf = buf | bit_dH;
//                    }
//                }
//                if(buf == (ucall[2] << 1)){
//                    dPacket[dPacketnum] = buf;
//                    dPacketnum ++;
//                    for(UINT j=0;j<8;j++){
//                        flag_judge = getbit();
//                        buf = buf >> 1;
//                        if(flag_judge == 0){
//                            buf = buf | bit_dL;
//                        }else{
//                            buf = buf | bit_dH;
//                        }
//                    }
//                    if(buf == (ucall[3] << 1)){
//                        dPacket[dPacketnum] = buf;
//                        dPacketnum ++;
//                        for(UINT j=0;j<8;j++){
//                            flag_judge = getbit();
//                            buf = buf >> 1;
//                            if(flag_judge == 0){
//                                buf = buf | bit_dL;
//                            }else{
//                                buf = buf | bit_dH;
//                            }
//                        }
//                        if(buf == (ucall[4] << 1)){
//                            dPacket[dPacketnum] = buf;
//                            dPacketnum ++;
//                            for(UINT j=0;j<8;j++){
//                                flag_judge = getbit();
//                                buf = buf >> 1;
//                                if(flag_judge == 0){
//                                    buf = buf | bit_dL;
//                                }else{
//                                    buf = buf | bit_dH;
//                                }
//                            }
//                            if(buf == (ucall[5] << 1)){
//                                dPacket[dPacketnum] = buf;
//                                dPacketnum ++;
//                                rcv_state ++;
//                            }else{
//                                dPacketnum = 0;
//                                rcv_state = 0;
//                            }
//                        }else{
//                            dPacketnum = 0;
//                            rcv_state = 0;
//                        }
//                    }else{
//                        dPacketnum = 0;
//                        rcv_state = 0;
//                    }
//                }else{
//                    dPacketnum = 0;
//                    rcv_state = 0;
//                }
//            }else{
//                dPacketnum = 0;
//                rcv_state = 0;
//            }
//        }else{
//            dPacketnum = 0;
//            rcv_state = 0;
//        }
//    }
//}
//
////  11111縺ｮ谺｡繝?繝溘?ｼ繝?繝ｼ繧ｿ蛻､螳壹〒縺阪ｋ繝励Ο繧ｰ繝ｩ繝?
///**/
//void getdata(void){
//    UBYTE buf = 0x00;
//    UINT  dstuff = 0;
//    UBYTE dammy_data;
//    UINT  bufstore_counter = 0;
//    UINT  dbitstatus = 0;
//    
//    while(rcv_state == 2){
//        dbitstatus = getbit();
//        if(dstuff == 5){
//            if(dbitstatus == 1){
//                rcv_state ++;
//                break;
//            }else{
//                dstuff = 0;
//            }
//        }else{
//            buf = buf >> 1;
//            if(dbitstatus == 0){
//                buf = buf | bit_dL;
//                dstuff = 0;
//            }else{
//                buf = buf | bit_dH;
//                dstuff ++;
//            }
//            bufstore_counter ++;
//            if(bufstore_counter == 8){  //繝?繝舌ャ繧ｯ逕ｨ?ｼ取悽譚･縺ｯ繝舌ャ繝輔ぃ縺ｫ縺溘ａ縺ｦ縺?縺擾ｼ?
//                bufstore_counter = 0;
//                dPacket[dPacketnum] = buf;
//                dPacketnum ++;
//            }
//        }
//    }
//}
//
//UINT fcscheck(void){
//    UBYTE bt,byte;//, dfcslo, dfcshi;
//    dfcslo = dfcshi = 0xff;
//    while(rcv_state == 3){
//        for(UINT i=0;i<dPacketnum-2;i++){      //dPacketnum縺ｯmycall,SSID,ucall,Control,PID,Data,FCS縺ｮbyte謨ｰ?ｼ?-2縺ｧFCS蛻?繧帝勁縺擾ｼ?
//            byte = dPacket[i];
//            for(UINT i=0;i<8;i++){
//                bt = byte & bit_H;
//                #asm
//                    BCF 03,0
//                    RRF _dfcshi,F
//                    RRF _dfcslo,F
//                #endasm
//                if(((STATUS & bit_H)^bt) == bit_H){
//                    dfcshi = dfcshi ^ 0x84;
//                    dfcslo = dfcslo ^ 0x08;
//                }
//                byte = byte >> 1;
//            }
//        }
//        dfcslo = dfcslo ^ 0xff;
//        dfcshi = dfcshi ^ 0xff;
//        
//        //  繝?繝舌ャ繧ｯ荳ｭ縺ｯ縺薙％縺ｯ繧ｳ繝｡繝ｳ繝医い繧ｦ繝医☆繧九⊇縺?縺瑚憶縺?縺九ｂ?ｼ?
//        if((dPacket[dPacketnum-2] == dfcslo) && (dPacket[dPacketnum-1] == dfcshi)){     //dPacketnum逡ｪ逶ｮ縺ｮ繝舌ャ繝輔ぃ縺ｯ繝輔Λ繧ｰ縺梧?ｼ邏阪＆繧後※縺励∪縺｣縺ｦ縺?繧九?ｯ縺?
//            //putAX25();
//            //putcrlf();
//            //dPacketnum = 0;
//            //rcv_state = 0;
//            return 1;
//        }else{
//            //put_error();
//            //putcrlf();
//            //dPacketnum = 0;
//            //rcv_state = 0;
//            return 0;
//        }
//    }
//}
//
//void ReceivePacket(void){
//    waitFlag();
//    getdata();
//    fcscheck();
//}
//
//UBYTE *ReceivePacket_data(void){
//    UINT fcschecker;
//    //UBYTE dData[];
//    
//    waitFlag();
//    getdata();
//    fcschecker = fcscheck();
//    
//    if(fcschecker == 1){
//        for(UINT i=4; i<dPacketnum-18; i++){    // i=0 -> i=4 ("ori1"縺ｮ谺｡縺九ｉ)
//            dData[i] = dPacket[i+16];
//        }
//        dPacketnum = 0;
//        rcv_state = 0;
//        return dData;
//    }else{
//        dPacketnum = 0;
//        rcv_state = 0;
//        return 0x00;
//    }
//}
//
//
//void putAX25(void){
//    for(UINT i=0;i<6;i++){
//        dPacket[i] = dPacket[i] >> 1;
//        dPacket[i+7] = dPacket[i+7] >> 1;
//    }
//    for(UINT i=0;i<6;i++){
//        putch(dPacket[i]);
//    }
//    putch('>');
//    for(UINT i=0;i<6;i++){
//        putch(dPacket[i+7]);
//    }
//    putch(':');
//    for(UINT i=0;i<dPacketnum-18;i++){
//        putch(dPacket[i+16]);
//    }
//    return;
//}
//
////  繝?繧ｸ繧ｿ繝ｫ繧､繝ｳ繝励ャ繝医ｒUARTRX縺ｨ縺励※菴ｿ縺医ｋ繝励Ο繧ｰ繝ｩ繝?
//void conv_DI_UARTRX(void){
//    UBYTE buf_dammy = 0x00;
//    UBYTE buf = 0x00;
//    
//    getbit();
//    while(FX614_RXD == 0){  //蠕後??縺ｯ蜑ｲ繧願ｾｼ縺ｿ蜃ｦ逅??ｼ惨TARTbit蠕?讖?
//        for(UINT i=0;i<8;i++){
//            getbit();
//            buf_dammy = buf_dammy << 1;
//            if(FX614_RXD == 0){
//                buf_dammy = buf_dammy | bit_L;
//            }else{
//                buf_dammy = buf_dammy | bit_H;
//            }
//        }
//        getbit();   //STOPbit?ｿｽp
//        buf = reverse_bit8(buf_dammy);
//        putch(buf);
//    }
//}
//
////  MNB,LNB縺ｮ騾?霆｢蜃ｦ逅?縺吶ｋ繝励Ο繧ｰ繝ｩ繝?
//UBYTE reverse_bit8(UBYTE x){
//	x = ((x & 0x55) << 1) | ((x & 0xAA) >> 1);
//	x = ((x & 0x33) << 2) | ((x & 0xCC) >> 2);
//	return (x << 4) | (x >> 4);
//>>>>>>> DH_dev:decode_AX25.c
//}