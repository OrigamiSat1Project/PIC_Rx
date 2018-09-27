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

// set the values for reference counter(Nref) and programmable counter(Nprg) //TODO: rewrite Nprg values from arrays to integers
int FMTX_Nref        =   2560;          // Nref = 2560 (const)
int FMTX_Nprg        =   87501;         // Nprg = 87501 = Ftx / 0.05 [437.505MHz]
int CWTX_Nref        =   128;           // Nref = * see 301ACWPLL-20080520.pdf *
int CWTX_Nprg        =   1400;          // Nprg = 1400(* see 301ACWPLL-20080520.pdf *) [437.505MHz]
int FMRX_Nref        =   2560;          // Nref = 2560 (const)
int FMRX_Nprg        =   24916;         // Nprg = 24916 = (Frx - 21.4) / 0.05 [145.980MHz]

//function declarations:
void FMTX(int Nref, int *Nprg);
void CWTX(int Nref, int *Nprg);
void FMRX(int Nref, int *Nprg);

int calculateNref(UBYTE, UBYTE);
int calculateNprg(UBYTE, UBYTE, UBYTE);

void commandSwitchFMCW(UBYTE, UBYTE, UBYTE, UBYTE, UBYTE, UBYTE);

/*unused function declaration*/
//void SetPLL(int FMTX_Nref, int FMTX_Nprg, int CWTX_Nref, int CWTX_Nprg, int FMRX_Nref, int FMRX_Nprg);

#endif	/* FMCW_H */

