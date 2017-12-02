
#include <xc.h>
#include "fmcw.h"

/* Do not change!! */
#define FMTX_   1
#define CWTX_   2
#define FMRX_   3

#include "time.h"

/*
 * 【FMCW設定の初期化】
 *  FM受信，FM送信，CW送信それぞれを設定
 *  1. CLK端子（クロック），DAT端子（データ），STB端子（ストローブ）を出力として使用
 *  2. 全てのポートをLowにする
 */
void Init_FMCW(void){
    /* Initialize (turn ports Low) */
    FMRX_CLK = 0;
    FMRX_DAT = 0;
    FMRX_STB = 0;
    FMTX_CLK = 0;
    FMTX_DAT = 0;
    FMTX_STB = 0;
    FMTX_PTT = 0;
    CWRX_CLK = 0;
    CWTX_DAT = 0;
    CWTX_STB = 0;
    CWTX_KEY = 0;
}


/*
 * 【無線機に'Low'を送る】
 *  1. どの無線機に送るか選択（FMTX or FMRX or CWTX）
 *  2. DAT端子をLowにする
 *  3. CLK端子を0→1→0と変化させる
 */
void L_OUT(int fmcwtxrx){
    if(fmcwtxrx == FMTX_)
    {
        FMTX_DAT = 0;
        _NOP();
        FMTX_CLK = 1;
        _NOP();
        FMTX_CLK = 0;
    }
    if(fmcwtxrx == CWTX_)
    {
        CWTX_DAT = 0;
        _NOP();
        CWRX_CLK = 1;
        _NOP();
        CWRX_CLK = 0;
    }
    if(fmcwtxrx == FMRX_)
    {
        FMRX_DAT = 0;
        _NOP();
        FMRX_CLK = 1;
        _NOP();
        FMRX_CLK = 0;
    }
}


/*
 * 【無線機に'High'を送る】
 *  1. どの無線機に送るか選択（FMTX or FMRX or CWTX）
 *  2. DAT端子をHighにする
 *  3. CLK端子を0→1→0と変化させる 
 */
void H_OUT(int fmcwtxrx){
    if(fmcwtxrx == FMTX_)
    {
        FMTX_DAT = 1;
        _NOP();
        FMTX_CLK = 1;
        _NOP();
        FMTX_CLK = 0;
    }
    if(fmcwtxrx == CWTX_)
    {
        CWTX_DAT = 1;
        _NOP();
        CWRX_CLK = 1;
        _NOP();
        CWRX_CLK = 0;
    }
    if(fmcwtxrx == FMRX_)
    {
        FMRX_DAT = 1;
        _NOP();
        FMRX_CLK = 1;
        _NOP();
        FMRX_CLK = 0;
    }
}


/*
 * 【無線機にSTB信号を送る】
 *  1. どの無線機に送るか選択（FMTX or FMRX or CWTX）
 *  2. STB端子を0→1→0と変化させる
 */
void STBOUT(int fmcwtxrx){
    if(fmcwtxrx == FMTX_)
    {
        FMTX_STB = 1;
        _NOP();
        FMTX_STB = 0;
    }
    if(fmcwtxrx == CWTX_)
    {
        CWTX_STB = 1;
        _NOP();
        CWTX_STB = 0;
    }
    if(fmcwtxrx == FMRX_)
    {
        FMRX_STB = 1;
        _NOP();
        FMRX_STB = 0;
    }
}


/*
 * 【無線機のプログラマブルカウンタを設定する】
 *  1. 引数から読み込んだプログラマブルカウンタを2進数に変換（配列として格納）
 *  2. 格納した2進数に合わせてHighかLowを無線機に送る（設定の肝）
 *  3. グループコードを送る'10'
 *  4. STB信号を送る
 */
void OUTFQ(int fmcwtxrx, int *Nprg){
    int count = 0;
    int Nprg_b[17];
    
    for(int i=0; i<17; i++){
        Nprg_b[i] = 0;
    }
    
    //Nprg transforms decimal to binary
    for(int i = 0; i < 17; i++){
        for(int j = 0; j<5; j++){
            if(Nprg[j] % 2 == 0) {
                if(j == 4){
                    Nprg[j] = Nprg[j] / 2;
                    Nprg_b[count] = 0;
                    count++;
                }
                else{
                    Nprg[j] = Nprg[j] / 2;
                }
            }
            else if(Nprg[j] % 2 == 1) {
                if(j == 4){
                    Nprg[j] = Nprg[j] / 2;
                    Nprg_b[count] = 1;
                    count++;
                }
                else{
                    Nprg[j] = Nprg[j] / 2;
                    Nprg[j+1] = Nprg[j+1] + 10;
                }
            }
        }
    }
    
    //Send Nprg data(binay) to communication module
    for (int i=0; i<17; i++)
    {
        if(Nprg_b[i] == 0)
        {
            L_OUT(fmcwtxrx);
        }
        if(Nprg_b[i] == 1)
        {
            H_OUT(fmcwtxrx);
        }
    }
    
    //GroupCode'10' is TX.DEV(?)
    H_OUT(fmcwtxrx);
    L_OUT(fmcwtxrx);
    
    //STB Signal
    STBOUT(fmcwtxrx);
}


/*
 * 【無線機のリファレンスカウンタを設定する】
 *  1. 引数から読み込んだリファレンスカウンタを2進数に変換（配列として格納）
 *  2. 格納した2進数に合わせてHighかLowを無線機に送る（設定の肝）
 *  3. グループコードを送る'11'
 *  4. STB信号を送る
 */
void RFDOUT(int fmcwtxrx, int Nref){
    int Nref_b[12];
    
    for(int i=0; i<12; i++){
        Nref_b[i] = 0;
    }
    
    //Nref transforms decimal to binary
    for(int i=0; Nref>0; i++){
        Nref_b[i] = Nref % 2;
        Nref = Nref / 2;
    }
    
    //Send Nref data(binay) to communication module
    for (int i=0; i<12; i++)
    {
        if(Nref_b[i] == 0)
        {
            L_OUT(fmcwtxrx);
        }
        if(Nref_b[i] == 1)
        {
            H_OUT(fmcwtxrx);
        }
    }
    
    //GroupCode'11' is REF.DEV
    H_OUT(fmcwtxrx);
    H_OUT(fmcwtxrx);
    
    //STB Signal
    STBOUT(fmcwtxrx);
}


/*
 * 【無線機のオプションレジスタを設定する（共通PLL設定）】
 *  1. (T1, T2, T3, CpT1, CpT2, Cpr1, Cpr2, LD1, LD2, Tx, Rx) = (0,0,0,1,1,0,0,0,0,0,1)を送る
 *  2. グループコードを送る'00'
 *  3. STB信号を送る
 */
void OPINIT(int fmcwtxrx){
    //Send PLL Common DataSet to communiction module
    L_OUT(fmcwtxrx);//T1
    L_OUT(fmcwtxrx);//T2
    L_OUT(fmcwtxrx);//T3
    H_OUT(fmcwtxrx);//CpT1
    H_OUT(fmcwtxrx);//CpT2
    L_OUT(fmcwtxrx);//Cpr1
    L_OUT(fmcwtxrx);//Cpr2
    L_OUT(fmcwtxrx);//LD1
    L_OUT(fmcwtxrx);//LD2
    L_OUT(fmcwtxrx);//Tx
    H_OUT(fmcwtxrx);//Rx
    
    //GroupCode'00' is option reg.
    L_OUT(fmcwtxrx);
    L_OUT(fmcwtxrx);
    
    //STB Signal
    STBOUT(fmcwtxrx);
}


/*
 * 【FMTXのPLL設定を行う】
 *  1. オプションレジスタの設定
 *  2. リファレンスカウンタの設定
 *  3. プログラマブルカウンタの設定
 */
void FMTX(int Nref, int *Nprg){
    int fmtx = FMTX_;
    OPINIT(fmtx);
    RFDOUT(fmtx, Nref);
    OUTFQ(fmtx, Nprg);
}


/*
 * 【CWTXのPLL設定を行う】
 *  1. オプションレジスタの設定
 *  2. リファレンスカウンタの設定
 *  3. プログラマブルカウンタの設定
 */
void CWTX(int Nref, int *Nprg){
    int cwtx = CWTX_;
    OPINIT(cwtx);
    RFDOUT(cwtx, Nref);
    OUTFQ(cwtx, Nprg);
}


/*
 * 【FMRXのPLL設定を行う】
 *  1. オプションレジスタの設定
 *  2. リファレンスカウンタの設定
 *  3. プログラマブルカウンタの設定
 */
void FMRX(int Nref, int *Nprg){
    int fmrx = FMRX_;
    OPINIT(fmrx);
    RFDOUT(fmrx, Nref);
    OUTFQ(fmrx, Nprg);
}


/*
 * 【PLL設定を行う】
 */
void SetPLL(int FMTX_Nref, int FMTX_Nprg, int CWTX_Nref, int CWTX_Nprg, int FMRX_Nref, int FMRX_Nprg){
    FMTX(FMTX_Nref, FMTX_Nprg);
    CWTX(CWTX_Nref, CWTX_Nprg);
    FMRX(FMRX_Nref, FMRX_Nprg);
}



/*
 * 【モールス信号の'V'を送る】
 *  1. CWKEY端子を0→1→0と変化させる
 *  2. ※1.を計３回行う
 */
void Morse_V(void){
    CWTX_KEY = 1;
    __delay_ms(50);
    CWTX_KEY = 0;
    __delay_ms(50);

    CWTX_KEY = 1;
    __delay_ms(50);
    CWTX_KEY = 0;
    __delay_ms(50);

    CWTX_KEY = 1;
    __delay_ms(50);
    CWTX_KEY = 0;
    __delay_ms(50);

    CWTX_KEY = 1;
    __delay_ms(150);
    CWTX_KEY = 0;
    __delay_ms(50);
}


/*
 * 【何も処理を行わない（待機）】
 *  5処理分待機する
 */
void _NOP(void) {
    for(int i=0; i<5; i++){
        NOP();
    }
}
