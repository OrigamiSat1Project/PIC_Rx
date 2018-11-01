
#include <xc.h>
#include "FMCW.h"
#include "pinDefine.h"
#include "time.h"
#include "typeDefine.h"
#include "OkError.h"

/*Identifiers for the radio units (called unitID if given to a functions)*/
/* Do not change!! */
#define FMTX_ID   1
#define CWTX_ID   2
#define FMRX_ID   3

/*Methods*/
void sendLow(UBYTE unitID);
void sendHigh(UBYTE unitID);
void sendSTB(UBYTE unitID);
void setNprg(UBYTE unitID, UBYTE *Nprg);
void setNref(UBYTE unitID, int Nref);
void setOptionRegister(UBYTE unitID);
void _NOP(void);


/* 
 * [Send 'Low' to radio]
 * 1. Select which radio unit to send to (FMTX or FMRX or CWTX)
 * 2. Set the DAT terminal to Low
 * 3. Change the CLK pin from 0 -> 1 -> 0
 */
void sendLow(UBYTE unitID){
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
 * 3. Change the CLK pin from 0 -> 1 -> 0
 */
void sendHigh(UBYTE unitID){
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
 * 2. Change the STB pin from 0 -> 1 -> 0
 */
void sendSTB(UBYTE unitID){
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
void setNprg(UBYTE unitID, UBYTE *Nprg){
    UBYTE count = 0;
    UBYTE Nprg_b[17];
    
    for(UBYTE i=0; i<17; i++){
        Nprg_b[i] = 0;
    }
    
    //Nprg transforms decimal to binary
    for(UBYTE i = 0; i < 17; i++){
        for(UBYTE j = 0; j<5; j++){
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
    
    //Send Nprg data(binary) to communication module
    for (UBYTE i=0; i<17; i++)
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
 * 2. Send High or Low to the radio according to the stored binary number (setting of the liver) ?¿½i?¿½Ý’ï¿½ÌŠÌj???
 * 3. Send group code '11'
 * 4. Send STB signal
 */
void setNref(UBYTE unitID, int Nref){
    UBYTE Nref_b[12];
    
    for(UBYTE i=0; i<12; i++){
        Nref_b[i] = 0;
    }
    
    //Nref transforms decimal to binary //Why not use same definition and Transformation for Nprg???
    for(int i=0; Nref>0; i++){
        Nref_b[i] = Nref % 2;
        Nref = Nref / 2;
    }
    
    //Send Nref data(binay) to communication module
    for (UBYTE i=0; i<12; i++)
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
void setOptionRegister(UBYTE unitID){
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
void FMTX(int Nref, UBYTE *Nprg){
   UBYTE fmtx = FMTX_ID;
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
void CWTX(int Nref, UBYTE *Nprg){
   UBYTE cwtx = CWTX_ID;
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
 void FMRX(int Nref, UBYTE *Nprg){
     UBYTE fmrx = FMRX_ID;
     setOptionRegister(fmrx);
     setNref(fmrx, Nref);
     setNprg(fmrx, Nprg);
 }

// void setPLL(void){    
//     /*---FMTX---*/
//     setOptionRegister(FMTX_ID);
//     setNref(FMTX_ID, FMTX_Nref);
//     setNprg(FMTX_ID, FMTX_Nprg);
//     /*---CWTX---*/
//     setOptionRegister(CWTX_ID);
//     setNref(CWTX_ID, CWTX_Nref);
//     setNprg(CWTX_ID, CWTX_Nprg);
//     /*---FMRX---*/
//     setOptionRegister(FMRX_ID);
//     setNref(FMRX_ID, FMRX_Nref);
//     setNprg(FMRX_ID, FMRX_Nprg);
// } 
 

/*
  * [Perform PLL setting]// TODO: check pointers and replace in the main.c, uncomment in FMCW.h
 */
//void SetPLL(int FMTX_Nref, int FMTX_Nprg, int CWTX_Nref, int CWTX_Nprg, int FMRX_Nref, int FMRX_Nprg){
//    FMTX(FMTX_Nref, FMTX_Nprg);
//    CWTX(CWTX_Nref, CWTX_Nprg);
//    FMRX(FMRX_Nref, FMRX_Nprg);
//}
void setPLL(void){
   FMTX(FMTX_Nref, FMTX_Nprg);
   CWTX(CWTX_Nref, CWTX_Nprg);
   FMRX(FMRX_Nref, FMRX_Nprg);
}


//process command data if the command type is 'radio unit'
void commandSwitchFMCW(UBYTE command, UBYTE Nref1, UBYTE Nref2, UBYTE Nprg1, UBYTE Nprg2, UBYTE Nprg3){ //TODO: specify which Nref and Nprg are which
    switch(command){    
        case 't': //FM TX
            //TODO: write method for FM TX
            break;
        case 'c': //CW TX
            //TODO: write method for CW TX
            break;
        case 'f': //FM RX
            //TODO: write method for FM RX
            break;
        default:
            switchError(error_FMCW_commandSwitchFMCW);
            break;
    }
}

/*
 * [Do not process anything (standby)]
 * (Wait for 5 loop iterations)
 */
void _NOP(void) {
    for(UBYTE i=0; i<5; i++){
        NOP();
    }
}
