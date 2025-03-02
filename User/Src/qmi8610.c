/*************************************************************************
 *  QMI8610驱动
 *  Copyright (C) 2025  Xu Ruijun
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <https://www.gnu.org/licenses/>.
 *************************************************************************/#include "qmi8610.h"
#include "stm32f1xx_hal.h"
#include "stm32f1xx_ll_bus.h"
#include "stm32f1xx_ll_gpio.h"

#define GPIO_PORT_CS     GPIOA
#define GPIO_PIN_CS      LL_GPIO_PIN_15
#define CS_ACTIVE()      LL_GPIO_ResetOutputPin(GPIO_PORT_CS, GPIO_PIN_CS)
#define CS_INACTIVE()    LL_GPIO_SetOutputPin(GPIO_PORT_CS, GPIO_PIN_CS)

#define SPI_TIMEOUT      100 //100ms

#define REG_WHO_AM_I     0x00

#define REG_CTRL1        0x02
#define REG_CTRL2        0x03
#define REG_CTRL3        0x04
#define REG_CTRL4        0x05
#define REG_CTRL5        0x06
#define REG_CTRL6        0x07
#define REG_CTRL7        0x08
#define REG_CTRL8        0x09
#define REG_CTRL9        0x0A

#define REG_CAL1_L       0x0B
#define REG_CAL1_H       0x0C
#define REG_CAL2_L       0x0D
#define REG_CAL2_H       0x0E
#define REG_CAL3_L       0x0F
#define REG_CAL3_H       0x10
#define REG_CAL4_L       0x11
#define REG_CAL4_H       0x12

#define REG_FIFO_CTRL    0x13
#define REG_FIFO_DATA    0x14
#define REG_FIFO_STATUS  0x15

#define REG_STATUS0      0x16
#define REG_STATUS1      0x17

#define REG_CNT_OUT      0x18

#define REG_AX_L         0x19
#define REG_AX_H         0x1A
#define REG_AY_L         0x1B
#define REG_AY_H         0x1C
#define REG_AZ_L         0x1D
#define REG_AZ_H         0x1E
#define REG_GX_L         0x1F
#define REG_GX_H         0x20
#define REG_GY_L         0x21
#define REG_GY_H         0x22
#define REG_GZ_L         0x23
#define REG_GZ_H         0x24
#define REG_MX_L         0x25
#define REG_MX_H         0x26
#define REG_MY_L         0x27
#define REG_MY_H         0x28
#define REG_MZ_L         0x29
#define REG_MZ_H         0x2A

#define REG_dQW_L        0x2D
#define REG_dQW_H        0x2E
#define REG_dQX_L        0x2F
#define REG_dQX_H        0x30
#define REG_dQY_L        0x31
#define REG_dQY_H        0x32
#define REG_dQZ_L        0x33
#define REG_dQZ_H        0x34
#define REG_dVX_L        0x35
#define REG_dVX_H        0x36
#define REG_dVY_L        0x37
#define REG_dVY_H        0x38
#define REG_dVZ_L        0x39
#define REG_dVZ_H        0x3A

#define REG_TEMP         0x3B
#define REG_AE_REG1      0x3C
#define REG_AE_REG2      0x3D

extern SPI_HandleTypeDef hspi;

static HAL_StatusTypeDef QMI8610_Write(uint8_t addr, uint8_t *data, uint16_t size)
{
  HAL_StatusTypeDef errcode;
  addr &= 0x3f;
  if(size > 1){
    addr |= 0x40;
  }
  CS_ACTIVE();
  if(errcode=HAL_SPI_Transmit(&hspi, &addr, 1, SPI_TIMEOUT) != HAL_OK){
    goto error;
  }
  if(errcode=HAL_SPI_Transmit(&hspi, data, size, SPI_TIMEOUT) != HAL_OK){
    goto error;
  }
 error:
  CS_INACTIVE();
  return errcode;
}

static HAL_StatusTypeDef QMI8610_Read(uint8_t addr, uint8_t *data, uint16_t size)
{
  HAL_StatusTypeDef errcode;
  addr = (addr&0x3f) | 0x80;
  if(size > 1){
    addr |= 0x40;
  }
  CS_ACTIVE();
  if(errcode=HAL_SPI_Transmit(&hspi, &addr, 1, SPI_TIMEOUT) != HAL_OK){
    goto error;
  }
  if(errcode=HAL_SPI_Receive(&hspi, data, size, SPI_TIMEOUT) != HAL_OK){
    goto error;
  }
 error:
  CS_INACTIVE();
  return errcode;
}

int QMI8610_Init()
{
  LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_GPIOA);
  //LL_GPIO_AF_DisableRemap_SWJ();
  LL_GPIO_SetOutputPin(GPIO_PORT_CS, GPIO_PIN_CS);
  LL_GPIO_SetPinMode(GPIO_PORT_CS, GPIO_PIN_CS, LL_GPIO_MODE_OUTPUT);
  LL_GPIO_SetPinOutputType(GPIO_PORT_CS, GPIO_PIN_CS, LL_GPIO_OUTPUT_PUSHPULL);
  uint8_t tmp = 0;
  if(QMI8610_Read(REG_WHO_AM_I, &tmp, 1) != HAL_OK){
    return 1;
  }
  if(tmp != 0xFC){
    return 2;
  }
  tmp = 0x03;
  QMI8610_Write(REG_CTRL7, &tmp, 1);
  return 0;
}

int QMI8610_ReadData(int16_t data[6])
{
  return QMI8610_Read(REG_AX_L, (int8_t *)data, 12);
}
