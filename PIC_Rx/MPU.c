#include <xc.h>
#include <__null.h>
#include "MPU.h"
#include "pinDefine.h"
#include "time.h"
#include "typeDefine.h"
#include "UART.h"
#include "FMCW.h"


void InitMPU(void)
{
	//Initialize Port Status
	PORTA  = 0x00;
	PORTB  = 0x00;
	PORTC  = 0x00;
    PORTD  = 0x00;
    PORTE  = 0x00;
	
	//AnalogorDigital Setting(All Digital)
//	ANSEL  = 0x00;	//AD設定// AD setting
//	ANSELH = 0x00;	//AD設定// AD setting
	
	//Port I/O Setting 
    //       0b76543210
	TRISA  = 0b10000000;
	TRISB  = 0b00100000;
    TRISC  = 0b10011000;
    TRISD  = 0b00000000;
    TRISE  = 0b00000000;	

	//Port Initial Value Setting	
	PORTA  = 0x00;
	PORTB  = 0x00;
	PORTC  = 0x00;
    PORTD  = 0x00;
    PORTE  = 0x00;
}
void LEDOnOff(void){
    LED_Y_ON();
    __delay_us(HALF_INTERVAL);
    LED_Y_OFF();
    __delay_us(HALF_INTERVAL);
}
void debugLEDyellow(void){
    if(LED_YELLOW == HIGH){
        LED_Y_OFF();
    }else{
        LED_Y_ON();
    }
}

//Used to switch PIN to the opposite status(high/low)
//bit invertState(bit pinState){
UINT invertState(UINT pinState){
    if(pinState==HIGH){
        return LOW;
    }else{
        return HIGH;
    }
}

void killEPS(void){
    SEP_SW = HIGH;  //Short Separation switch 1&2 //High--->EPS OFF
    RBF_SW = HIGH;  //Short Remove before flight switch 1&2  //High--->EPS OFF
}

//TODO:check
/*---switchPowerSpply---*/
//5.8, OBC, WDT --- switchPowerSpply1pin
//EPS --- switchPowerSpply2pin
void switchPowerSpply1pin(UINT POWER_PIN, UBYTE onOff, UBYTE timeHigh, UBYTE timeLow){  
    if ( onOff == 0x00 ){        //switch off
            POWER_PIN = LOW;
    } else {                     //switch on
            POWER_PIN = HIGH;
    }
  
    if(timeHigh == 0x00 && timeLow == 0x00){      //1. not chage until next Uplink
    }else if(timeLow == 0x00){                    //2. wait time is short ( 1byte:1~255[ms])
        __delay_ms(timeHigh);                     
        POWER_PIN =invertState(POWER_PIN);
    }else {                                       //3. wait time is long ( 2byte:266~65535[ms)
        UWORD wait_time;
        wait_time = (timeHigh << 8 | timeLow);
        __delay_ms(wait_time);
        POWER_PIN =invertState(POWER_PIN);
    }
}

void switchPowerSpply2pin(UINT POWER_PIN_1, UINT POWER_PIN_2, UBYTE onOff, UBYTE timeHigh, UBYTE timeLow){  
    if ( onOff == 0x00 ){        //switch off
            POWER_PIN_1 = LOW;   //TODO:ピン1ピン2は同じ値でよいのか？
            POWER_PIN_2 = LOW;
    } else {                     //switch on
            POWER_PIN_1 = HIGH;
            POWER_PIN_2 = HIGH;
    }
  
    if(timeHigh == 0x00 && timeLow == 0x00){      //1. not chage until next Uplink
    }else if(timeLow == 0x00){                    //2. wait time is short ( 1byte:1~255[ms])
        __delay_ms(timeHigh);                     
        POWER_PIN_1 =invertState(POWER_PIN_1);
        POWER_PIN_2 =invertState(POWER_PIN_2);
    }else {                                       //3. wait time is long ( 2byte:266~65535[ms)
        UWORD wait_time;
        wait_time = (timeHigh << 8 | timeLow);
        __delay_ms(wait_time);
        POWER_PIN_1 =invertState(POWER_PIN_1);
        POWER_PIN_2 =invertState(POWER_PIN_2);
    }
}

void changeInOut(UINT pin_select_command, UBYTE inOut){
    switch (pin_select_command){
        case 0x0a:
            TRISA = inOut;
            break;
        case 0x0b:
            TRISB = inOut;
            break;
        case 0x0c:
            TRISC = inOut;
            break;
        case 0x0d:
            TRISD = inOut;
            break;
        case 0x0e:
            TRISE = inOut;
            break;
    }
}

void changeHighLow(UINT pin_select_command, UBYTE highLow){
    switch (pin_select_command){
        case 0x0a:
            PORTA = highLow;
            break;
        case 0x0b:
            PORTB = highLow;
            break;
        case 0x0c:
            PORTC = highLow;
            break;
        case 0x0d:
            PORTD = highLow;
            break;
        case 0x0e:
            PORTE = highLow;
            break;
    }
}

//TODO:コメントアウトとったときのエラー改善
//time.cとtime.hもいじった
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
    }
}

//process command data if the command type is 'change satellite mode'
void commandSwitchSatMode(UBYTE command, UBYTE timeHigh, UBYTE timeLow){ //times are given in ms
    switch(command){    
        case 0x00: //Nominal mode //ON: CIB, EPS, OBC, Tx(CW), Rx
            //TODO: write method for nominal mode
            break;
        case 0x0F: //Power saving mode //ON: CIB, Tx(CW), Rx //OFF: EPS, OBC
            /*----------------------------*/
            /*method for power saving mode
             * 1.first kill EPS (this also kills Rx/Tx/OBC)
             * 2.send command to TXCOBC to turn back on RX and TX  (Radio Unit)
             *      task target:t(TX COBC)
             *      CommandType:m(change satellite mode)
             *      Parameter1:0x0F(Power Saving) / 2:timeHigh / 3:timeLow
             * 3.RXCOBC reset PLL data*/
            /*----------------------------*/
            killEPS();
            sendCommand('t', 'm', 0x0F, timeHigh, timeLow, NULL);
            FMTX(FMTX_Nref, FMTX_Nprg);
            CWTX(CWTX_Nref, CWTX_Nprg);
            FMRX(FMRX_Nref, FMRX_Nprg);
            break;
        case 0xFF: //Survival mode //ON: CIB //OFF: EPS, OBC, Tx(CW), Rx
            //only enter if time in survival mode is specified //set automatical revival time
            //if time has run out switch to power saving mode
            //TODO: write method for survival mode
            break;
        default:
            //TODO: error message
            break;
    }
}

//process command data if the command type is 'power supply'
void commandSwitchPowerSupply(UBYTE command, UBYTE onOff, UBYTE timeHigh, UBYTE timeLow){ //times are given in ms
    switch(command){    
        case '5': //5R8G
            switchPowerSpply1pin(POWER_5R8G, onOff, timeHigh, timeLow);
            break;
        case 'e': //EPS
            //TODO: write method for EPS using pins: SEP_SW and RBF_SW---finish
            switchPowerSpply2pin(SEP_SW, RBF_SW, onOff, timeHigh, timeLow);
            break;
        case 'o': //OBC
            //TODO: write method for OBC using pin: POWER_OBC ---finish
            switchPowerSpply1pin(POWER_OBC, onOff, timeHigh, timeLow);
            break;
        case 'w': //WDT
            //TODO: write method for WDT using pin: POWER_WDT---finish
            switchPowerSpply1pin(POWER_WDT, onOff, timeHigh, timeLow);
            break;
        default:
            //TODO: error message
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
            //TODO: error message
            break;
    }
}