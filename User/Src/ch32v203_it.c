/*
 * 中断服务程序
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
#include "ch32v203_it.h"
#include "stm32f1xx_ll_exti.h"
#include "stm32f1xx_ll_gpio.h"
#include "stm32f1xx_ll_tim.h"
#include "stm32f1xx_hal.h"
#include "usart.h"
#include "hall.h"

void Default_Handler(void) __attribute__((interrupt));
void EXTI0_IRQHandler(void) __attribute__((interrupt));
void TIM2_IRQHandler(void) __attribute__((interrupt));
void SysTick_Handler(void) __attribute__((interrupt));

void Default_Handler(void)
{
  while(1){
  }
}

void EXTI0_IRQHandler(void)
{
  if(LL_EXTI_IsActiveFlag_0_31(LL_EXTI_LINE_0) != RESET)
  {
    LL_EXTI_ClearFlag_0_31(LL_EXTI_LINE_0);

    LL_GPIO_TogglePin(GPIOC, LL_GPIO_PIN_13);
  }
}

void TIM2_IRQHandler(void)
{
  if(LL_TIM_IsActiveFlag_CC1(TIM2)){
    LL_TIM_ClearFlag_CC1(TIM2);
    uint32_t cap_val = LL_TIM_IC_GetCaptureCH1(TIM2);
    TIM_CaptrueCallback(cap_val);
  }
}

void SysTick_Handler(void)
{
  SysTick->SR = 0;
  HAL_IncTick();
}
