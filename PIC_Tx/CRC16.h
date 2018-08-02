/* 
 * File:   CRC16.h
 * Author: Descartes
 *
 * Created on 2018/05/23, 0:45
 */

#ifndef CRC16_H
#define	CRC16_H

#include "typedefine.h"

UWORD crc16(UWORD, UBYTE *, UINT);
UWORD Identify_CRC16(UBYTE *);
UWORD CRC_check(UBYTE *, UINT);

#endif	/* CRC16_H */

