/* 
 * File:   UART.h
 * Author: Curie
 *
 * Created on 2017/01/21, 18:51
 */


#ifndef UART_H
#define	UART_H

#include <xc.h> // include processor files - each processor file is guarded.  
#include "Type_define.h"

void Init_SERIAL(void);
void putChar(UBYTE c);                  // Writes a character to the serial port
//void putstr(UBYTE *);

//void putcrlf(void);

//void put_error(void);
//void put_ok(void);
//void NM_waddress(UBYTE, UBYTE, UBYTE);
void interrupt InterReceiver(void);
void UplinkDownlink(UBYTE RXDATA[]);
void CwDownlink(UBYTE RXDATA[]);
void FmDownlink(UBYTE RXDATA[]);
void Antenna(UBYTE RXDATA[]);
UBYTE getChar(void);
//void interrupt InterReceiver(void);
#ifdef	__cplusplus
}
#endif

#endif	/* UART_H */

