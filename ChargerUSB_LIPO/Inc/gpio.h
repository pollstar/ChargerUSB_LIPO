/**
  ******************************************************************************
  * File Name          : gpio.h
  * Description        : This file contains all the functions prototypes for 
  *                      the gpio  
  ******************************************************************************
  *
  *
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __gpio_H
#define __gpio_H
#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f0xx_hal.h"
#include "main.h"

/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* USER CODE BEGIN Private defines */
#define GPIO_LEDRED			GPIO_PIN_10
#define GPIO_LEDGREEN1	GPIO_PIN_7
#define GPIO_LEDGREEN2	GPIO_PIN_9
	 
#define GPIO_Q2					GPIO_PIN_0
#define GPIO_Q3					GPIO_PIN_1
#define GPIO_Q4					GPIO_PIN_2
#define GPIO_Q5					GPIO_PIN_3
#define GPIO_Q1					GPIO_PIN_4


void MX_GPIO_Init(void);


#ifdef __cplusplus
}
#endif
#endif /*__ pinoutConfig_H */

/**
  * @}
  */

/**
  * @}
  */

/************************ (C)   *****END OF FILE****/
