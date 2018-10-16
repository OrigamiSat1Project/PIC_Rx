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
#include "Type_define.h"
#include "time.h"

#ifndef ENCODE_AX25_H
#define	ENCODE_AX25_H

void SendPacketWithDataSize(UBYTE *,UINT);
void SendPacket(UBYTE *);

/*--for debug--*/
//void test_Packetmaker(UBYTE *);

#endif	/* ENCODE_AX25_H */

