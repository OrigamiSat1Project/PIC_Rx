#include <xc.h>
//#include <string.h>
#include <stdio.h>
#include "pinDefine.h"
#include "UART.h"
#include "typeDefine.h"
#include "time.h"
#include "CRC16.h"
#include "OkError.h"

void InitSerial(void){
    // SPBRG  = 4;                   // boudrate is 115200 bps
    // BRGH   = 1;                   	// slow baudrate
    SPBRG  = 10;                   // boudrate is 14400 bps
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
// void putCrLf(void){
//     putChar('\r');//Carriage return
//     putChar('\n');//Line feed
// }

void put_error(void){
    putChar('E');
    putChar('R');
    putChar('R');
    putChar('O');
    putChar('R');
    putChar('!');
}

void put_ok(void){
    putChar('O');
    putChar('K');
    putChar('!');
}

//void NM_waddress(UBYTE NM_wad_header, UBYTE whigh_address, UBYTE wlow_address){ //TODO change name from NM to OBC
//    putChar(NM_wad_header);
//    putChar(whigh_address);
//    putChar(wlow_address);
//}
//
//void TXOBC_waddress(UBYTE TXOBC_wad_header, UBYTE whigh_address, UBYTE wlow_address){
//    __delay_ms(50);
//    putChar(TXOBC_wad_header);
//    __delay_ms(50);
//    putChar(whigh_address);
//    __delay_ms(50);
//    putChar(wlow_address);
//    __delay_ms(50);
//}

void sendCommandByPointer(UBYTE* Parameter){
    UBYTE Command[10];
    UWORD CRC;
    Command[0] = Parameter[0];
    Command[1] = Parameter[1];
    Command[2] = Parameter[2];
    Command[3] = Parameter[3];
    Command[4] = Parameter[4];
    Command[5] = Parameter[5];
    Command[6] = Parameter[6];
    Command[7] = Parameter[7];
    CRC = crc16(0, Command, 8);
    Command[8] = (UBYTE)(CRC >> 8);
    Command[9] = (UBYTE)(CRC & 0x00FF);
    
    for(UBYTE i=0; i<10; i++){
        putChar(Command[i]);
    }        
}

void sendCommand(UBYTE TaskTarget, UBYTE CommandType, UBYTE Parameter1,UBYTE Parameter2,UBYTE Parameter3,UBYTE Parameter4,UBYTE Parameter5,UBYTE Parameter6){
    UBYTE Command[10];
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
    Command[8] = (UBYTE)(CRC >> 8);
    Command[9] = (UBYTE)(CRC & 0x00FF);
//    putString(Command);
    for(UBYTE i=0; i<10; i++){
    putChar(Command[i]);
    }
}

//TODO:check
//Write UART
void WriteUART( UBYTE *TXDATA ){
    UBYTE command_size;
    command_size = 5 ;  //TODO:chage command size
    for ( UBYTE i=0; i<command_size; i++ ){
        putChar(TXDATA[i]); 
        NOP();
    }
}

void UART_buffer_clear(void){
    RCREG = 0;   //USART Receive Register
}

void changeBaudRate(UBYTE type_select,UBYTE SPBRG_number,UBYTE BRGH_number){
    if(type_select=='h'){ //115200bps
        SPBRG  = 4;                   
        BRGH   = 1;   
    } else if(type_select=='l'){ //14400bps
        SPBRG  = 10;                   
        BRGH   = 0;  
    } else if(type_select=='a'){
        SPBRG  = SPBRG_number;                   
        BRGH   = BRGH_number;            
    } else {
        //TODO:add error
    }
}

/*
 *	change Interrupt Permission
 *	arg      :   GIE_status, PEIE_status
 *               GIE: Global Interrupt Enable bit / PEIE: Peripheral Interrupt Enable bit
 *	return   :   GIE_status : 1 -> Enables all unmasked interrupts, 0 -> Disables all interrupts
 *               PEIE_status : 1 -> Enables all unmasked peripheral interrupts, 0 -> Disables all peripheral interrupts
 *	TODO     :   debug
 *	FIXME    :   not yet
 *	XXX      :   not yet
 */
void changeInterruptPermission(UBYTE GIE_status, UBYTE PEIE_status){
    if (GIE_status == 0x01){
        INTCONbits.GIE  = 1;
    } else {
        INTCONbits.GIE  = 0;
    }
    
    if (PEIE_status == 0x01){
        INTCONbits.PEIE  = 1;
    } else {
        INTCONbits.PEIE  = 0;
    }
}

//process command data if the command type is UART
void commandSwitchUART(UBYTE command, UBYTE data1, UBYTE data2, UBYTE data3, UBYTE data4, UBYTE data5){ //TODO: different format for writedataUART
    
    int BaudRate;
    
    switch(command){    
        case 'w': //UART write
            //TODO: write method for UART writ
            WriteUART( data1 );  //TODO:change "data1" ??øΩ?øΩC??øΩ?øΩ”ÇÃêÔøΩ??øΩ?øΩ…ëŒâÔøΩ??øΩ?øΩ≈ÇÔøΩ??øΩ?øΩ??øΩ?øΩÊÇ§??øΩ?øΩ??øΩ?øΩ
            break;
        case 'c': //UART buffer clear
            //TODO: write method for UART buffer clear---finish?
            UART_buffer_clear();
            break;
        case 'b': //change UART baud rate
            changeBaudRate(data1,data2,data3);
            break;
        case 'i': //interrupt permission
            //TODO: write method for interrupt permission
            changeInterruptPermission(data1,data2);
            break;
        default:
            switchError(error_UART_commandSwitchUART);
            break;
    }
}