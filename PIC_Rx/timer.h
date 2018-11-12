

#ifndef TIMER_H
#define	TIMER_H

#include "typeDefine.h"

#define _XTAL_FREQ       10000000       // Clock frequency
#define __delay_us(x) _delay((unsigned long)((x)*(_XTAL_FREQ/4000000.0)))
#define __delay_ms(x) _delay((unsigned long)((x)*(_XTAL_FREQ/4000.0)))

#define one_second 38
#define one_minute 60
#define one_hour 60
#define one_day 24
#define one_week 7

#define NTRX_PLL_INTERVAL        1
//#define WDT_INTERVAL             5
#define EPS_RSET_INTERVAL_LONG   10
#define EPS_RSET_INTERVAL_SHORT  3
#define INITIAL_OPE_INTERVAL     7
#define COMMAND_COUNTER_INTERVAL 2       //for debug 1min
#define EPS_MEASURE_INTERVAL     3          //for debug 1min
//#define WDT_PULSE_INTERVAL       8


void initTimer(void);
void interrupt TimerCheck(void);

void set_NTRX_pll_setting_counter(UINT, UINT, UINT, UINT);
UINT get_NTRX_pll_setting_counter_sec(void);  
UINT get_NTRX_pll_setting_counter_min(void);
UINT get_NTRX_pll_setting_counter_hour(void);  
UINT get_NTRX_pll_setting_counter_day(void); 

void set_eps_reset_counter(UINT, UINT);  //FIXME:for debug
UINT get_eps_reset_counter_sec(void);   //FIXME:for debug
UINT get_eps_reset_counter_min(void);   //FIXME:for debug

void set_receive_command_counter(UINT, UINT);
UINT get_receive_command_counter_sec(void);  //FIXME:for debug
UINT get_receive_command_counter_min(void);

void set_init_ope_counter(UINT, UINT);
UINT get_init_ope_counter_sec(void);   //FIXME:for debug
UINT get_init_ope_counter_min(void);

void set_bat_meas_counter(UINT, UINT);
UINT get_bat_meas_counter_sec(void);   //FIXME:for debug
UINT get_bat_meas_counter_min(void);  

UINT get_timer_counter(UBYTE unit);

void reset_timer(void);

//void interrupt TimerReset(void);

#endif	/* TIME_H */