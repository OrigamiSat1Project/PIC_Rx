/* 
 * File:   FMCW.h
 * Author: Kosuke
 *
 * Created on 2017/04/18, 17:45
 */

#ifndef FMCW_H
#define	FMCW_H

//#include "time.h"

int FMTX_Nref        =   2560;          // Nref = 2560 (const)
int FMTX_Nprg[5]     =   {8,7,3,0,0};   // Nprg = 87300 = Ftx / 0.05 [→436.500MHz]
int CWTX_Nref        =   160;           // Nref = * see 301ACWPLL-20080520.pdf *
int CWTX_Nprg[5]     =   {0,1,7,4,7};   // Nprg = 1747(* see 301ACWPLL-20080520.pdf *) [→436.750MHz]
int FMRX_Nref        =   2560;          // Nref = 2560 (const)
int FMRX_Nprg[5]     =   {2,4,8,8,7};   // Nprg = 24887 = (Frx - 21.4) / 0.05 [→145.835MHz]

/* FMRXのピン配置を定義 */
#define FMRX_CLK        PORTAbits.RA2
#define FMRX_DAT        PORTAbits.RA3
#define FMRX_STB        PORTAbits.RA4
/* FMTXのピン配置を定義 */
#define FMTX_CLK        PORTEbits.RE1
#define FMTX_DAT        PORTEbits.RE0
#define FMTX_STB        PORTAbits.RA5
#define FMTX_PTT        PORTCbits.RC2
/* CWTXのピン配置を定義 */
#define CWRX_CLK        PORTDbits.RD2//RX→TX?
#define CWTX_DAT        PORTDbits.RD1
#define CWTX_STB        PORTDbits.RD0
#define CWTX_KEY        PORTDbits.RD3

void Init_FMCW(void);

void FMTX(int Nref, int *Nprg);
void CWTX(int Nref, int *Nprg);
void FMRX(int Nref, int *Nprg);
/*void OUTFQ(int fmcwtxrx, int *Nprg);
void RFDOUT(int fmcwtxrx, int Nref);
void OPINIT(int fmcwtxrx);
void L_OUT(int fmcwtxrx);
void H_OUT(int fmcwtxrx);
void STBOUT(int fmcwtxrx);
void Morse_V(void);*/
void _NOP(void);
//void SetPLL(int FMTX_Nref, int FMTX_Nprg, int CWTX_Nref, int CWTX_Nprg, int FMRX_Nref, int FMRX_Nprg);

#endif	/* FMCW_H */

