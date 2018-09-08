#include <xc.h>
#include "UART.h"
#include "Type_define.h"
#include "time.h"
#include "CRC16.h"

void Init_SERIAL(void){
    //SPBRG  = 10;                   // boudrate is  14400 bps
    SPBRG  = 0;                     // boudrate is  115200 bps
    BRGH   = 0;                   	// Fast baudrate
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

UBYTE getch(void){
    /**/
	if(FERR || OERR) // If over run error, then reset the receiver
	{
//        if(CREN = 0){
//            do{
//                CREN = 1; 
//            }while(CREN = 1);
//        }
        CREN = 0;
        NOP();
        CREN = 1;
    }
	while(!RCIF);  
	return RCREG;
}

void putch(UBYTE byte){
    while(!TXIF);  
	TXREG = byte;
}

void putString(UBYTE *x)
{
    while(*x != '\0'){
        putch(*x);
        x++;
    }
}

void putcrlf(void){
    putch('\r');
    putch('\n');
}

void put_error(void){
    putch('E');
    putch('R');
    putch('R');
    putch('O');
    putch('R');
    putch('!');
}

void put_ok(void){
    putch('O');
    putch('K');
    putch('!');
}

void NM_waddress(UBYTE NM_wad_header, UBYTE whigh_address, UBYTE wlow_address){
    putch(NM_wad_header);
    putch(whigh_address);
    putch(wlow_address);
}

void sendCommand(UBYTE TaskTarget, UBYTE CommandType, UBYTE Parameter1, UBYTE Parameter2, UBYTE Parameter3, UBYTE Parameter4, UBYTE Parameter5, UBYTE Parameter6){
    UBYTE Command[10];
    int UARTCommandSize;
    UARTCommandSize = 10;
    UWORD CRC;
    Command[0] = TaskTarget;
    Command[1] = CommandType;
    Command[2] = Parameter1;
    Command[3] = Parameter2;
    Command[4] = Parameter3;
    Command[5] = Parameter4;
    Command[6] = Parameter5;
    Command[7] = Parameter6;
    CRC = crc16(0, Command, 8);
    Command[8] = CRC >> 8;
    Command[9] = CRC & 0x00FF;
    //putString(Command);
    for(int i = 0; i<UARTCommandSize;i++){
        putch(Command[i]);
        NOP();
    } 
}
