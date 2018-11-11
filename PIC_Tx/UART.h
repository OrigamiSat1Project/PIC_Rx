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
void putch(UBYTE c);                  // Writes a character to the serial port
void putstr(UBYTE *);
UBYTE getch(void);
void putcrlf(void);

void put_error(void);
void put_ok(void);
void NM_waddress(UBYTE, UBYTE, UBYTE);

#ifdef	__cplusplus
}
#endif

#endif	/* UART_H */

