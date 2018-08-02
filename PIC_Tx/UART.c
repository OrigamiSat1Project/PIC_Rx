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
    UBYTE RXDATA[];
    UBYTE EEPROMCmdData[32];
    UINT EEPROMCmdDataLength = 32;
    if (RCIF == 1) {
        for (int i = 0; i <= 7; i++){
            RXDATA[i] = getch();
            NOP();
        }
       
        if(crc16(0,RXDATA,6) == CRC_check(RXDATA,6){
            switch(RXDATA[1]){
                case 0x75:
                    UpLinkDownLink(RXDATA);
                    break;
                case 0x63:
                    CwDownLink(RXDATA);
                    break;
                case 0x66:
                    FmDownLink(RXDATA);
                    break;
                case 0x61:
                    Antenna(RXDATA);
                    break;
            }
        }else{
            ///コマンドCRCダメだった時の処理
        }
        
  
         /*
        }else if (RXDATA[0] == 0xCC && UHFstart==1){
            led_yellow = 1;
            RXDATA[1] = getch();
            RXDATA[2] = getch();
            RCIF = 0 ;

            __delay_ms(200);
            UBYTE EEPROMCmdData[];
            UINT EEPROMCmdDataLength;
            EEPROMCmdDataLength = 1;
            EEPROM_Read_b(EEPROM_address,RXDATA[1],RXDATA[2], EEPROMCmdData,EEPROMCmdDataLength);
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
        
        //FM_photo_downlink
        }else if (RXDATA[0] == 0xEE && UHFstart==1){
            led_yellow = 1;
            RXDATA[1] = getch();
            RXDATA[2] = getch();
            RXDATA[3] = getch();//No. of data
            RXDATA[4] = getch();//No. of data
            RCIF = 0 ;
            
            short int PhotoAdd;
            PhotoAdd = RXDATA[1] * 256 + RXDATA[2];
            short int RestPhotoSize;
            RestPhotoSize = RXDATA[3] * 256 + RXDATA[4];
            
            while (RestPhotoSize > 0) {
                __delay_ms(50);
                UBYTE Packet[32];
                UINT PacketSize;
                PacketSize = 32;
                UBYTE PhotoHAdd;
                UBYTE PhotoLAdd;
                PhotoHAdd = PhotoAdd / 256;
                PhotoLAdd = PhotoAdd % 256;
                EEPROM_Read_b(EEPROM_Maddress,PhotoHAdd,PhotoLAdd, Packet,PacketSize);
                __delay_ms(200);
                FMPTT = 1;
                CWKEY = 0;
//                for(int i = 0; i<20;i++){
//                    SendPacket(Packet);
//                    __delay_ms(250);
//                }
                for(int i = 0; i<32;i++){
                    putch(Packet[i]);
                    __delay_ms(10);
                }
                PhotoAdd = PhotoAdd + PacketSize;
                RestPhotoSize = RestPhotoSize - PacketSize;
            }
            
            FMPTT = 0;
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
    */
    }
}

void UplinkDownlink(UBYTE RXDATA[]){
    UINT B0_select = (UINT)RXDATA[2] & 0x80;
    UINT DownlinkTimes = (UINT)RXDATA[2] & 0x7F;
    switch(B0_select){
        case 0x00:
            EEPROM_Read_b(EEPROM_address,RXDATA[3],RXDATA[4], EEPROMCmdData,EEPROMCmdDataLength);
            if(crc16(0,EEPROMCmdData,29) == CRC_check(EEPROMDATA,29)){
                EEPROMCmdData[31] = 0x0F;
            }else{
                EEPROM_Read_b(EEPROM_subaddress,RXDATA[3],RXDATA[4], EEPROMCmdData,EEPROMCmdDataLength);
                if(crc16(0,EEPROMCmdData,29) == CRC_check(EEPROMDATA,29)){
                    EEPROMCmdData[31] = 0x6F;
                }else{
                    EEPROMCmdData[31] = 0xFF;
                }
            }
            break;
        case 0x80:
            EEPROM_Read_b(EEPROM_Maddress,RXDATA[3],RXDATA[4], EEPROMCmdData,EEPROMCmdDataLength);
            if(crc16(0,EEPROMCmdData,29) == CRC_check(EEPROMDATA,29)){
                EEPROMCmdData[31] = 0x0F;
            }else{
                EEPROM_Read_b(EEPROM_subMaddress,RXDATA[3],RXDATA[4], EEPROMCmdData,EEPROMCmdDataLength);
                if(crc16(0,EEPROMCmdData,29) == CRC_check(EEPROMDATA,29)){
                    EEPROMCmdData[31] = 0x6F;
                }else{
                    EEPROMCmdData[31] = 0xFF;
                }
            }
            break;
    }
    __delay_ms(200);
    FMPTT = 1;
    for(int SendCounter = 0; SendCounter < DownlinkTimes; SendCounter++){
        SendPacket(EEPROMCmdData);
        __delay_ms(300);
    }
}

void CwDownlink(UBYTE RXDATA[]){
    
}

void FmDownlink(UBYTE RXDATA[]){
    UINT B0_select = (UINT)RXDATA[2] & 0x80;
    UINT DownlinkTimes = (UINT)RXDATA[2] & 0x7F
    switch(B0_select){
        case 0x00:
            EEPROM_Read_b(EEPROM_address,RXDATA[3],RXDATA[4], EEPROMCmdData,EEPROMCmdDataLength);
            if(crc16(0,EEPROMCmdData,29) == CRC_check(EEPROMDATA,29)){
                EEPROMCmdData[31] = 0x0F;
            }else{
                EEPROM_Read_b(EEPROM_subaddress,RXDATA[3],RXDATA[4], EEPROMCmdData,EEPROMCmdDataLength);
                if(crc16(0,EEPROMCmdData,29) == CRC_check(EEPROMDATA,29)){
                    EEPROMCmdData[31] = 0x6F;
                }else{
                    EEPROMCmdData[31] = 0xFF;
                }
            }
            break;
        case 0x80:
            EEPROM_Read_b(EEPROM_Maddress,RXDATA[3],RXDATA[4], EEPROMCmdData,EEPROMCmdDataLength);
            if(crc16(0,EEPROMCmdData,29) == CRC_check(EEPROMDATA,29)){
                EEPROMCmdData[31] = 0x0F;
            }else{
                EEPROM_Read_b(EEPROM_subMaddress,RXDATA[3],RXDATA[4], EEPROMCmdData,EEPROMCmdDataLength);
                if(crc16(0,EEPROMCmdData,29) == CRC_check(EEPROMDATA,29)){
                    EEPROMCmdData[31] = 0x6F;
                }else{
                    EEPROMCmdData[31] = 0xFF;
                }
            }
            break;
    }
    __delay_ms(200);
    FMPTT = 1;
    for(int SendCounter = 0; SendCounter < DownlinkTimes; SendCounter++){
        SendPacket(EEPROMCmdData);
        __delay_ms(300);
}

void Antenna(UBYTE RXDATA[]){
    UINT CutTime;
    CutTime = (UINT)(RXDATA[2] << 8) + RXDATA[3];
    HEAT = 1;
    __delay_ms(CutTime);
    HEAT = 0;
}