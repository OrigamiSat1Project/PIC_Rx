/* 
 * File:   ADC.h
 * Author: kosuk
 *
 * Created on 2018/08/25, 16:55
 */

#ifndef ADC_H
#define	ADC_H

#define Channel_Size 4
#define SatMode_Nominal 0x50
#define SatMode_Saving 0x60
#define SatMode_Survival 0xA0

#define ADC_Value_4V 620//4*10/20*1024/3.3

void measureAllChanelADC(void);
void measureDcDcTemperature(void); 
void measureChannel2(void);
void measureChannel3(void);
void measureChannel4(void);
void commandSwitchHKdata(UBYTE, UBYTE, UBYTE, UBYTE);
UBYTE read5VBusAndSwitchNtrxPower(void);

#endif	/* ADC_H */

