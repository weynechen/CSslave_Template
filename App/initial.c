/**
 * @file         initial.c
 * @author       Weyne Chen
 * @version      V01
 * @date         2017.09.24
 * @brief
 * @note
 * @attention    COPYRIGHT WEYNE
 **/

#include "initial.h"
#include "bsp.h"

void LCD_SendInitialCode(void)
{
  MIPI_GenericLongWrite(2);
  MIPI_WriteData(0xB0);
  MIPI_WriteData(0x00);

  MIPI_GenericLongWrite(2);
  MIPI_WriteData(0xE3);
  MIPI_WriteData(0x01);

  MIPI_GenericLongWrite(3);
  MIPI_WriteData(0xB6);
  MIPI_WriteData(0x61);
  MIPI_WriteData(0x2C);

  MIPI_GenericLongWrite(7);
  MIPI_WriteData(0xC0);
  MIPI_WriteData(0x23);
  MIPI_WriteData(0xB2);
  MIPI_WriteData(0x10);
  MIPI_WriteData(0x10);
  MIPI_WriteData(0x02);
  MIPI_WriteData(0x7F);

  MIPI_WriteData(22);
  MIPI_WriteData(0x0B);
  MIPI_WriteData(0x8A);
  MIPI_WriteData(0xA0);
  MIPI_WriteData(0x8A);
  MIPI_WriteData(0x00);
  MIPI_WriteData(0x00);
  MIPI_WriteData(0x00);
  MIPI_WriteData(0x00);
  MIPI_WriteData(0x00);
  MIPI_WriteData(0x03);
  MIPI_WriteData(0x00);
  MIPI_WriteData(0x00);
  MIPI_WriteData(0x00);
  MIPI_WriteData(0x00);
  MIPI_WriteData(0x00);
  MIPI_WriteData(0x00);
  MIPI_WriteData(0x00);
  MIPI_WriteData(0x00);
  MIPI_WriteData(0x00);
  MIPI_WriteData(0x00);
  MIPI_WriteData(0x00);
  MIPI_WriteData(0x00);

  MIPI_GenericLongWrite(2);
  MIPI_WriteData(0xC3);
  MIPI_WriteData(0x75);

  MIPI_GenericLongWrite(28);
  MIPI_WriteData(0xC5);
  MIPI_WriteData(0x07);
  MIPI_WriteData(0x04);
  MIPI_WriteData(0x41);
  MIPI_WriteData(0x04);
  MIPI_WriteData(0x00);
  MIPI_WriteData(0x00);
  MIPI_WriteData(0x03);
  MIPI_WriteData(0x01);
  MIPI_WriteData(0x80);
  MIPI_WriteData(0x07);
  MIPI_WriteData(0x00);
  MIPI_WriteData(0x00);
  MIPI_WriteData(0x00);
  MIPI_WriteData(0x00);
  MIPI_WriteData(0x00);
  MIPI_WriteData(0x00);
  MIPI_WriteData(0x00);
  MIPI_WriteData(0x00);
  MIPI_WriteData(0x00);
  MIPI_WriteData(0x00);
  MIPI_WriteData(0x00);
  MIPI_WriteData(0x00);
  MIPI_WriteData(0x00);
  MIPI_WriteData(0x00);
  MIPI_WriteData(0x00);
  MIPI_WriteData(0x00);
  MIPI_WriteData(0x00);
  MIPI_WriteData(0x0E);

  MIPI_GenericLongWrite(3);
  MIPI_WriteData(0xC6);
  MIPI_WriteData(0x01);
  MIPI_WriteData(0x02);

  MIPI_GenericLongWrite(58);
  MIPI_WriteData(0xC8);
  MIPI_WriteData(0x01);
  MIPI_WriteData(0x20);
  MIPI_WriteData(0x22);
  MIPI_WriteData(0x24);
  MIPI_WriteData(0x26);
  MIPI_WriteData(0x00);
  MIPI_WriteData(0x00);
  MIPI_WriteData(0x1D);
  MIPI_WriteData(0x1D);
  MIPI_WriteData(0x00);
  MIPI_WriteData(0x00);
  MIPI_WriteData(0x00);
  MIPI_WriteData(0x1D);
  MIPI_WriteData(0x1D);
  MIPI_WriteData(0x1D);
  MIPI_WriteData(0x00);
  MIPI_WriteData(0x00);
  MIPI_WriteData(0x00);
  MIPI_WriteData(0x00);
  MIPI_WriteData(0x11);
  MIPI_WriteData(0x13);
  MIPI_WriteData(0x09);
  MIPI_WriteData(0x02);
  MIPI_WriteData(0x21);
  MIPI_WriteData(0x23);
  MIPI_WriteData(0x25);
  MIPI_WriteData(0x27);
  MIPI_WriteData(0x00);
  MIPI_WriteData(0x00);
  MIPI_WriteData(0x1D);
  MIPI_WriteData(0x1D);
  MIPI_WriteData(0x00);
  MIPI_WriteData(0x00);
  MIPI_WriteData(0x00);
  MIPI_WriteData(0x1D);
  MIPI_WriteData(0x1D);
  MIPI_WriteData(0x1D);
  MIPI_WriteData(0x00);
  MIPI_WriteData(0x00);
  MIPI_WriteData(0x00);
  MIPI_WriteData(0x00);
  MIPI_WriteData(0x11);
  MIPI_WriteData(0x13);
  MIPI_WriteData(0x0A);
  MIPI_WriteData(0x89);
  MIPI_WriteData(0x00);
  MIPI_WriteData(0x00);
  MIPI_WriteData(0x00);
  MIPI_WriteData(0x01);
  MIPI_WriteData(0x00);
  MIPI_WriteData(0x00);
  MIPI_WriteData(0x00);
  MIPI_WriteData(0x02);
  MIPI_WriteData(0x00);
  MIPI_WriteData(0x00);
  MIPI_WriteData(0x00);
  MIPI_WriteData(0x03);
  MIPI_WriteData(0x00);

  MIPI_GenericLongWrite(38);
  MIPI_WriteData(0xCA);
  MIPI_WriteData(0x1B);
  MIPI_WriteData(0x28);
  MIPI_WriteData(0x32);
  MIPI_WriteData(0x40);
  MIPI_WriteData(0x4D);
  MIPI_WriteData(0x58);
  MIPI_WriteData(0x6F);
  MIPI_WriteData(0x81);
  MIPI_WriteData(0x8F);
  MIPI_WriteData(0x9C);
  MIPI_WriteData(0x50);
  MIPI_WriteData(0x5C);
  MIPI_WriteData(0x6A);
  MIPI_WriteData(0x7D);
  MIPI_WriteData(0x87);
  MIPI_WriteData(0x93);
  MIPI_WriteData(0xA4);
  MIPI_WriteData(0xB1);
  MIPI_WriteData(0xBF);
  MIPI_WriteData(0x1B);
  MIPI_WriteData(0x28);
  MIPI_WriteData(0x32);
  MIPI_WriteData(0x40);
  MIPI_WriteData(0x4D);
  MIPI_WriteData(0x58);
  MIPI_WriteData(0x6F);
  MIPI_WriteData(0x81);
  MIPI_WriteData(0x8F);
  MIPI_WriteData(0x9C);
  MIPI_WriteData(0x50);
  MIPI_WriteData(0x5C);
  MIPI_WriteData(0x6A);
  MIPI_WriteData(0x7D);
  MIPI_WriteData(0x87);
  MIPI_WriteData(0x93);
  MIPI_WriteData(0xA4);
  MIPI_WriteData(0xB1);
  MIPI_WriteData(0xBF);

  MIPI_GenericLongWrite(7);
  MIPI_WriteData(0xD0);
  MIPI_WriteData(0x02);
  MIPI_WriteData(0x4B);
  MIPI_WriteData(0x1F);
  MIPI_WriteData(0x00);
  MIPI_WriteData(0x26);
  MIPI_WriteData(0x8F);

  MIPI_GenericLongWrite(2);
  MIPI_WriteData(0xD1);
  MIPI_WriteData(0x03);

  MIPI_GenericLongWrite(3);
  MIPI_WriteData(0xD2);
  MIPI_WriteData(0x8E);
  MIPI_WriteData(0x0B);

  MIPI_GenericLongWrite(2);
  MIPI_WriteData(0xE5);
  MIPI_WriteData(0x02);

  MIPI_GenericLongWrite(3);
  MIPI_WriteData(0xD4);
  MIPI_WriteData(0x00);
  MIPI_WriteData(0x9B);

  MIPI_GenericLongWrite(3);
  MIPI_WriteData(0xD5);
  MIPI_WriteData(0x20);
  MIPI_WriteData(0x20);

  MIPI_DcsShortWrite(1);
  MIPI_WriteData(0x11);

  HAL_Delay(120);

  MIPI_DcsShortWrite(1);
  MIPI_WriteData(0x29);

  HAL_Delay(120);
}


/********************* (C) COPYRIGHT WEYNE CHEN *******END OF FILE ********/
