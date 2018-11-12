#include <xc.h>
#include "UART.h"
#include "Type_define.h"
#include "encode_AX25.h"
#include "CRC16.h"
#include "time.h"
#include "I2C.h"

void Init_SERIAL(void){

    SPBRG  = 10;                   // boudrate is  14400 bps at BRGH = 0
//    SPBRG  = 4;                    // boudrate is 115200 bps at BRGH = 1
//    INTCONbits.GIE    = 1;
//    INTCONbits.PEIE   = 1;
    BRGH   = 0;                   	// Fast baudrate
//    BRGH   = 1;                   	// slow baudrate
	SYNC   = 0;						// Asynchronous
	SPEN   = 1;						// Enable serial port pins
	CREN   = 1;						// Enable reception
	SREN   = 0;						// No effect
    INTCON  = 0b11000000;
    PIR1    = 0b00100000;                     // RX frag Reset
    PIE1    = 0b00100000;
    PIE2    = 0b00000000;
    
//	PIE1bits.TXIE   = 0;						// Disble tx interrupts
//	PIE1bits.RCIE   = 1;						// Enable rx interrupts
	TX9    = 0;						// 8-bit transmission
	RX9    = 0;						// 8-bit reception
	TXEN   = 0;						// Reset transmitter
	TXEN   = 1;						// Enable the transmitter

}

UBYTE getChar(void){                //TODO: add time out feature
    /**/
	if(FERR || OERR) // If over run error, then reset the receiver
	{
        CREN = 0;
        NOP();
        CREN = 1;
    }
    
    UINT break_counter = 0;
    
//    while(RCIF != 1);
    
	while(!RCIF){
        break_counter ++;
        if(break_counter >= 100){
//            putChar(0xbb);
            NOP();
            break_counter = 0;
            break;
        }
    }
    RCIF = 0;
    return RCREG;
}

void putChar(UBYTE byte){
    while(!TXIF);  
	TXREG = byte;
}

void putString(UBYTE *x)
{
    while(*x != '\0'){
        putChar(*x);
        x++;
    }
}

void sendCommand(UBYTE TaskTarget, UBYTE CommandType, UBYTE Parameter1, UBYTE Parameter2, UBYTE Parameter3, UBYTE Parameter4, UBYTE Parameter5, UBYTE Parameter6){
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
    Command[8] = CRC >> 8;
    Command[9] = CRC & 0x00FF;
    putString(Command);
}

/*
 *	change Interrupt Permission
 *	arg      :   GIE_status, PEIE_status
 *               GIE: Global Interrupt Enable bit / PEIE: Peripheral Interrupt Enable bit
 *	return   :   GIE_status : 1 -> Enables all unmasked interrupts, 0 -> Disables all interrupts
 *               PEIE_status : 1 -> Enables all unmasked peripheral interrupts, 0 -> Disables all peripheral interrupts
 *	TODO     :   debug--->finish
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

/*---for debug---*/
void put_error(void){
   putChar('E');
   putChar('R');
   putChar('R');
   putChar('O');
   putChar('R');
   putChar('\r');
   putChar('\n');
}

void put_ok(void){
   putChar('O');
   putChar('K');
   putChar('\r');
   putChar('\n');
   
}

void put_lf(void){
   putChar('\r');
   putChar('\n');
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

void UARTbufferClear(void){
    RCREG = 0;   //USART Receive Register
}

void readEEPROMandUARTwrite(UBYTE slaveAddress, UBYTE highAddress, UBYTE lowAddress, UBYTE *ReadData, UBYTE dataLength){
    UWORD CRC;
    ReadDataFromEEPROM(slaveAddress, highAddress, lowAddress, ReadData, dataLength);
    for(UBYTE i=0;i<dataLength;i++){
        putChar(ReadData[i]);
        NOP();
    }
    CRC = crc16(0, ReadData, dataLength);
    putChar(CRC >> 8);
    NOP();
    putChar(CRC & 0x00FF);
}

void UARTwrite5byte(UBYTE data1,UBYTE data2,UBYTE data3,UBYTE data4,UBYTE data5){
    UBYTE data[7];
    UWORD CRC;
    data[0] = data1;
    data[1] = data2;
    data[2] = data3;
    data[3] = data4;
    data[4] = data5;
    CRC = crc16(0, data, 5);
    data[5] = CRC >> 8;
    data[6] = CRC & 0x00FF;
    for(UBYTE i=0; i<7; i++){
        putChar(data[i]);
    }
}

// UBYTE get3byte(void){                //TODO: add time out feature
//     /**/
// 	if(FERR || OERR) // If over run error, then reset the receiver
// 	{
//         CREN = 0;
//         NOP();
//         CREN = 1;
//     }
// 	while(RCIF != 0);
//     return RCREG;
// }

//void putstr(UBYTE *x)
//{
//    while(*x != '\0'){
//        putch(*x);
//        x++;
//    }
//}

//void putcrlf(void){
//    putch('\r');
//    putch('\n');
//}
//

//void NM_waddress(UBYTE NM_wad_header, UBYTE whigh_address, UBYTE wlow_address){
//    putch(NM_wad_header);
//    putch(whigh_address);
//    putch(wlow_address);
//}

//process command data if the command type is UART
void commandSwitchUART(UBYTE command, UBYTE data1, UBYTE data2, UBYTE data3, UBYTE data4, UBYTE data5){
    UBYTE ReadData[];
    switch(command){    
        case 'w': //UART write
            UARTwrite5byte(data1,data2,data3,data4,data5);      
            break;
        case 'e': //read EEPROM and send datas to RXCOBC
            //data1:slave address / data2:high address / data3:low address / data4:data size
            readEEPROMandUARTwrite(data1, data2, data3, ReadData, data4);
            break;
        case 'c': //UART buffer clear
            UARTbufferClear();
            break;
        case 'b': //change UART baud rate
            changeBaudRate(data1,data2,data3);
            break;
        case 'i': //interrupt permission
            changeInterruptPermission(data1,data2);
            break;
        default:
//            switchError(error_UART_commandSwitchUART);
            break;
    }
}
