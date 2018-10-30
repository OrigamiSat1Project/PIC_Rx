#include <xc.h>
#include <__null.h>
#include "MPU.h"
#include "pinDefine.h"
#include "time.h"
#include "typeDefine.h"
#include "UART.h"
#include "FMCW.h"
#include "EEPROM.h"
#include "I2C.h"
#include "OkError.h"

/*******************************************************************************
*Initialize MPU 
******************************************************************************/
void InitMPU(void)
{
	//Initialize Port Status
	PORTA  = 0x00;
	PORTB  = 0x00;
	PORTC  = 0x00;
    PORTD  = 0x00;
    PORTE  = 0x00;
	
	//AnalogorDigital Setting(All Digital)
//	ANSEL  = 0x00;	
//	ANSELH = 0x00;	

	//Port I/O Setting 
    //       0b76543210
	TRISA  = 0b10000000;
	TRISB  = 0b00100000;
    TRISC  = 0b10011000;
//    TRISD  = 0b00000000;
    TRISD  = 0b10000000;
    TRISE  = 0b00000000;	

	//Port Initial Value Setting	
	PORTA  = 0x00;
//	PORTB  = 0x00; //WDT on
    PORTB  = 0b00001000;  //WDT off
	PORTC  = 0x00;
    PORTD  = 0x00;
    PORTE  = 0x00;
}

/*******************************************************************************
*LED ON/OFF
******************************************************************************/
/*---for debug---*/
// void LEDOnOff(void){
//     LED_Y_ON();
//     __delay_us(HALF_INTERVAL);
//     LED_Y_OFF();
//     __delay_us(HALF_INTERVAL);
// }

// void debugLEDyellow(void){
//     if(LED_YELLOW == HIGH){
//         LED_Y_OFF();
//     }else{
//         LED_Y_ON();
//     }
// }


//Used to switch PIN to the opposite status(high/low)
//bit invertState(bit pinState){
UBYTE invertState(UBYTE pinState){
    if(pinState==HIGH){
        return LOW;
    }else{
        return HIGH;
    }
}

/*******************************************************************************
*Swticch Power 5.8/OBC/WDT
******************************************************************************/
/*
 *	witch Power Spply for 1pin
 *	arg      :   POWER_PIN, onOff, timeHigh, timeLow
 *	return   :   target_select :  5->5R8G O->OBC W->WDT 
 *               0x00->off 0x01->on
 *               timeHigh == 0x00 && timeLow == 0x00 -> not chage until next Uplink
 *               timeHigh != 0x00 && timeLow == 0x00 -> wait time is short ( 1byte:1~255[ms])  -> invert state
 *               timeHigh != 0x00 && timeLow != 0x00 -> wait time is long ( 2byte:266~65535[ms)  -> invert state
 *	TODO     :   need debug
 *	FIXME    :   not yet
 *	XXX      :   add function to change OnOff time
 */
void onOff5R8G(UBYTE onOff, UBYTE timeHigh, UBYTE timeLow){ //high->on
    if ( onOff == 0x00 ){        
            POWER_5R8G = LOW;  
    } else {                     
            POWER_5R8G = HIGH;
    }

    if(timeHigh == 0x00 && timeLow == 0x00){ 
    }else {        
        UWORD wait_time;
        wait_time = (timeHigh << 8 | timeLow);
        delay_ms(wait_time);
        POWER_5R8G =invertState(onOff);
    }
}

void onOffOBC(UBYTE onOff, UBYTE timeHigh, UBYTE timeLow){ //high->on
    if ( onOff == 0x00 ){        
            POWER_OBC = LOW;  
    } else {                     
            POWER_OBC = HIGH;
    }

    if(timeHigh == 0x00 && timeLow == 0x00){ 
    }else {        
        UWORD wait_time;
        wait_time = (timeHigh << 8 | timeLow);
        delay_ms(wait_time);
        POWER_OBC =invertState(onOff);
    }
}

void onOffWDT(UBYTE onOff, UBYTE timeHigh, UBYTE timeLow){ //high->off
    if ( onOff == 0x00 ){        
            POWER_WDT = HIGH;  
    } else {                     
            POWER_WDT = LOW;
    }

    if(timeHigh == 0x00 && timeLow == 0x00){ 
    }else {        
        UWORD wait_time;
        wait_time = (timeHigh << 8 | timeLow);
        delay_ms(wait_time);
        POWER_WDT =invertState(onOff);
    }
}

void switchPowerSpply1pin(UBYTE target_select, UBYTE onOff, UBYTE timeHigh, UBYTE timeLow){  
    switch(target_select){
        case '5':
            onOff5R8G(onOff, timeHigh, timeLow);
            break;
        case 'O':
            onOffOBC(onOff, timeHigh, timeLow);
            break;
        case 'W':
            onOffWDT(onOff, timeHigh, timeLow);
            break;
        default:
            switchError(error_MPU_switchPowerSpply1pin);
            break;
    }  
    //TODO:add function to change OnOff time
}

/*******************************************************************************
*Swticch Power EPS 
******************************************************************************/
/*
 *	EPS Power OFF
 *	arg      :   void
 *	return   :   SEP_SW(Short Separation switch 1&2) & RBF_SW(Short Remove before flight switch 1&2) = HIGH  -> EPS Power OFF
 *	TODO     :   need debug
 *	FIXME    :   not yet
 *	XXX      :   not yet
 */
void killEPS(void){
    SEP_SW = HIGH;  
    RBF_SW = LOW;  
}

/*
 *	EPS Power ON
 *	arg      :   void
 *	return   :   SEP_SW(Short Separation switch 1&2) & RBF_SW(Short Remove before flight switch 1&2) = LOW  -> EPS Power ON
 *	TODO     :   need debug
 *	FIXME    :   not yet
 *	XXX      :   not yet
 */
void onEPS(void){
    SEP_SW = LOW;  
    RBF_SW = LOW;  
}

/*
 *	switch Power EPS
 *	arg      :   POWER_PIN_1, POWER_PIN_2, onOff, timeHigh, timeLow
 *	return   :   0x00 -> SEP_SW&RBF_SW : HIGH -> EPS : OFF, 0x01 -> SEP_SW&RBF_SW : LOW -> EPS : ON
 *               timeHigh == 0x00 && timeLow == 0x00 -> not chage until next Uplink
 *               timeHigh != 0x00 && timeLow == 0x00 -> wait time is short ( 1byte:1~255[ms]) -> invert state
 *               timeHigh != 0x00 && timeLow != 0x00 -> wait time is long ( 2byte:266~65535[ms) -> invert state
 *	TODO     :   need debug
 *	FIXME    :   not yet
 *	XXX      :   add function to change OnOff time
 */
void switchPowerEPS(UBYTE onOff, UBYTE timeHigh, UBYTE timeLow){  
    /*-------------------------------------------------------*/
    //FIXME:for debug to test switch start
//     if ( onOff == 0x00 ){        
//             putChar(0xA0);
//             SEP_SW = HIGH;
//             RBF_SW = HIGH;
//             putChar(0xA0);
//     } else {                     
//             putChar(0xA1);
//             SEP_SW = LOW;
//             RBF_SW = LOW;
//             putChar(0xA1);
//     }
    
//     if(timeHigh == 0x00 && timeLow == 0x00){     
//         putChar(0xB0);
//         putChar(0xB0);
// //    }else if(timeLow == 0x00){                    
// //        __delay_s(timeHigh);                     
// //        SEP_SW =invertState(onOff);
// //        RBF_SW =invertState(onOff);
// //        putChar(0xFF);
// //        putChar(timeHigh);
// //        putChar(0x0c);
//     }else {        
//         putChar(0xB1);
//         putChar(0xB1);
//         UWORD wait_time;
//         wait_time = (timeHigh << 8 | timeLow);
//         delay_s(wait_time);
//         putChar(0xB2);
//         putChar(0xB2);
//         delay_ms(1500);
//         SEP_SW =invertState(onOff);
//         RBF_SW =invertState(onOff);
//         putChar(0xB3);
//         putChar(0xB3);
//         delay_ms(100);
//         putChar(0xB4);
//         putChar(0xB4);
//         delay_ms(100);
//     }
    //FIXME:for debug to test switch finish
    /*-------------------------------------------------------*/

    if ( onOff == 0x00 ){        //EPS off
            SEP_SW = HIGH;
            RBF_SW = LOW;
    } else {                     //EPS on
            SEP_SW = LOW;
            RBF_SW = LOW;
    }
    
    if(timeHigh == 0x00 && timeLow == 0x00){   
        //////////////////
    }else {        
        UWORD wait_time;
        wait_time = (timeHigh << 8 | timeLow);
        delay_ms(wait_time);
        SEP_SW =invertState(onOff);
    }

}

/*
 *	reive EPS
 *	arg      :   timeHigh, timeLow
 *	return   :   timeHigh == 0x00 && timeLow == 0x00 -> not chage until next Uplink
 *               timeHigh != 0x00 && timeLow == 0x00 -> wait time is short ( 1byte:1~255[ms]) -> revive EPS
 *               timeHigh != 0x00 && timeLow != 0x00 -> wait time is long ( 2byte:266~65535[ms) -> revive EPS           
 *	TODO     :   need debug
 *	FIXME    :   not yet
 *	XXX      :   not yet
 */
void reviveEPS(UBYTE timeHigh, UBYTE timeLow){
    if(timeHigh == 0x00 && timeLow == 0x00){     
    }else if(timeLow == 0x00){                    
        __delay_ms(timeHigh);                     
        onEPS();
    }else {                                      
        UWORD wait_time;
        wait_time = (timeHigh << 8 | timeLow);
        __delay_ms(wait_time);
        onEPS();
    }
}

/*******************************************************************************
*functions for commandSwitchIntProcess
******************************************************************************/
void changeInOut(UINT pin_select_command, UBYTE inOut){
    UBYTE inOutStatus_addressHigh;
    UBYTE inOutStatus_addressLow;
    switch (pin_select_command){
        case 0x0a:
            TRISA = inOut;
            inOutStatus_addressHigh = TRISA_addressHigh;
            inOutStatus_addressLow  = TRISA_addressLow;
            break;
        case 0x0b:
            TRISB = inOut;
            inOutStatus_addressHigh = TRISB_addressHigh;
            inOutStatus_addressLow  = TRISB_addressLow;
            break;
        case 0x0c:
            TRISC = inOut;
            inOutStatus_addressHigh = TRISC_addressHigh;
            inOutStatus_addressLow  = TRISC_addressLow;
            break;
        case 0x0d:
            TRISD = inOut;
            inOutStatus_addressHigh = TRISD_addressHigh;
            inOutStatus_addressLow  = TRISD_addressLow;
            break;
        case 0x0e:
            TRISE = inOut;
            inOutStatus_addressHigh = TRISE_addressHigh;
            inOutStatus_addressLow  = TRISE_addressLow;
            break;
        default:
            switchError(error_MPU_changeInOut);
            break;
    }
    WriteOneByteToMainAndSubB0EEPROM(inOutStatus_addressHigh, inOutStatus_addressLow, inOut);
}

void changeHighLow(UINT pin_select_command, UBYTE highLow){
    UBYTE highLowStatus_addressHigh;
    UBYTE highLowStatus_addressLow;
    switch (pin_select_command){
        case 0x0a:
            PORTA = highLow;
            highLowStatus_addressHigh = PORTA_addressHigh;
            highLowStatus_addressLow  = PORTA_addressLow;
            break;
        case 0x0b:
            PORTB = highLow;
            highLowStatus_addressHigh = PORTB_addressHigh;
            highLowStatus_addressLow  = PORTB_addressLow;
            break;
        case 0x0c:
            PORTC = highLow;
            highLowStatus_addressHigh = PORTC_addressHigh;
            highLowStatus_addressLow  = PORTC_addressLow;
            break;
        case 0x0d:
            PORTD = highLow;
            highLowStatus_addressHigh = PORTD_addressHigh;
            highLowStatus_addressLow  = PORTD_addressLow;
            break;
        case 0x0e:
            PORTE = highLow;
            highLowStatus_addressHigh = PORTE_addressHigh;
            highLowStatus_addressLow  = PORTE_addressLow;
            break;
        default:
            switchError(error_MPU_changeHighLow);
            break;
    }
    WriteOneByteToMainAndSubB0EEPROM(highLowStatus_addressHigh, highLowStatus_addressLow, highLow);
}

void changeXtalFrequency(UBYTE XTAL_FREQUENCY_TYPE){
    switch (XTAL_FREQUENCY_TYPE){                // Clock frequency
        case 'h':
            //_XTAL_FREQ = _XTAL_FREQ_HIGH;
            break;
        case 'm':
            //_XTAL_FREQ = _XTAL_FREQ_MIDDLE;  //10M[Hz]
            break;
        case 'l':
            //_XTAL_FREQ = _XTAL_FREQ_LOW;
            break;
        default:
            //TODO:add error
            break;
    }
}

/*******************************************************************************
*command swtich feature 
******************************************************************************/
/*
 *	change satellite mode
 *	arg      :   command, timeHigh, timeLow
 *	return   :   0x00 -> Nominal mode (ON: CIB, EPS, OBC, Tx(CW), Rx)
 *               0x0F -> Power saving mode (ON: CIB, Tx(CW), Rx / OFF: EPS, OBC)
 *               0xFF -> Survival mode (ON: CIB / OFF: EPS, OBC, Tx(CW), Rx)             
 *	TODO     :   need debug
 *	FIXME    :   not yet
 *	XXX      :   add error messege
 */
void commandSwitchSatMode(UBYTE command, UBYTE timeHigh, UBYTE timeLow){ //times are given in ms
    switch(command){    
        case 0x00: 
            /*---------------------------*/
            /* method for nominal mode (ON: CIB, EPS, OBC, Tx(CW), Rx)
             * 1.SEP_SW & RBF_SW = LOW -> EPS switch ON
             * 2.then Rx/Tx/OBC switch  ON
             * TODO:how to turn on CIB? */
            /*---------------------------*/
            switchPowerEPS(0x00, timeHigh, timeLow);
            WriteOneByteToMainAndSubB0EEPROM(deviceOnOff_addressHigh, deviceOnOff_addressLow, all_device_ON);
            break;
        case 0x0F: 
            /*----------------------------*/
            /*method for power saving mode (ON: CIB, Tx(CW), Rx / OFF: EPS, OBC)
             * 1.first kill EPS (this also kills Rx/Tx/OBC)
             * 2.send command to TXCOBC to turn back on RX and TX  (Radio Unit)
             *      task target:t(TX COBC)
             *      CommandType:p(power)
             *      Parameter1:0x01(on) / 2:timeHigh / 3:timeLow
             * 3.RXCOBC reset PLL data
             * 4.after setting time, revive EPS (this also revive OBC)
            /*----------------------------*/
            killEPS();
            onNtrxPowerSupplyCIB(timeHigh,timeLow);
            WriteOneByteToMainAndSubB0EEPROM(deviceOnOff_addressHigh, deviceOnOff_addressLow, CIB_TX_RX_ON);
            break;
        case 0xFF: 
            /*---------------------------*/
            /* method for nominal mode (ON: CIB / OFF: EPS, OBC, Tx(CW), Rx)
             * only enter if time in survival mode is specified 
             * set automatical revival time
             * 1.SEP_SW & RBF_SW = HIGH -> EPS switch OFF
             * 2.then Rx/Tx/OBC switch  OFF
             * 3.if time has run out switch to power saving mode
            /*---------------------------*/
            switchPowerEPS(0x01, timeHigh, timeLow);
            FMTX(FMTX_Nref, FMTX_Nprg);
            CWTX(CWTX_Nref, CWTX_Nprg);
            FMRX(FMRX_Nref, FMRX_Nprg);
            WriteOneByteToMainAndSubB0EEPROM(deviceOnOff_addressHigh, deviceOnOff_addressLow, CIB_ON);
            break;
        default:
            switchError(error_MPU_commandSwitchSatMode);
            break;
    }
}

//process command data if the command type is 'power supply'
void commandSwitchPowerSupply(UBYTE command, UBYTE onOff, UBYTE timeHigh, UBYTE timeLow){ //times are given in ms
    switch(command){    
        case '5': //5R8G
            switchPowerSpply1pin('5', onOff, timeHigh, timeLow);
            break;
        case 'e': //EPS
            switchPowerEPS(onOff, timeHigh, timeLow);
//            UBYTE deviceOnOff_status[2];
//            deviceOnOff_status[0] = ReadEEPROM(MAIN_EEPROM_ADDRESS, deviceOnOff_addressHigh, deviceOnOff_addressLow);
//            deviceOnOff_status[1] = ReadEEPROM(SUB_EEPROM_ADDRESS, deviceOnOff_addressHigh, deviceOnOff_addressLow);            
//            for (UBYTE i=1; i<2; i++){
//                if(onOff = 0x01){
//                    deviceOnOff_status[i] = deviceOnOff_status[i] | 0b01000000;
//                }
//            }
            break;
        case 'o': //OBC
            switchPowerSpply1pin('O', onOff, timeHigh, timeLow);
            break;
        case 'w': //WDT
            switchPowerSpply1pin('W', onOff, timeHigh, timeLow);
            break;
        default:
            switchError(error_MPU_commandSwitchSatMode);
            break;
    }
}


//process command data if the command type is 'internal processing'
void commandSwitchIntProcess(UBYTE command, UBYTE data1, UBYTE data2){ 
    switch(command){    
        case 'i': //change in/out
            //TODO: write method for change in/out---finish
            changeInOut(data1, data2);
            break;
        case 'h': //change high/low
            //TODO: write method for change high/low---finish
            changeHighLow(data1, data2);     
            break;
        case 'o': //change frequency
            //TODO: write method for change frequency---not finish
            changeXtalFrequency(data1);
            break;
        default:
            switchError(error_MPU_commandSwitchIntProcess);
            break;
    }
}

void onNtrxPowerSupplyCIB(UBYTE timeHigh,UBYTE timeLow){
    UBYTE send_command[8];
    send_command[0] = 't';
    send_command[1] = 'p';
    send_command[2] = 'n';
    send_command[3] = 0x01;
    send_command[4] = timeHigh;
    send_command[5] = timeLow;
    send_command[6] = 0x00;
    send_command[7] = 0x00;
    sendCommandByPointer(send_command);
    FMTX(FMTX_Nref, FMTX_Nprg);
    CWTX(CWTX_Nref, CWTX_Nprg);
    FMRX(FMRX_Nref, FMRX_Nprg);
//    reviveEPS(timeHigh, timeLow);           
}

void offNtrxPowerSupplyCIB(void){
    UBYTE send_command[8];
    send_command[0] = 't';
    send_command[1] = 'p';
    send_command[2] = 'n';
    send_command[3] = 0x00;
    send_command[4] = 0x00;
    send_command[5] = 0x00;
    send_command[6] = 0x00;
    send_command[7] = 0x00;
    sendCommandByPointer(send_command);         
}