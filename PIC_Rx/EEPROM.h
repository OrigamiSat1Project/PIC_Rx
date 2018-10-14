/* 
 * File:   EEPROM.h
 * Author: Kosuke
 *
 * Created on 2017/04/18, 2:17
 */

#include <xc.h>


#ifndef EEPROM_H
#define	EEPROM_H

#include "typeDefine.h"

/*******************************************************************************
*EEPROM address
******************************************************************************/
#define MAIN_EEPROM_ADDRESS 0x50
#define SUB_EEPROM_ADDRESS 0x52

//for test EEPROM
//TODO:change address
#define datalength_for_test         4
#define B0select_for_testEEPROM     0x00
#define HighAddress_for_testEEPROM  0xE0
#define LowAddress_for_testEEPROM   0x00


/*******************************************************************************
*MPU.c
******************************************************************************/
//address for satellite mode
//TODO:change address
#define deviceOnOff_addressHigh 0x00
#define deviceOnOff_addressLow 0x00
//7:CIB / 6:EPS / 5:OBC / 4:Radio(TX) / 3:Radio(RX)
//                    0b76543210
#define all_device_ON 0b11111000
#define CIB_TX_RX_ON  0b10011000
#define CIB_ON        0b10000000

/*-----------in/out status-----------------*/
//TODO:change address
#define TRISA_addressHigh 0x00
#define TRISA_addressLow  0x00
#define TRISB_addressHigh 0x00
#define TRISB_addressLow  0x00
#define TRISC_addressHigh 0x00
#define TRISC_addressLow  0x00
#define TRISD_addressHigh 0x00
#define TRISD_addressLow  0x00
#define TRISE_addressHigh 0x00
#define TRISE_addressLow  0x00

/*-----------high/low status-----------------*/
//TODO:change address
#define PORTA_addressHigh 0x00
#define PORTA_addressLow  0x00
#define PORTB_addressHigh 0x00
#define PORTB_addressLow  0x00
#define PORTC_addressHigh 0x00
#define PORTC_addressLow  0x00
#define PORTD_addressHigh 0x00
#define PORTD_addressLow  0x00
#define PORTE_addressHigh 0x00
#define PORTE_addressLow  0x00


/*******************************************************************************
*OK and ERROR
******************************************************************************/
#define B0select_for_commandID     0x00
#define HighAddress_for_commandID  0xE0
#define LowAddress_for_commandID   0x00

#define B0select_for_Ok       0x00
#define HighAddress_for_Ok    0xE0
#define LowAddress_for_Ok     0x00

#define B0select_for_Error     0x00
#define HighAddress_for_Error  0x97
#define LowAddress_for_Error   0x09


/*******************************************************************************
*Last Command ID
******************************************************************************/
#define B0select_for_RXCOBCLastCommand     0x00
#define HighAddress_for_RXCOBCLastCommand  0x97
#define LowAddress_for_RXCOBCLastCommand   0x08


/*******************************************************************************
*Initial Operation
******************************************************************************/
#define HighAddress_for_meltingCompelationFlag      0x97
#define LowAddress_for_meltingCompelationFlag       0x07


#endif	/* EEPROM_H */

