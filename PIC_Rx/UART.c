#include <xc.h>
//#include <string.h>
#include <stdio.h>
#include "pinDefine.h"
#include "UART.h"
#include "typeDefine.h"
#include "time.h"
#include "CRC16.h"

void InitSerial(void){
    SPBRG  = 4;                   // boudrate is 115200 bps
    BRGH   = 1;                   	// slow baudrate
//    SPBRG  = 10;                   // boudrate is 14400 bps
//    BRGH   = 0;                   	// Fast baudrate
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

UBYTE getChar(void){        //TODO: add time out feature---finish
	if(FERR || OERR) // If over run error, then reset the receiver //FERR = Framin Error bit// OERR = Overrun Error bit
	{
        CREN = 0;                   //Continuous Receive Enable bit
        NOP();
        CREN = 1;
    }
    set_timer_counter_only_getChar(0);
	while(!RCIF){                   //USART Receive Interrupt Flag bit
        if(get_timer_counter_only_getChar() > 1000) break;
    }
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
    putString(Command);
}

//TODO:check
//Write UART
void WriteUART( UBYTE *TXDATA ){
    int command_size;
    command_size = 5 ;  //TODO:chage command size
    for ( int i=0; i<command_size; i++ ){
        putChar(TXDATA[i]); 
        NOP();
    }
}

void UART_buffer_clear(void){
    RCREG = 0;   //USART Receive Register
}

//TODO:check
int change_baud_rate( UBYTE command_baud_rate ){
    switch( command_baud_rate ){
        case '1':
            return 9600;
            break;
        case '2':
            return 19200;
            break;
        case '3':
            return 38400;
            break;
        case '4':
            return 57600;
            break;
        case '5':
            return 115200;
            break;
    }
}

//TODO:check
//TODO:SPBRG,BRGH,SYNC‚ªmain.c‚ÌInitSerial();‚Å‰Šú‰»‚³‚ê‚é‚¯‚Ç‘åä•v‚©
//UART_speed: high_speed = 1  / low_speed =0
//UART_type : synchronous = 1 / asynchronous = 0
//Data sheet : p113
void calculate_SPBRG(int baud_rate, UBYTE UART_speed, UBYTE UART_type){
    int spbrg;
    if ( UART_speed == high_speed ){
        spbrg = _XTAL_FREQ / ( 16 * baud_rate ) - 1;
        SPBRG = spbrg;
        BRGH = high_speed;
        SYNC = asynchronous;
    } else if ( UART_speed == low_speed && UART_type == asynchronous ){
        spbrg = _XTAL_FREQ / ( 64 * baud_rate ) - 1;
        SPBRG = spbrg;
        BRGH = low_speed;
        SYNC = asynchronous;
    } else if ( UART_speed == low_speed && UART_type == synchronous ){
        spbrg = _XTAL_FREQ / ( 4 * baud_rate ) - 1;
        SPBRG = spbrg;
        BRGH = low_speed;
        SYNC = synchronous;
    }
}

//process command data if the command type is UART
void commandSwitchUART(UBYTE command, UBYTE data1, UBYTE data2, UBYTE data3, UBYTE data4, UBYTE data5){ //TODO: different format for writedataUART
    
    int BaudRate;
    
    switch(command){    
        case 'w': //UART write
            //TODO: write method for UART writ
            WriteUART( data1 );  //TODO:change "data1" ”CˆÓ‚Ì”‚É‘Î‰ž‚Å‚«‚é‚æ‚¤‚É
            break;
        case 'c': //UART buffer clear
            //TODO: write method for UART buffer clear---finish?
            UART_buffer_clear();
            break;
        case 'b': //change UART baud rate
            //TODO: write method for change UART baud rate---finish?
            //TODO: check
            BaudRate = change_baud_rate(data1);       //data1:command_baud_rate
            calculate_SPBRG(BaudRate, data2, data3);  //data2:UART_speed / data3:UART_type
            break;
        case 'i': //interrupt permission
            //TODO: write method for interrupt permission
            break;
        default:
            //TODO: error message
            break;
    }
}