//#include <string.h>
#include <xc.h>
#include "EPS.h"
#include "time.h"

/*
 * 【EPSリセ�?ト機�?�の初期化�??
 *  1. EPSリセ�?トスイ�?チ�?�端子を'出�?'に設�?
 *  2. EPSリセ�?トスイ�?チ�?�ポ�?�トを'LOW'にする
 */

/*
 * 【EPSリセ�?トスイ�?チ�?��?�?
 *   1. EPSリセ�?トスイ�?チ�?�端子を0�?1�?0と変化させ�?
 */
//UBYTE EPS_kill[];


void Reset_EPS(void){
    SEP_SW = 1;
    RBF_SW = 1;
    __delay_ms(5000);
    SEP_SW = 0;
    RBF_SW = 0;
    __delay_ms(500);
}

