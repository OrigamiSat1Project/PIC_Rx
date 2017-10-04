/* 
 * File:   EPS.h
 * Author: Kosuke
 *
 * Created on 2017/04/18, 21:17
 */

#ifndef EPS_H
#define	EPS_H

/* EPSキルスイッチのピン配置を定義 */
#define SEP_SW        RE0
#define RBF_SW        RE1


void Reset_EPS(void);                       // Reset EPS kill switch

#endif	/* EPS_H */

