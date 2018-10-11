
#include <xc.h>
#include <stdio.h>
#include "FMCW_for_TXPIC.h"
#include "EEPROM.h"
#include "pinDefine.h"
#include "I2C.h"
#include "CRC16.h"
#include "UART.h"
//#include "encode_AX25.h"

#include "time.h"

/*******************************************************************************
*Marco, Enum / Global data / Method 
******************************************************************************/
#define EEPROM_COMMAND_DATA_SIZE 32
#define MAX_DOWNLINK_DATA_SIZE 32

//TXPIC->CWHIGH=0 / RXPIC->CWLOW=1
#define CWHIGH 0
#define CWLOW  1

// #define SHORT_DELAYTIMES_FOR_MORSE     15  //80wpm
// #define MIDDLE_DELAYTIMES_FOR_MORSE    45
// #define LONG_DELAYTIMES_FOR_MORSE      105
#define SHORT_DELAYTIMES_FOR_MORSE     65800  //80wpm
#define MIDDLE_DELAYTIMES_FOR_MORSE    197400
#define LONG_DELAYTIMES_FOR_MORSE      460600
//#define ADD_BLANK_FOR_MORSE            LONG_DELAYTIMES_FOR_MORSE-MIDDLE_DELAYTIMES_FOR_MORSE

UBYTE commandData[EEPROM_COMMAND_DATA_SIZE];

long changeCharMorse (char);
char changeBinaryToChar(UBYTE);
void DevideDataAndChangeBinaryToChar (UBYTE, UBYTE*);
void CwDownlinkFR0(void);
void CwDownlinkFR1(void);
void CwDownlinkFR2(void);
void CwDownlinkFRXXX(void);

///*******************************************************************************
//*Downlink Command
//******************************************************************************/
//void downlinkReceivedCommand(UBYTE B0Select, UBYTE addressHigh, UBYTE addressLow, UBYTE downlinlkTimes){
//    UBYTE mainAddress;
//    UBYTE subAddress;
//    mainAddress = EEPROM_address | B0Select;
//    subAddress = EEPROM_subaddress | B0Select;
//    ReadDataFromEEPROM(mainAddress,addressHigh,addressLow, commandData,EEPROM_COMMAND_DATA_SIZE);
//    if(crc16(0,commandData,29) == CRC_check(commandData,29)){
//        commandData[31] = 0x0F;
//    }else{
//        ReadDataFromEEPROM(subAddress,addressHigh,addressLow, commandData,EEPROM_COMMAND_DATA_SIZE);
//        if(crc16(0,commandData,29) == CRC_check(commandData,29)){
//            commandData[31] = 0x6F;
//        }else{
//            commandData[31] = 0xFF;
//        }
//    }
//    WriteCheckByteToEEPROMs(B0Select,addressHigh,addressLow, commandData[31]);
//    __delay_us(200);
//    FMPTT = 1;
//    for(int sendCounter = 0; sendCounter < downlinlkTimes; sendCounter++){
//        SendPacketWithDataSize(commandData,EEPROM_COMMAND_DATA_SIZE);
//        __delay_ms(300);
//    }
//    FMPTT = 0;
//    
//    /*-------------------------------------------------*/
//    if(commandData[0]=='T'){                //command target = PIC_TX
//        //Task target
//        if(commandData[2] == 't'){          //task target =  PIC_TX
//        // Command type
//            switch(commandData[3]){         //Process command type
//                case 'm':/*get satellite mode*/
//                    downlinkFMSignal(commandData[4], deviceOnOff_EEPROMAndB0Select, deviceOnOff_addressHigh, deviceOnOff_addressLow, commandData[5], deviceOnOff_DataSize);
//                    break;               
//                case 'p': /*power supply*/
//                    commandSwitchPowerSupply(commandData[4], commandData[5], commandData[6], commandData[7]);
//                    break;
//                case 'n': /*radio unit*/
//    //                commandSwitchFMCW(commandData[4], commandData[5], commandData[6], commandData[7], commandData[8], commandData[9]);
//                    break;
//                case 'i':/*I2C*/
//                    //commandSwitchI2C(commandData[4], commandData[5], commandData[6], commandData[7]);
//                    break;
//                case 'u':/*UART*/
//    //                commandSwitchUART(commandData[4], commandData[5], commandData[6], commandData[7], commandData[8], commandData[9]);
//                    break;
//                case 'w':/*WDT (watch dog timer)*/
//    //                commandWDT(commandData[4], commandData[5], commandData[6]);
//                    break;
//                case 'h':/*update HK data (BAT_POS V) (HK = house keeping)*/
//                    //TODO: write function directly here or in MPU.c
//                    break;
//                case 't':/*internal processing*/
//    //                commandSwitchIntProcess(commandData[4], commandData[5], commandData[6]);
//                    break;
//                case 'C':/*downlink CW Signal*/
//                    //downlinkCWSignal(commandData[4],commandData[5],commandData[6],commandData[7],commandData[8], commandData[9]);
//                    break;
//                case 'f':/*downlink FM Signal*/
//                    downlinkFMSignal(commandData[4],commandData[5],commandData[6],commandData[7],commandData[8], commandData[9]);
//                    break;
//                default:
//                    //TODO: error message
//                    break;
//            }
//        }
//        
//    }
//}
//
///*******************************************************************************
//*Function : FM and CW downkink 
//******************************************************************************/
///*
// *  Do not process anything (standby)
// *  Wait 5 processing
// */
//void _NOP(void) {
//    for(int i=0; i<5; i++){
//        NOP();
//    }
//}
//
///*******************************************************************************
//*FM
//******************************************************************************/
//void downlinkFMSignal(UBYTE EEPROM_select, UBYTE EEPROMAndB0Select, UBYTE addressHigh, UBYTE addressLow, UBYTE downlinlTimes,UBYTE DataSize){
//    UBYTE readAddress;
//    switch(EEPROM_select){
//        case 0x00: //main EEPROM
//            readAddress = EEPROM_address | EEPROMAndB0Select;
//            break;
//        case 0x01: //sub EEPROM
//            readAddress = EEPROM_subaddress | EEPROMAndB0Select;
//            break;
//    }
//    UBYTE readData[];
//    ReadDataFromEEPROM(readAddress,addressHigh,addressLow, readData,DataSize);
//    FMPTT = 1;
//    __delay_ms(100);//TODO check time
//    for(int sendCounter = 0; sendCounter < downlinlTimes; sendCounter++){
//        SendPacketWithDataSize(readData,DataSize);
//        __delay_ms(300);
//    }
//    FMPTT = 0;
//}


/*******************************************************************************
/**Function : CW downlink
 * tu = 3ton
 * [ton] CWKEY = 1; -> SHORT_DELAYTIMES_FOR_MORSE -> CWKEY = 0; -> SHORT_DELAYTIMES_FOR_MORSE
 * [tu ] CWKEY = 1; -> MIDDLE_DELAYTIMES_FOR_MORSE -> CWKEY = 0; -> SHORT_DELAYTIMES_FOR_MORSE
 * [Interval between ton-tu / ton-ton / tu-tu] SHORT_DELAYTIMES_FOR_MORSE
 * [Interval between moji]                     MIDDLE_DELAYTIMES_FOR_MORSE
 * [Interval between tango]                    LONG_DELAYTIMES_FOR_MORSE
******************************************************************************/
/*
 *  send morse 'V' for test
 *  arg     : void
 *  return  : send morse singnal 'V' (ton-ton-ton-tu)
 */
void Morse_V(void){
    CWKEY = CWHIGH;
    __delay_us(SHORT_DELAYTIMES_FOR_MORSE);
    CWKEY = CWLOW;
    __delay_us(SHORT_DELAYTIMES_FOR_MORSE);

    CWKEY = CWHIGH;
    __delay_us(SHORT_DELAYTIMES_FOR_MORSE);
    CWKEY = CWLOW;
    __delay_us(SHORT_DELAYTIMES_FOR_MORSE);

    CWKEY = CWHIGH;
    __delay_us(SHORT_DELAYTIMES_FOR_MORSE);
    CWKEY = CWLOW;
    __delay_us(SHORT_DELAYTIMES_FOR_MORSE);

    CWKEY = CWHIGH;
    __delay_us(MIDDLE_DELAYTIMES_FOR_MORSE);
    CWKEY = CWLOW;
    __delay_us(MIDDLE_DELAYTIMES_FOR_MORSE);
}

/*
 *  send morse 'Y' for test
 *  arg     : void
 *  return  : send morse singnal 'Y' (tu-ton-tu-tu)
 */
void Morse_Y(void){
    CWKEY = CWHIGH;
    __delay_us(MIDDLE_DELAYTIMES_FOR_MORSE);
    CWKEY = CWLOW;
    __delay_us(SHORT_DELAYTIMES_FOR_MORSE);

    CWKEY = CWHIGH;
    __delay_us(SHORT_DELAYTIMES_FOR_MORSE);
    CWKEY = CWLOW;
    __delay_us(SHORT_DELAYTIMES_FOR_MORSE);

    CWKEY = CWHIGH;
    __delay_us(MIDDLE_DELAYTIMES_FOR_MORSE);
    CWKEY = CWLOW;
    __delay_us(SHORT_DELAYTIMES_FOR_MORSE);

    CWKEY = CWHIGH;
    __delay_us(MIDDLE_DELAYTIMES_FOR_MORSE);
    CWKEY = CWLOW;
    __delay_us(MIDDLE_DELAYTIMES_FOR_MORSE);
}

/*
 *  send morse 'J' for test
 *  arg     : void
 *  return  : send morse singnal 'J' (ton-tu-tu-tu)
 */
void Morse_J(void){
    CWKEY = CWHIGH;
    __delay_us(SHORT_DELAYTIMES_FOR_MORSE);
    CWKEY = CWLOW;
    __delay_us(SHORT_DELAYTIMES_FOR_MORSE);

    CWKEY = CWHIGH;
    __delay_us(MIDDLE_DELAYTIMES_FOR_MORSE);
    CWKEY = CWLOW;
    __delay_us(SHORT_DELAYTIMES_FOR_MORSE);

    CWKEY = CWHIGH;
    __delay_us(MIDDLE_DELAYTIMES_FOR_MORSE);
    CWKEY = CWLOW;
    __delay_us(SHORT_DELAYTIMES_FOR_MORSE);

    CWKEY = CWHIGH;
    __delay_us(MIDDLE_DELAYTIMES_FOR_MORSE);
    CWKEY = CWLOW;
    __delay_us(MIDDLE_DELAYTIMES_FOR_MORSE); 
}

/*
 *  translate char to morse signal
 *  arg     : char
 *  return  : bin (morse)
 *            ton -> 1 / tu ->111 / delay -> 0
 *  TODO    : need debug
 */
long changeCharMorse (char _c){
    switch(_c){
        case '0': return 0b1110111011101110111;
        case '1': return 0b11101110111011101;
        case '2': return 0b111011101110101;
        case '3': return 0b1110111010101;
        case '4': return 0b11101010101;
        case '5': return 0b101010101;
        case '6': return 0b10101010111;
        case '7': return 0b1010101110111;
        case '8': return 0b101011101110111;
        case '9': return 0b10111011101110111;
        case 'a':
        case 'A': return 0b11101;
        case 'b':
        case 'B': return 0b101010111;
        case 'c':
        case 'C': return 0b10111010111;
        case 'd':
        case 'D': return 0b1010111;
        case 'e':
        case 'E': return 0b1;
        case 'f':
        case 'F': return 0b101110101;
        case 'g':
        case 'G': return 0b101110111;
        case 'h':
        case 'H': return 0b1010101;
        case 'i':
        case 'I': return 0b101;
        case 'j':
        case 'J': return 0b1110111011101;
        case 'k':
        case 'K': return 0b111010111;
        case 'l':
        case 'L': return 0b101011101;
        case 'm':
        case 'M': return 0b1110111;
        case 'n':
        case 'N': return 0b10111;
        case 'o':
        case 'O': return 0b11101110111;
        case 'p':
        case 'P': return 0b10111011101;
        case 'q':
        case 'Q': return 0b1110101110111;
        case 'r':
        case 'R': return 0b1011101;
        case 's':
        case 'S': return 0b10101;
        case 't':
        case 'T': return 0b111;
        case 'u':
        case 'U': return 0b1110101;
        case 'v':
        case 'V': return 0b111010101;
        case 'w':
        case 'W': return 0b111011101;
        case 'x':
        case 'X': return 0b11101010111;
        case 'y':
        case 'Y': return 0b1110111010111;
        case 'z':
        case 'Z': return 0b10101110111;
        default : return 0;
    }
}

/*
 *  translate binary to char
 *  arg     : UBYTE
 *  return  : char
 *  TODO    : need debug
 */
char changeBinaryToChar(UBYTE _binary){
    switch(_binary){
        case 0x00 : return '0';
        case 0x01 : return '1';
        case 0x02 : return '2';
        case 0x03 : return '3';
        case 0x04 : return '4';
        case 0x05 : return '5';
        case 0x06 : return '6';
        case 0x07 : return '7';
        case 0x08 : return '8';
        case 0x09 : return '9';
        case 0x0A : return 'A';
        case 0x0B : return 'B';
        case 0x0C : return 'C';
        case 0x0D : return 'D';
        case 0x0E : return 'E';
        case 0x0F : return 'F';
        default   : return 'X'; //TODO:change 'X' to error messege    
    }
}

/*
 *  DevideDataAndChangeBinaryToChar 
 *  1. devide data (high/low) 
 *  2. translate binary to char
 *  arg     : binary
 *  return  : char 
 *  TODO    : need debug
 */
void DevideDataAndChangeBinaryToChar (UBYTE binary_data, UBYTE *char_data_highLow){
    UBYTE binary_data_high;
    UBYTE binary_data_low;

    binary_data_high = binary_data >> 4;    //7654bit
    binary_data_low  = binary_data & 0x0F;  //3210bit
//    putChar(binary_data_high);
//    putChar(binary_data_low);
    
    char_data_highLow[0] = (changeBinaryToChar (binary_data_high));
    char_data_highLow[1] = (changeBinaryToChar (binary_data_low));
//    putChar(char_data_highLow[0]);
//    putChar(char_data_highLow[1]);
}

/*
 *  send morse
 *  arg     : char *
 *  return  : N/A
 *  TODO    : need debug
 */
void sendMorse(char *HK_Data,size_t data_size){
    for (int i = 0;i<data_size;i++){
        long mo = changeCharMorse(HK_Data[i]);
        putChar(0xFF);
        for (int n=0;n<19;n++){
            if(mo==0){
                break;
            } else if((mo&1)==1){
                CWKEY = CWHIGH;
                __delay_us(SHORT_DELAYTIMES_FOR_MORSE);
            } else {
                CWKEY = CWLOW;
                __delay_us(SHORT_DELAYTIMES_FOR_MORSE);
            }
            mo=mo>>1;
        }
        CWKEY = CWLOW;
        __delay_us(MIDDLE_DELAYTIMES_FOR_MORSE);
    }
}



/*
 *  ReadOneByteDataFromEEPROMandSendMorse 
 *  1. read only 1byte data from EEPROM
 *  2. send a data by morse signal
 *  arg     : UBYTE
 *  return  : N/A
 *  TODO    : send morse debug-->finish
 *            add blank
 */
void ReadOneByteDataFromEEPROMandSendMorse(UBYTE Address7Bytes, UBYTE high_address, UBYTE low_address){
    UBYTE read_data;
    UBYTE char_data_highLow[2];
    read_data = ReadEEPROM(Address7Bytes, high_address, low_address); 
    DevideDataAndChangeBinaryToChar (read_data, char_data_highLow);
    sendMorse(char_data_highLow,sizeof(char_data_highLow)/sizeof(char_data_highLow[0]));
    // delay_us(ADD_BLANK_FOR_MORSE);
}

/*
*  ReadDatasFromEEPROMWithDataSizeAndSendMorse
*  1. read one or more data from EEPROM
*  2. send data by morse signal
*  arg     : UBYTE
*  return  : N/A
*  TODO    : send morse debug --->finish
 *           add blank
*/
void ReadDatasFromEEPROMWithDataSizeAndSendMorse(UBYTE Address7Bytes, UBYTE high_address, UBYTE low_address, UBYTE *ReadData, UINT EEPROMDataLength){
    UBYTE char_data_highLow[2];
    UBYTE send_data[];
    //forTXPIC
    // ReadDataFromEEPROM(Address7Bytes, high_address, low_address, ReadData, EEPROMDataLength); 
    //forRXPIC
    ReadDataFromEEPROMWithDataSize(Address7Bytes, high_address, low_address, ReadData, EEPROMDataLength); 
    putChar(0xB0);
    for(UBYTE i=0; i<EEPROMDataLength; i++){
        DevideDataAndChangeBinaryToChar (ReadData[i], char_data_highLow);
        sendMorse(char_data_highLow,sizeof(char_data_highLow)/sizeof(char_data_highLow[0]));
        // delay_us(ADD_BLANK_FOR_MORSE);
    }
    putChar(0xB1);
}
/*
*  ReadDatasFromEEPROMWithDataSizeAndSendMorseWithDownlinkTimes
*  1. read one or more data from EEPROM
*  2. send data by morse signal
*  3. repeat step2 DownlinkTimes
*  arg     : UBYTE
*  return  : N/A
*  TODO    : send morse debug-->finish
 *           add blank
*/
void ReadDatasFromEEPROMWithDataSizeAndSendMorseWithDownlinkTimes(UBYTE Address7Bytes, UBYTE high_address, UBYTE low_address, UBYTE *ReadData, UINT EEPROMDataLength, UBYTE downlink_times){
    UBYTE char_data_highLow[2];
    UBYTE send_data[];
    //forTXPIC
    // ReadDataFromEEPROM(Address7Bytes, high_address, low_address, ReadData, EEPROMDataLength); 
    //forRXPIC
    ReadDataFromEEPROMWithDataSize(Address7Bytes, high_address, low_address, ReadData, EEPROMDataLength); 

    for(UBYTE downlink_counter =0; downlink_counter<downlink_times; downlink_counter++){
        for(UBYTE i=0; i<EEPROMDataLength; i++){
            DevideDataAndChangeBinaryToChar (ReadData[i], char_data_highLow);
            sendMorse(char_data_highLow,sizeof(char_data_highLow)/sizeof(char_data_highLow[0]));
            // delay_us(ADD_BLANK_FOR_MORSE);
        }
    }
}

/*******************************************************************************
Frame
******************************************************************************/
void CwDownlinkFR0(void){
    UBYTE satellite_call_sign[6] = {'J', 'S', '1', 'Y', 'A', 'X'};
    sendMorse(satellite_call_sign, sizeof(satellite_call_sign)/sizeof(satellite_call_sign[0]));
}

void CwDownlinkFR1(void){
    //TODO: add meessage 
}

void CwDownlinkFR2(void){
    //TODO : add function for send satellite status
}

void CwDownlinkFRXXX(void){
    UBYTE end_sign[3] = {'E', 'N', 'D'};
    sendMorse(end_sign, sizeof(end_sign)/sizeof(end_sign[0]));
}

/*******************************************************************************
/**Main : CW downlink
 * ---
 * FR0  :  satellite call sign 'JS1YAX'
 * FR1  :  message 'XXX'
 * FR2  :  satellite status
 * FR3  :  
 * FRXX :  END
 * ---
 * interval between frames is 10 seconds (normalmode)
******************************************************************************/
void downlinkCWSignal(void){
    CwDownlinkFR0();
    delay_s(10);
    CwDownlinkFR1();
    delay_s(10);
    CwDownlinkFR2();
    delay_s(10);
    //TODO:add frame
    CwDownlinkFRXXX();
    delay_s(10);
}

void testForCwFunctions(void){
    UBYTE EEPROM_address = 0x50;
    UBYTE whigh_address = 0xE0;
    UBYTE wlow_address = 0x00;
//        FMPTT = 0; 
//        CWKEY = 1;

    //debug:send morse 'V'->'Y'->'J' -->finish
    Morse_V();
    __delay_us(LONG_DELAYTIMES_FOR_MORSE);
    Morse_Y();
    __delay_us(LONG_DELAYTIMES_FOR_MORSE);
    Morse_J();
    __delay_us(LONG_DELAYTIMES_FOR_MORSE);
//    debug send morse char--->finish
    //  UBYTE test_morse[4];
    // test_morse[0] = '1';
    // test_morse[1] = '2';
    // test_morse[2] = 'a';
    // test_morse[3] = 'b';
    // sendMorse(test_morse,sizeof(test_morse)/sizeof(test_morse[0]));
    
    //check 16shinsu
//    UBYTE test_morse[16];
//    test_morse[0] = '0';
//    test_morse[1] = '1';
//    test_morse[2] = '2';
//    test_morse[3] = '3';
//    test_morse[4] = '4';
//    test_morse[5] = '5';
//    test_morse[6] = '6';
//    test_morse[7] = '7';
//    test_morse[8] = '8';
//    test_morse[9] = '9';
//    test_morse[10] = 'A';
//    test_morse[11] = 'B';
//    test_morse[12] = 'C';
//    test_morse[13] = 'D';
//    test_morse[14] = 'E';
//    test_morse[15] = 'F';
//    sendMorse(test_morse,sizeof(test_morse)/sizeof(test_morse[0]));

    //check suji --> finish
//    UINT binary_data[5];
//    UBYTE char_data_highLow[2];
//    for (UINT i=0; i<5; i++){
//        binary_data[i]=i;
//        DevideDataAndChangeBinaryToChar (binary_data[i], char_data_highLow);
//        sendMorse(char_data_highLow,sizeof(char_data_highLow)/sizeof(char_data_highLow[0])); 
//        __delay_us(LONG_DELAYTIMES_FOR_MORSE);
//    }
    
    //debug:DevideDataAndChangeBinaryToChar--->finish
    // UBYTE binary_data = 0xa2;
    // UBYTE char_data_highLow[2]; 
    // DevideDataAndChangeBinaryToChar (binary_data, char_data_highLow);
    // sendMorse(char_data_highLow,sizeof(char_data_highLow)/sizeof(char_data_highLow[0])); 
    // Morse_Y();
    // binary_data = 0xb1;
    // DevideDataAndChangeBinaryToChar (binary_data, char_data_highLow);
    // sendMorse(char_data_highLow,sizeof(char_data_highLow)/sizeof(char_data_highLow[0]));
    // Morse_Y();

      
    //debug:send ReadOneByteDataFromEEPROMan dSendMorse -->finish
    putChar(0xA0);
    UBYTE TEST_DATA[3] = {0xa2, 0b10110001, 'C'};  //0b10110001 = 0xb1 / 0x43 = 'C'
//     UBYTE TEST_DATA[3] = {0xa2, 0b10110001, 0xab};  //0b10110001 = 0xb1 
//    UBYTE TEST_DATA[3] = {0xa2, 0xb1, 0xab};  
    WriteToEEPROM(EEPROM_address, whigh_address, wlow_address, TEST_DATA);
//    ReadOneByteDataFromEEPROMandSendMorse(EEPROM_address, whigh_address, wlow_address);
        
    //debug:ReadDatasFromEEPROMWithDataSizeAndSendMorse--->finish
    putChar(0xA1);
    UBYTE ReadData[];
    UBYTE size = 3;
    UBYTE sendtime = 5;
//    ReadDatasFromEEPROMWithDataSizeAndSendMorse(EEPROM_address, whigh_address, wlow_address, ReadData, size);
       
    //debug:ReadDatasFromEEPROMWithDataSizeAndSendMorseWithDownlinkTimes-->finish
    ReadDatasFromEEPROMWithDataSizeAndSendMorseWithDownlinkTimes(EEPROM_address, whigh_address, wlow_address, ReadData, size, sendtime); //morse signal 'T'-> 0x5F -> 0b10100111 -> 5times->success    
//         //FIXME:[finish]debug for downlink CW signal
//         /*---------------------------------------------------------------*/
}