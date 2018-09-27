
#include <xc.h>
#include "FMCW.h"
#include "pinDefine.h"
#include "time.h"
#include "typeDefine.h"

/*Identifiers for the radio units (called unitID if given to a functions)*/
/* Do not change!! */
#define FMTX_ID   1
#define CWTX_ID   2
#define FMRX_ID   3

/*Methods*/
void sendLow(int unitID);
void sendHigh(int unitID);
void sendSTB(int unitID);
int binaryToDecimal(int n);
void setNprg(int unitID, int *Nprg);
void setNref(int unitID, int Nref);
void setOptionRegister(int unitID);
void _NOP(void);


/* 
 * [Send 'Low' to radio]
 * 1. Select which radio unit to send to (FMTX or FMRX or CWTX)
 * 2. Set the DAT terminal to Low
 * 3. Change the CLK pin from 0 �� 1 �� 0
 */
void sendLow(int unitID){
    if(unitID == FMTX_ID)
    {
        FMTX_DAT = 0;
        _NOP();
        FMTX_CLK = 1;
        _NOP();
        FMTX_CLK = 0;
    }
    if(unitID == CWTX_ID)
    {
        CWTX_DAT = 0;
        _NOP();
        CWTX_CLK = 1;
        _NOP();
        CWTX_CLK = 0;
    }
    if(unitID == FMRX_ID)
    {
        FMRX_DAT = 0;
        _NOP();
        FMRX_CLK = 1;
        _NOP();
        FMRX_CLK = 0;
    }
}


/*
 * [Send 'High' to radio]
 * 1. Select which radio unit to send to (FMTX or FMRX or CWTX)
 * 2. Set the DAT terminal to High
 * 3. Change the CLK pin from 0 �� 1 �� 0
 */
void sendHigh(int unitID){
    if(unitID == FMTX_ID)
    {
        FMTX_DAT = 1;
        _NOP();
        FMTX_CLK = 1;
        _NOP();
        FMTX_CLK = 0;
    }
    if(unitID == CWTX_ID)
    {
        CWTX_DAT = 1;
        _NOP();
        CWTX_CLK = 1;
        _NOP();
        CWTX_CLK = 0;
    }
    if(unitID == FMRX_ID)
    {
        FMRX_DAT = 1;
        _NOP();
        FMRX_CLK = 1;
        _NOP();
        FMRX_CLK = 0;
    }
}


/*
 * [Send STB signal to radio]
 * 1. Select which radio unit to send  to (FMTX or FMRX or CWTX)
 * 2. Change the STB pin from 0 �� 1 �� 0
 */
void sendSTB(int unitID){
    if(unitID == FMTX_ID)
    {
        FMTX_STB = 1;
        _NOP();
        FMTX_STB = 0;
    }
    if(unitID == CWTX_ID)
    {
        CWTX_STB = 1;
        _NOP();
        CWTX_STB = 0;
    }
    if(unitID == FMRX_ID)
    {
        FMRX_STB = 1;
        _NOP();
        FMRX_STB = 0;
    }
}


/*
 * [Setting the programmable counter of the radio]
 * 1. Convert programmable counter read from argument to binary number (stored as array)
 * 2. Send High or Low to the radio according to the stored binary number 
 * 3. Send group code '10'
 * 4. Send STB signal
 */
void setNprg(int unitID, int *Nprg){
    int Nprg_b[17];
    
    for(int i=0; i<17; i++){
        Nprg_b[i] = 0;
    }
    //Nref transforms decimal to binary //Why not use same definition and Transformation for Nprg???
    for(int i=0; Nprg>0; i++){
        Nprg_b[i] = Nprg % 2;
        Nprg = Nprg / 2;
    }
    
//    //OLD METHOD!! delete if new one works ! //Nprg transforms decimal to binary
//    int count = 0;
//    for(int i = 0; i < 17; i++){
//        for(int j = 0; j<5; j++){
//            if(Nprg[j] % 2 == 0) {
//                if(j == 4){
//                    Nprg[j] = Nprg[j] / 2;
//                    Nprg_b[count] = 0;
//                    count++;
//                }
//                else{
//                    Nprg[j] = Nprg[j] / 2;
//                }
//            }
//            else if(Nprg[j] % 2 == 1) {
//                if(j == 4){
//                    Nprg[j] = Nprg[j] / 2;
//                    Nprg_b[count] = 1;
//                    count++;
//                }
//                else{
//                    Nprg[j] = Nprg[j] / 2;
//                    Nprg[j+1] = Nprg[j+1] + 10;
//                }
//            }
//        }
//    }
    
    //Send Nprg data(binary) to communication module
    
    for (int i=0; i<17; i++)
    {
        if(Nprg_b[i] == 0)
        {
            sendLow(unitID);
        }
        if(Nprg_b[i] == 1)
        {
            sendHigh(unitID);
        }
    }
    
    //GroupCode'10' is TX.DEV(?)
    sendHigh(unitID);
    sendLow(unitID);
    
    //STB Signal
    sendSTB(unitID);
}


/*
 * [Setting the reference counter of the radio]
 * 1. Convert reference counter read from argument to binary number (stored as array)
 * 2. Send High or Low to the radio according to the stored binary number (setting of the liver) �i�ݒ�̊́j???
 * 3. Send group code '11'
 * 4. Send STB signal
 */
void setNref(int unitID, int Nref){
    int Nref_b[12];
    
    for(int i=0; i<12; i++){
        Nref_b[i] = 0;
    }
    
    //Nref transforms decimal to binary //Why not use same definition and Transformation for Nprg???
    for(int i=0; Nref>0; i++){
        Nref_b[i] = Nref % 2;
        Nref = Nref / 2;
    }
    
    //Send Nref data(binay) to communication module
    for (int i=0; i<12; i++)
    {
        if(Nref_b[i] == 0)
        {
            sendLow(unitID);
        }
        if(Nref_b[i] == 1)
        {
            sendHigh(unitID);
        }
    }
    
    //GroupCode'11' is REF.DEV
    sendHigh(unitID);
    sendHigh(unitID);
    
    //STB Signal
    sendSTB(unitID);
}


/*
 * [Setting Option Register of Radio Equipment (Common PLL Setting)(PLL = Phase-Locked Loop)
 * 1 (T1, T2, T3, CpT1, CpT2, Cpr1, Cpr2, LD1, LD2, Tx, Rx) = (0,0,0,1,1,0,0,0,0,0,1) Send
 * 2. Send group code '00'
 * 3. Send STB signal
 */
void setOptionRegister(int unitID){
    //Send PLL Common DataSet to communiction module
    sendLow(unitID);//T1
    sendLow(unitID);//T2
    sendLow(unitID);//T3
    sendHigh(unitID);//CpT1
    sendHigh(unitID);//CpT2
    sendLow(unitID);//Cpr1
    sendLow(unitID);//Cpr2
    sendLow(unitID);//LD1
    sendLow(unitID);//LD2
    sendLow(unitID);//Tx
    sendHigh(unitID);//Rx
    
    //GroupCode'00' is option reg.
    sendLow(unitID);
    sendLow(unitID);
    
    //STB Signal
    sendSTB(unitID);
}


/*
 * [Set PLL for FMTX]
 * 1. Setting of Option register
 * 2. Setting of Reference counter
 * 3. Setting of programmable counter
 */
void FMTX(int Nref, int *Nprg){
    int fmtx = FMTX_ID;
    setOptionRegister(fmtx);
    setNref(fmtx, Nref);
    setNprg(fmtx, Nprg);
}


/*
 * [Set PLL for CWTX]
 * 1. Setting of Option register
 * 2. Setting of Reference counter
 * 3. Setting of programmable counter
 */
void CWTX(int Nref, int *Nprg){
    int cwtx = CWTX_ID;
    setOptionRegister(cwtx);
    setNref(cwtx, Nref);
    setNprg(cwtx, Nprg);
}


/*
 * [Set PLL for FMRX]
 * 1. Setting of Option register
 * 2. Setting of Reference counter
 * 3. Setting of programmable counter
 */
void FMRX(int Nref, int *Nprg){
    int fmrx = FMRX_ID;
    setOptionRegister(fmrx);
    setNref(fmrx, Nref);
    setNprg(fmrx, Nprg);
}


int binaryToDecimal(int n){
    int num = n; 
    int dec_value = 0;
    
    // Initializing count value to 1, i.e 2^0 
    int count = 1; 
      
    int temp = num; 
    while (temp) 
    { 
        int last_digit = temp % 10; 
        temp = temp/10; 
          
        dec_value += last_digit*count; 
          
        count = count*2; 
    } 
      
    return dec_value; 
}

int calculateNref(UBYTE Nref_high, UBYTE Nref_low){
    int Nref;
    Nref = Nref_high<<8 | Nref_low;
    
    Nref = binaryToDecimal(Nref);
    
    return Nref;
}

int calculateNprg(UBYTE Nprg_high, UBYTE Nprg_middle, UBYTE Nprg_low){
    int Nprg;
    Nprg = Nprg_high<<8 | Nprg_middle;
    Nprg = Nprg<<8 | Nprg_low;
    
    Nprg = binaryToDecimal(Nprg);
    
    return Nprg;
}

/*
  * [Perform PLL setting]// TODO: check pointers and replace in the main.c, uncomment in FMCW.h
 */
//void SetPLL(int FMTX_Nref, int *FMTX_Nprg, int CWTX_Nref, int *CWTX_Nprg, int FMRX_Nref, int *FMRX_Nprg){
//    FMTX(FMTX_Nref, FMTX_Nprg);
//    CWTX(CWTX_Nref, CWTX_Nprg);
//    FMRX(FMRX_Nref, FMRX_Nprg);
//}

//process command data if the command type is 'radio unit'
void commandSwitchFMCW(UBYTE command, UBYTE Nref_high, UBYTE Nref_low, UBYTE Nprg_high, UBYTE Nprg_middle, UBYTE Nprg_low){ //TODO: specify which Nref and Nprg are which    
    int Nref;
    int Nprg;
    Nref = calculateNref(Nref_high, Nref_low);
    Nprg = calculateNprg(Nprg_high, Nprg_middle, Nprg_low);
    switch(command){    
        case 't': //FM TX
            FMTX(Nref, Nprg);
            break;
        case 'c': //CW TX
            CWTX(Nref, Nprg);
            break;
        case 'f': //FM RX
            FMRX(Nref, Nprg);
            break;
        default:
            //TODO: error message
            break;
    }
}

/*
 * [Do not process anything (standby)]
 * (Wait for 5 loop iterations)
 */
void _NOP(void) {
    for(int i=0; i<5; i++){
        NOP();
    }
}
