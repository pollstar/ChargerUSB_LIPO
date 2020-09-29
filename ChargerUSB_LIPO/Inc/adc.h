/**
  ******************************************************************************
  * File Name          : ADC.h
  * Description        : This file provides code for the configuration
  *                      of the ADC instances.
  ******************************************************************************
  *
  ******************************************************************************
  */
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __adc_H
#define __adc_H
#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f0xx_hal.h"
#include "main.h"

/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

extern ADC_HandleTypeDef hadc;

/* USER CODE BEGIN Private defines */
#define GPIO_BAT1_ADC		GPIO_PIN_5
#define GPIO_BAT2_ADC		GPIO_PIN_6
	 
#define ADC_CHANNEL_BAT1		ADC_CHANNEL_5
#define ADC_CHANNEL_BAT2		ADC_CHANNEL_6
	 
/* USER CODE END Private defines */

extern void Error_Handler(void);

void MX_ADC_Init(void);

/* USER CODE BEGIN Prototypes */

/* USER CODE END Prototypes */

#ifdef __cplusplus
}
#endif
#endif /*__ adc_H */

/**
  * @}
  */

/**
  * @}
  */

/************************ (C)   *****END OF FILE****/
