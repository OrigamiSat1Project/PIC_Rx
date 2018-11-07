/* 
 * File:   CRC16.h
 * Author: Descartes
 * content: functions for cyclic redundancy check
 * Created on 2018/05/23, 0:45
 */

#ifndef initial_operation_H
#define	initial_operation_H

#include "typeDefine.h"

#define SATMODE_NOMINAL    0x50 //FIX ME: need change
#define SATMODE_SAVING    0x60 //FIX ME: need change
#define SATMODE_SURVIVAL  0xA0 //FIX ME: need change

void testInitialOpe(void); //for debug
void errorCheckInitialOpe(void); //for debug
UBYTE InitialOperation(void);
UBYTE bitCalResult(UBYTE);
UBYTE checkMeltingStatus(UBYTE);


#endif	/* initial_operation_H */

