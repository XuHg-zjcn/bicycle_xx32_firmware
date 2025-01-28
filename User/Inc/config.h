/*
 * 配置文件
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
#ifndef CONFIG_H
#define CONFIG_H

#include "stm32f1xx_ll_gpio.h"

#define GPIO_PORT_HALL_SIG  GPIOA
#define GPIO_PIN_HALL_SIG   LL_GPIO_PIN_0

#define GPIO_PORT_HALL_EN   GPIOA
#define GPIO_PIN_HALL_EN    LL_GPIO_PIN_7

#define USART_ESP_EN 1
#define USARTx_ESP             USART1
#define USARTx_ESP_IRQn        USART1_IRQn
#define GPIO_PORT_USART_TX     GPIOA
#define LL_GPIO_PIN_USART_TX   LL_GPIO_PIN_9
#define GPIO_PORT_USART_RX     GPIOA
#define LL_GPIO_PIN_USART_RX   LL_GPIO_PIN_10
#define USART_BAUD             115200

#define I2C_INTL               I2C1
#define I2C_EXTL               I2C2

#endif
