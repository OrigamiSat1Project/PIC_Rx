
#include <xc.h>
#include <stdio.h>
#include "FMCW.h"
#include "EEPROM.h"
#include "pinDefine.h"
#include "I2C.h"
#include "CRC16.h"
#include "encode_AX25.h"

#include "time.h"

/*******************************************************************************
*Marco, Enum / Global data / Method 
******************************************************************************/
#define EEPROM_COMMAND_DATA_SIZE 32
#define MAX_DOWNLINK_DATA_SIZE 32

// #define SHORT_DELAYTIMES_FOR_MORSE     15  //80wpm
// #define MIDDLE_DELAYTIMES_FOR_MORSE    45
// #define LONG_DELAYTIMES_FOR_MORSE      105
#define SHORT_DELAYTIMES_FOR_MORSE     65800  //80wpm
#define MIDDLE_DELAYTIMES_FOR_MORSE    197400
#define LONG_DELAYTIMES_FOR_MORSE      460600

UBYTE commandData[EEPROM_COMMAND_DATA_SIZE];

int changeCharMorse (char);
char changeBinaryToChar(UBYTE);
void DevideDataAndChangeBinaryToChar (UBYTE, UBYTE*);
//void sendMorse(char*);
void CwDownlinkFR0(void);
void CwDownlinkFR1(void);
void CwDownlinkFR2(void);
void CwDownlinkFRXXX(void);

/*******************************************************************************
*Downlink Command
******************************************************************************/
void downlinkReceivedCommand(UBYTE B0Select, UBYTE addressHigh, UBYTE addressLow, UBYTE downlinlkTimes){
    UBYTE mainAddress;
    UBYTE subAddress;
    mainAddress = EEPROM_address | B0Select;
    subAddress = EEPROM_subaddress | B0Select;
    ReadDataFromEEPROM(mainAddress,addressHigh,addressLow, commandData,EEPROM_COMMAND_DATA_SIZE);
    if(crc16(0,commandData,29) == CRC_check(commandData,29)){
        commandData[31] = 0x0F;
    }else{
        ReadDataFromEEPROM(subAddress,addressHigh,addressLow, commandData,EEPROM_COMMAND_DATA_SIZE);
        if(crc16(0,commandData,29) == CRC_check(commandData,29)){
            commandData[31] = 0x6F;
        }else{
            commandData[31] = 0xFF;
        }
    }
    WriteCheckByteToEEPROMs(B0Select,addressHigh,addressLow, commandData[31]);
    __delay_us(200);
    FMPTT = 1;
    for(int sendCounter = 0; sendCounter < downlinlkTimes; sendCounter++){
        SendPacketWithDataSize(commandData,EEPROM_COMMAND_DATA_SIZE);
        __delay_ms(300);
    }
    FMPTT = 0;
    
    /*-------------------------------------------------*/
    if(commandData[0]=='T'){                //command target = PIC_TX
        //Task target
        if(commandData[2] == 't'){          //task target =  PIC_TX
        // Command type
            switch(commandData[3]){         //Process command type
                case 'm':/*get satellite mode*/
                    downlinkFMSignal(commandData[4], deviceOnOff_EEPROMAndB0Select, deviceOnOff_addressHigh, deviceOnOff_addressLow, commandData[5], deviceOnOff_DataSize);
                    break;               
                case 'p': /*power supply*/
                    commandSwitchPowerSupply(commandData[4], commandData[5], commandData[6], commandData[7]);
                    break;
                case 'n': /*radio unit*/
    //                commandSwitchFMCW(commandData[4], commandData[5], commandData[6], commandData[7], commandData[8], commandData[9]);
                    break;
                case 'i':/*I2C*/
                    //commandSwitchI2C(commandData[4], commandData[5], commandData[6], commandData[7]);
                    break;
                case 'u':/*UART*/
    //                commandSwitchUART(commandData[4], commandData[5], commandData[6], commandData[7], commandData[8], commandData[9]);
                    break;
                case 'w':/*WDT (watch dog timer)*/
    //                commandWDT(commandData[4], commandData[5], commandData[6]);
                    break;
                case 'h':/*update HK data (BAT_POS V) (HK = house keeping)*/
                    //TODO: write function directly here or in MPU.c
                    break;
                case 't':/*internal processing*/
    //                commandSwitchIntProcess(commandData[4], commandData[5], commandData[6]);
                    break;
                case 'C':/*downlink CW Signal*/
                    //downlinkCWSignal(commandData[4],commandData[5],commandData[6],commandData[7],commandData[8], commandData[9]);
                    break;
                case 'f':/*downlink FM Signal*/
                    downlinkFMSignal(commandData[4],commandData[5],commandData[6],commandData[7],commandData[8], commandData[9]);
                    break;
                default:
                    //TODO: error message
                    break;
            }
        }
        
    }
}

/*******************************************************************************
*Function : FM and CW downkink 
******************************************************************************/
/*
 *  Do not process anything (standby)
 *  Wait 5 processing
 */
void _NOP(void) {
    for(int i=0; i<5; i++){
        NOP();
    }
}

/*******************************************************************************
*FM
******************************************************************************/
void downlinkFMSignal(UBYTE EEPROM_select, UBYTE EEPROMAndB0Select, UBYTE addressHigh, UBYTE addressLow, UBYTE downlinlTimes,UBYTE DataSize){
    UBYTE readAddress;
    switch(EEPROM_select){
        case 0x00: //main EEPROM
            readAddress = EEPROM_address | EEPROMAndB0Select;
            break;
        case 0x01: //sub EEPROM
            readAddress = EEPROM_subaddress | EEPROMAndB0Select;
            break;
    }
    UBYTE readData[];
    ReadDataFromEEPROM(readAddress,addressHigh,addressLow, readData,DataSize);
    FMPTT = 1;
    __delay_ms(100);//TODO check time
    for(int sendCounter = 0; sendCounter < downlinlTimes; sendCounter++){
        SendPacketWithDataSize(readData,DataSize);
        __delay_ms(300);
    }
    FMPTT = 0;
}


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
    CWKEY = 1;
    __delay_us(SHORT_DELAYTIMES_FOR_MORSE);
    CWKEY = 0;
    __delay_us(SHORT_DELAYTIMES_FOR_MORSE);

    CWKEY = 1;
    __delay_us(SHORT_DELAYTIMES_FOR_MORSE);
    CWKEY = 0;
    __delay_us(SHORT_DELAYTIMES_FOR_MORSE);

    CWKEY = 1;
    __delay_us(SHORT_DELAYTIMES_FOR_MORSE);
    CWKEY = 0;
    __delay_us(SHORT_DELAYTIMES_FOR_MORSE);

    CWKEY = 1;
    __delay_us(MIDDLE_DELAYTIMES_FOR_MORSE);
    CWKEY = 0;
    __delay_us(SHORT_DELAYTIMES_FOR_MORSE);
}

/*
 *  translate char to morse signal
 *  arg     : char
 *  return  : bin (morse)
 *            ton -> 1 / tu ->111 / delay -> 0
 *  TODO    : need debug
 */
int changeCharMorse (char _c){
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

    putChar(0xFA);
    binary_data_high = binary_data >> 4;    //7654bit
    binary_data_low  = binary_data & 0x0F;  //3210bit
    putChar(binary_data_high);
    putChar(binary_data_low);
    
    char_data_highLow[0] = (changeBinaryToChar (binary_data_high));
    char_data_highLow[1] = (changeBinaryToChar (binary_data_low));
    putChar(0x00);
    putChar(char_data_highLow[0]);
    putChar(char_data_highLow[1]);
}

/*
 *  send morse
 *  arg     : char *
 *  return  : N/A
 *  TODO    : need debug
 */
void sendMorse(char *HK_Data){
    int data_size = sizeof(HK_Data)/sizeof(HK_Data[0]);
    putChar(0xAA);
    putChar(sizeof(HK_Data));
    putChar(0xBB);
    putChar(sizeof(HK_Data[0]));
    //int data_size = 4;
    for (int i = 0;i<data_size;i++){
        putChar(0xCC);
        putChar(i);
        int mo = changeCharMorse(HK_Data[i]);
        for (int n=0;n<19;n++){
            if(mo==0){
                break;
            } else if((mo&1)==1){
                CWKEY = 1;
                __delay_us(SHORT_DELAYTIMES_FOR_MORSE);
            } else {
                CWKEY = 0;
                __delay_us(SHORT_DELAYTIMES_FOR_MORSE);
            }
            mo=mo>>1;
        }
        CWKEY = 0;
        __delay_us(MIDDLE_DELAYTIMES_FOR_MORSE);
    }
}

void sendMorsenew(char *HK_Data,size_t data_size){
    for (int i = 0;i<data_size;i++){
        int mo = changeCharMorse(HK_Data[i]);
        for (int n=0;n<19;n++){
            if(mo==0){
                break;
            } else if((mo&1)==1){
                CWKEY = 1;
                __delay_us(SHORT_DELAYTIMES_FOR_MORSE);
            } else {
                CWKEY = 0;
                __delay_us(SHORT_DELAYTIMES_FOR_MORSE);
            }
            mo=mo>>1;
        }
        CWKEY = 0;
        __delay_us(MIDDLE_DELAYTIMES_FOR_MORSE);
    }
}



/*
 *  ReadOneByteDataFromEEPROMandSendMorse 
 *  1. read only 1byte data from EEPROM
 *  2. send a data by morse signal
 *  arg     : UBYTE
 *  return  : N/A
 *  TODO    : need debug
 */
void ReadOneByteDataFromEEPROMandSendMorse(UBYTE Address7Bytes, UBYTE high_address, UBYTE low_address){
    UBYTE read_data;
    UBYTE char_data_highLow[2];
    read_data = ReadEEPROM(Address7Bytes, high_address, low_address); 
    DevideDataAndChangeBinaryToChar (read_data, char_data_highLow);
    sendMorsenew(char_data_highLow,sizeof(char_data_highLow)/sizeof(char_data_highLow[0]));
}

/*
 *  ReadDatasFromEEPROMWithDataSizeAndSendMorse
 *  1. read one or more data from EEPROM
 *  2. send data by morse signal
 *  arg     : UBYTE
 *  return  : N/A
 *  TODO    : need debug
 */
void ReadDatasFromEEPROMWithDataSizeAndSendMorse(UBYTE Address7Bytes, UBYTE high_address, UBYTE low_address, UBYTE *ReadData, UINT EEPROMDataLength){
    UBYTE char_data_highLow[2];
    UBYTE send_data[];
    ReadDataFromEEPROM(Address7Bytes, high_address, low_address, ReadData, EEPROMDataLength); 
    putChar('1');
    putChar('1');
    putChar('1');
    for(int i=0;i<3;i++){
        putChar(ReadData[i]);
    }
    putChar('1');
    putChar('1');
    putChar('1');
    //***READ ME : if debug of this function is not success, please revive comment out!!!
//     for(UBYTE i=0; i<EEPROMDataLength; i++){
//         DevideDataAndChangeBinaryToChar (ReadData[i], char_data_highLow);
//         sendMorse(char_data_highLow);
//     }
  
    for(UBYTE i=0; i<EEPROMDataLength; i++){
        putChar(i);
        putChar(i);
        putChar(i);
        putChar(i);
        DevideDataAndChangeBinaryToChar (ReadData[i], char_data_highLow);
        sendMorsenew(char_data_highLow,sizeof(char_data_highLow)/sizeof(char_data_highLow[0]));
//        send_data[i] = char_data_highLow[0];
//        send_data[i+1] = char_data_highLow[1];
    }
}

/*
 *  ReadDatasFromEEPROMWithDataSizeAndSendMorseWithDownlinkTimes
 *  1. read one or more data from EEPROM
 *  2. send data by morse signal
 *  3. repeat step2 DownlinkTimes
 *  arg     : UBYTE
 *  return  : N/A
 *  TODO    : need debug
 */
void ReadDatasFromEEPROMWithDataSizeAndSendMorseWithDownlinkTimes(UBYTE Address7Bytes, UBYTE high_address, UBYTE low_address, UBYTE *ReadData, UINT EEPROMDataLength, UBYTE downlink_times){
    UBYTE char_data_highLow[2];
    UBYTE send_data[];
    ReadDataFromEEPROM(Address7Bytes, high_address, low_address, ReadData, EEPROMDataLength); 
    
    //***READ ME : if debug of this function is not success, please revive comment out!!! and You need to change the place of  send morse
    // for(UBYTE i=0; i<EEPROMDataLength; i++){
    //     DevideDataAndChangeBinaryToChar (ReadData[i], char_data_highLow);
    //     sendMorse(char_data_highLow);
    // }

    for(UBYTE i=0; i<(2*EEPROMDataLength); i++){
        DevideDataAndChangeBinaryToChar (ReadData[i], char_data_highLow);
        char_data_highLow[0] = send_data[i];
        char_data_highLow[1] = send_data[i+1];
        i++;
    }
    
    for(UBYTE downlink_counter =0; downlink_counter<downlink_times; downlink_counter++){
        sendMorse(send_data);
        __delay_us(LONG_DELAYTIMES_FOR_MORSE);
    }
}

/*******************************************************************************
Frame
******************************************************************************/
void CwDownlinkFR0(void){
    UBYTE satellite_name[4] = {'o', 'r', 'i', '1'};
    sendMorse(satellite_name,sizeof(satellite_name)/sizeof(satellite_name[0]));
}

void CwDownlinkFR1(void){
    UBYTE message[5] = {'H', 'E', 'L', 'L', 'O'};
    sendMorse(message,sizeof(message)/sizeof(message[0]));
}

void CwDownlinkFR2(void){
    //battery status (data from OBC)
    ReadOneByteDataFromEEPROMandSendMorse(EEPROM_address, batteryStatus_addressHigh, batteryStatus_addressLow);
    //satellite bus status (data from OBC)
    ReadOneByteDataFromEEPROMandSendMorse(EEPROM_address, satelliteBusStatus_addressHigh, satelliteBusStatus_addressLow);
    //satellite mode (data from COBC)
    ReadOneByteDataFromEEPROMandSendMorse(EEPROM_address, satelliteMode_addressHigh, satelliteMode_addressLow);
}

void CwDownlinkFR3(void){
    //OBC last command
    ReadOneByteDataFromEEPROMandSendMorse(EEPROM_address, OBC_LastCommand_addressHigh, OBC_LastCommand_addressLow);
    //RXCOBC last command
    ReadOneByteDataFromEEPROMandSendMorse(EEPROM_address, RXCOBC_LastCommand_addressHigh, RXCOBC_LastCommand_addressLow);
    //TXCOBC last command
    ReadOneByteDataFromEEPROMandSendMorse(EEPROM_address, TXCOBC_LastCommand_addressHigh, TXCOBC_LastCommand_addressLow);
}

/*******************************************************************************
/**Main : CW downlink
 * ---
 * FR0  :  satellite name('ori') + satellite number('1')
 * FR1  :  message('HELLO')
 * FR2  :  satellite status(battery status + satellite bus status + satellite mode)
 * FR3  :  command
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
    CwDownlinkFR3();
    delay_s(10);
}