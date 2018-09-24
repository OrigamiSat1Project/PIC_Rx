#include <xc.h>
//#include <string.h>
#include <stdio.h>
#include "pinDefine.h"
#include "UART.h"
#include "typeDefine.h"
#include "time.h"
#include "CRC16.h"

void InitSerial(void){
//    SPBRG  = 10;                   // boudrate is 14400 bps
//    BRGH   = 0;                   	// Fast baudrate
    SPBRG  = 4;                     // baudrate is 115200 bps
    BRGH   = 1;                     // slow baudrate
	SYNC   = 0;						// Asynchronous
	SPEN   = 1;						// Enable serial port pins
	CREN   = 1;						// Enable reception
	SREN   = 0;						// No effect
    RCIF   = 0;                     // RX frag Reset
	TXIE   = 0;						// Disble tx interrupts
	RCIE   = 1;						// Enable rx interrupts
	TX9    = 0;						// 8-bit transmission
	RX9    = 0;						// 8-bit reception
	TXEN   = 0;						// Reset transmitter
	TXEN   = 1;						// Enable the transmitter
}

UBYTE getChar(void){        //TODO: add time out feature
	if(FERR || OERR) // If over run error, then reset the receiver //FERR = Framin Error bit// OERR = Overrun Error bit
	{
        CREN = 0;                   //Continuous Receive Enable bit
        NOP();
        CREN = 1;
    }
	while(!RCIF);                   //USART Receive Interrupt Flag bit
	return RCREG;                   //USART Receive Data Register
}

void putChar(UBYTE byte){
    while(!TXIF);                   //USART Transmit Interrupt Flag bit
	TXREG = byte;                   //USART Transmit Data Register
}

void putString(UBYTE *x)
{
    while(*x != '\0'){
        putChar(*x);
        x++;
    }
}
//starts new line for debugging
void putCrLf(void){
    putChar('\r');//Carriage return
    putChar('\n');//Line feed
}

void putError(void){
    putChar('E');
    putChar('R');
    putChar('R');
    putChar('O');
    putChar('R');
    putChar('!');
}

void putOk(void){
    putChar('O');
    putChar('K');
    putChar('!');
}

void NM_waddress(UBYTE NM_wad_header, UBYTE whigh_address, UBYTE wlow_address){ //TODO change name from NM to OBC
    putChar(NM_wad_header);
    putChar(whigh_address);
    putChar(wlow_address);
}

void TXOBC_waddress(UBYTE TXOBC_wad_header, UBYTE whigh_address, UBYTE wlow_address){
    __delay_ms(50);
    putChar(TXOBC_wad_header);
    __delay_ms(50);
    putChar(whigh_address);
    __delay_ms(50);
    putChar(wlow_address);
    __delay_ms(50);
}

void sendCommand(UBYTE TaskTarget, UBYTE CommandType, UBYTE Parameter1, UBYTE Parameter2, UBYTE Parameter3, UBYTE Parameter4){
    UBYTE Command[8];
    UWORD CRC;
    Command[0] = TaskTarget;
    Command[1] = CommandType;
    Command[2] = Parameter1;
    Command[3] = Parameter2;
    Command[4] = Parameter3;
    Command[5] = Parameter4;
    CRC = crc16(0, Command, 6);
    Command[6] = CRC >> 8;
    Command[7] = CRC && 0x00FF;
    //putString(Command);
    for(int i =0; i<8;i++){
        putChar(Command[i]);
        NOP();
    }
}