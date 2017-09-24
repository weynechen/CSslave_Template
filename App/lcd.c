/**
 * @file         lcd.c
 * @author       Weyne Chen
 * @version      V01
 * @date         2017.09.24
 * @brief
 * @note
 * @attention    COPYRIGHT WEYNE
 **/

#include "lcd.h"
#include "stdio.h"
#include "bsp.h"
#include "initial.h"
#include "patterns.h"

/**
 * @brief  SSD2828 初始化
 * @param  lane : 通道数
 * @param data_rate : 速率
 * @retval None
 */
void SSD2828_Init(uint8_t lane, uint16_t data_rate)
{
  SSD2828_Reset();
  MIPI_SetMode(LP);
  if (SSD2828_ReadReg(0xB0) == 0x2828)
  {
    printf("Info:SSD2828 OK %d\n", SSD2828_ReadReg(0xB7));
  }
  else
  {
    printf("Error:SSD2828 configuration failed\n");
  }

  /*
   *  SSD2828_WriteReg(0xb7,0x00,0X50);
   *  SSD2828_WriteReg(0xb8,0x00,0X00);
   *  SSD2828_WriteReg(0xb9,0x00,0X00);
   *  SSD2828_WriteReg(0xb1, LCDTiming.VSPW, LCDTiming.HSPW);
   *  SSD2828_WriteReg(0xb2, LCDTiming.VBPD, LCDTiming.HBPD);
   *  SSD2828_WriteReg(0xb3, LCDTiming.VFPD, LCDTiming.HFPD);
   *  SSD2828_WriteReg(0xb4, (LCDTiming.LCDH >> 8) & 0xff, LCDTiming.LCDH & 0xff);
   *  SSD2828_WriteReg(0xb5, (LCDTiming.LCDV >> 8) & 0xff, LCDTiming.LCDV & 0xff);
   *  SSD2828_WriteReg(0xb6,0x00,0xCB);
   *  SSD2828_WriteReg(0xC9, 0x10, 0x03);
   *  SSD2828_WriteReg(0xCA, 0x21, 0x02);
   *  SSD2828_WriteReg(0xCB, 0x04, 0x48);
   *  SSD2828_WriteReg(0xCC, 0x08, 0x09);
   *  SSD2828_WriteReg(0xCD, 0x10, 0x00);
   *  SSD2828_WriteReg(0xCE, 0x04, 0x05);
   *  SSD2828_WriteReg(0xCF, 0x00, 0x00);
   *  SSD2828_WriteReg(0xD0, 0x00, 0x10);
   *
   *  SSD2828_WriteReg(0xD8, 0x03, 0xC3);
   *  SSD2828_WriteReg(0xD9, 0x62, 0x03);
   *  SSD2828_WriteReg(0xDA, 0xBF, 0x24);
   *  SSD2828_WriteReg(0xDB, 0x10, 0x98);
   *
   *  if (data_rate < 500)
   *  {
   *  SSD2828_WriteReg(0xba, 0x82, data_rate / 12);
   *  }
   *  else
   *  {
   *  SSD2828_WriteReg(0xba, 0xc1, data_rate / 24);
   *  }
   *  SSD2828_WriteReg(0xBB, 0x00, 0x0A);
   *  SSD2828_WriteReg(0xBC, 0x00, 0x00);
   *  SSD2828_WriteReg(0xBD, 0x00, 0x00);
   *  SSD2828_WriteReg(0xBE, 0x1D, 0xFF);
   *  SSD2828_WriteReg(0xDE, 0x00, lane - 1);
   *  SSD2828_WriteReg(0xD6, 0x00, 0x05);
   *  SSD2828_WriteReg(0xB8, 0x00, 0x00);
   *  SSD2828_WriteReg(0xC4, 0x00, 0x01);
   *  SSD2828_WriteReg(0xB9, 0x00, 0x01);
   *  SSD2828_WriteReg(0xb7, 0x02, 0x50);
   */

  SSD2828_WriteReg(0x00b9, 0x00, 0x00);
  SSD2828_WriteReg(0x00b1, LCDTiming.VSPW, LCDTiming.HSPW);
  SSD2828_WriteReg(0x00b2, LCDTiming.VBPD, LCDTiming.HBPD + 10);
  SSD2828_WriteReg(0x00b3, LCDTiming.VFPD, LCDTiming.HFPD);
  SSD2828_WriteReg(0xb4, (LCDTiming.LCDH >> 8) & 0xff, LCDTiming.LCDH & 0xff);
  SSD2828_WriteReg(0xb5, (LCDTiming.LCDV >> 8) & 0xff, LCDTiming.LCDV & 0xff);
  SSD2828_WriteReg(0x00b6, 0x20, 0x07);
  if (data_rate < 500)
  {
    SSD2828_WriteReg(0x00ba, 0x82, data_rate / 12);
  }
  else
  {
    SSD2828_WriteReg(0x00ba, 0xc1, data_rate / 24);
  }
  SSD2828_WriteReg(0x00bb, 0x00, 0x06);
  SSD2828_WriteReg(0x00b8, 0x00, 0x00);
  SSD2828_WriteReg(0x00c9, 0x23, 0x02);
  SSD2828_WriteReg(0x00ca, 0x23, 0x01);
  SSD2828_WriteReg(0x00cb, 0x05, 0x10);
  SSD2828_WriteReg(0x00cc, 0x10, 0x05);
  SSD2828_WriteReg(0x00de, 0x00, lane - 1);
  SSD2828_WriteReg(0x00d6, 0x00, 0x05);

  HAL_Delay(10);
  SSD2828_WriteReg(0x00b9, 0x00, 0x01);
  HAL_Delay(20);
}


void LCD_PowerOnSequence(void)
{
  Power_SetState(POWER_2V8, ON);
  HAL_Delay(50);

  Power_SetState(POWER_1V8, ON);
  HAL_Delay(50);

  Power_SetState(POWER_VSP, ON);
  HAL_Delay(50);

  Power_SetState(POWER_VSN, ON);
  HAL_Delay(50);

  Power_SetState(POWER_OUT5V, ON);  //此电源也供给背光
  HAL_Delay(50);

  LCD_Reset(1);
  HAL_Delay(50);
  LCD_Reset(0);
  HAL_Delay(120);
  LCD_Reset(1);
  HAL_Delay(50);
}


void LCD_PowerOffSequence(void)
{
}


static void LoadPatterns(void)
{
  LCDDrv_SetPattern();
  Frame();
}


void LCD_Init(void)
{
  LCDTiming.DCLK = 64;
  LCDTiming.LCDH = 720;
  LCDTiming.LCDV = 1280;

  LCDTiming.HBPD = 30;
  LCDTiming.HFPD = 30;
  LCDTiming.HSPW = 10;

  LCDTiming.VBPD = 15;
  LCDTiming.VFPD = 16;
  LCDTiming.VSPW = 8;

  LCDDrv_SetTiming();
  SSD2828_Init(4, 480);
  Power_SetBLCurrent(0);
  LCD_PowerOnSequence();
  LCD_SendInitialCode();
  Power_SetBLCurrent(40);
  LCDDrv_OpenRGB();
  MIPI_SetMode(VD);
  LoadPatterns();
}


/********************* (C) COPYRIGHT WEYNE CHEN *******END OF FILE ********/
