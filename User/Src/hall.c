/*
 * 霍尔传感器驱动程序
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
#include "stm32f1xx_ll_bus.h"
#include "stm32f1xx_ll_gpio.h"
#include "stm32f1xx_ll_tim.h"
#include "usart.h"
#include "config.h"

static uint32_t update_count = 0;
static uint32_t last_captrue = 0;

void Hall_init()
{
  LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_GPIOA);
  LL_GPIO_SetPinMode(GPIO_PORT_HALL_EN, GPIO_PIN_HALL_EN, LL_GPIO_MODE_OUTPUT);
  LL_GPIO_SetPinOutputType(GPIO_PORT_HALL_EN, GPIO_PIN_HALL_EN, LL_GPIO_OUTPUT_PUSHPULL);
  LL_GPIO_ResetOutputPin(GPIO_PORT_HALL_EN, GPIO_PIN_HALL_EN); //外接PMOS，低电平开启

  LL_GPIO_SetPinMode(GPIO_PORT_HALL_SIG, GPIO_PIN_HALL_SIG, LL_GPIO_MODE_INPUT);
  LL_GPIO_SetPinPull(GPIO_PORT_HALL_SIG, GPIO_PIN_HALL_SIG, LL_GPIO_PULL_UP);

  LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_TIM2);
  LL_TIM_SetPrescaler(TIM2, 400);
  LL_TIM_CC_DisableChannel(TIM2, LL_TIM_CHANNEL_CH1);
  LL_TIM_IC_Config(TIM2, LL_TIM_CHANNEL_CH1,
		   LL_TIM_ACTIVEINPUT_DIRECTTI|//直接输入
		   LL_TIM_ICPSC_DIV1|//不分频
		   LL_TIM_IC_FILTER_FDIV1|//关闭滤波
		   LL_TIM_IC_POLARITY_FALLING);//下降沿触发
  LL_TIM_EnableIT_CC1(TIM2); //开捕获中断
  LL_TIM_EnableIT_UPDATE(TIM2); //开定时器溢出中断
  LL_TIM_CC_EnableChannel(TIM2, LL_TIM_CHANNEL_CH1);

  NVIC_EnableIRQ(TIM2_IRQn);
  NVIC_SetPriority(TIM2_IRQn, 0);
  LL_TIM_EnableCounter(TIM2);
}

void Hall_TIM_Callback()
{
  _Bool isUpdate = false;
  struct{
    char magic[4];
    uint32_t delta;
  }pack = {{'H', 'a', 't', 'D'}, 0};
  if(LL_TIM_IsActiveFlag_UPDATE(TIM2)){
    LL_TIM_ClearFlag_UPDATE(TIM2);
    isUpdate = true;
    update_count++;
  }
  if(LL_TIM_IsActiveFlag_CC1(TIM2)){
    LL_TIM_ClearFlag_CC1(TIM2);
    uint32_t cap_val = LL_TIM_IC_GetCaptureCH1(TIM2);
    if(isUpdate && cap_val>=32768 && (((update_count-1)<<16)|cap_val)>last_captrue){
      cap_val |= ((update_count-1)<<16);
    }else{
      cap_val |= (update_count<<16);
    }
    pack.delta = cap_val - last_captrue;
    last_captrue = cap_val;
    USART_Send((uint8_t *)&pack, sizeof(pack));
  }
}
