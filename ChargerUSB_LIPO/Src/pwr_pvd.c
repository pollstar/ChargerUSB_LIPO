/**
  ******************************************************************************
  * File Name          : pwr_pvd.c
  * Description        : Main program body
  ******************************************************************************
  *
  ******************************************************************************
  */
/* Includes ------------------------------------------------------------------*/
#include "pwr_pvd.h"
#include "stm32f0xx_hal.h"
/*----------------------------------------------------------------------------*/
/* Configure PVD                                                              */
/*----------------------------------------------------------------------------*/

//#include "stm32f0xx_hal_pwr_ex.h"
//#include "stm32f0xx_hal_pwr.h"

PWR_PVDTypeDef sConfigPVD;

/** 
  * Enable PVD
  */
void MX_PVD_Init(void) 
{
  /*##-1- Enable Power Clock #################################################*/
  __HAL_RCC_PWR_CLK_ENABLE();

  /*##-2- Configure the NVIC for PVD #########################################*/
  HAL_NVIC_SetPriority(PVD_VDDIO2_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(PVD_VDDIO2_IRQn);

  /* Configure the PVD Level to 3 and generate an interrupt on rising and falling
     edges(PVD detection level set to 2.5V, refer to the electrical characteristics
     of you device datasheet for more details) */
  sConfigPVD.PVDLevel = PWR_PVDLEVEL_3;
  sConfigPVD.Mode = PWR_PVD_MODE_IT_RISING_FALLING;
  HAL_PWR_ConfigPVD(&sConfigPVD);

  /* Enable the PVD Output */
  HAL_PWR_EnablePVD();
}
//  HAL_NVIC_SetPriority(DMA1_Channel1_IRQn, 0, 0);
//  HAL_NVIC_EnableIRQ(DMA1_Channel1_IRQn);

}



/****************************END OF FILE****/
