/* 
 * File:   CRC16.h
 * Author: Descartes
 * content: functions for cyclic redundancy check
 * Created on 2018/05/23, 0:45
 */

#ifndef initial_operation_H
#define	initial_operation_H

#include "typeDefine.h"

/*---Initial Operation---*/
#define MELTING_FINISH        4  
#define WAIT_TIME_FOR_SETTING 4  //[s] 
#define MELTING_COUNTER_LIMIT 71


void testInitialOpe(void); //for debug
void errorCheckInitialOpe(void); //for debug
UBYTE InitialOperation(void);
UBYTE bitCalResult(UBYTE);
UBYTE checkMeltingStatus(UBYTE);


#endif	/* initial_operation_H */

