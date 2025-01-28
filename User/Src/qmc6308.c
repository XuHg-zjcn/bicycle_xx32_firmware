/*************************************************************************
 *  QMC6308驱动
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
 *************************************************************************/
#include "qmc6308.h"
#include "stm32f1xx_hal.h"
#include "config.h"

extern I2C_HandleTypeDef hi2c_intl;
#define hi2c hi2c_intl
#define I2C_TIMEOUT_QMC6308 10

//register address and names from datasheet
#define QMC6308_I2C_ADDR   (0x2C<<1)

#define REG_CHIPID         0x00
#define REG_DATA_BEGIN     0x01
#define REG_XOUTL          0x01
#define REG_XOUTH          0x02
#define REG_YOUTL          0x03
#define REG_YOUTH          0x04
#define REG_ZOUTL          0x05
#define REG_ZOUTH          0x06
#define REG_STAT           0x09
#define REG_CONF1          0x0A
#define REG_CONF2          0x0B
#define REG_CONF3          0x0D
#define REG_CONF4          0x29

#define CHIPID_VAL         0x80
#define STAT_DRDY          (1U<<0)
#define STAT_OVFL          (1U<<1)
#define STAT_NVM_RDY       (1U<<4)
#define STAT_NVM_LD        (1U<<5)

//use '|' to combile values
#define CONF1_MODE_SUSPEND (0x00<<0)
#define CONF1_MODE_NORMAL  (0x01<<0)
#define CONF1_MODE_SINGLE  (0x02<<0)
#define CONF1_MODE_CONTIN  (0x03<<0)
#define CONF1_ODR_10HZ     (0x00<<2)
#define CONF1_ODR_50HZ     (0x01<<2)
#define CONF1_ODR_100HZ    (0x02<<2)
#define CONF1_ODR_200HZ    (0x03<<2)
#define CONF1_OSR1_8       (0x00<<4)
#define CONF1_OSR1_4       (0x01<<4)
#define CONF1_OSR1_2       (0x02<<4)
#define CONF1_OSR1_1       (0x03<<4)
#define CONF1_OSR2_1       (0x00<<6)
#define CONF1_OSR2_2       (0x01<<6)
#define CONF1_OSR2_4       (0x02<<6)
#define CONF1_OSR2_8       (0x03<<6)

#define CONF2_SRM_SRON     (0x00<<0)
#define CONF2_SRM_SETONLY  (0x01<<0)
#define CONF2_SRM_SROFF    (0x02<<0)
//#define CONF2_SRM_SROFF    (0x03<<0)
#define CONF2_RNG_30GUASS  (0x00<<2)
#define CONF2_RNG_12GUASS  (0x01<<2)
#define CONF2_RNG_8GUASS   (0x02<<2)
#define CONF2_RNG_2GUASS   (0x03<<2)
#define CONF2_SELF_TEST    (1U<<6)
#define CONF2_SOFT_RST     (1U<<7)

#define CONF3_SRC_NORM     (0U<<6)
#define CONF3_SRC_INC      (1U<<6)

#define CONF4_SIGNX_POS    (0U<<0)
#define CONF4_SIGNX_NEG    (1U<<0)
#define CONF4_SIGNY_POS    (0U<<1)
#define CONF4_SIGNY_NEG    (1U<<1)
#define CONF4_SIGNZ_POS    (0U<<2)
#define CONF4_SIGNZ_NEG    (1U<<2)

void QMC6308_ReadData(QMC6308_Data *data)
{
  HAL_I2C_Mem_Read(&hi2c, QMC6308_I2C_ADDR, REG_DATA_BEGIN,
		   1, (uint8_t *)data, 6, I2C_TIMEOUT_QMC6308);
}

void QMC6308_SelfTest()
{
  uint8_t temp[8];
  temp[0] = CONF1_MODE_CONTIN;
  HAL_I2C_Mem_Write(&hi2c, QMC6308_I2C_ADDR, REG_CONF1,
		    1, temp, 1, I2C_TIMEOUT_QMC6308);
}

int QMC6308_Init()
{
  uint8_t temp[8];
  HAL_I2C_Mem_Read(&hi2c, QMC6308_I2C_ADDR, REG_CHIPID,
		   1, temp, 1, I2C_TIMEOUT_QMC6308);
  if(temp[0] != CHIPID_VAL){
    //read chip id failed
    return 1;
  }

  temp[0] = CONF2_SRM_SRON | CONF2_RNG_30GUASS;
  HAL_I2C_Mem_Write(&hi2c, QMC6308_I2C_ADDR, REG_CONF2,
		    1, temp, 1, I2C_TIMEOUT_QMC6308);
  temp[0] = CONF1_MODE_NORMAL | CONF1_ODR_200HZ | CONF1_OSR1_8 | CONF1_OSR2_8;
  HAL_I2C_Mem_Write(&hi2c, QMC6308_I2C_ADDR, REG_CONF1,
		    1, temp, 1, I2C_TIMEOUT_QMC6308);
  return 0;
}

