#include "stm32f1xx_hal.h"
#include "stm32f1xx_ll_bus.h"
#include "stm32f1xx_ll_rcc.h"
#include "stm32f1xx_ll_gpio.h"
//#include "stm32f1xx_ll_i2c.h"

#define I2C_SPEEDCLOCK 100000
#define I2C_DUTYCYCLE  I2C_DUTYCYCLE_2
#define I2C_ADDRESS    0x30

I2C_HandleTypeDef hi2c_intl;

int Internal_I2C_Init()
{
  //LL_RCC_ClocksTypeDef rcc_clocks;

  //Config GPIO PB8, PB9
  LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_GPIOB);
  LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_AFIO);
  __HAL_RCC_I2C1_CLK_ENABLE();

  LL_GPIO_SetPinMode(GPIOB, LL_GPIO_PIN_8, LL_GPIO_MODE_ALTERNATE);
  LL_GPIO_SetPinOutputType(GPIOB, LL_GPIO_PIN_8, LL_GPIO_OUTPUT_OPENDRAIN);
  LL_GPIO_SetPinMode(GPIOB, LL_GPIO_PIN_9, LL_GPIO_MODE_ALTERNATE);
  LL_GPIO_SetPinOutputType(GPIOB, LL_GPIO_PIN_9, LL_GPIO_OUTPUT_OPENDRAIN);

  //I2C1 Remap to PB8, PB9
  LL_GPIO_AF_EnableRemap_I2C1();

    /*##-1- Configure the I2C peripheral ######################################*/
  hi2c_intl.Instance              = I2C1;
  hi2c_intl.Init.ClockSpeed       = I2C_SPEEDCLOCK;
  hi2c_intl.Init.DutyCycle        = I2C_DUTYCYCLE;
  hi2c_intl.Init.AddressingMode   = I2C_ADDRESSINGMODE_7BIT;
  hi2c_intl.Init.DualAddressMode  = I2C_DUALADDRESS_DISABLE;
  hi2c_intl.Init.GeneralCallMode  = I2C_GENERALCALL_DISABLE;
  hi2c_intl.Init.NoStretchMode    = I2C_NOSTRETCH_DISABLE;
  hi2c_intl.Init.OwnAddress1      = I2C_ADDRESS;
  hi2c_intl.Init.OwnAddress2      = 0xFF;

  if(HAL_I2C_Init(&hi2c_intl) != HAL_OK)
  {
    /* Initialization Error */
    return 1;
  }
  return 0;
}
