/* 
 * File:            PIN_define
 * Author:          kosuke
 * Comments:        defines/asigns all PINs of the PIC
 *                  see also COBC_pin_assignment file on google drive
 *
 * Created on 2018/08/19, 18:53
 */


#include <xc.h>  // </editor-fold>



#ifndef PIN_DEFINE_H
#define	PIN_DEFINE_H
//------------------------------------------------------------------------------
// All PIN Asignments:

// PINs that are already used in code
//-----------------------------------

//Original Source: encode_AX25.h
#define txd             RC5  //transmit radio data
// #define UHFstart        RC2  //not used this pin
#define FMPTT           RC0
#define CWKEY           RB2
//Original Source: Init_MPU.h
#define led_yellow      RB1
//#define FX614_RXD       RB0

#define WIRE_CUTTER     RB3
#define HEATER          RC1
#define NTRX            RB0
#define SW_5R8G         RB5
#define WDT_POWER       RA1
#define OBC_STATUS      RC2

#endif	/* NEWFILE_H */

