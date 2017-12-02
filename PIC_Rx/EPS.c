//#include <string.h>
#include <xc.h>
#include "EPS.h"
#include "time.h"

/*
 * 【EPSリセット機能の初期化】
 *  1. EPSリセットスイッチの端子を'出力'に設定
 *  2. EPSリセットスイッチのポートを'LOW'にする
 */

/*
 * 【EPSリセットスイッチ処理】
 *   1. EPSリセットスイッチの端子を0→1→0と変化させる
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

