//<<<<<<< HEAD:PIC_Rx/decode_AX25.h
/* 
 * File:   decode_AX25.h
 * Author: Curie
 *
 * Created on 2017/01/23, 2:52
 */

#include <stdio.h>
#include <stdlib.h>
#include <xc.h>
#include <PIC16F887.h>
#include "UART.h"
#include "Init_MPU.h"
#include "Type_define.h"
#include "time.h"

#ifndef DECODE_AX25_H
#define	DECODE_AX25_H

//  AX25用のフラグ上位4bit(Mnib)と下位4bit(Lnib)
#define Flag_Mnib 7
#define Flag_Lnib 14
#define Flag_AX25[] "01111110"
#define getbitloop 62//20MHz:130  10MHz:66  西無線周波数設定含めると60が適正値？
#define Flag_AX25 0x7e
#define flagcounter_max 9
// 1/25 
// 0x7Eを判定できるgetbitloop
// waitflag()        だと   91 -113
// conv_DI_UARTRX()  だと   111-132
// 1/28
// 833us→111，720us→100


UINT getbit(void);
/*void waitFlag(void);
void getdata(void);
UINT fcscheck(void);
void putAX25(void);
void conv_DI_UARTRX(void);
UBYTE reverse_bit8(UBYTE x);*/

void ReceivePacket(void);
UBYTE *ReceivePacket_data(void);

#endif	/* DECODE_AX25_H */

//=======
///* 
// * File:   decode_AX25.h
// * Author: Curie
// *
// * Created on 2017/01/23, 2:52
// */
//
//#include <stdio.h>
//#include <stdlib.h>
//#include <xc.h>
//#include <PIC16F887.h>
//#include "UART.h"
//#include "Init_MPU.h"
//#include "Type_define.h"
//#include "time.h"
//
//#ifndef DECODE_AX25_H
//#define	DECODE_AX25_H
//
////  AX25逕ｨ縺ｮ繝輔Λ繧ｰ荳贋ｽ?4bit(Mnib)縺ｨ荳倶ｽ?4bit(Lnib)
//#define Flag_Mnib 7
//#define Flag_Lnib 14
//#define Flag_AX25[] "01111110"
//#define getbitloop 60//20MHz:130  10MHz:66  隘ｿ辟｡邱壼捉豕｢謨ｰ險ｭ螳壼性繧√ｋ縺ｨ60縺碁←豁｣蛟､?ｼ?
//#define Flag_AX25 0x7e
//#define flagcounter_max 9
//// 1/25 
//// 0x7E繧貞愛螳壹〒縺阪ｋgetbitloop
//// waitflag()        :   91 -113
//// conv_DI_UARTRX()  :   111-132
//// 1/28
//// 833us:111, 720us:100
//
//
//UINT getbit(void);
///*void waitFlag(void);
//void getdata(void);
//UINT fcscheck(void);
//void putAX25(void);
//void conv_DI_UARTRX(void);
//UBYTE reverse_bit8(UBYTE x);*/
//
//void ReceivePacket(void);
//UBYTE *ReceivePacket_data(void);
//
//#endif	/* DECODE_AX25_H */
//
//>>>>>>> DH_dev:decode_AX25.h
