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
#include "patterns.h"

static bool AutoRun         = true;
static bool KeyCtrl         = false;
static uint32_t LastPattern = 0xff;
static uint32_t TickStart   = 0;

void APP_Run(void)
{
  uint16_t curr;
  bool power_on = true;

  BSP_SetIndicatorLight(LIGHT_RED, ON);

  BSP_SetLCDType(MIPI);

  LCD_Init();

  while (1)
  {
    KeyTypeDef key = Key_Scan();

    switch (key)
    {
    case KEY_UP:
      Patterns.curr++;
      KeyCtrl = true;
      break;

    case KEY_DOWN:
      Patterns.curr--;
      KeyCtrl = true;
      break;

    case KEY_POWER:
      if (power_on)
      {
        LCD_PowerOffSequence();
        power_on = false;
      }
      else
      {
        LCD_ReLight();
        Patterns.curr = 0;
        TickStart = 0;
        KeyCtrl   = false;
        power_on = true;
      }
      break;

    case KEY_MTP:
      break;

    case KEY_TP:
      break;

    default:
      break;
    }

    /*下面部分为切换画面和自动跑逻辑，一般不需修改*/
    curr = Patterns.curr % Patterns.counter;
    if ((AutoRun) && (KeyCtrl == false))
    {
      if (HAL_GetTick() - TickStart > Patterns.delay[curr])
      {
        TickStart = HAL_GetTick();
        Patterns.curr++;
      }
    }
    if (LastPattern != Patterns.data[curr])
    {
      TG_ShowPattern(Patterns.data[curr]);
      LastPattern = Patterns.data[curr];
    }
  }
}


/********************* (C) COPYRIGHT WEYNE CHEN *******END OF FILE ********/
