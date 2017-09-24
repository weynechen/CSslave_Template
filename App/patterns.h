#ifndef _PATTERNS_H_
#define _PATTERNS_H_
#include "stdint.h"

typedef enum
{
  GRA_RED   = 0xff,
  GRA_GREEN = 0xff00,
  GRA_BLUE  = 0xff0000,
  GRA_GRAY  = 0xffffff,
} GradientColorTypeDef;


typedef enum
{
  DIR_H,
  DIR_V,
} DirTypeDef;

void Flicker(void);
void FillFull(uint8_t r, uint8_t g, uint8_t b);
void Crosstalk(void);
void Chessboard(void);
void Gradient(GradientColorTypeDef type, DirTypeDef dir);
void Frame(void);
void ColorBarH(void);
void ColorBarV(void);
void SnowDot(void);
void RGBBar(void);
void RGBLevel(void);

#endif
