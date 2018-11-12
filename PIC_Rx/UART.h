/* 
 * File:   UART.h
 * Author: Reo
 *
 * Created on 2017/01/21, 18:51
 */


#ifndef UART_H
#define	UART_H


#include <xc.h> // include processor files - each processor file is guarded.  
#include "typeDefine.h"

//Baud Rate Setting
#define low_speed    0
#define high_speed   1
#define asynchronous 0
#define synchronous  1

void InitSerial(void);
void putChar(UBYTE c);                  // Writes a character to the serial port
void putString(UBYTE *);
UBYTE getChar(void);
void putCrLf(void);

void put_error(void);
void put_ok(void);
void kaigyou(void);
void NM_waddress(UBYTE, UBYTE, UBYTE);
void TXOBC_waddress(UBYTE, UBYTE, UBYTE);
void sendCommandByPointer(UBYTE*);
void sendCommand(UBYTE, UBYTE, UBYTE,UBYTE, UBYTE, UBYTE,UBYTE, UBYTE);
void WriteUART( UBYTE* );
void UART_buffer_clear(void);
void changeBaudRate(UBYTE,UBYTE,UBYTE);
void changeInterruptPermission(UBYTE, UBYTE);
void commandSwitchUART(UBYTE, UBYTE, UBYTE, UBYTE, UBYTE, UBYTE);

#endif	/* UART_H */

