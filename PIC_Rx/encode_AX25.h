/* 
 * File:   encode_AX25.h
 * Author: Curie
 *
 * Created on 2017/04/02, 20:49
 */

#include <stdio.h>
#include <stdlib.h>
#include <xc.h>
#include <PIC16F887.h>
#include "UART.h"
#include "Init_MPU.h"
#include "Type_define.h"
#include "time.h"

#ifndef ENCODE_AX25_H
#define	ENCODE_AX25_H

#define mycall "JQ1YCZ"
#define ucall  "JQ1ZHX"
#define txd RB2

void SendPacket(void);
/*
//void SendByte(UBYTE);
//void flipout(void);
//void fcsbit(UBYTE);
//UINT Packetmaker(void);
//void test_Packetmaker(void);
*/

#ifdef	__cplusplus
extern "C" {
#endif




#ifdef	__cplusplus
}
#endif

#endif	/* ENCODE_AX25_H */

