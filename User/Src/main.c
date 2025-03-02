/*
 * 自行车xx32固件主程序文件
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
#include <stdbool.h>
#include "stm32f1xx_hal.h"
#include "stm32f1xx_ll_gpio.h"
#include "stm32f1xx_ll_bus.h"
#include "stm32f1xx_ll_exti.h"
#include "ch32v203x8.h"
#include "hall.h"
#include "usart.h"
#include "internal_i2c.h"
#include "spi.h"
#include "qmc6308.h"
#include "qmi8610.h"

#define LED1_GPIO_PORT GPIOC
#define LED1_PIN       LL_GPIO_PIN_13

bool ava_ii2c=false, ava_spi=false;
bool ava_mag=false, ava_imu=false;

void main()
{
  SystemCoreClockUpdate();
  HAL_Init();
  NVIC_EnableIRQ(SysTick_IRQn);
  USART_Init();
  USART_SendAutosize("Hello\n");
  Hall_init();

  if(Internal_I2C_Init() == 0){ 
    USART_SendAutosize("Internal i2c init failed\n");
  }else{
    ava_ii2c = true;
    if((ava_mag=QMC6308_Init()) != 0){
      USART_SendAutosize("QMC6308 init failed\n");
    }else{
      ava_mag = true;
    }
  }

  if(SPI_Init() != 0){
    USART_SendAutosize("SPI init failed\n");
  }else{
    ava_spi = true;
    if(QMI8610_Init() != 0){
      USART_SendAutosize("QMI8610 init failed\n");
    }else{
      ava_imu = true;
    }
  }
  while(1){
    struct{
      char head[4];
      QMC6308_Data mag_data;
    }pack_mag = {{'M', 'A', 'G', 'd'}, {0, 0, 0}};
    struct{
      char head[4];
      int16_t data[6];
    }pack_imu = {{'I', 'M', 'U', 'd'}, {0, 0, 0, 0, 0, 0}};
    if(ava_mag){
      QMC6308_ReadData(&pack_mag.mag_data);
      USART_Send((const uint8_t *)&pack_mag, sizeof(pack_mag));
    }
    if(ava_imu){
      QMI8610_ReadData(&pack_imu.data);
      USART_Send((const uint8_t *)&pack_imu, sizeof(pack_imu));
    }
    HAL_Delay(10);
  }
}
