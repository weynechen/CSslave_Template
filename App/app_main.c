/**
 * @file         app_main.c
 * @author       Weyne Chen
 * @version      V01
 * @date         2017.09.24
 * @brief
 * @note
 * @attention    COPYRIGHT WEYNE
 **/

#include "lcd.h"
#include "bsp.h"

void APP_Run(void)
{
  BSP_SetIndicatorLight(LIGHT_RED, ON);
  BSP_SetLCDType(MIPI);

  LCD_Init();

  while (1)
  {
    KeyTypeDef key = Key_Scan();

    switch (key)
    {
    case KEY_UP:
      break;

    case KEY_DOWN:
      break;

    case KEY_POWER:
      break;

    case KEY_MTP:
      break;

    case KEY_TP:
      break;

    default:
      break;
    }
  }
}


/********************* (C) COPYRIGHT WEYNE CHEN *******END OF FILE ********/
