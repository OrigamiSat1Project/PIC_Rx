#include <xc.h>
#include "MPU.h"
#include "Type_define.h"
#include "time.h"
//#include "decode_AX25.h"
#include "encode_AX25.h"
#include "pinDefine.h"
void Init_MPU(void)
{
	//Initialize Port Status
	PORTA  = 0x00;
	PORTB  = 0x00;
	PORTC  = 0x00;
    //PORTD  = 0x00;
    PORTE  = 0x00;
	
	//AnalogorDigital Setting(All Digital)
	ANSEL  = 0x00;	//ADê›íË
	ANSELH = 0x00;	//ADê›íË
	
	//Port I/O Setting 
    //       0b76543210
	TRISA  = 0b10000000;
	TRISB  = 0b00010000;
    TRISC  = 0b01011100;
    //TRISD  = 0b00000000;
    TRISE  = 0b00000000;	

	//Port Initial Value Setting	
	PORTA  = 0x00;
	PORTB  = 0x00;
	PORTC  = 0x00;
    //PORTD  = 0x00;
    PORTE  = 0x00;
}
void led_onoff(void){
    ledy_on();
    __delay_us(espan);
    ledy_off();
    __delay_us(espan);
}
void debug_ledy(void){
    if(led_yellow == high){
        ledy_off();
    }else{
        ledy_on();
    }
}

void cutWire(UBYTE timeHigh, UBYTE timeLow){
    UWORD cutTime;
    cutTime = (UWORD)timeHigh * 0x100 + timeLow;
    WIRE_CUTTER = 1;
    delay_ms(cutTime);
    WIRE_CUTTER = 0;
}
