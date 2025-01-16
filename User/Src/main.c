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
#include "stm32f1xx_ll_gpio.h"
#include "stm32f1xx_ll_bus.h"
#include "stm32f1xx_ll_exti.h"
#include "ch32v203x8.h"
#include "hall.h"
#include "usart.h"

#define LED1_GPIO_PORT GPIOC
#define LED1_PIN       LL_GPIO_PIN_13


void main()
{
  SystemCoreClockUpdate();
  USART_Init();
  USART_Send("Hello\n", 6);
  Hall_init();
  while(1){
    
  }
}
