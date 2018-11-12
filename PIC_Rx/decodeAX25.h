/* 
 * File:   decode_AX25.h
 * Author: Reo
 *
 * Created on 2017/01/23, 2:52
 */

//#include <stdio.h>
//#include <stdlib.h>
#include <xc.h>


#ifndef DECODE_AX25_H
#define	DECODE_AX25_H

#define GET_BIT_WAIT_LOOP 62           //Sets the wait time for a change //20MHz:130  10MHz:66 //TODO: check runtime
#define FLAG_AX25 0x7e
#define PACKET_SIZE 52
#define DATA_SIZE 30

//UBYTE commandData[DATA_SIZE];             //only information byte of uplink command


UINT getBit(void);
void receiveDataPacket(UBYTE*);

#endif	/* DECODE_AX25_H */