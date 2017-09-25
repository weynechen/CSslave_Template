#include "patterns.h"
#include "bsp.h"
#include "stdbool.h"


PatternTypeDef Patterns =
{
  .extra   = 0,
  .counter = 0,
  .curr    = 0,
};


static bool IsLoadPattern = false;


void StartLoadPattern(void)
{
  IsLoadPattern = true;
}


void LoadPatternEnd(void)
{
  IsLoadPattern = false;
}


static void Start(void)
{
  TG_StartLoad();
}


static void End(void)
{
  (IsLoadPattern) ? (Patterns.data[Patterns.counter++] = Patterns.counter) : (Patterns.extra += (Patterns.extra == 0) ? Patterns.counter : 1);
  HAL_Delay(500);
}


void Flicker(void)
{
  uint16_t i, j;
  uint16_t half_lcd_h = LCDTiming.LCDH / 2;

  Start();
  for (j = 0; j < LCDTiming.LCDV; j++)
  {
    for (i = 0; i < half_lcd_h; i++)
    {
      TG_WriteData(0x7f);
      TG_WriteData(0x00);
      TG_WriteData(0x7f);
      TG_WriteData(0x00);
      TG_WriteData(0x7f);
      TG_WriteData(0x00);
    }
  }
  End();
}


void FillFullMemory(uint8_t r, uint8_t g, uint8_t b)
{
  uint32_t i, j;

  Start();
  for (j = 0; j < LCDTiming.LCDV; j++)
  {
    for (i = 0; i < LCDTiming.LCDH; i++)
    {
      TG_WriteData(r);
      TG_WriteData(g);
      TG_WriteData(b);
    }
  }
  End();
}


void FillFullDirect(uint8_t r, uint8_t g, uint8_t b)
{
  uint32_t color = (r << 16) | (g << 8) | b;

  TG_DirectIO(r, g, b);
  (IsLoadPattern) ? (Patterns.data[Patterns.counter++] = color) : (Patterns.extra += (Patterns.extra == 0) ? Patterns.counter : 1);
}


void Crosstalk(void)
{
  uint32_t h;
  uint32_t v1_2;
  uint32_t v1_4, h1_4;
  uint32_t mod;

  Start();
  mod = LCDTiming.LCDV % 4;
  for (v1_4 = 0; v1_4 < LCDTiming.LCDV / 4; v1_4++)
  {
    for (h = 0; h < LCDTiming.LCDH; h++)
    {
      TG_WriteData(0x80);
      TG_WriteData(0x80);
      TG_WriteData(0x80);
    }
  }

  for (v1_2 = 0; v1_2 < LCDTiming.LCDV / 4; v1_2++)
  {
    for (h1_4 = 0; h1_4 < LCDTiming.LCDH / 4; h1_4++)
    {
      TG_WriteData(0x80);
      TG_WriteData(0x80);
      TG_WriteData(0x80);
    }

    for (h1_4 = 0; h1_4 < LCDTiming.LCDH / 4; h1_4++)
    {
      TG_WriteData(0x00);
      TG_WriteData(0x00);
      TG_WriteData(0x00);
      TG_WriteData(0x00);
      TG_WriteData(0x00);
      TG_WriteData(0x00);
    }
    for (h1_4 = 0; h1_4 < LCDTiming.LCDH / 4; h1_4++)
    {
      TG_WriteData(0x80);
      TG_WriteData(0x80);
      TG_WriteData(0x80);
    }
  }

  for (v1_2 = 0; v1_2 < LCDTiming.LCDV / 4; v1_2++)
  {
    for (h1_4 = 0; h1_4 < LCDTiming.LCDH / 4; h1_4++)
    {
      TG_WriteData(0x80);
      TG_WriteData(0x80);
      TG_WriteData(0x80);
    }

    for (h1_4 = 0; h1_4 < LCDTiming.LCDH / 4; h1_4++)
    {
      TG_WriteData(0x00);
      TG_WriteData(0x00);
      TG_WriteData(0x00);
      TG_WriteData(0x00);
      TG_WriteData(0x00);
      TG_WriteData(0x00);
    }
    for (h1_4 = 0; h1_4 < LCDTiming.LCDH / 4; h1_4++)
    {
      TG_WriteData(0x80);
      TG_WriteData(0x80);
      TG_WriteData(0x80);
    }
  }

  for (v1_4 = 0; v1_4 < LCDTiming.LCDV / 4 + mod; v1_4++)
  {
    for (h = 0; h < LCDTiming.LCDH; h++)
    {
      TG_WriteData(0x80);
      TG_WriteData(0x80);
      TG_WriteData(0x80);
    }
  }
  End();
}


void Chessboard(void)
{
  uint16_t i;
  uint16_t j;
  uint16_t y;
  uint16_t mod = LCDTiming.LCDV % 8;

  Start();
  for (j = 0; j < 4; j++)
  {
    for (y = 0; y < LCDTiming.LCDV / 8; y++)
    {
      for (i = 0; i < LCDTiming.LCDH / 5; i++)
      {
        TG_WriteData(0x00);
        TG_WriteData(0x00);
        TG_WriteData(0x00);
      }
      for ( ; i < LCDTiming.LCDH * 2 / 5; i++)
      {
        TG_WriteData(0xff);
        TG_WriteData(0xff);
        TG_WriteData(0xff);
      }
      for ( ; i < LCDTiming.LCDH * 3 / 5; i++)
      {
        TG_WriteData(0x00);
        TG_WriteData(0x00);
        TG_WriteData(0x00);
      }
      for ( ; i < LCDTiming.LCDH * 4 / 5; i++)
      {
        TG_WriteData(0xff);
        TG_WriteData(0xff);
        TG_WriteData(0xff);
      }
      for ( ; i < LCDTiming.LCDH; i++)
      {
        TG_WriteData(0x00);
        TG_WriteData(0x00);
        TG_WriteData(0x00);
      }
    }

    for (y = 0; y < LCDTiming.LCDV / 8; y++)
    {
      for (i = 0; i < LCDTiming.LCDH / 5; i++)
      {
        TG_WriteData(0xff);
        TG_WriteData(0xff);
        TG_WriteData(0xff);
      }
      for ( ; i < LCDTiming.LCDH * 2 / 5; i++)
      {
        TG_WriteData(0x00);
        TG_WriteData(0x00);
        TG_WriteData(0x00);
      }
      for ( ; i < LCDTiming.LCDH * 3 / 5; i++)
      {
        TG_WriteData(0xff);
        TG_WriteData(0xff);
        TG_WriteData(0xff);
      }
      for ( ; i < LCDTiming.LCDH * 4 / 5; i++)
      {
        TG_WriteData(0x00);
        TG_WriteData(0x00);
        TG_WriteData(0x00);
      }
      for ( ; i < LCDTiming.LCDH; i++)
      {
        TG_WriteData(0xff);
        TG_WriteData(0xff);
        TG_WriteData(0xff);
      }
    }
  }

  for (y = 0; y < mod; y++)
  {
    for (i = 0; i < LCDTiming.LCDH / 5; i++)
    {
      TG_WriteData(0xff);
      TG_WriteData(0xff);
      TG_WriteData(0xff);
    }
    for ( ; i < LCDTiming.LCDH * 2 / 5; i++)
    {
      TG_WriteData(0x00);
      TG_WriteData(0x00);
      TG_WriteData(0x00);
    }
    for ( ; i < LCDTiming.LCDH * 3 / 5; i++)
    {
      TG_WriteData(0xff);
      TG_WriteData(0xff);
      TG_WriteData(0xff);
    }
    for ( ; i < LCDTiming.LCDH * 4 / 5; i++)
    {
      TG_WriteData(0x00);
      TG_WriteData(0x00);
      TG_WriteData(0x00);
    }
    for ( ; i < LCDTiming.LCDH; i++)
    {
      TG_WriteData(0xff);
      TG_WriteData(0xff);
      TG_WriteData(0xff);
    }
  }
  End();
}


void Gradient(GradientColorTypeDef type, DirTypeDef dir)
{
  uint32_t x, y;
  uint8_t gra_color;
  uint32_t color;
  uint32_t lcd_h  = LCDTiming.LCDH;
  uint32_t lcd_v  = LCDTiming.LCDV;
  uint32_t factor = (dir == DIR_H) ? lcd_h : lcd_v;

  Start();
  for (y = 0; y < lcd_v; y++)
  {
    if (dir == DIR_V)
    {
      gra_color = (x * 255) / factor;
    }
    for (x = 0; x < lcd_h; x++)
    {
      if (dir == DIR_H)
      {
        gra_color = (x * 255) / factor;
      }
      color = (gra_color << 16) | (gra_color << 8) | gra_color;
      TG_WriteData(color & type);
      TG_WriteData((color & type) >> 8);
      TG_WriteData((color & type) >> 16);
    }
  }
  End();
}


void Frame(void)
{
  uint32_t x, y;

  Start();
  for (y = 0; y < LCDTiming.LCDV; y++)
  {
    for (x = 0; x < LCDTiming.LCDH; x++)
    {
      if ((x == 0) ||                  \
          (x == LCDTiming.LCDH - 1) || \
          (y == 0) ||                  \
          (y == LCDTiming.LCDV - 1)    \
          )
      {
        TG_WriteData(0xff);
        TG_WriteData(0xff);
        TG_WriteData(0xff);
      }
      else
      {
        TG_WriteData(0x00);
        TG_WriteData(0x00);
        TG_WriteData(0x00);
      }
    }
  }
  End();
}


void ColorBarH(void)
{
  uint32_t i, j, x;
  uint8_t mod;

  Start();

  x   = LCDTiming.LCDH / 8;
  mod = LCDTiming.LCDH % 8;

  for (j = 0; j < LCDTiming.LCDV; j++)
  {
    for (i = 0; i < x; i++)
    {
      TG_WriteData(0xff);
      TG_WriteData(0xff);
      TG_WriteData(0xff);
    }
    for (i = 0; i < x; i++)
    {
      TG_WriteData(0xff);
      TG_WriteData(0xff);
      TG_WriteData(0x00);
    }
    for (i = 0; i < x; i++)
    {
      TG_WriteData(0x00);
      TG_WriteData(0xff);
      TG_WriteData(0xff);
    }
    for (i = 0; i < x; i++)
    {
      TG_WriteData(0x00);
      TG_WriteData(0xff);
      TG_WriteData(0x00);
    }
    for (i = 0; i < x; i++)
    {
      TG_WriteData(0xff);
      TG_WriteData(0x00);
      TG_WriteData(0xff);
    }
    for (i = 0; i < x; i++)
    {
      TG_WriteData(0xff);
      TG_WriteData(0x00);
      TG_WriteData(0x00);
    }
    for (i = 0; i < x; i++)
    {
      TG_WriteData(0x00);
      TG_WriteData(0x00);
      TG_WriteData(0xff);
    }
    for (i = 0; i < x; i++)
    {
      TG_WriteData(0x00);
      TG_WriteData(0x00);
      TG_WriteData(0x00);
    }
    for (i = 0; i < mod; i++)
    {
      TG_WriteData(0x00);
      TG_WriteData(0x00);
      TG_WriteData(0x00);
    }
  }
  End();
}


void ColorBarV(void)
{
  uint32_t i, j, y;

  Start();

  y = LCDTiming.LCDV / 8;

  for (i = 0; i < y; i++)
  {
    for (j = 0; j < LCDTiming.LCDH; j++)
    {
      TG_WriteData(0xff);
      TG_WriteData(0xff);
      TG_WriteData(0xff);
    }
  }
  for (i = 0; i < y; i++)
  {
    for (j = 0; j < LCDTiming.LCDH; j++)
    {
      TG_WriteData(0xff);
      TG_WriteData(0xff);
      TG_WriteData(0x00);
    }
  }
  for (i = 0; i < y; i++)
  {
    for (j = 0; j < LCDTiming.LCDH; j++)
    {
      TG_WriteData(0x00);
      TG_WriteData(0xff);
      TG_WriteData(0xff);
    }
  }
  for (i = 0; i < y; i++)
  {
    for (j = 0; j < LCDTiming.LCDH; j++)
    {
      TG_WriteData(0x00);
      TG_WriteData(0xff);
      TG_WriteData(0x00);
    }
  }
  for (i = 0; i < y; i++)
  {
    for (j = 0; j < LCDTiming.LCDH; j++)
    {
      TG_WriteData(0xff);
      TG_WriteData(0x00);
      TG_WriteData(0xff);
    }
  }
  for (i = 0; i < y; i++)
  {
    for (j = 0; j < LCDTiming.LCDH; j++)
    {
      TG_WriteData(0xff);
      TG_WriteData(0x00);
      TG_WriteData(0x00);
    }
  }
  for (i = 0; i < y; i++)
  {
    for (j = 0; j < LCDTiming.LCDH; j++)
    {
      TG_WriteData(0x00);
      TG_WriteData(0x00);
      TG_WriteData(0xff);
    }
  }
  for (i = 0; i < y; i++)
  {
    for (j = 0; j < LCDTiming.LCDH; j++)
    {
      TG_WriteData(0x00);
      TG_WriteData(0x00);
      TG_WriteData(0x00);
    }
  }
  y = LCDTiming.LCDV % 8;
  for (i = 0; i < y; i++)
  {
    for (j = 0; j < LCDTiming.LCDH; j++)
    {
      TG_WriteData(0x00);
      TG_WriteData(0x00);
      TG_WriteData(0x00);
    }
  }
  End();
}


void SnowDot(void)
{
  uint32_t i, j;

  Start();

  for (j = 0; j < LCDTiming.LCDV / 2; j++)
  {
    for (i = 0; i < LCDTiming.LCDH / 2; i++)
    {
      TG_WriteData(0xff);
      TG_WriteData(0x00);
      TG_WriteData(0xff);
      TG_WriteData(0x00);
      TG_WriteData(0xff);
      TG_WriteData(0x00);
    }
    for (i = 0; i < LCDTiming.LCDH / 2; i++)
    {
      TG_WriteData(0x00);
      TG_WriteData(0xff);
      TG_WriteData(0x00);
      TG_WriteData(0xff);
      TG_WriteData(0x00);
      TG_WriteData(0xff);
    }
  }
  End();
}


void RGBBar(void)
{
  uint32_t i, j, y, mod;

  Start();

  y   = LCDTiming.LCDV / 3;
  mod = LCDTiming.LCDV % 3;

  for (i = 0; i < y; i++)
  {
    for (j = 0; j < LCDTiming.LCDH; j++)
    {
      TG_WriteData(0xff);
      TG_WriteData(0x00);
      TG_WriteData(0x00);
    }
  }
  for (i = 0; i < y; i++)
  {
    for (j = 0; j < LCDTiming.LCDH; j++)
    {
      TG_WriteData(0x00);
      TG_WriteData(0xff);
      TG_WriteData(0x00);
    }
  }
  for (i = 0; i < y; i++)
  {
    for (j = 0; j < LCDTiming.LCDH; j++)
    {
      TG_WriteData(0x00);
      TG_WriteData(0x00);
      TG_WriteData(0xff);
    }
  }

  for (i = 0; i < mod; i++)
  {
    for (j = 0; j < LCDTiming.LCDH; j++)
    {
      TG_WriteData(0x00);
      TG_WriteData(0x00);
      TG_WriteData(0xff);
    }
  }
  End();
}


void RGBLevel(void)
{
  uint32_t i, j, y, mod, color = 0;

  Start();

  y   = LCDTiming.LCDV / 3;
  mod = LCDTiming.LCDV % 3;

  for (i = 0; i < y; i++)
  {
    for (j = 0; j < LCDTiming.LCDH; j++)
    {
      color = j * 255 / LCDTiming.LCDH;
      TG_WriteData(0xff - color);
      TG_WriteData(0x00);
      TG_WriteData(0x00);
    }
  }
  for (i = 0; i < y; i++)
  {
    for (j = 0; j < LCDTiming.LCDH; j++)
    {
      color = j * 255 / LCDTiming.LCDH;
      TG_WriteData(0x00);
      TG_WriteData(0xff - color);
      TG_WriteData(0x00);
    }
  }
  for (i = 0; i < y; i++)
  {
    for (j = 0; j < LCDTiming.LCDH; j++)
    {
      color = j * 255 / LCDTiming.LCDH;
      TG_WriteData(0x00);
      TG_WriteData(0x00);
      TG_WriteData(0xff - color);
    }
  }

  for (i = 0; i < mod; i++)
  {
    for (j = 0; j < LCDTiming.LCDH; j++)
    {
      color = j * 255 / LCDTiming.LCDH;
      TG_WriteData(0x00);
      TG_WriteData(0x00);
      TG_WriteData(0xff - color);
    }
  }
  End();
}
