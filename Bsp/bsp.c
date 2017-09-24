/**
 * @file         bsp.c
 * @author       Weyne Chen
 * @version      V01
 * @date         2017.09.23
 * @brief
 * @note
 * @attention    COPYRIGHT WEYNE
 **/

#include "bsp.h"
#include "gpio.h"
#include <stdio.h>
#include "stdlib.h"
#include "math.h"
#include "sys.h"
#include "tim.h"

#define ADDR_FLASH_PAGE_10     ((uint32_t)0x08005000) /* Base @ of Page 10, 2 Kbytes */
#define ADDR_FLASH_PAGE_5      ((uint32_t)0x08002800) /* Base @ of Page 5, 2 Kbytes */

#define KEY_STORE_ADDRESS ADDR_FLASH_PAGE_10
#define BOOT_KEY_ADDRESS ADDR_FLASH_PAGE_5

//KEY
const uint32_t __attribute__((at(KEY_STORE_ADDRESS + 20))) KeyStore = 0x5A2F5D81;


KeyTypeDef Key_Scan(void)
{
  KeyTypeDef ctrl_key = KEY_NULL;

  if (HAL_GPIO_ReadPin(GPIOE, KEY_POWER_Pin) == GPIO_PIN_RESET)
  {
    HAL_Delay(30);
    while (HAL_GPIO_ReadPin(GPIOE, KEY_POWER_Pin) == GPIO_PIN_RESET)
    {
      ctrl_key = KEY_POWER;
    }
  }
  else if (HAL_GPIO_ReadPin(GPIOE, KEY_UP_Pin) == GPIO_PIN_RESET)
  {
    uint32_t times = 0;
    HAL_Delay(30);
    while (HAL_GPIO_ReadPin(GPIOE, KEY_UP_Pin) == GPIO_PIN_RESET)
    {
      times++;
      HAL_Delay(1);
    }
    if (times > 2000)
    {
      ctrl_key = KEY_TP;
    }
    else
    {
      ctrl_key = KEY_UP;
    }
  }
  else if (HAL_GPIO_ReadPin(GPIOE, KEY_DOWN_Pin) == GPIO_PIN_RESET)
  {
    HAL_Delay(30);
    while (HAL_GPIO_ReadPin(GPIOE, KEY_DOWN_Pin) == GPIO_PIN_RESET)
    {
      ctrl_key = KEY_DOWN;
    }
  }
  else if (HAL_GPIO_ReadPin(GPIOB, KEY_MTP_Pin) == GPIO_PIN_RESET)
  {
    HAL_Delay(30);
    while (HAL_GPIO_ReadPin(GPIOB, KEY_MTP_Pin) == GPIO_PIN_RESET)
    {
      ctrl_key = KEY_MTP;
    }
  }

  return ctrl_key;
}


/**************************CDCE***********************************/
typedef enum
{
  I2C_NO_ACK,
  I2C_ACK
} AckTypeDef;

/**
 * @brief CDCE913 接口 PIN 的配置区域
 */
#define CDCE_ADDRESS    0x65
#define CDCE_ID         0x81
#define I2C_SCL         PBout(8)
#define I2C_SDA         PBout(9)
#define READ_SDA        PBin(9)
#define S0              PCout(6)
#define I2C_WRITE       CDCE_ADDRESS << 1
#define I2C_READ        ((CDCE_ADDRESS << 1) | 0x01)

static void I2C_Delay(volatile uint8_t t)
{
  uint8_t i, n;

  for (n = 0; n < t; n++)
  {
    for (i = 0; i < 35; i++)
    {
    }
  }
}


static void I2C_Start(void)
{
  I2C_SDA = 1;
  I2C_SCL = 1;
  I2C_Delay(40);
  I2C_SDA = 0;
  I2C_Delay(6);
  I2C_SCL = 0;
}


static void I2C_Stop(void)
{
  I2C_SCL = 0;
  I2C_SDA = 0;
  I2C_Delay(4);
  I2C_SCL = 1;
  I2C_Delay(4);
  I2C_SDA = 1;
  I2C_Delay(4);
}


static AckTypeDef I2C_WaitAck(void)
{
  uint8_t timeout = 0;

  I2C_SDA = 1;
  I2C_Delay(1);
  I2C_SCL = 1;
  I2C_Delay(3);
  while (READ_SDA)
  {
    timeout++;
    if (timeout > 250)
    {
      I2C_Stop();

      return I2C_NO_ACK;
    }
  }
  I2C_SCL = 0;
  return I2C_ACK;
}


static void I2C_Ack(void)
{
  I2C_SCL = 0;
  I2C_SDA = 0;
  I2C_Delay(3);
  I2C_SCL = 1;
  I2C_Delay(3);
  I2C_SCL = 0;
  I2C_Delay(1);
  I2C_SDA = 1;
  I2C_Delay(4);
}


static void I2C_NAck(void)
{
  I2C_SCL = 0;
  I2C_SDA = 1;
  I2C_Delay(3);
  I2C_SCL = 1;
  I2C_Delay(3);
  I2C_SCL = 0;
}


static void I2C_SendByte(uint8_t txd)
{
  uint8_t t;

  I2C_SCL = 0;
  for (t = 0; t < 8; t++)
  {
    I2C_SDA = (txd & 0x80) >> 7;
    txd   <<= 1;
    I2C_Delay(2);
    I2C_SCL = 1;
    I2C_Delay(4);
    I2C_SCL = 0;
    I2C_Delay(2);
  }
  I2C_SDA = 0;
}


static uint8_t I2C_ReadByte(AckTypeDef ack)
{
  uint8_t i, receive = 0;

  for (i = 0; i < 8; i++)
  {
    I2C_SCL = 0;
    I2C_Delay(3);
    I2C_SCL   = 1;
    receive <<= 1;
    if (READ_SDA)
    {
      receive++;
    }
    I2C_Delay(2);
  }
  if (!ack)
  {
    I2C_NAck();
  }
  else
  {
    I2C_Ack();
  }
  return receive;
}


static AckTypeDef CDCE_WriteByte(uint8_t reg, uint8_t data)
{
  I2C_Start();
  I2C_SendByte(I2C_WRITE);
  if (I2C_WaitAck() == I2C_NO_ACK)
  {
    return I2C_NO_ACK;
  }
  I2C_SendByte(reg | 0x80);
  if (I2C_WaitAck() == I2C_NO_ACK)
  {
    return I2C_NO_ACK;
  }
  I2C_SendByte(data);
  if (I2C_WaitAck() == I2C_NO_ACK)
  {
    return I2C_NO_ACK;
  }
  I2C_Stop();
  return I2C_ACK;
}


static AckTypeDef CDCE_Read8bit(uint8_t reg, uint16_t para_amount, uint8_t *data)
{
  uint16_t i;
  AckTypeDef ack;

  if (para_amount == 0)
  {
    para_amount = 1;
  }

  I2C_Start();
  I2C_SendByte(I2C_WRITE);
  ack = I2C_WaitAck();
  if (ack != I2C_ACK)
  {
    return I2C_NO_ACK;
  }
  I2C_SendByte(reg | 0x80);
  ack = I2C_WaitAck();

  if (ack != I2C_ACK)
  {
    return I2C_NO_ACK;
  }

  I2C_Stop();

  I2C_Start();
  I2C_SendByte(I2C_READ);
  ack = I2C_WaitAck();

  if (ack != I2C_ACK)
  {
    return I2C_NO_ACK;
  }
  for (i = 0; i < para_amount - 1; i++)
  {
    data[i] = I2C_ReadByte(I2C_ACK);
    I2C_Delay(100);
  }
  data[para_amount - 1] = I2C_ReadByte(I2C_NO_ACK);
  I2C_Stop();

  return I2C_ACK;
}


static void MCLK_Init(uint16_t f_out)
{
  uint8_t read_back;
  AckTypeDef ack;
  uint16_t M, N, Pdiv, Q, R;
  uint8_t reg18, reg19, reg1A, reg1B;
  int P;
  uint8_t fvco;
  float t, temp;

  for (Pdiv = 1; Pdiv < 127; Pdiv++)
  {
    t = (float)(f_out * Pdiv) / 8;
    if ((t > 14) && (t < 28.75))
    {
      break;
    }

    if (t > 28.75)
    {
      printf("Error:unsupport pclk\n");
      return;
    }
  }

  for (N = 4095; N > 0; N--)
  {
    if (temp == t)
    {
      N++;
      break;
    }
    for (M = 1; M < 511; M++)
    {
      temp = (float)N / (float)M;
      if (temp == t)
      {
        break;
      }
    }
  }

  if (N == 0)
  {
    printf("Error:unsupport pclk\n");
    return;
  }

  P = 4 - abs(log((double)N / (double)M));
  if (P < 0)
  {
    P = 0;
  }
  Q = (int)((double)N * pow(2, (double)P) / (double)M);
  R = (double)N * pow(2, (double)P) - M * Q;

  fvco = 8 * N / M;

  if (fvco < 125)
  {
    fvco = 0;
  }
  else if ((fvco >= 125) && (fvco < 150))
  {
    fvco = 1;
  }
  else if ((fvco >= 150) && (fvco < 175))
  {
    fvco = 2;
  }
  else
  {
    fvco = 3;
  }

  S0 = 0;

  ack = CDCE_Read8bit(0x00, 1, &read_back);
  if (ack != I2C_ACK)
  {
    printf("Error:clk configuration failed , maybe no pullup res\n");
    return;
  }

  if (read_back != CDCE_ID)
  {
    printf("Error:clk device ID error\n");
    return;
  }

  CDCE_WriteByte(0x02, 0xB4);
  CDCE_WriteByte(0x03, (uint8_t)Pdiv);
  CDCE_WriteByte(0x04, 0x02);
  CDCE_WriteByte(0x05, 0xA0);
  CDCE_WriteByte(0x06, 0x40);
  CDCE_WriteByte(0x12, 0x00);
  CDCE_WriteByte(0x13, 0x01);
  CDCE_WriteByte(0x14, 0x6D);
  CDCE_WriteByte(0x15, 0x02);
  CDCE_WriteByte(0x16, 0);
  CDCE_WriteByte(0x17, 0);

  reg18 = (N >> 4) & 0xFFF;
  reg19 = (N & 0xf) << 4 | (R & 0xf0) >> 5;
  reg1A = (R & 0x1f) << 3 | ((Q >> 3) & 0x7);
  reg1B = (Q & 0x7) << 5 | (P & 0x07) << 2 | (fvco & 0x03);

  CDCE_WriteByte(0x18, reg18);
  CDCE_WriteByte(0x19, reg19);
  CDCE_WriteByte(0x1A, reg1A);
  CDCE_WriteByte(0x1B, reg1B);

  CDCE_WriteByte(0x1C, N);
  CDCE_WriteByte(0x1D, ((N & 0xf) << 4) | (R & 0xf0));
  CDCE_WriteByte(0x1E, (R & 0x0f) | (Q & 0xf0));
  CDCE_WriteByte(0x1F, ((Q & 0x07) << 5) | ((P & 0x07) << 2) | (fvco & 0x03));

  S0 = 1;
  printf("Info:clk well configured\n");
}


/*****************FPGA******************************/
LCDTimingParaTypeDef LCDTiming;

#define FSMC_NE1         PDout(7)
#define FSMC_LCD_CMD     ((uint32_t)0x60000000) //A16
#define FSMC_LCD_DATA    ((uint32_t)0x60010000)
#define FPGA_WRITE_CMD(x)              \
                                       \
  do                                   \
  {                                    \
    FSMC_NE1 = 0;                      \
                                       \
    *(__IO uint8_t *)FSMC_LCD_CMD = x; \
    FSMC_NE1 = 1;                      \
                                       \
  }                                    \
  while (0)

#define FPGA_WRITE_DATA(x)              \
                                        \
  do                                    \
  {                                     \
    FSMC_NE1 = 0;                       \
                                        \
    *(__IO uint8_t *)FSMC_LCD_DATA = x; \
    FSMC_NE1 = 1;                       \
                                        \
  }                                     \
  while (0)

/* 当NE使能以后，可以使用下面的简化操作*/
//#define FPGA_WRITE_CMD(x) *(__IO uint8_t *)FSMC_LCD_CMD = x
//#define FPGA_WRITE_DATA(x) *(__IO uint8_t *)FSMC_LCD_DATA = x

#define FPGA_READ_DATA()            \
                                    \
  do                                \
  {                                 \
    FSMC_NE1 = 0;                   \
                                    \
    *(__IO uint8_t *)FSMC_LCD_DATA; \
    FSMC_NE1 = 1;                   \
                                    \
  }                                 \
  while (0)

static void FPGAWrite16BitData(uint16_t data)
{
  if (data > 0)
  {
    data = data - 1;
  }
  FPGA_WRITE_DATA(data >> 8);
  FPGA_WRITE_DATA(data);
}


void LCDDrv_WriteData(uint8_t para)
{
  FPGA_WRITE_DATA(para);
}


void LCDDrv_SetTiming(void)
{
  MCLK_Init(LCDTiming.DCLK);
  HAL_Delay(50);
  FPGA_WRITE_CMD(0xB0);
  FPGAWrite16BitData(LCDTiming.LCDH);
  FPGAWrite16BitData(LCDTiming.LCDV);

  FPGA_WRITE_CMD(0xB4);
  FPGAWrite16BitData(LCDTiming.HSPW);
  FPGAWrite16BitData(LCDTiming.HFPD);
  FPGAWrite16BitData(LCDTiming.HBPD);

  FPGA_WRITE_CMD(0xB6);
  FPGAWrite16BitData(LCDTiming.VSPW);
  FPGAWrite16BitData(LCDTiming.VFPD);
  FPGAWrite16BitData(LCDTiming.VBPD);

  FPGA_WRITE_CMD(0x28);
}


void LCDDrv_OpenRGB(void)
{
  FPGA_WRITE_CMD(0x29);
}


void LCDDrv_CloseRGB(void)
{
  FPGA_WRITE_CMD(0x28);
}


void LCDDrv_EnterLVDS(uint8_t mode)
{
  FPGA_WRITE_CMD(0x25);
  FPGA_WRITE_DATA(mode);
}


void LCDDrv_SetPattern(void)
{
  FPGA_WRITE_CMD(0x2C);
}


static int32_t CalRAMAddress(uint8_t frame)
{
  uint32_t address;

  address  = LCDTiming.LCDH;
  address *= LCDTiming.LCDV;
  address *= frame;

  return address;
}


void LCDDrv_SetXY(uint16_t x, uint16_t y)
{
  uint32_t address = 0;
  uint8_t i        = 0;
  uint8_t *p       = (uint8_t *)&address;

  address  = LCDTiming.LCDH;
  address *= y;
  address += x;

  FPGA_WRITE_CMD(0x2A);

  for ( ; i < sizeof(address); i++)
  {
    FPGA_WRITE_DATA(*(p + 3 - i));
  }

  FPGA_WRITE_CMD(0x2C);
}


void LCDDrv_SetCharIndex(uint8_t frame)
{
  uint32_t address = 0;
  uint8_t i        = 0;
  uint8_t *p       = (uint8_t *)&address;

  address = CalRAMAddress(frame);

  FPGA_WRITE_CMD(0x3A);

  for ( ; i < sizeof(address); i++)
  {
    FPGA_WRITE_DATA(*(p + 3 - i));
  }
}


#define FPGA_IO_MODE    0xFF000000

void LCDDrv_ShowPattern(uint32_t data)
{
  uint32_t address = 0;
  uint8_t i        = 0;
  uint8_t *p       = (uint8_t *)&address;

  if ((data & FPGA_IO_MODE) == FPGA_IO_MODE)
  {
    uint8_t r, g, b;
    r = (data & 0xffffff) >> 16;
    g = (data & 0xffffff) >> 8;
    b = (data & 0xffffff) >> 0;
    FPGA_WRITE_CMD(0x1B);
    FPGA_WRITE_DATA(r);
    FPGA_WRITE_DATA(g);
    FPGA_WRITE_DATA(b);
  }
  else
  {
    FPGA_WRITE_CMD(0x1B);
    FPGA_WRITE_DATA(0xAA);
    FPGA_WRITE_DATA(0xAA);
    FPGA_WRITE_DATA(0xAA);

    HAL_Delay(1);

    address = CalRAMAddress(data);

    FPGA_WRITE_CMD(0x0B);

    for ( ; i < sizeof(address); i++)
    {
      FPGA_WRITE_DATA(*(p + 3 - i));
    }
  }
}


/********************************POWRE************************************/

static const uint32_t PowerMap[] =
{
  BITBAND(GPIOB_ODR_Addr,  0),
  BITBAND(GPIOC_ODR_Addr,  7),
  BITBAND(GPIOC_ODR_Addr,  5),
  BITBAND(GPIOE_ODR_Addr, 14),
  BITBAND(GPIOB_ODR_Addr, 12),
  BITBAND(GPIOC_ODR_Addr,  0),
  BITBAND(GPIOD_ODR_Addr,  3),
  BITBAND(GPIOB_ODR_Addr,  2),
  BITBAND(GPIOB_ODR_Addr, 11),
  BITBAND(GPIOB_ODR_Addr,  1),
  BITBAND(GPIOC_ODR_Addr,  3),
};

void Power_SetState(PowerTypeDef power, StateTypeDef state)
{
  MEM_ADDR(PowerMap[power]) = (uint8_t)state;
}


void Power_SetBLCurrent(uint16_t value)
{
  TIM1->CCR1 = value;
  HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1);
}


/***************RGB************************/

typedef enum
{
  RGB_SPI_WRITE,
  RGB_SPI_READ,
} SPIRWTypeDef;

typedef enum
{
  RGB_SPI_LOW_BYTE,
  RGB_SPI_HIGH_BYTE,
} SPIByteTypeDef;

#define RGB_SPI_SDI    PEin(5)
#define RGB_SPI_SDA    PEout(4)
#define RGB_SPI_SCK    PEout(3)
#define RGB_SPI_CS     PEout(0)
#define RGB_RESET      PEout(6)
#define RGB_SPI_RS     PEout(2)

static void Delay_10us(volatile uint8_t t)
{
  volatile uint8_t i = 15;

  while (t--)
  {
    while (i--)
    {
    }
  }
}


static void RGB_SPI16BitFallingEdgeWrite(SPIRWTypeDef rw, SPIDataCommandTypeDef data_command, SPIByteTypeDef high_low_byte, uint8_t data)
{
  uint16_t tmp = 0;
  uint8_t i;

  tmp  = (((uint8_t)rw & 0x01) << 15);
  tmp |= (((uint8_t)data_command & 0x01) << 14);
  tmp |= (((uint8_t)high_low_byte & 0x01) << 13);
  tmp |= data;

  RGB_SPI_CS  = 0;
  RGB_SPI_SCK = 1;
  Delay_10us(5);

  for (i = 0; i < 16; i++)
  {
    if ((tmp & 0x8000) == 0x8000)
    {
      RGB_SPI_SDA = 1;
    }
    else
    {
      RGB_SPI_SDA = 0;
    }

    Delay_10us(2);
    RGB_SPI_SCK = 0;
    Delay_10us(2);
    RGB_SPI_SCK = 1;
    Delay_10us(2);
    tmp = tmp << 1;
  }
  Delay_10us(1);
  RGB_SPI_CS = 1;
}


static void RGB_SPI16BitRisingEdgeWrite(SPIRWTypeDef rw, SPIDataCommandTypeDef data_command, SPIByteTypeDef high_low_byte, uint8_t data)
{
  uint16_t tmp = 0;
  uint8_t i;

  tmp  = (((uint8_t)rw & 0x01) << 15);
  tmp |= (((uint8_t)data_command & 0x01) << 14);
  tmp |= (((uint8_t)high_low_byte & 0x01) << 13);
  tmp |= data;

  RGB_SPI_CS  = 0;
  RGB_SPI_SCK = 0;
  Delay_10us(5);

  for (i = 0; i < 16; i++)
  {
    if ((tmp & 0x8000) == 0x8000)
    {
      RGB_SPI_SDA = 1;
    }
    else
    {
      RGB_SPI_SDA = 0;
    }

    Delay_10us(2);
    RGB_SPI_SCK = 1;
    Delay_10us(2);
    RGB_SPI_SCK = 0;
    Delay_10us(2);
    tmp = tmp << 1;
  }
  Delay_10us(1);
  RGB_SPI_CS = 1;
}


static uint8_t RGB_SPI16BitRisingEdgeRead(void)
{
  uint16_t i;
  uint8_t tmp = 0xC0;

  RGB_SPI_CS  = 0;
  RGB_SPI_SCK = 0;
  Delay_10us(1);

  for (i = 0; i < 8; i++)
  {
    if ((tmp & 0x80) == 0x80)
    {
      RGB_SPI_SDA = 1;
    }
    else
    {
      RGB_SPI_SDA = 0;
    }

    Delay_10us(2);
    RGB_SPI_SCK = 0;
    Delay_10us(2);
    RGB_SPI_SCK = 1;
    Delay_10us(2);
    tmp = tmp << 1;
  }

  tmp = 0;
  for (i = 0; i < 8; i++)
  {
    RGB_SPI_SCK = 1;
    Delay_10us(1);
    RGB_SPI_SCK = 0;
    Delay_10us(1);
    tmp <<= 1;
    if (RGB_SPI_SDI)
    {
      tmp |= 0x01;
    }
  }

  RGB_SPI_CS = 1;
  return tmp;
}


static uint8_t RGB_SPI16BitFallingEdgeRead(void)
{
  uint16_t i;
  uint8_t tmp = 0xC0;

  RGB_SPI_CS  = 0;
  RGB_SPI_SCK = 1;
  Delay_10us(1);

  for (i = 0; i < 8; i++)
  {
    if ((tmp & 0x80) == 0x80)
    {
      RGB_SPI_SDA = 1;
    }
    else
    {
      RGB_SPI_SDA = 0;
    }

    Delay_10us(2);
    RGB_SPI_SCK = 1;
    Delay_10us(2);
    RGB_SPI_SCK = 0;
    Delay_10us(2);
    tmp = tmp << 1;
  }

  tmp = 0;
  for (i = 0; i < 8; i++)
  {
    RGB_SPI_SCK = 0;
    Delay_10us(1);
    RGB_SPI_SCK = 1;
    Delay_10us(1);
    tmp <<= 1;
    if (RGB_SPI_SDI)
    {
      tmp |= 0x01;
    }
  }

  RGB_SPI_CS = 1;
  return tmp;
}


void RGB_SPIWrite_16Bit(uint16_t address, uint8_t data, SPIEdgeTypeDef edge)
{
  if (edge == RGB_SPI_RISING)
  {
    RGB_SPI16BitRisingEdgeWrite(RGB_SPI_WRITE, RGB_SPI_COMMAND, RGB_SPI_HIGH_BYTE, address >> 8);
    RGB_SPI16BitRisingEdgeWrite(RGB_SPI_WRITE, RGB_SPI_COMMAND, RGB_SPI_LOW_BYTE, (uint8_t)address);
    RGB_SPI16BitRisingEdgeWrite(RGB_SPI_WRITE, RGB_SPI_DATA, RGB_SPI_LOW_BYTE, data);
  }
  else
  {
    RGB_SPI16BitFallingEdgeWrite(RGB_SPI_WRITE, RGB_SPI_COMMAND, RGB_SPI_HIGH_BYTE, address >> 8);
    RGB_SPI16BitFallingEdgeWrite(RGB_SPI_WRITE, RGB_SPI_COMMAND, RGB_SPI_LOW_BYTE, (uint8_t)address);
    RGB_SPI16BitFallingEdgeWrite(RGB_SPI_WRITE, RGB_SPI_DATA, RGB_SPI_LOW_BYTE, data);
  }
}


uint8_t RGB_SPIRead_16Bit(uint16_t address, SPIEdgeTypeDef edge)
{
  if (edge == RGB_SPI_RISING)
  {
    RGB_SPI16BitRisingEdgeWrite(RGB_SPI_WRITE, RGB_SPI_COMMAND, RGB_SPI_HIGH_BYTE, address >> 8);
    RGB_SPI16BitRisingEdgeWrite(RGB_SPI_WRITE, RGB_SPI_COMMAND, RGB_SPI_LOW_BYTE, (uint8_t)address);
    return RGB_SPI16BitRisingEdgeRead();
  }
  else
  {
    RGB_SPI16BitFallingEdgeWrite(RGB_SPI_WRITE, RGB_SPI_COMMAND, RGB_SPI_HIGH_BYTE, address >> 8);
    RGB_SPI16BitFallingEdgeWrite(RGB_SPI_WRITE, RGB_SPI_COMMAND, RGB_SPI_LOW_BYTE, (uint8_t)address);
    return RGB_SPI16BitFallingEdgeRead();
  }
}


void RGB_SPIWrite8Bit(uint8_t data, SPIDataCommandTypeDef rs)
{
  uint8_t i = 0;

  RGB_SPI_CS  = 0;
  RGB_SPI_RS  = rs;
  RGB_SPI_SCK = 0;
  Delay_10us(2);

  for (i = 0; i < 8; i++)
  {
    RGB_SPI_SDA = data >> (7 - i);

    Delay_10us(2);
    RGB_SPI_SCK = 1;
    Delay_10us(2);
    RGB_SPI_SCK = 0;
    Delay_10us(2);
  }

  RGB_SPI_CS = 1;
}


void RGB_SPIWrite9Bit(uint8_t data, SPIDataCommandTypeDef rs)
{
  uint8_t i = 0;

  RGB_SPI_CS  = 0;
  RGB_SPI_SCK = 0;
  Delay_10us(2);

  RGB_SPI_SDA = rs;
  Delay_10us(2);
  RGB_SPI_SCK = 1;
  Delay_10us(2);
  RGB_SPI_SCK = 0;

  for (i = 0; i < 8; i++)
  {
    RGB_SPI_SDA = data >> (7 - i);

    Delay_10us(2);
    RGB_SPI_SCK = 1;
    Delay_10us(2);
    RGB_SPI_SCK = 0;
    Delay_10us(2);
  }

  RGB_SPI_CS = 1;
}


/**************************SSD2828***********************/

/**
 * @brief SSD2828 IO PIN 的配置区域
 */
#define SPI_SDI          PEin(5)  /*< SPI SDI */
#define SPI_SDO          PEout(4) /*< SPI SDO */
#define SPI_SCK          PEout(3) /*< SPI CLK */
#define SPI_CS           PEout(1) /*< SSD2828 1 的SPI使能PIN，低电平有效 */
#define SSD2828_RESET    PEout(6) /*< SSD2828 Reset pin */
#define SSD2828_SHUT     PEout(2) /*< SPI Shut down 功能脚 */

static uint16_t mode = LP;

static void Delay(volatile uint8_t t)
{
  //volatile uint8_t i = 1;

  while (t--)
  {
    //    while (i--)
    //    {
    //    }
  }
}


uint8_t SSD2828_ACK(void)
{
  uint16_t i;
  uint8_t tmp = 0;

  SPI_CS = 0;
  Delay(4);
  for (i = 0; i < 8; i++)
  {
    SPI_SCK = 0;
    Delay(4);
    SPI_SCK = 1;
    Delay(4);
    tmp <<= 1;
    if (SPI_SDI)
    {
      tmp |= 0x01;
    }
  }

  SPI_CS = 1;
  return tmp;
}


void SSD2828_WriteCmd(uint8_t cmd)
{
  uint8_t i;

  SPI_CS  = 0;
  SPI_SDO = 0;
  Delay(4);
  SPI_SCK = 0;
  Delay(4);
  SPI_SCK = 1;
  Delay(4);
  for (i = 0; i < 8; i++)
  {
    if ((cmd & 0x80) == 0x80)
    {
      SPI_SDO = 1;
    }
    else
    {
      SPI_SDO = 0;
    }
    Delay(4);
    SPI_SCK = 0;
    Delay(4);
    SPI_SCK = 1;
    Delay(4);
    cmd = cmd << 1;
  }
  Delay(0);
  SPI_CS = 1;
}


void MIPI_WriteData(uint8_t data)
{
  uint8_t i;

  SPI_CS  = 0;
  SPI_SDO = 1;
  Delay(4);
  SPI_SCK = 0;
  Delay(4);
  SPI_SCK = 1;
  Delay(4);
  for (i = 0; i < 8; i++)
  {
    if ((data & 0x80) == 0x80)
    {
      SPI_SDO = 1;
    }
    else
    {
      SPI_SDO = 0;
    }
    Delay(4);
    SPI_SCK = 0;
    Delay(4);
    SPI_SCK = 1;
    Delay(4);
    data = data << 1;
  }
  Delay(4);
  SPI_CS = 1;
}


void SSD2828_SetReset(uint8_t state)
{
  SSD2828_RESET = state;
}


void SSD2828_WriteReg(uint8_t cmd, uint8_t data1, uint8_t data2)
{
  uint8_t i;

  SPI_CS  = 0;
  SPI_SDO = 0;
  SPI_SCK = 0;
  SPI_SCK = 0;
  SPI_SCK = 0;
  SPI_SCK = 1;
  for (i = 0; i < 8; i++)
  {
    if ((cmd & 0x80) == 0x80)
    {
      SPI_SDO = 1;
    }
    else
    {
      SPI_SDO = 0;
    }
    SPI_SCK = 0;
    SPI_SCK = 0;
    SPI_SCK = 0;
    SPI_SCK = 1;
    cmd     = cmd << 1;
  }
  SPI_SDO = 1;
  SPI_SCK = 0;
  SPI_SCK = 0;
  SPI_SCK = 0;
  SPI_SCK = 1;
  for (i = 0; i < 8; i++)
  {
    if ((data2 & 0x80) == 0x80)
    {
      SPI_SDO = 1;
    }
    else
    {
      SPI_SDO = 0;
    }
    SPI_SCK = 0;
    SPI_SCK = 0;
    SPI_SCK = 0;
    SPI_SCK = 1;
    data2   = data2 << 1;
  }
  SPI_SDO = 1;
  SPI_SCK = 0;
  SPI_SCK = 0;
  SPI_SCK = 0;
  SPI_SCK = 1;
  for (i = 0; i < 8; i++)
  {
    if ((data1 & 0x80) == 0x80)
    {
      SPI_SDO = 1;
    }
    else
    {
      SPI_SDO = 0;
    }
    SPI_SCK = 0;
    SPI_SCK = 0;
    SPI_SCK = 0;
    SPI_SCK = 1;
    data1   = data1 << 1;
  }
  SPI_CS = 1;
}


uint16_t SSD2828_ReadReg(uint8_t reg)
{
  uint16_t tmp;
  uint8_t data_l, data_h;

  SSD2828_WriteReg(0xd4, 0x00, 0xfa);
  SSD2828_WriteCmd(reg);
  SSD2828_WriteCmd(0xfa);
  data_l = SSD2828_ACK();
  data_h = SSD2828_ACK();
  tmp    = data_h;
  tmp  <<= 8;
  tmp   |= data_l;
  return tmp;
}


void SSD2828_Reset(void)
{
  SSD2828_RESET = 0;
  SSD2828_SHUT  = 1;
  HAL_Delay(50);
  SSD2828_RESET = 1;
  HAL_Delay(10);
}


void SSD2828_ShutDown(uint8_t state)
{
  SSD2828_SHUT = (uint8_t)state & 0x01;
}


void MIPI_DcsShortWrite(uint8_t n)
{
  if (mode == LP)
  {
    SSD2828_WriteReg(0x00b7, 0x02, 0x50);
  }
  else if (mode == HS)
  {
    SSD2828_WriteReg(0x00b7, 0x02, 0x50 & 0XEF | 0X03);
  }
  else if (mode == VD)
  {
    SSD2828_WriteReg(0x00b7, 0x02 | 0x01, 0x50 & 0XEF | 0X0B);
  }
  Delay(10);
  SSD2828_WriteReg(0xbc, 0x00, n);
  SSD2828_WriteReg(0xbd, 0x00, 0x00);
  SSD2828_WriteReg(0xbe, 0x00, n);
  SSD2828_WriteCmd(0xbf);
}


void MIPI_DcsLongWrite(uint32_t n)
{
  if (mode == LP)
  {
    SSD2828_WriteReg(0x00b7, 0x06, 0x50);
  }
  else if (mode == HS)
  {
    SSD2828_WriteReg(0x00b7, 0x06, 0x50 & 0XEF | 0X03);
  }
  else if (mode == VD)
  {
    SSD2828_WriteReg(0x00b7, 0x06 | 0x01, 0x50 & 0XEF | 0X0B);
  }
  Delay(10);
  SSD2828_WriteReg(0xbc, n >> 8, n);
  SSD2828_WriteReg(0xbd, n >> 24, n >> 16);
  SSD2828_WriteReg(0xbe, 0x0f, 0xff);
  SSD2828_WriteCmd(0xbf);
}


void MIPI_GenericShortWrite(uint8_t n)
{
  if (mode == LP)
  {
    SSD2828_WriteReg(0x00b7, 0x02, 0x10);
  }
  else if (mode == HS)
  {
    SSD2828_WriteReg(0x00b7, 0x02, 0x10 & 0XEF | 0X03);
  }
  else if (mode == VD)
  {
    SSD2828_WriteReg(0x00b7, 0x02 | 0x01, 0x10 & 0XEF | 0X0B);
  }
  Delay(10);
  SSD2828_WriteReg(0xbc, 0x00, n);
  SSD2828_WriteReg(0xbd, 0x00, 0x00);
  SSD2828_WriteReg(0xbe, 0x00, n);
  SSD2828_WriteCmd(0xbf);
}


void MIPI_GenericLongWrite(uint32_t n)
{
  if (mode == LP)
  {
    SSD2828_WriteReg(0x00b7, 0x06, 0x10);
  }
  else if (mode == HS)
  {
    SSD2828_WriteReg(0x00b7, 0x06, 0x10 & 0XEF | 0X03);
  }
  else if (mode == VD)
  {
    SSD2828_WriteReg(0x00b7, 0x06 | 0X01, 0x10 & 0XEF | 0X0B);
  }
  Delay(10);
  SSD2828_WriteReg(0xbc, n >> 8, n);
  SSD2828_WriteReg(0xbd, n >> 24, n >> 16);
  SSD2828_WriteReg(0xbe, 0x0f, 0xff);
  SSD2828_WriteCmd(0xbf);
}


MIPI_ReadTypeDef MIPI_DcsRead(uint8_t adr, uint16_t l, uint8_t *p)
{
  uint16_t state;
  uint16_t i;
  uint16_t timeout_cnt = 0;

  do
  {
    if (mode == LP)
    {
      SSD2828_WriteReg(0x00b7, 0x03, 0xc2);
    }
    else if (mode == HS)
    {
      SSD2828_WriteReg(0x00b7, 0x03, 0xc3);
    }
    else if (mode == VD)
    {
      SSD2828_WriteReg(0x00b7, 0x03, 0xcb);
    }
    SSD2828_WriteReg(0x00bb, 0x00, 8);
    SSD2828_WriteReg(0x00c1, l >> 8, l);
    SSD2828_WriteReg(0x00c0, 0x00, 0x01);
    SSD2828_WriteReg(0x00BC, 0x00, 0x01);
    SSD2828_WriteReg(0x00BF, 0x00, adr);
    HAL_Delay(10);
    state = SSD2828_ReadReg(0xc6);
    if (++timeout_cnt > 10)
    {
      return MIPI_READ_FAIL;
    }
  } while ((state & 0x01) == 0);
  SSD2828_WriteCmd(0xff);
  for (i = 0; i < l; )
  {
    SSD2828_WriteCmd(0xfa);
    *p++ = SSD2828_ACK();
    if (++i >= l)
    {
      SSD2828_ACK();
      break;
    }
    *p++ = SSD2828_ACK();
    ++i;
  }
  return MIPI_READ_SUCCEED;
}


MIPI_ReadTypeDef MIPI_GenericRead(uint8_t adr, uint16_t l, uint8_t *p)
{
  uint16_t state;
  uint16_t i;
  uint16_t timeout_cnt = 0;

  do
  {
    if (mode == LP)
    {
      SSD2828_WriteReg(0x00b7, 0x03, 0x82);
    }
    else if (mode == HS)
    {
      SSD2828_WriteReg(0x00b7, 0x03, 0x83);
    }
    else if (mode == VD)
    {
      SSD2828_WriteReg(0x00b7, 0x03, 0x8B);
    }
    SSD2828_WriteReg(0x00bb, 0x00, 8);
    SSD2828_WriteReg(0x00c1, l >> 8, l);
    SSD2828_WriteReg(0x00c0, 0x00, 0x01);
    SSD2828_WriteReg(0x00BC, 0x00, 1);
    SSD2828_WriteReg(0x00BF, 0x00, adr);
    HAL_Delay(10);
    state = SSD2828_ReadReg(0xc6);
    if (++timeout_cnt > 10)
    {
      return MIPI_READ_FAIL;
    }
  } while ((state & 0x01) == 0);
  SSD2828_WriteCmd(0xff);
  for (i = 0; i < l; )
  {
    SSD2828_WriteCmd(0xfa);
    *p++ = SSD2828_ACK();
    if (++i >= l)
    {
      SSD2828_ACK();
      break;
    }
    *p++ = SSD2828_ACK();
    ++i;
  }
  return MIPI_READ_SUCCEED;
}


void MIPI_SetMode(MIPI_ModeTypeDef m)
{
  mode = m;
  if (mode == VD)
  {
    SSD2828_WriteReg(0x00b7, 0x02, 0x4B);     //024B//030b
    SSD2828_SHUT = 0;
  }
}


/***********************其他*******************************************/
static struct
{
  GPIO_TypeDef *gpio_port;
  uint16_t     gpio_pin;
}
ResetPortPin;


void BSP_SetLCDType(LCDTypeDef type)
{
  if (type == MIPI)
  {
    ResetPortPin.gpio_port = MIPIRESET_GPIO_Port;
    ResetPortPin.gpio_pin  = MIPIRESET_Pin;
  }
  else
  {
    ResetPortPin.gpio_port = GPIOE;
    ResetPortPin.gpio_pin  = GPIO_PIN_6;
  }
}


void BSP_Init(void)
{
  HAL_GPIO_WritePin(LS245_OE_GPIO_Port, LS245_OE_Pin, GPIO_PIN_RESET);
}


void LCD_Reset(uint8_t high_low)
{
  HAL_GPIO_WritePin(ResetPortPin.gpio_port, ResetPortPin.gpio_pin, high_low ? GPIO_PIN_SET : GPIO_PIN_RESET);
}


void BSP_SetIndicatorLight(LightTypeDef type, StateTypeDef state)
{
  if (type == LIGHT_GREEN)
  {
    HAL_GPIO_WritePin(GREEN_LED_GPIO_Port, GREEN_LED_Pin, (state == ON) ? GPIO_PIN_SET : GPIO_PIN_RESET);
  }

  if (type == LIGHT_RED)
  {
    HAL_GPIO_WritePin(RED_LED_GPIO_Port, RED_LED_Pin, (state == ON) ? GPIO_PIN_SET : GPIO_PIN_RESET);
  }
}


/********************* (C) COPYRIGHT WEYNE CHEN *******END OF FILE ********/
