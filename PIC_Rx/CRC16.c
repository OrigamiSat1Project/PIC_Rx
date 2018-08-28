//#include "string.h"
#include "Type_define.h"


/*
 * CRC-16-CCITT with the polynom: x^16+x^12+x^5+1
 * initial value 0, left shift, no out put inversion
 */
UWORD crc16(UWORD crc, UBYTE *ptr, UINT len){
    const UWORD CRC16POLY = 0xa001;
	UINT i, j;
	crc = ~crc;
	for (i = 0; i < len; i++) {
		crc ^= ptr[i];
		for (j = 0; j < 8; j++) {
			if (crc & 1) {
				crc = (crc >> 1) ^ CRC16POLY;
			}
			else {
				crc >>= 1;
			}
		}
	}
	return ~crc;
}


UWORD checkCRC(UBYTE *command, UINT sizeBeforeCRC){
    UWORD checkReceive = 0x0000;
    checkReceive |= command[sizeBeforeCRC];
    checkReceive = checkReceive << 8;
    checkReceive |= command[sizeBeforeCRC +1];
    return checkReceive;
}