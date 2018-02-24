#include <xc.h>
#include "UART.h"
#include "Type_define.h"
#include "EEPROM.h"
#include "encode_AX25.h"
#include "I2C.h"
#include "CW.h"
#include "WDT.h"

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
    volatile static int intr_counter;
    if (RCIF == 1) {
        UBYTE RXDATA[32];
//        led_yellow = 1;
        
        RXDATA[0] = getch();
        
        if (RXDATA[0] == 0x74){
            led_yellow = 1;
            RXDATA[1] = getch();
            RXDATA[2] = getch();
            RCIF = 0 ;

            __delay_ms(200);
            UBYTE EEPROMCmdData[6];
            UINT EEPROMCmdDataLength;
            EEPROMCmdDataLength = 6;
            EEPROM_Read2(EEPROM_address,RXDATA[1],RXDATA[2], EEPROMCmdData,EEPROMCmdDataLength);
            __delay_ms(200);
            FMPTT = 1;
            CWKEY = 0;
//            UBYTE EEPROMTestData[5];
//            EEPROMTestData[0] = 'u';
//            EEPROMTestData[0] = EEPROMCmdData[0];
//            EEPROMTestData[1] = EEPROMCmdData[1];
//            EEPROMTestData[2] = EEPROMCmdData[2];
//            EEPROMTestData[3] = EEPROMCmdData[3];
//            EEPROMTestData[4] = EEPROMCmdData[4];
            
//            EEPROMTestData[1] = 'n';
//            EEPROMTestData[2] = 'k';
//            EEPROMTestData[3] = 'o';
//            EEPROMTestData[4] = 0x0d;
            for(int i = 0; i<5;i++){
                SendPacket(EEPROMCmdData);
                __delay_ms(300);
//                SendPacket(EEPROMTestData);
//                __delay_ms(300);
            }
            FMPTT = 0;
            led_yellow = 0;
        }else if (RXDATA[0] == 0xCC && UHFstart==1){
            led_yellow = 1;
            RXDATA[1] = getch();
            RXDATA[2] = getch();
            RCIF = 0 ;

            __delay_ms(200);
            UBYTE EEPROMCmdData[1];
            UINT EEPROMCmdDataLength;
            EEPROMCmdDataLength = 1;
            EEPROM_Read2(EEPROM_address,RXDATA[1],RXDATA[2], EEPROMCmdData,EEPROMCmdDataLength);
            __delay_ms(200);
            FMPTT = 1;
            CWKEY = 0;
            for(int i = 0; i<5;i++){
                SendPacket(EEPROMCmdData);
                __delay_ms(300);
            }
            FMPTT = 0;
            led_yellow = 0;
        }else if (RXDATA[0] == 0xDD && UHFstart==1){
            led_yellow = 1;
            RXDATA[1] = getch();
            RCIF = 0 ;

            __delay_ms(200);
            if (RXDATA[1] == 0xDD && UHFstart==1){
                __delay_ms(200);
                FMPTT = 0;
                for (int i = 0; i< 5;i++){
                    CWKEY = 1;
                    __delay_ms(Morse_Short);
                    CWKEY = 0;
                    __delay_ms(Morse_Short);

                    CWKEY = 1;
                    __delay_ms(Morse_Short);
                    CWKEY = 0;
                    __delay_ms(Morse_Short);

                    CWKEY = 1;
                    __delay_ms(Morse_Short);
                    CWKEY = 0;
                    __delay_ms(Morse_Short);

                    CWKEY = 1;
                    __delay_ms(Morse_Long);
                    CWKEY = 0;
                    __delay_ms(Morse_Short);
                    __delay_ms(200);
                } 
                
            }
            
            led_yellow = 0;
        }else{
            RCIF = 0 ;
//            led_yellow = 0;
        }
    }else if(PIR1bits.TMR1IF == 1){
        TMR1 = TIMER_INTERVAL;  // ?????????
 
        intr_counter++;
        if (intr_counter >= 2) {
            CLRWDT();
            intr_counter = 0;
        }
 
        PIR1bits.TMR1IF = 0;    // ???????????
    }
}
