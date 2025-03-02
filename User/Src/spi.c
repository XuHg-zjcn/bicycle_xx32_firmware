/*
 * SPI驱动程序
 * Copyright (C) 2025  徐瑞骏(科技骏马)
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */
/*
 * Copyright 2016 STMicroelectronics.
 * All rights reserved.
 *
 * code snaps from:
 * repo https://github.com/STMicroelectronics/STM32CubeF1
 * file SPI/SPI_FullDuplex_ComPolling/Src/main.c
 */
#include "stm32f1xx_hal.h"
#define SPIx  SPI1

SPI_HandleTypeDef hspi;

int SPI_Init()
{
  hspi.Instance               = SPIx;
  hspi.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_8;
  hspi.Init.Direction         = SPI_DIRECTION_2LINES;
  hspi.Init.CLKPhase          = SPI_PHASE_2EDGE;
  hspi.Init.CLKPolarity       = SPI_POLARITY_HIGH;
  hspi.Init.DataSize          = SPI_DATASIZE_8BIT;
  hspi.Init.FirstBit          = SPI_FIRSTBIT_MSB;
  hspi.Init.TIMode            = SPI_TIMODE_DISABLE;
  hspi.Init.CRCCalculation    = SPI_CRCCALCULATION_DISABLE;
  hspi.Init.CRCPolynomial     = 7;
  hspi.Init.NSS               = SPI_NSS_SOFT;
  hspi.Init.Mode              = SPI_MODE_MASTER;

  if(HAL_SPI_Init(&hspi) != HAL_OK)
  {
    return 1;
  }
  else
  {
    __HAL_SPI_ENABLE(&hspi);
    return 0;
  }
}
