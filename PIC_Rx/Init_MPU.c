<<<<<<< HEAD:PIC_Rx/Init_MPU.c
#include <xc.h>
#include "Init_MPU.h"
#include "Type_define.h"
#include "time.h"
#include "decode_AX25.h"
#include "encode_AX25.h"

void Init_MPU(void)
{
	//Initialize Port Status
	PORTA  = 0x00;
	PORTB  = 0x00;
	PORTC  = 0x00;
    PORTD  = 0x00;
    PORTE  = 0x00;
	
	//AnalogorDigital Setting(All Digital)
	ANSEL  = 0x00;	//ADê›íË
	ANSELH = 0x00;	//ADê›íË
	
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
=======
#include <xc.h>
#include "Init_MPU.h"
#include "Type_define.h"
#include "time.h"
#include "decode_AX25.h"
#include "encode_AX25.h"

void Init_MPU(void)
{
	//Initialize Port Status
	PORTA  = 0x00;
	PORTB  = 0x00;
	PORTC  = 0x00;
    PORTD  = 0x00;
    PORTE  = 0x00;
	
	//AnalogorDigital Setting(All Digital)
	ANSEL  = 0x00;
	ANSELH = 0x00;
	
	//Port I/O Setting 
    //       0b76543210
	TRISA  = 0b10000000;
	TRISB  = 0b00000001;
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
>>>>>>> DH_dev:Init_MPU.c
