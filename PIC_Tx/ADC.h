/* 
 * File:   ADC.h
 * Author: kosuk
 *
 * Created on 2018/08/25, 16:55
 */

#ifndef ADC_H
#define	ADC_H

#define CHANEL_SIZE 4

void measureAllChanelADC(void);
void measureDcDcTemperature(UBYTE, UBYTE, UBYTE); 
void commandSwitchHKdata(UBYTE, UBYTE, UBYTE, UBYTE);

#endif	/* ADC_H */

