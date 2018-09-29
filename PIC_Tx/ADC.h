/* 
 * File:   ADC.h
 * Author: kosuk
 *
 * Created on 2018/08/25, 16:55
 */

#ifndef ADC_H
#define	ADC_H

#define CHANEL_SIZE 4

void initADC(void);
UWORD adc_read();
void measureAllChanelADC(void);
void measure1ChanelADC(UBYTE, UBYTE, UBYTE);

#endif	/* ADC_H */

