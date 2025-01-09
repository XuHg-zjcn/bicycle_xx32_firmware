/*******************************************************************************
 * Default main file for CH32V203
 * Copyright (C) 2023-2024 Xu Ruijun
 *
 * License under BSD 3-clause, see LICENSE.BSD3 file
 ******************************************************************************/
#include "stm32f1xx_ll_gpio.h"
#include "stm32f1xx_ll_bus.h"
#include "stm32f1xx_ll_exti.h"
#include "ch32v203x8.h"

#define LED1_GPIO_PORT GPIOC
#define LED1_PIN       LL_GPIO_PIN_13


void main()
{
  LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_GPIOC);
  LL_GPIO_SetPinMode(LED1_GPIO_PORT, LED1_PIN, LL_GPIO_MODE_OUTPUT);
  LL_GPIO_ResetOutputPin(LED1_GPIO_PORT, LED1_PIN);

  LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_GPIOA);
  LL_GPIO_SetPinMode(GPIOA, LL_GPIO_PIN_0, LL_GPIO_MODE_INPUT);
  LL_GPIO_SetPinPull(GPIOA, LL_GPIO_PIN_0, LL_GPIO_PULL_DOWN);

  LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_AFIO);
  LL_GPIO_AF_SetEXTISource(LL_GPIO_AF_EXTI_PORTA, LL_GPIO_AF_EXTI_LINE0);
  LL_EXTI_EnableIT_0_31(LL_EXTI_LINE_0);
  LL_EXTI_EnableFallingTrig_0_31(LL_EXTI_LINE_0);

  NVIC_EnableIRQ(EXTI0_IRQn);
  NVIC_SetPriority(EXTI0_IRQn, 0);

  while(1){
  }
}
