<<<<<<< HEAD:PIC_Rx/decode_AX25.h
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

//  AX25—p‚Ìƒtƒ‰ƒOãˆÊ4bit(Mnib)‚Æ‰ºˆÊ4bit(Lnib)
#define Flag_Mnib 7
#define Flag_Lnib 14
#define Flag_AX25[] "01111110"
#define getbitloop 62//20MHz:130  10MHz:66  ¼–³üü”g”İ’èŠÜ‚ß‚é‚Æ60‚ª“K³’lH
#define Flag_AX25 0x7e
#define flagcounter_max 9
// 1/25 
// 0x7E‚ğ”»’è‚Å‚«‚égetbitloop
// waitflag()        ‚¾‚Æ   91 -113
// conv_DI_UARTRX()  ‚¾‚Æ   111-132
// 1/28
// 833us¨111C720us¨100


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

=======
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

//  AX25ç”¨ã®ãƒ•ãƒ©ã‚°ä¸Šä½4bit(Mnib)ã¨ä¸‹ä½4bit(Lnib)
#define Flag_Mnib 7
#define Flag_Lnib 14
#define Flag_AX25[] "01111110"
#define getbitloop 60//20MHz:130  10MHz:66  è¥¿ç„¡ç·šå‘¨æ³¢æ•°è¨­å®šå«ã‚ã‚‹ã¨60ãŒé©æ­£å€¤ï¼Ÿ
#define Flag_AX25 0x7e
#define flagcounter_max 9
// 1/25 
// 0x7Eã‚’åˆ¤å®šã§ãã‚‹getbitloop
// waitflag()        :   91 -113
// conv_DI_UARTRX()  :   111-132
// 1/28
// 833us:111, 720us:100


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

>>>>>>> DH_dev:decode_AX25.h
