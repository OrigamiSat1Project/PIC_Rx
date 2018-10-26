/* 
 * File:   FMCW.h
 * Author: Kosuke
 *
 * Created on 2017/04/18, 17:45
 * 
 * Content: Set-up PLL 
 */

#ifndef FMCW_H
#define	FMCW_H

//#include "time.h"
#include "typeDefine.h"

////// set the values for reference counter(Nref) and programmable counter(Nprg) //TODO: rewrite Nprg values from arrays to integers
//#define FMTX_Nref           2560          // Nref = 2560 (const)
//UBYTE FMTX_Nprg[5]     =   {8,7,5,0,1};   // Nprg = 87501 = Ftx / 0.05 [437.505MHz]
//#define CWTX_Nref           128           // Nref = * see 301ACWPLL-20080520.pdf *
//UBYTE CWTX_Nprg[5]     =   {0,1,4,0,0};   // Nprg = 1400(* see 301ACWPLL-20080520.pdf *) [437.505MHz]
//#define FMRX_Nref           2560          // Nref = 2560 (const)
//UBYTE FMRX_Nprg[5]     =   {2,4,9,1,6};   // Nprg = 24916 = (Frx - 21.4) / 0.05 [145.980MHz]
//
////function declarations:
//void FMTX(int Nref, UBYTE *Nprg);
//void CWTX(int Nref, UBYTE *Nprg);
//void FMRX(int Nref, UBYTE *Nprg);
//void commandSwitchFMCW(UBYTE, UBYTE, UBYTE, UBYTE, UBYTE, UBYTE);
////void FMTX(int Nref, int *Nprg);
////void CWTX(int Nref, int *Nprg);
////void FMRX(int Nref, int *Nprg);
//
//
///*unused function declaration*/
////void SetPLL(int FMTX_Nref, int FMTX_Nprg, int CWTX_Nref, int CWTX_Nprg, int FMRX_Nref, int FMRX_Nprg);
//void setPLL(void);

// set the values for reference counter(Nref) and programmable counter(Nprg) //TODO: rewrite Nprg values from arrays to integers
#define FMTX_Nref           2560         // Nref = 2560 (const)
#define FMTX_Nprg           87501         // Nprg = 87501 = Ftx / 0.05 [437.505MHz]
#define CWTX_Nref           128           // Nref = * see 301ACWPLL-20080520.pdf *
#define CWTX_Nprg           1400          // Nprg = 1400(* see 301ACWPLL-20080520.pdf *) [437.505MHz]
#define FMRX_Nref           2560          // Nref = 2560 (const)
#define FMRX_Nprg           24916         // Nprg = 24916 = (Frx - 21.4) / 0.05 [145.980MHz]

/*---*/
//                            0b65432109876543210
#define FMTX_Nref_binary           0b101000000000         // Nref = 2560 (const)
#define FMTX_Nprg_binary      0b10101010111001101         // Nprg = 87501 = Ftx / 0.05 [437.505MHz]
#define CWTX_Nref_binary           0b000010000000           // Nref = * see 301ACWPLL-20080520.pdf *
#define CWTX_Nprg_binary      0b00000010101111000          // Nprg = 1400(* see 301ACWPLL-20080520.pdf *) [437.505MHz]
#define FMRX_Nref_binary           0b101000000000   // Nref = 2560 (const)
#define FMRX_Nprg_binary      0b00110000101010100         // Nprg = 24916 = (Frx - 21.4) / 0.05 [145.980MHz]

//function declarations:
void FMTX(UWORD Nref, USLONG Nprg);
void CWTX(UWORD Nref, USLONG Nprg);
void FMRX(UWORD Nref, USLONG Nprg);

UWORD calculateNref(UBYTE, UBYTE);
USLONG calculateNprg(UBYTE, UBYTE, UBYTE);

void commandSwitchFMCW(UBYTE, UBYTE, UBYTE, UBYTE, UBYTE, UBYTE);


#endif	/* FMCW_H */

