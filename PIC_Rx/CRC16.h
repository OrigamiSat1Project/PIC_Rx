/* 
 * File:   CRC16.h
 * Author: Descartes
 * content: functions for cyclic redundancy check
 * Created on 2018/05/23, 0:45
 */

#ifndef CRC16_H
#define	CRC16_H

#include "Type_define.h"

UWORD crc16(UWORD, UBYTE *, UINT);
UWORD identifyCRC16(UBYTE *);                                          //???   this method does not exist in cfile
UWORD checkCRC(UBYTE *, UINT);

#endif	/* CRC16_H */

