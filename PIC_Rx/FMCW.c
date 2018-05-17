
#include <xc.h>
#include "FMCW.h"

/* Do not change!! */
#define FMTX_   1
#define CWTX_   2
#define FMRX_   3

#include "time.h"

/*
 * 縲色MCW險ｭ螳壹?ｮ蛻晄悄蛹悶??
 *  FM蜿嶺ｿ｡?ｼ熊M騾∽ｿ｡?ｼ靴W騾∽ｿ｡縺昴ｌ縺槭ｌ繧定ｨｭ螳?
 *  1. CLK遶ｯ蟄撰ｼ医け繝ｭ繝?繧ｯ?ｼ会ｼ轡AT遶ｯ蟄撰ｼ医ョ繝ｼ繧ｿ?ｼ会ｼ郡TB遶ｯ蟄撰ｼ医せ繝医Ο繝ｼ繝厄ｼ峨ｒ蜃ｺ蜉帙→縺励※菴ｿ逕ｨ
 *  2. 蜈ｨ縺ｦ縺ｮ繝昴?ｼ繝医ｒLow縺ｫ縺吶ｋ
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
 * 縲千┌邱壽ｩ溘↓'Low'繧帝?√ｋ縲?
 *  1. 縺ｩ縺ｮ辟｡邱壽ｩ溘↓騾√ｋ縺矩∈謚橸ｼ?FMTX or FMRX or CWTX?ｼ?
 *  2. DAT遶ｯ蟄舌ｒLow縺ｫ縺吶ｋ
 *  3. CLK遶ｯ蟄舌ｒ0竊?1竊?0縺ｨ螟牙喧縺輔○繧?
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
 * 縲千┌邱壽ｩ溘↓'High'繧帝?√ｋ縲?
 *  1. 縺ｩ縺ｮ辟｡邱壽ｩ溘↓騾√ｋ縺矩∈謚橸ｼ?FMTX or FMRX or CWTX?ｼ?
 *  2. DAT遶ｯ蟄舌ｒHigh縺ｫ縺吶ｋ
 *  3. CLK遶ｯ蟄舌ｒ0竊?1竊?0縺ｨ螟牙喧縺輔○繧? 
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
 * 縲千┌邱壽ｩ溘↓STB菫｡蜿ｷ繧帝?√ｋ縲?
 *  1. 縺ｩ縺ｮ辟｡邱壽ｩ溘↓騾√ｋ縺矩∈謚橸ｼ?FMTX or FMRX or CWTX?ｼ?
 *  2. STB遶ｯ蟄舌ｒ0竊?1竊?0縺ｨ螟牙喧縺輔○繧?
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
 * 縲千┌邱壽ｩ溘?ｮ繝励Ο繧ｰ繝ｩ繝槭ヶ繝ｫ繧ｫ繧ｦ繝ｳ繧ｿ繧定ｨｭ螳壹☆繧九??
 *  1. 蠑墓焚縺九ｉ隱ｭ縺ｿ霎ｼ繧薙□繝励Ο繧ｰ繝ｩ繝槭ヶ繝ｫ繧ｫ繧ｦ繝ｳ繧ｿ繧?2騾ｲ謨ｰ縺ｫ螟画鋤?ｼ磯?榊?励→縺励※譬ｼ邏搾ｼ?
 *  2. 譬ｼ邏阪＠縺?2騾ｲ謨ｰ縺ｫ蜷医ｏ縺帙※High縺記ow繧堤┌邱壽ｩ溘↓騾√ｋ?ｼ郁ｨｭ螳壹?ｮ閧晢ｼ?
 *  3. 繧ｰ繝ｫ繝ｼ繝励さ繝ｼ繝峨ｒ騾√ｋ'10'
 *  4. STB菫｡蜿ｷ繧帝?√ｋ
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
 * 縲千┌邱壽ｩ溘?ｮ繝ｪ繝輔ぃ繝ｬ繝ｳ繧ｹ繧ｫ繧ｦ繝ｳ繧ｿ繧定ｨｭ螳壹☆繧九??
 *  1. 蠑墓焚縺九ｉ隱ｭ縺ｿ霎ｼ繧薙□繝ｪ繝輔ぃ繝ｬ繝ｳ繧ｹ繧ｫ繧ｦ繝ｳ繧ｿ繧?2騾ｲ謨ｰ縺ｫ螟画鋤?ｼ磯?榊?励→縺励※譬ｼ邏搾ｼ?
 *  2. 譬ｼ邏阪＠縺?2騾ｲ謨ｰ縺ｫ蜷医ｏ縺帙※High縺記ow繧堤┌邱壽ｩ溘↓騾√ｋ?ｼ郁ｨｭ螳壹?ｮ閧晢ｼ?
 *  3. 繧ｰ繝ｫ繝ｼ繝励さ繝ｼ繝峨ｒ騾√ｋ'11'
 *  4. STB菫｡蜿ｷ繧帝?√ｋ
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
 * 縲千┌邱壽ｩ溘?ｮ繧ｪ繝励す繝ｧ繝ｳ繝ｬ繧ｸ繧ｹ繧ｿ繧定ｨｭ螳壹☆繧具ｼ亥?ｱ騾啀LL險ｭ螳夲ｼ峨??
 *  1. (T1, T2, T3, CpT1, CpT2, Cpr1, Cpr2, LD1, LD2, Tx, Rx) = (0,0,0,1,1,0,0,0,0,0,1)繧帝?√ｋ
 *  2. 繧ｰ繝ｫ繝ｼ繝励さ繝ｼ繝峨ｒ騾√ｋ'00'
 *  3. STB菫｡蜿ｷ繧帝?√ｋ
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
 * 縲色MTX縺ｮPLL險ｭ螳壹ｒ陦後≧縲?
 *  1. 繧ｪ繝励す繝ｧ繝ｳ繝ｬ繧ｸ繧ｹ繧ｿ縺ｮ險ｭ螳?
 *  2. 繝ｪ繝輔ぃ繝ｬ繝ｳ繧ｹ繧ｫ繧ｦ繝ｳ繧ｿ縺ｮ險ｭ螳?
 *  3. 繝励Ο繧ｰ繝ｩ繝槭ヶ繝ｫ繧ｫ繧ｦ繝ｳ繧ｿ縺ｮ險ｭ螳?
 */
void FMTX(int Nref, int *Nprg){
    int fmtx = FMTX_;
    OPINIT(fmtx);
    RFDOUT(fmtx, Nref);
    OUTFQ(fmtx, Nprg);
}


/*
 * 縲燭WTX縺ｮPLL險ｭ螳壹ｒ陦後≧縲?
 *  1. 繧ｪ繝励す繝ｧ繝ｳ繝ｬ繧ｸ繧ｹ繧ｿ縺ｮ險ｭ螳?
 *  2. 繝ｪ繝輔ぃ繝ｬ繝ｳ繧ｹ繧ｫ繧ｦ繝ｳ繧ｿ縺ｮ險ｭ螳?
 *  3. 繝励Ο繧ｰ繝ｩ繝槭ヶ繝ｫ繧ｫ繧ｦ繝ｳ繧ｿ縺ｮ險ｭ螳?
 */
void CWTX(int Nref, int *Nprg){
    int cwtx = CWTX_;
    OPINIT(cwtx);
    RFDOUT(cwtx, Nref);
    OUTFQ(cwtx, Nprg);
}


/*
 * 縲色MRX縺ｮPLL險ｭ螳壹ｒ陦後≧縲?
 *  1. 繧ｪ繝励す繝ｧ繝ｳ繝ｬ繧ｸ繧ｹ繧ｿ縺ｮ險ｭ螳?
 *  2. 繝ｪ繝輔ぃ繝ｬ繝ｳ繧ｹ繧ｫ繧ｦ繝ｳ繧ｿ縺ｮ險ｭ螳?
 *  3. 繝励Ο繧ｰ繝ｩ繝槭ヶ繝ｫ繧ｫ繧ｦ繝ｳ繧ｿ縺ｮ險ｭ螳?
 */
void FMRX(int Nref, int *Nprg){
    int fmrx = FMRX_;
    OPINIT(fmrx);
    RFDOUT(fmrx, Nref);
    OUTFQ(fmrx, Nprg);
}


/*
 * 縲娠LL險ｭ螳壹ｒ陦後≧縲?
 */
void SetPLL(int FMTX_Nref, int FMTX_Nprg, int CWTX_Nref, int CWTX_Nprg, int FMRX_Nref, int FMRX_Nprg){
    FMTX(FMTX_Nref, FMTX_Nprg);
    CWTX(CWTX_Nref, CWTX_Nprg);
    FMRX(FMRX_Nref, FMRX_Nprg);
}



/*
 * 縲舌Δ繝ｼ繝ｫ繧ｹ菫｡蜿ｷ縺ｮ'V'繧帝?√ｋ縲?
 *  1. CWKEY遶ｯ蟄舌ｒ0竊?1竊?0縺ｨ螟牙喧縺輔○繧?
 *  2. 窶ｻ1.繧定ｨ茨ｼ灘屓陦後≧
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
 * 縲蝉ｽ輔ｂ蜃ｦ逅?繧定｡後ｏ縺ｪ縺?ｼ亥ｾ?讖滂ｼ峨??
 *  5蜃ｦ逅?蛻?蠕?讖溘☆繧?
 */
void _NOP(void) {
    for(int i=0; i<5; i++){
        NOP();
    }
}
