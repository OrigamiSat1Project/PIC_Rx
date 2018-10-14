
#include <xc.h>
#include <stdio.h>
#include "FMCW.h"
#include "EEPROM.h"
#include "pinDefine.h"
#include "I2C.h"
#include "CRC16.h"
#include "encode_AX25.h"
#include "Type_define.h"
#include "time.h"
#include "OkError.h"

/*******************************************************************************
*Marco, Enum / Global data / Method 
******************************************************************************/
#define EEPROM_COMMAND_DATA_SIZE 32
#define MAX_DOWNLINK_DATA_SIZE 32

#define CWHIGH 1
#define CWLOW 0

UBYTE commandData[EEPROM_COMMAND_DATA_SIZE];

/*******************************************************************************
*Downlink Command
******************************************************************************/
void downlinkReceivedCommand(UBYTE B0Select, UBYTE addressHigh, UBYTE addressLow, UBYTE downlinlkTimes){
    UBYTE mainAddress;
    UBYTE subAddress;
    mainAddress = EEPROM_address | B0Select;
    subAddress = EEPROM_subaddress | B0Select;
    ReadDataFromEEPROM(mainAddress,addressHigh,addressLow, commandData,EEPROM_COMMAND_DATA_SIZE);

    //TODO:is thhis corrrect?
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
        SendPacket(commandData);
        // SendPacketWithDataSize(commandData,EEPROM_COMMAND_DATA_SIZE);
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
                    downlinkFMSignal(satelliteMode_EEPROMAndB0Select, satelliteMode_addressHigh, satelliteMode_addressLow, commandData[4], satelliteMode_DataSize);
                    WriteLastCommandIdToEEPROM(commandData[1]);
                case 'C':/*downlink CW Signal*/
                    commandSwitchCWDownlink(commandData[4],commandData[5],commandData[6],commandData[7],commandData[8], commandData[9], commandData[10]);
                    WriteLastCommandIdToEEPROM(commandData[1]);
                    break;
                case 'f':/*downlink FM Signal*/
                    downlinkFMSignal(commandData[4],commandData[5],commandData[6],commandData[7],commandData[8]);
                    WriteLastCommandIdToEEPROM(commandData[1]);
                    break;           
                case 'p': /*power supply*/
                    commandSwitchPowerSupply(commandData[4], commandData[5], commandData[6], commandData[7]);
                    WriteLastCommandIdToEEPROM(commandData[1]);
                    break;
                case 'n': /*radio unit*/
    //                commandSwitchFMCW(commandData[4], commandData[5], commandData[6], commandData[7], commandData[8], commandData[9]);
                    WriteLastCommandIdToEEPROM(commandData[1]);
                    break;
                case 'i':/*I2C*/
                    //commandSwitchI2C(commandData[4], commandData[5], commandData[6], commandData[7]);
                    WriteLastCommandIdToEEPROM(commandData[1]);
                    break;
                case 'u':/*UART*/
    //                commandSwitchUART(commandData[4], commandData[5], commandData[6], commandData[7], commandData[8], commandData[9]);
                    WriteLastCommandIdToEEPROM(commandData[1]);
                    break;
                case 'w':/*WDT (watch dog timer)*/
    //                commandWDT(commandData[4], commandData[5], commandData[6]);             
                    WriteLastCommandIdToEEPROM(commandData[1]);
                    break;
                case 'h':/*update HK data (BAT_POS V) (HK = house keeping)*/
                    //TODO: write function directly here or in MPU.c
//                   commandSwitchHKdata(commandData[4], commandData[5], commandData[6], commandData[7]);
                    WriteLastCommandIdToEEPROM(commandData[1]);
                    break;
                case 't':/*internal processing*/
    //                commandSwitchIntProcess(commandData[4], commandData[5], commandData[6]);
                    WriteLastCommandIdToEEPROM(commandData[1]);
                    break;
                default:
                    switchError(error_FMCW_downlinkReceivedCommand);
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
void downlinkFMSignal(UBYTE EEPROMAndB0Select, UBYTE addressHigh, UBYTE addressLow, UBYTE downlinlTimes,UBYTE DataSize){
    UBYTE mainAddress;
    UBYTE subAddress;
    mainAddress = EEPROM_address | EEPROMAndB0Select;
    subAddress = EEPROM_subaddress | EEPROMAndB0Select;

    UBYTE readData[];
    ReadDataFromEEPROM(mainAddress,addressHigh,addressLow, readData,DataSize);
    
    //TODO:eror->read sub EEPROM
//    ReadDataFromEEPROM(subAddress,addressHigh,addressLow, readData,DataSize);

    FMPTT = 1;
    __delay_ms(100);//TODO check time
    for(int sendCounter = 0; sendCounter < downlinlTimes; sendCounter++){
        SendPacket(readData);
//         SendPacketWithDataSize(readData,DataSize);
        __delay_ms(300);
    }
    FMPTT = 0;
}

/*******************************************************************************
*CW swtich
******************************************************************************/
void commandSwitchCWDownlink(UBYTE type_select, UBYTE Address7bit, UBYTE high_address_forData, UBYTE low_address_forData, UBYTE downlink_times, UINT EEPROMDataLength_or_high_address_forDataSize, UBYTE low_address_forDataSize){
    UBYTE read_data_forCW[];
    switch(type_select){
        case 's':   //start CW downlink
            CWdownlinkStart();
            switchOk(ok_FMCW_commandSwitchCWDownlink_Start);
            break;
        case 0xaa:  //the size of data is specified by the command
            ReadDatasFromEEPROMWithDataSizeAndSendMorseWithDownlinkTimes(Address7bit, high_address_forData, low_address_forData, read_data_forCW, EEPROMDataLength_or_high_address_forDataSize, downlink_times);
            switchOk(ok_FMCW_commandSwitchCWDownlink_aa);
            break;
        case 0xbb:  //the size of data is written in EEPROM
            GetDatasizeAndReadDatasFromEEPROMWithDataSizeAndSendMorseWithDownlinkTimes(Address7bit, high_address_forData, low_address_forData, read_data_forCW, EEPROMDataLength_or_high_address_forDataSize, low_address_forDataSize, downlink_times);
            switchOk(ok_FMCW_commandSwitchCWDownlink_bb);
            break;
        default:
            switchError(error_FMCW_commandSwitchCWDownlink);
            break;

    }
}


/*******************************************************************************
/**Function : CW downlink
 * tu = 3ton
 * [ton] CWKEY = 1; -> SHORT_DELAYTIMES_FOR_MORSE -> CWKEY = 0; -> SHORT_DELAYTIMES_FOR_MORSE
 * [tu ] CWKEY = 1; -> MIDDLE_DELAYTIMES_FOR_MORSE -> CWKEY = 0; -> SHORT_DELAYTIMES_FOR_MORSE
 * [Interval between ton-tu / ton-ton / tu-tu] SHORT_DELAYTIMES_FOR_MORSE (=1ton)
 * [Interval between moji]                     MIDDLE_DELAYTIMES_FOR_MORSE (=3ton)
 * [Interval between tango]                    LONG_DELAYTIMES_FOR_MORSE (=7ton)
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

void sendMorse(char *HK_Data,size_t data_size){
    for (int i = 0;i<data_size;i++){
        int mo = changeCharMorse(HK_Data[i]);
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
 *  TODO    : need debug
 */
void ReadOneByteDataFromEEPROMandSendMorse(UBYTE Address7Bytes, UBYTE high_address, UBYTE low_address){
    UBYTE read_data;
    UBYTE char_data_highLow[2];
    read_data = ReadEEPROM(Address7Bytes, high_address, low_address); 
    DevideDataAndChangeBinaryToChar (read_data, char_data_highLow);
    sendMorse(char_data_highLow,sizeof(char_data_highLow)/sizeof(char_data_highLow[0]));
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
//    putChar('1');
//    putChar('1');
//    putChar('1');
//    for(int i=0;i<3;i++){
//        putChar(ReadData[i]);
//    }
//    putChar('1');
//    putChar('1');
//    putChar('1');
  
    for(UBYTE i=0; i<EEPROMDataLength; i++){
//        putChar(i);
//        putChar(i);
//        putChar(i);
//        putChar(i);
        DevideDataAndChangeBinaryToChar (ReadData[i], char_data_highLow);
        sendMorse(char_data_highLow,sizeof(char_data_highLow)/sizeof(char_data_highLow[0]));
        delay_us(ADD_BLANK_FOR_MORSE);
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
    
    for(UBYTE downlink_counter =0; downlink_counter<downlink_times; downlink_counter++){
        for(UBYTE i=0; i<EEPROMDataLength; i++){
            DevideDataAndChangeBinaryToChar (ReadData[i], char_data_highLow);
            sendMorse(char_data_highLow,sizeof(char_data_highLow)/sizeof(char_data_highLow[0]));
            delay_us(ADD_BLANK_FOR_MORSE);
        }
    }
}

void GetDatasizeAndReadDatasFromEEPROMWithDataSizeAndSendMorseWithDownlinkTimes(UBYTE Address7Bytes, UBYTE high_address, UBYTE low_address, UBYTE *ReadData, UINT high_address_forDataSize, UBYTE low_address_forDataSize, UBYTE downlink_times){
    UBYTE char_data_highLow[2];
    UBYTE send_data[];
    UBYTE EEPROMDataLength;
    EEPROMDataLength = ReadEEPROM(Address7Bytes,high_address_forDataSize,low_address_forDataSize);
    ReadDataFromEEPROM(Address7Bytes, high_address, low_address, ReadData, EEPROMDataLength); 
    
    for(UBYTE downlink_counter =0; downlink_counter<downlink_times; downlink_counter++){
        for(UBYTE i=0; i<EEPROMDataLength; i++){
            DevideDataAndChangeBinaryToChar (ReadData[i], char_data_highLow);
            sendMorse(char_data_highLow,sizeof(char_data_highLow)/sizeof(char_data_highLow[0]));
            delay_us(ADD_BLANK_FOR_MORSE);
        }
    }
}

/*******************************************************************************
/**Main : HK downlink
 * ---
 * FR0  :  satellite name('ori') + satellite number('1') + message('HELLO')
 * FR1  :  satellite status() which can be write by OBC and CIB
 * FR2  :  satellite status() which can be write by only OBC 
 * ---
 * interval between frames is 10 seconds (normalmode)
******************************************************************************/
void HKDownlink(UBYTE SatMode){
    
    switch(SatMode){
        case 0x50:/*------------Nominal Mode------------*/
            HKDownlinkFR0();
            // delay_s(10);
            HKDownlinkFR1();
            // delay_s(10);
            HKDownlinkFR2();
            // delay_s(10);
            break;
        case 0x60:/*------------Saving Mode------------*/
            HKDownlinkFR0();
            // delay_s(10);
            HKDownlinkFR1();
            // delay_s(10);
            break;
        case 0xF0:/*------------Survival Mode------------*/
            break;
        default:
            break;
    }
}

/*******************************************************************************
Frame
******************************************************************************/
void HKDownlinkFR0(void){
    UBYTE SatName[4] = {'o', 'r', 'i', '1'};
    sendMorse(SatName,sizeof(SatName)/sizeof(SatName[0]));
    UBYTE message[5] = {'H', 'E', 'L', 'L', 'O'};
    sendMorse(message,sizeof(message)/sizeof(message[0]));
}

void HKDownlinkFR1(void){
    UBYTE DATA[];//for ReadDatasFromEEPROMWithDataSizeAndSendMorse()
    //battery Voltage 
    ReadOneByteDataFromEEPROMandSendMorse(EEPROM_address,BatteryVoltage_addressHigh,BatteryVoltage_addressLow);
    //3.3VBus Voltage 
    ReadDatasFromEEPROMWithDataSizeAndSendMorse(EEPROM_address,adcValue_CH3_DATAHIGH_addressHigh,adcValue_CH3_DATAHIGH_addressLow,DATA,2);
    //5VBus Voltage 
    ReadDatasFromEEPROMWithDataSizeAndSendMorse(EEPROM_address,adcValue_CH2_DATAHIGH_addressHigh,adcValue_CH2_DATAHIGH_addressLow,DATA,2);
    //melting status
    ReadOneByteDataFromEEPROMandSendMorse(EEPROM_address, MeltingStatus_addressHigh, MeltingStatus_addressLow);
    //latest execution command ID(RX)
    ReadOneByteDataFromEEPROMandSendMorse(EEPROM_address,HighAddress_for_RXCOBCLastCommandID,LowAddress_for_RXCOBCLastCommandID);
    //command error status(RX)
    ReadOneByteDataFromEEPROMandSendMorse(EEPROM_address,HighAddress_for_RXCOBCLastCommandID,LowAddress_for_RXCOBCLastCommandID);
    //latest execution command ID(TX)
    ReadOneByteDataFromEEPROMandSendMorse(EEPROM_address,HighAddress_for_TXCOBCLastCommandID,LowAddress_for_TXCOBCLastCommandID);
    //command error status(TX)
    ReadOneByteDataFromEEPROMandSendMorse(EEPROM_address,HighAddress_for_TXCOBCLastCommandID,LowAddress_for_TXCOBCLastCommandID);
}

void HKDownlinkFR2(void){
    UBYTE DATA[];//for ReadDatasFromEEPROMWithDataSizeAndSendMorse()
    //latest execution command ID (OBC)
    ReadOneByteDataFromEEPROMandSendMorse(EEPROM_address,LatestExcutionCommandID_addressHigh,LatestExcutionCommandID_addressLow);
    //Battery Current
    ReadDatasFromEEPROMWithDataSizeAndSendMorse(EEPROM_address,BatteryCurrent_addressHigh,BatteryCurrent_addressLow,DATA,2);
    //Sattellite Bus status
    ReadDatasFromEEPROMWithDataSizeAndSendMorse(EEPROM_address,SatelliteBusStatus_addressHigh,SatelliteBusStatus_addressLow,DATA,2);
    //EPS switch status
    ReadDatasFromEEPROMWithDataSizeAndSendMorse(EEPROM_address,EpsSwitchStatus_addressHigh,EpsSwitchStatus_addressLow,DATA,2);
    //TX temperature
    ReadOneByteDataFromEEPROMandSendMorse(EEPROM_address,TxTemperature_addressHigh,TxTemperature_addressLow);
    //RX temperature
    ReadOneByteDataFromEEPROMandSendMorse(EEPROM_address,RxTemperature_addressHigh,RxTemperature_addressLow);
    //HK filenumber
    ReadOneByteDataFromEEPROMandSendMorse(EEPROM_address,HkFilenumber_addressHigh,HkFilenumber_addressLow);
    //EPS telemtry downlink
    ReadDatasFromEEPROMWithDataSizeAndSendMorse(EEPROM_address,EpsTelemtryDownlink_addressHigh,EpsTelemtryDownlink_addressLow,DATA,2);
}

void CWdownlinkStart(void){
    //TODO:
}

/*******************************************************************************
for debug
******************************************************************************/
void testForCwFunctions(void){
    
//        FMPTT = 0; 
//        CWKEY = 1;

//    //debug:send morse 'V'->'Y'->'J' 3 times
//     for(UBYTE i=0; i<3; i++){
//           Morse_V();
//           __delay_us(LONG_DELAYTIMES_FOR_MORSE);
//           Morse_Y();
//           __delay_us(LONG_DELAYTIMES_FOR_MORSE);
//           Morse_J();
//           __delay_us(LONG_DELAYTIMES_FOR_MORSE);
//           delay_us(ADD_BLANK_FOR_MORSE);
//        }
      
//    //debug send morse char
//    UBYTE test_morse[4];
//    test_morse[0] = 'O';
//    test_morse[1] = 'r';
//    test_morse[2] = 'i';
//    test_morse[3] = '1';
//    sendMorse(test_morse,sizeof(test_morse)/sizeof(test_morse[0]));
        
//        
//        //debug:translate binary to char
//        UBYTE _binary;
//        _binary = 0x05;
//        putChar(changeBinaryToChar(_binary));
//        _binary = 0xFF;
//        putChar(changeBinaryToChar(_binary));  //for check to defalt / X -> success
//        __delay_ms(1000);
//
//    //debug:DevideDataAndChangeBinaryToChar
//    UBYTE binary_data = 0xF3;
//    UBYTE char_data_highLow[2]; 
//    DevideDataAndChangeBinaryToChar (binary_data, char_data_highLow);
//        putChar(char_data_highLow[0]);  //'5'->success
//        putChar(char_data_highLow[1]);  //'A'->success
////        __delay_ms(1000);
////
//    //debug:send morse
//    for(UBYTE i=0; i<5; i++){
//        sendMorse(char_data_highLow,sizeof(char_data_highLow)/sizeof(char_data_highLow[0])); //morse signal '5'->delay(150ms)->'A'->success
//        delay_us(ADD_BLANK_FOR_MORSE);
//    } 

//    //debug:send ReadOneByteDataFromEEPROMandSendMorse
//    UBYTE TEST_DATA[3] = {'T', 0x5F, 0b10100111};  //'T'=0x54 / 0b10100111 = 0xA7
//    WriteToEEPROM(EEPROM_address, whigh_address, wlow_address, TEST_DATA);
//    ReadOneByteDataFromEEPROMandSendMorse(EEPROM_address, whigh_address, wlow_address); //morse signal 'T'->success
//    delay_us(ADD_BLANK_FOR_MORSE);
        
//    //debug:ReadDatasFromEEPROMWithDataSizeAndSendMorse
//    UBYTE ReadData[];
//    ReadDatasFromEEPROMWithDataSizeAndSendMorse(EEPROM_address, whigh_address, wlow_address, ReadData, 3); //morse signal 'T'-> 0x5F -> 0b10100111 -> success
        
//    //debug:ReadDatasFromEEPROMWithDataSizeAndSendMorseWithDownlinkTimes
//    ReadDatasFromEEPROMWithDataSizeAndSendMorseWithDownlinkTimes(EEPROM_address, whigh_address, wlow_address, ReadData, 3, 5); //morse signal 'T'-> 0x5F -> 0b10100111 -> 5times->success
    
    //debug:GetDatasizeAndReadDatasFromEEPROMWithDataSizeAndSendMorseWithDownlinkTimes
    UBYTE size_high_address = 0x00;
    UBYTE size_low_address = 0x1F;
    UBYTE TEST_DATA[3] = {0xa2, 0xb1, 0xab};
    UBYTE ReadData[];
    UBYTE Size_DATA = 3;
    WriteToEEPROM(EEPROM_address, whigh_address, wlow_address, TEST_DATA);
    WriteOneByteToEEPROM(EEPROM_address, size_high_address, size_low_address, Size_DATA);
    GetDatasizeAndReadDatasFromEEPROMWithDataSizeAndSendMorseWithDownlinkTimes(EEPROM_address, whigh_address, wlow_address, ReadData, size_high_address, size_low_address, 5);
        
    //FIXME:[finish]debug for downlink CW signal
    /*---------------------------------------------------------------*/
}
