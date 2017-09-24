#include "patterns.h"
#include "bsp.h"


void Flicker(void)
{
  uint16_t i, j;
  uint16_t half_lcd_h = LCDTiming.LCDH / 2;

  for (j = 0; j < LCDTiming.LCDV; j++)
  {
    for (i = 0; i < half_lcd_h; i++)
    {
      LCDDrv_WriteData(0x7f);
      LCDDrv_WriteData(0x00);
      LCDDrv_WriteData(0x7f);
      LCDDrv_WriteData(0x00);
      LCDDrv_WriteData(0x7f);
      LCDDrv_WriteData(0x00);
    }
  }
}

void FillFull(uint8_t r, uint8_t g, uint8_t b)
{
  uint32_t i, j;

  for (j = 0; j < LCDTiming.LCDV; j++)
  {
    for (i = 0; i < LCDTiming.LCDH; i++)
    {
      LCDDrv_WriteData(r);
      LCDDrv_WriteData(g);
      LCDDrv_WriteData(b);
    }
  }
}

void Crosstalk(void)
{
  uint32_t h;
  uint32_t v1_2;
  uint32_t v1_4, h1_4;
  uint32_t mod;

  mod = LCDTiming.LCDV % 4;
  for (v1_4 = 0; v1_4 < LCDTiming.LCDV / 4; v1_4++)
  {
    for (h = 0; h < LCDTiming.LCDH; h++)
    {
      LCDDrv_WriteData(0x80);
      LCDDrv_WriteData(0x80);
      LCDDrv_WriteData(0x80);
    }
  }

  for (v1_2 = 0; v1_2 < LCDTiming.LCDV / 4; v1_2++)
  {
    for (h1_4 = 0; h1_4 < LCDTiming.LCDH / 4; h1_4++)
    {
      LCDDrv_WriteData(0x80);
      LCDDrv_WriteData(0x80);
      LCDDrv_WriteData(0x80);
    }

    for (h1_4 = 0; h1_4 < LCDTiming.LCDH / 4; h1_4++)
    {
      LCDDrv_WriteData(0x00);
      LCDDrv_WriteData(0x00);
      LCDDrv_WriteData(0x00);
      LCDDrv_WriteData(0x00);
      LCDDrv_WriteData(0x00);
      LCDDrv_WriteData(0x00);
    }
    for (h1_4 = 0; h1_4 < LCDTiming.LCDH / 4; h1_4++)
    {
      LCDDrv_WriteData(0x80);
      LCDDrv_WriteData(0x80);
      LCDDrv_WriteData(0x80);
    }
  }

  for (v1_2 = 0; v1_2 < LCDTiming.LCDV / 4; v1_2++)
  {
    for (h1_4 = 0; h1_4 < LCDTiming.LCDH / 4; h1_4++)
    {
      LCDDrv_WriteData(0x80);
      LCDDrv_WriteData(0x80);
      LCDDrv_WriteData(0x80);
    }

    for (h1_4 = 0; h1_4 < LCDTiming.LCDH / 4; h1_4++)
    {
      LCDDrv_WriteData(0x00);
      LCDDrv_WriteData(0x00);
      LCDDrv_WriteData(0x00);
      LCDDrv_WriteData(0x00);
      LCDDrv_WriteData(0x00);
      LCDDrv_WriteData(0x00);
    }
    for (h1_4 = 0; h1_4 < LCDTiming.LCDH / 4; h1_4++)
    {
      LCDDrv_WriteData(0x80);
      LCDDrv_WriteData(0x80);
      LCDDrv_WriteData(0x80);
    }
  }

  for (v1_4 = 0; v1_4 < LCDTiming.LCDV / 4 + mod; v1_4++)
  {
    for (h = 0; h < LCDTiming.LCDH; h++)
    {
      LCDDrv_WriteData(0x80);
      LCDDrv_WriteData(0x80);
      LCDDrv_WriteData(0x80);
    }
  }
}


void Chessboard(void)
{
  uint16_t i;
  uint16_t j;
  uint16_t y;
  uint16_t mod = LCDTiming.LCDV % 8;

  for (j = 0; j < 4; j++)
  {
    for (y = 0; y < LCDTiming.LCDV / 8; y++)
    {
      for (i = 0; i < LCDTiming.LCDH / 5; i++)
      {
        LCDDrv_WriteData(0x00);
        LCDDrv_WriteData(0x00);
        LCDDrv_WriteData(0x00);
      }
      for ( ; i < LCDTiming.LCDH * 2 / 5; i++)
      {
        LCDDrv_WriteData(0xff);
        LCDDrv_WriteData(0xff);
        LCDDrv_WriteData(0xff);
      }
      for ( ; i < LCDTiming.LCDH * 3 / 5; i++)
      {
        LCDDrv_WriteData(0x00);
        LCDDrv_WriteData(0x00);
        LCDDrv_WriteData(0x00);
      }
      for ( ; i < LCDTiming.LCDH * 4 / 5; i++)
      {
        LCDDrv_WriteData(0xff);
        LCDDrv_WriteData(0xff);
        LCDDrv_WriteData(0xff);
      }
      for ( ; i < LCDTiming.LCDH; i++)
      {
        LCDDrv_WriteData(0x00);
        LCDDrv_WriteData(0x00);
        LCDDrv_WriteData(0x00);
      }
    }

    for (y = 0; y < LCDTiming.LCDV / 8; y++)
    {
      for (i = 0; i < LCDTiming.LCDH / 5; i++)
      {
        LCDDrv_WriteData(0xff);
        LCDDrv_WriteData(0xff);
        LCDDrv_WriteData(0xff);
      }
      for ( ; i < LCDTiming.LCDH * 2 / 5; i++)
      {
        LCDDrv_WriteData(0x00);
        LCDDrv_WriteData(0x00);
        LCDDrv_WriteData(0x00);
      }
      for ( ; i < LCDTiming.LCDH * 3 / 5; i++)
      {
        LCDDrv_WriteData(0xff);
        LCDDrv_WriteData(0xff);
        LCDDrv_WriteData(0xff);
      }
      for ( ; i < LCDTiming.LCDH * 4 / 5; i++)
      {
        LCDDrv_WriteData(0x00);
        LCDDrv_WriteData(0x00);
        LCDDrv_WriteData(0x00);
      }
      for ( ; i < LCDTiming.LCDH; i++)
      {
        LCDDrv_WriteData(0xff);
        LCDDrv_WriteData(0xff);
        LCDDrv_WriteData(0xff);
      }
    }
  }

  for (y = 0; y < mod; y++)
  {
    for (i = 0; i < LCDTiming.LCDH / 5; i++)
    {
      LCDDrv_WriteData(0xff);
      LCDDrv_WriteData(0xff);
      LCDDrv_WriteData(0xff);
    }
    for ( ; i < LCDTiming.LCDH * 2 / 5; i++)
    {
      LCDDrv_WriteData(0x00);
      LCDDrv_WriteData(0x00);
      LCDDrv_WriteData(0x00);
    }
    for ( ; i < LCDTiming.LCDH * 3 / 5; i++)
    {
      LCDDrv_WriteData(0xff);
      LCDDrv_WriteData(0xff);
      LCDDrv_WriteData(0xff);
    }
    for ( ; i < LCDTiming.LCDH * 4 / 5; i++)
    {
      LCDDrv_WriteData(0x00);
      LCDDrv_WriteData(0x00);
      LCDDrv_WriteData(0x00);
    }
    for ( ; i < LCDTiming.LCDH; i++)
    {
      LCDDrv_WriteData(0xff);
      LCDDrv_WriteData(0xff);
      LCDDrv_WriteData(0xff);
    }
  }
}


void Gradient(GradientColorTypeDef type, DirTypeDef dir)
{
  uint32_t x, y;
  uint8_t gra_color;
  uint32_t color;
  uint32_t lcd_h  = LCDTiming.LCDH;
  uint32_t lcd_v  = LCDTiming.LCDV;
  uint32_t factor = (dir == DIR_H) ? lcd_h : lcd_v;

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
      LCDDrv_WriteData(color & type);
      LCDDrv_WriteData((color & type) >> 8);
      LCDDrv_WriteData((color & type) >> 16);
    }
  }
}


void Frame(void)
{
  uint32_t x, y;

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
        LCDDrv_WriteData(0xff);
        LCDDrv_WriteData(0xff);
        LCDDrv_WriteData(0xff);
      }
      else
      {
        LCDDrv_WriteData(0x00);
        LCDDrv_WriteData(0x00);
        LCDDrv_WriteData(0x00);
      }
    }
  }
}


void ColorBarH(void)
{
  uint32_t i, j, x;
  uint8_t mod;

  x   = LCDTiming.LCDH / 8;
  mod = LCDTiming.LCDH % 8;

  for (j = 0; j < LCDTiming.LCDV; j++)
  {
    for (i = 0; i < x; i++)
    {
      LCDDrv_WriteData(0xff);
      LCDDrv_WriteData(0xff);
      LCDDrv_WriteData(0xff);
    }
    for (i = 0; i < x; i++)
    {
      LCDDrv_WriteData(0xff);
      LCDDrv_WriteData(0xff);
      LCDDrv_WriteData(0x00);
    }
    for (i = 0; i < x; i++)
    {
      LCDDrv_WriteData(0x00);
      LCDDrv_WriteData(0xff);
      LCDDrv_WriteData(0xff);
    }
    for (i = 0; i < x; i++)
    {
      LCDDrv_WriteData(0x00);
      LCDDrv_WriteData(0xff);
      LCDDrv_WriteData(0x00);
    }
    for (i = 0; i < x; i++)
    {
      LCDDrv_WriteData(0xff);
      LCDDrv_WriteData(0x00);
      LCDDrv_WriteData(0xff);
    }
    for (i = 0; i < x; i++)
    {
      LCDDrv_WriteData(0xff);
      LCDDrv_WriteData(0x00);
      LCDDrv_WriteData(0x00);
    }
    for (i = 0; i < x; i++)
    {
      LCDDrv_WriteData(0x00);
      LCDDrv_WriteData(0x00);
      LCDDrv_WriteData(0xff);
    }
    for (i = 0; i < x; i++)
    {
      LCDDrv_WriteData(0x00);
      LCDDrv_WriteData(0x00);
      LCDDrv_WriteData(0x00);
    }
    for (i = 0; i < mod; i++)
    {
      LCDDrv_WriteData(0x00);
      LCDDrv_WriteData(0x00);
      LCDDrv_WriteData(0x00);
    }
  }
}


void ColorBarV(void)
{
  uint32_t i, j, y;

  y = LCDTiming.LCDV / 8;

  for (i = 0; i < y; i++)
  {
    for (j = 0; j < LCDTiming.LCDH; j++)
    {
      LCDDrv_WriteData(0xff);
      LCDDrv_WriteData(0xff);
      LCDDrv_WriteData(0xff);
    }
  }
  for (i = 0; i < y; i++)
  {
    for (j = 0; j < LCDTiming.LCDH; j++)
    {
      LCDDrv_WriteData(0xff);
      LCDDrv_WriteData(0xff);
      LCDDrv_WriteData(0x00);
    }
  }
  for (i = 0; i < y; i++)
  {
    for (j = 0; j < LCDTiming.LCDH; j++)
    {
      LCDDrv_WriteData(0x00);
      LCDDrv_WriteData(0xff);
      LCDDrv_WriteData(0xff);
    }
  }
  for (i = 0; i < y; i++)
  {
    for (j = 0; j < LCDTiming.LCDH; j++)
    {
      LCDDrv_WriteData(0x00);
      LCDDrv_WriteData(0xff);
      LCDDrv_WriteData(0x00);
    }
  }
  for (i = 0; i < y; i++)
  {
    for (j = 0; j < LCDTiming.LCDH; j++)
    {
      LCDDrv_WriteData(0xff);
      LCDDrv_WriteData(0x00);
      LCDDrv_WriteData(0xff);
    }
  }
  for (i = 0; i < y; i++)
  {
    for (j = 0; j < LCDTiming.LCDH; j++)
    {
      LCDDrv_WriteData(0xff);
      LCDDrv_WriteData(0x00);
      LCDDrv_WriteData(0x00);
    }
  }
  for (i = 0; i < y; i++)
  {
    for (j = 0; j < LCDTiming.LCDH; j++)
    {
      LCDDrv_WriteData(0x00);
      LCDDrv_WriteData(0x00);
      LCDDrv_WriteData(0xff);
    }
  }
  for (i = 0; i < y; i++)
  {
    for (j = 0; j < LCDTiming.LCDH; j++)
    {
      LCDDrv_WriteData(0x00);
      LCDDrv_WriteData(0x00);
      LCDDrv_WriteData(0x00);
    }
  }
  y = LCDTiming.LCDV % 8;
  for (i = 0; i < y; i++)
  {
    for (j = 0; j < LCDTiming.LCDH; j++)
    {
      LCDDrv_WriteData(0x00);
      LCDDrv_WriteData(0x00);
      LCDDrv_WriteData(0x00);
    }
  }
}


void SnowDot(void)
{
  uint32_t i, j;

  for (j = 0; j < LCDTiming.LCDV / 2; j++)
  {
    for (i = 0; i < LCDTiming.LCDH / 2; i++)
    {
      LCDDrv_WriteData(0xff);
      LCDDrv_WriteData(0x00);
      LCDDrv_WriteData(0xff);
      LCDDrv_WriteData(0x00);
      LCDDrv_WriteData(0xff);
      LCDDrv_WriteData(0x00);
    }
    for (i = 0; i < LCDTiming.LCDH / 2; i++)
    {
      LCDDrv_WriteData(0x00);
      LCDDrv_WriteData(0xff);
      LCDDrv_WriteData(0x00);
      LCDDrv_WriteData(0xff);
      LCDDrv_WriteData(0x00);
      LCDDrv_WriteData(0xff);
    }
  }
}


void RGBBar(void)
{
  uint32_t i, j, y, mod;

  y   = LCDTiming.LCDV / 3;
  mod = LCDTiming.LCDV % 3;

  for (i = 0; i < y; i++)
  {
    for (j = 0; j < LCDTiming.LCDH; j++)
    {
      LCDDrv_WriteData(0xff);
      LCDDrv_WriteData(0x00);
      LCDDrv_WriteData(0x00);
    }
  }
  for (i = 0; i < y; i++)
  {
    for (j = 0; j < LCDTiming.LCDH; j++)
    {
      LCDDrv_WriteData(0x00);
      LCDDrv_WriteData(0xff);
      LCDDrv_WriteData(0x00);
    }
  }
  for (i = 0; i < y; i++)
  {
    for (j = 0; j < LCDTiming.LCDH; j++)
    {
      LCDDrv_WriteData(0x00);
      LCDDrv_WriteData(0x00);
      LCDDrv_WriteData(0xff);
    }
  }

  for (i = 0; i < mod; i++)
  {
    for (j = 0; j < LCDTiming.LCDH; j++)
    {
      LCDDrv_WriteData(0x00);
      LCDDrv_WriteData(0x00);
      LCDDrv_WriteData(0xff);
    }
  }
}


void RGBLevel(void)
{
  uint32_t i, j, y, mod, color = 0;

  y   = LCDTiming.LCDV / 3;
  mod = LCDTiming.LCDV % 3;

  for (i = 0; i < y; i++)
  {
    for (j = 0; j < LCDTiming.LCDH; j++)
    {
      color = j * 255 / LCDTiming.LCDH;
      LCDDrv_WriteData(0xff - color);
      LCDDrv_WriteData(0x00);
      LCDDrv_WriteData(0x00);
    }
  }
  for (i = 0; i < y; i++)
  {
    for (j = 0; j < LCDTiming.LCDH; j++)
    {
      color = j * 255 / LCDTiming.LCDH;
      LCDDrv_WriteData(0x00);
      LCDDrv_WriteData(0xff - color);
      LCDDrv_WriteData(0x00);
    }
  }
  for (i = 0; i < y; i++)
  {
    for (j = 0; j < LCDTiming.LCDH; j++)
    {
      color = j * 255 / LCDTiming.LCDH;
      LCDDrv_WriteData(0x00);
      LCDDrv_WriteData(0x00);
      LCDDrv_WriteData(0xff - color);
    }
  }

  for (i = 0; i < mod; i++)
  {
    for (j = 0; j < LCDTiming.LCDH; j++)
    {
      color = j * 255 / LCDTiming.LCDH;
      LCDDrv_WriteData(0x00);
      LCDDrv_WriteData(0x00);
      LCDDrv_WriteData(0xff - color);
    }
  }
}
