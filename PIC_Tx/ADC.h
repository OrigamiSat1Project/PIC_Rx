/* 
 * File:   ADC.h
 * Author: kosuk
 *
 * Created on 2018/08/25, 16:55
 */

#ifndef ADC_H
#define	ADC_H

#define Channel_Size 4

void measureAllChanelADC(void);
void measureDcDcTemperature(void); 
void measureChannel2(void);
void measureChannel3(void);
void measureChannel4(void);
void commandSwitchHKdata(UBYTE, UBYTE, UBYTE, UBYTE);

#endif	/* ADC_H */

