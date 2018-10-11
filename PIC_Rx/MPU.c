#include <xc.h>
#include "MPU.h"
#include "pinDefine.h"
#include "time.h"
#include "typeDefine.h"



void InitMPU(void)
{
	//Initialize Port Status
	PORTA  = 0x00;
	PORTB  = 0x00;
	PORTC  = 0x00;
    PORTD  = 0x00;
    PORTE  = 0x00;
	
	//AnalogorDigital Setting(All Digital)
//	ANSEL  = 0x00;	//ADê›íË// AD setting
//	ANSELH = 0x00;	//ADê›íË// AD setting
	
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
