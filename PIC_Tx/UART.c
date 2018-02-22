#include <xc.h>
#include "UART.h"
#include "Type_define.h"
#include "EEPROM.h"
#include "encode_AX25.h"
#include "I2C.h"

void Init_SERIAL(void){
    SPBRG  = 10;                   // boudrate is 1200 bps
    GIE    = 1;
    PEIE   = 1;
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
//    char RXD;
//    RXD = RCREG;
//	return RXD;
    return RCREG;
}

void putch(UBYTE byte){
    while(!TXIF);  
	TXREG = byte;
}




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
//void put_error(void){
//    putch('E');
//    putch('R');
//    putch('R');
//    putch('O');
//    putch('R');
//    putch('!');
//}
//
//void put_ok(void){
//    putch('O');
//    putch('K');
//    putch('!');
//}

//void NM_waddress(UBYTE NM_wad_header, UBYTE whigh_address, UBYTE wlow_address){
//    putch(NM_wad_header);
//    putch(whigh_address);
//    putch(wlow_address);
//}

void interrupt InterReceiver( void ){
    UBYTE RXDATA[32];
    if (RCIF == 1) {
//        led_yellow = 1;
        
        RXDATA[0] = getch();
        
        if (RXDATA[0] == 0x74){
            led_yellow = 1;
            RXDATA[1] = getch();
            RXDATA[2] = getch();
            RCIF = 0 ;

            __delay_ms(200);
            UBYTE EEPROMCmdData[1];
            UINT EEPROMCmdDataLength;
            EEPROM_Read(EEPROM_address,RXDATA[1],RXDATA[2], EEPROMCmdData,&EEPROMCmdDataLength);
            FMPTT = 1;
            CWKEY = 0;
    //          UBYTE SendData[3];
    //          for (int i = 0;i<3;i++){
    //              SendData[i] = EEPROMData[i];
    //          }
    //          SendData = EEPROMData;
            for(int i = 0; i<5;i++){
    //            SendPacket(SendData);
                SendPacket(EEPROMCmdData);
                __delay_ms(200);
            }
            FMPTT = 0;
            led_yellow = 0;
        }else{
            RCIF = 0 ;
//            led_yellow = 0;
        }
    }
}
