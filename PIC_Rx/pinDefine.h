/* 
 * File:            PIN_define
 * Author:          Britta
 * Comments:        defines/asigns all PINs of the PIC
 *                  see also COBC_pin_assignment file on google drive
 */


#include <xc.h>  // </editor-fold>



#ifndef PIN_DEFINE_H
#define	PIN_DEFINE_H
//------------------------------------------------------------------------------
// All PIN Asignments:

// PINs that are already used in code
//-----------------------------------
#define LED_YELLOW      PORTCbits.RC2 
#define LED_WHITE       PORTBbits.RB2
#define FX614_RXD       PORTBbits.RB5 //Receive radio data
#define POWER_5R8G      PORTCbits.RC5 //Turn on 5R8G (high -> on)
#define POWER_OBC       PORTDbits.RD3 //Turn on OBC sub power (high -> on)??
#define POWER_WDT       PORTBbits.RB3 //turn off WDT(high -> WDT off)
        
    /* PIN assignment for FMRX */
#define FMRX_CLK        PORTAbits.RA2 //clock
#define FMRX_DAT        PORTAbits.RA3 //data
#define FMRX_STB        PORTBbits.RB0 //strobe signal
    /* PIN assignment for FMTX */
#define FMTX_CLK        PORTEbits.RE2
#define FMTX_DAT        PORTEbits.RE1
#define FMTX_STB        PORTEbits.RE0
    /* PIN assignment for CWTX */
#define CWTX_CLK        PORTDbits.RD2
#define CWTX_DAT        PORTDbits.RD1
#define CWTX_STB        PORTDbits.RD0
     
    /* PIN assignment for EPS * */
#define SEP_SW          PORTBbits.RB4 //Short Separation switch 1&2  //High--->EPS OFF
#define RBF_SW          PORTAbits.RA1 //Short Remove before flight switch 1&2  //High--->EPS OFF

    /* PIN assignment for OBC*/
#define OBC_STATUS      PORTDbits.RD7  //check whether OBC are alive or died  //High--->OBC are alive 

//New definitions
#define WDT             PORTBbits.RB3 //turn off WDT(high -> WDT off)

// PINs NOT used in code
//--------------------------
//PORTAbits.RA0 //BAT_POS voltage(10/43)
//PORTAbits.RA4 //Pull down to GND
//PORTAbits.RA5 //Pull down to GND
//PORTBbits.RB1 //Send pulse to WDT
//PORTBbits.RB6 //PGC
//PORTBbits.RB7 //PGD
//PORTCbits.RC0 //TXCOBC-3(RA4)
//PORTCbits.RC1 //TXCOBC-22(RB4)
//PORTCbits.RC3 //I2C SCL
//PORTCbits.RC4 //I2C SCL
//PORTCbits.RC6 //UART TX
//PORTCbits.RC7 //UART RX (to debug connector)
//PORTDbits.RD4 //12V status (Low -> over current 12V shut down)
//PORTDbits.RD5 //OBC H2-50 (49)
//PORTDbits.RD6 //Pull down to GND
//PORTDbits.RD7 //OBC H1-49 (55)
//------------------------------------------------------------------------------
#endif	/* PIN_DEFINE_H */

