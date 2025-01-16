/*************************************************************************
 *  串口驱动程序
 *  Copyright (C) 2023-2024  Xu Ruijun
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
#include "usart.h"
#include "stm32f1xx_ll_bus.h"
#include "stm32f1xx_ll_gpio.h"
#include "stm32f1xx_ll_usart.h"
#include "stm32f1xx_ll_dma.h"
#include "config.h"

void USART_Init()
{
  LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_GPIOA);
  LL_GPIO_SetPinMode(GPIO_PORT_USART_TX, LL_GPIO_PIN_USART_TX, LL_GPIO_MODE_ALTERNATE);
  LL_GPIO_SetPinSpeed(GPIO_PORT_USART_TX, LL_GPIO_PIN_USART_TX, LL_GPIO_SPEED_FREQ_MEDIUM);
  LL_GPIO_SetPinOutputType(GPIO_PORT_USART_TX, LL_GPIO_PIN_USART_TX, LL_GPIO_OUTPUT_PUSHPULL);

  LL_GPIO_SetPinMode(GPIO_PORT_USART_RX, LL_GPIO_PIN_USART_RX, LL_GPIO_MODE_INPUT);
  LL_GPIO_SetPinPull(GPIO_PORT_USART_RX, LL_GPIO_PIN_USART_RX, LL_GPIO_PULL_UP);

  LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_USART1);
  LL_USART_SetTransferDirection(USARTx_ESP, LL_USART_DIRECTION_TX_RX);
  LL_USART_ConfigCharacter(USARTx_ESP, LL_USART_DATAWIDTH_8B, LL_USART_PARITY_NONE, LL_USART_STOPBITS_1);
  LL_USART_SetBaudRate(USARTx_ESP, SystemCoreClock, USART_BAUD);
  LL_USART_Enable(USARTx_ESP);
}

void USART_Send(const uint8_t *data, uint32_t length)
{
  while(length--){
    while(!LL_USART_IsActiveFlag_TXE(USARTx_ESP));
    LL_USART_TransmitData8(USARTx_ESP, *data++);
  }
}
