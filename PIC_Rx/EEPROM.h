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

#define MAIN_EEPROM_ADDRESS 0x50
#define SUB_EEPROM_ADDRESS 0x52

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


#endif	/* EEPROM_H */

