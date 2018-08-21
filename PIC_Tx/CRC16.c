#include "string.h"
#include "Type_define.h"

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


UWORD CRC_check(UBYTE *Command, UINT Size_before_CRC){
    UWORD CRC_check_receive = 0x0000;
    CRC_check_receive |= Command[Size_before_CRC];
    CRC_check_receive = CRC_check_receive << 8;
    CRC_check_receive |= Command[Size_before_CRC +1];
    return CRC_check_receive;
}
