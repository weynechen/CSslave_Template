/**
* @file         bsp.h
* @author       Weyne Chen
* @version      V01
* @date         2017.09.23
* @brief         
* @note          
* @attention    COPYRIGHT WEYNE
**/

#ifndef __BSP_H
#define __BSP_H
#include "stdint.h"

typedef enum {
    KEY_UP,
    KEY_DOWN,
    KEY_POWER,
    KEY_MTP,
    KEY_TP,
    KEY_NULL = 0xff,
} KeyTypeDef;

void ScanKey(void);
void MCLK_Init(uint16_t f_out);

extern KeyTypeDef CtrlKey;

/**
 * @breif  LCD时序参数定义
 */
 typedef struct
 {
   uint16_t DCLK;
   uint16_t LCDH;
   uint16_t LCDV;
 
   uint16_t HBPD;
   uint16_t HFPD;
   uint16_t HSPW;
 
   uint16_t VBPD;
   uint16_t VFPD;
   uint16_t VSPW;
 } LCDTimingParaTypeDef;
 extern LCDTimingParaTypeDef LCDTiming;
 

#endif
/********************* (C) COPYRIGHT WEYNE CHEN *******END OF FILE ********/
