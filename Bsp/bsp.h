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

/*****************按键接口*************************/
typedef enum
{
  KEY_UP,
  KEY_DOWN,
  KEY_POWER,
  KEY_MTP,
  KEY_TP,
  KEY_NULL = 0xff,
} KeyTypeDef;

KeyTypeDef Key_Scan(void);
/********************************************/

/***************时钟配置********************/
void MCLK_Init(uint16_t frequency);

/*****************************************/

/***********LCD 驱动接口***********************/

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

void LCDDrv_WriteData(uint8_t para);
void LCDDrv_SetTiming(void);
void LCDDrv_OpenRGB(void);
void LCDDrv_CloseRGB(void);
void LCDDrv_EnterLVDS(uint8_t mode);
void LCDDrv_SetPattern(void);
void LCDDrv_SetXY(uint16_t x, uint16_t y);
void LCDDrv_SetCharIndex(uint8_t frame);
void LCDDrv_ShowPattern(uint32_t data);

/***************************************/

/***********电源接口********************/
typedef enum
{
  POWER_1V8,
  POWER_2V8,
  POWER_3V3,
  POWER_VSP,
  POWER_VSN,
  POWER_OUT5V,
  POWER_MTP,
  POWER_AVDD,
  POWER_VCOM,
  POWER_VGH,
  POWER_VGL,
  POWER_AMOUNT,
} PowerTypeDef;

typedef enum
{
  OFF,
  ON
} StateTypeDef;
void Power_SetState(PowerTypeDef power, StateTypeDef state);
void Power_SetBLCurrent(uint16_t value);

/***************************************/

/***************RGB接口***********************/
typedef enum
{
  RGB_SPI_RISING,
  RGB_SPI_FALLING,
} SPIEdgeTypeDef;

typedef enum
{
  RGB_SPI_COMMAND,
  RGB_SPI_DATA,
} SPIDataCommandTypeDef;

uint8_t RGB_SPIRead_16Bit(uint16_t address, SPIEdgeTypeDef edge);
void RGB_SPIWrite_16Bit(uint16_t address, uint8_t data, SPIEdgeTypeDef edge);
void RGB_SPIWrite8Bit(uint8_t data, SPIDataCommandTypeDef rs);
void RGB_SPIWrite9Bit(uint8_t data, SPIDataCommandTypeDef rs);

/***************************************/

/***************SSD2828寄存器接口***********************/
typedef enum
{
  MIPI_READ_FAIL,
  MIPI_READ_SUCCEED
} MIPI_ReadTypeDef;

typedef enum
{
  LP,
  HS,
  VD
} MIPI_ModeTypeDef;

void SSD2828_SetReset(uint8_t state);
void SSD2828_Reset(void);
void SSD2828_ShutDown(uint8_t state);

void SSD2828_WriteCmd(uint8_t cmd);
void SSD2828_WriteData(uint8_t data);
uint8_t SSD2828_ACK(void);

void SSD2828_WriteReg(uint8_t cmd, uint8_t data_high, uint8_t data_low);
uint16_t SSD2828_ReadReg(uint8_t reg);
/****************************************************/

/***********SSD2828 MIPI操作接口**********************/
void MIPI_SetMode(MIPI_ModeTypeDef mode);
void MIPI_DcsShortWrite(uint8_t data);
void MIPI_DcsLongWrite(uint32_t data);
void MIPI_GenericShortWrite(uint8_t data);
void MIPI_GenericLongWrite(uint32_t data);
MIPI_ReadTypeDef MIPI_DcsRead(uint8_t address, uint16_t length, uint8_t *return_value);
MIPI_ReadTypeDef MIPI_GenericRead(uint8_t adr, uint16_t l, uint8_t *return_value);
/****************************************************/

void APP_Run(void);
#endif
/********************* (C) COPYRIGHT WEYNE CHEN *******END OF FILE ********/
