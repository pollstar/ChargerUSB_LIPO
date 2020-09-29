/**
  ******************************************************************************
  * File Name          : main.c
  * Description        : Main program body
  ******************************************************************************
  *
  ******************************************************************************
  */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "stm32f0xx_hal.h"
#include "adc.h"
#include "gpio.h"
#include "stdbool.h"
#include "dma.h"

#include "filter.h"

/* Macro definitions for code readability ---------------------------------------------------------*/

//All the GPIO are configured in the gpio.c file

// Switching on / off the transistor of the upper arm Q2,Q3
// The outputs are configured as an open drain, so the switching is done by pulling the GATE output of the transistors to
// ground potential
#define Q2_ON() 		  HAL_GPIO_WritePin(GPIOA, GPIO_Q2, GPIO_PIN_RESET)
#define Q3_ON() 		  HAL_GPIO_WritePin(GPIOA, GPIO_Q3, GPIO_PIN_RESET)
#define Q2_OFF() 		  HAL_GPIO_WritePin(GPIOA, GPIO_Q2, GPIO_PIN_SET)
#define Q3_OFF() 		  HAL_GPIO_WritePin(GPIOA, GPIO_Q3, GPIO_PIN_SET)

// On / off transistors of the lower arm Q4,Q5
//The GPIOs are configured as push / pull, so switching is done by feeding GATE transistors with the required potential
#define Q4_ON() 		  HAL_GPIO_WritePin(GPIOA, GPIO_Q4, GPIO_PIN_SET)
#define Q5_ON() 		  HAL_GPIO_WritePin(GPIOA, GPIO_Q5, GPIO_PIN_SET)
#define Q4_OFF() 		  HAL_GPIO_WritePin(GPIOA, GPIO_Q4, GPIO_PIN_RESET)
#define Q5_OFF() 		  HAL_GPIO_WritePin(GPIOA, GPIO_Q5, GPIO_PIN_RESET)

// Switching on / off transistor Q1, for connecting the battery to the zero potential of the circuit
//The GPIOs are configured as push / pull, so switching is done by feeding GATE transistors 
//with the required potential
#define Q1_ON() 		  HAL_GPIO_WritePin(GPIOA, GPIO_Q1, GPIO_PIN_SET)
#define Q1_OFF() 		  HAL_GPIO_WritePin(GPIOA, GPIO_Q1, GPIO_PIN_RESET)

// On / off LEDs for indication
#define LED1_GREEN_ON()			HAL_GPIO_WritePin(GPIOA, GPIO_LEDGREEN1, GPIO_PIN_SET)
#define LED2_GREEN_ON()			HAL_GPIO_WritePin(GPIOA, GPIO_LEDGREEN2, GPIO_PIN_SET)
#define LED1_GREEN_OFF()		HAL_GPIO_WritePin(GPIOA, GPIO_LEDGREEN1, GPIO_PIN_RESET)
#define LED2_GREEN_OFF()		HAL_GPIO_WritePin(GPIOA, GPIO_LEDGREEN2, GPIO_PIN_RESET)
 
#define LED_RED_ON()				HAL_GPIO_WritePin(GPIOA, GPIO_LEDRED, GPIO_PIN_SET)
#define LED_RED_OFF()				HAL_GPIO_WritePin(GPIOA, GPIO_LEDRED, GPIO_PIN_RESET)

// ADC charge limits
// 0x0A00  -  4,2ВV
#define BAT1_MAX			0x09FF
#define BAT2_MAX			BAT1_MAX/2+0x50
#define BAT1_MIN			0x100
#define BAT2_MIN			0x100


/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
void Error_Handler(void);

/* Private function prototypes -----------------------------------------------*/
uint16_t BatReadADC(uint32_t adcBatChannel);
static uint16_t aADCxConvertedData[2]={0,0};
static HAL_StatusTypeDef status;
int main(void)
{
  /* MCU Configuration----------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* Configure the system clock */
  SystemClock_Config();

	HAL_Delay(1000);  //Delay to enter operating mode
  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_DMA_Init();
  MX_ADC_Init();

	Q2_OFF();
	Q3_OFF();
	Q4_OFF();
	Q5_OFF();
	Q1_OFF();
	
  static  uint16_t	bat1Power, bat2Power;			//Measured battery voltage after filter treatment
	tFilter bat1Filter, bat2Filter;							//Filters
	
	bat1Filter.numsValue = 0;										//Zero the accumulation filters
	bat2Filter.numsValue = 0;										//
	//ADC calibration
  if (HAL_ADCEx_Calibration_Start(&hadc) != HAL_OK)
  {
    Error_Handler();
  }

  while (1)
  {
/* 
* We measure the stresses on the banks
*/
		Q2_OFF(); //Closes all the transistors, to measure the voltage
		Q3_OFF(); // 
		Q4_OFF();
		Q5_OFF();

		Q1_ON();  // Open the transistor to connect the battery with zero potential circuit
		
		HAL_Delay(100);  //Delay for stabilization of transients and charge of capacitors in the resistive divider

		uint16_t cnt=0;
		do // Start ADC for measuring two channels
    {
			status = HAL_ADC_Start_DMA(&hadc, (uint32_t *)aADCxConvertedData, 2);
			if(cnt++ > 100)
			{
				Error_Handler();
			}
		}while(status!=HAL_OK);
		
 		Q1_OFF(); // To disconnect the battery from the zero potential of the circuit
	
		//We process the values of the ADC through a filter
		bat1Power = (uint16_t)GetFilterNumDelta(&bat1Filter, aADCxConvertedData[0]/* & 0xFFFC*/, 0x50);
		bat2Power = (uint16_t)GetFilterNumDelta(&bat2Filter, aADCxConvertedData[1]/* & 0xFFFC*/, 0x50);
		
/*
*	Charge cycle
*/
		if((bat1Power < BAT1_MAX)||(bat2Power < BAT2_MAX))
		{ //If at least one bank is not charged, start charging
			{
				if((bat1Power > BAT1_MIN)&&(bat2Power > BAT2_MIN)) //Check for connected battery or recharge
				{
					if((bat1Power < BAT1_MAX)) //check the first bank and start charge if necessary
					{
						LED_RED_ON();
						LED1_GREEN_OFF();
						Q2_ON();
						Q4_ON();
						HAL_Delay(400);
						LED1_GREEN_ON();
						Q2_OFF();
						Q4_OFF();
					}
					else	//If charged, just turn on the display
					{
						LED1_GREEN_ON();
					}

					if((bat2Power < BAT2_MAX))//a second bank check and start charge if necessary
					{
						LED_RED_ON();
						LED2_GREEN_OFF();
						Q3_ON();
						Q5_ON();
						HAL_Delay(400);
						LED2_GREEN_ON();
						Q3_OFF();
						Q5_OFF();
					}
					else
					{
						LED2_GREEN_ON();
					}
				}
				else //If the battery is disconnected or redrawn - we signal this and try to charge both banks
				{
					LED_RED_ON();
					LED1_GREEN_ON();
					LED2_GREEN_ON();
					Q2_ON();
					Q4_ON();
					HAL_Delay(300);
					Q2_OFF();
					Q4_OFF();

					HAL_Delay(10);

					LED_RED_OFF();
					LED1_GREEN_OFF();
					LED2_GREEN_OFF();
					Q3_ON();
					Q5_ON();
					HAL_Delay(300);
					Q3_OFF();
					Q5_OFF();

					HAL_Delay(10);
				}
			}
		}
		else //If everything is charged, we turn on the two green LEDs and turn off the red
		{
			LED1_GREEN_ON();
			LED2_GREEN_ON();
			LED_RED_OFF();
		}
	}
}


/** System Clock Configuration
*/
void SystemClock_Config(void)
{

  RCC_OscInitTypeDef RCC_OscInitStruct;
  RCC_ClkInitTypeDef RCC_ClkInitStruct;

    /**Initializes the CPU, AHB and APB busses clocks 
    */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI|RCC_OSCILLATORTYPE_HSI14;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSI14State = RCC_HSI14_ON;
  RCC_OscInitStruct.HSICalibrationValue = 16;
  RCC_OscInitStruct.HSI14CalibrationValue = 16;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

    /**Initializes the CPU, AHB and APB busses clocks 
    */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler();
  }

    /**Configure the Systick interrupt time 
    */
  HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq()/1000);

    /**Configure the Systick 
    */
  HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);

  /* SysTick_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(SysTick_IRQn, 0, 0);
}


/**
  * @brief  This function is executed in case of error occurrence.
  * @param  None
  * @retval None
	* Обработчик системных ошибок, типа сбой инициализации перефирии, чтения АЦП и пр.
  */
void Error_Handler(void)
{
	LED1_GREEN_OFF();
	LED2_GREEN_OFF();
	Q2_OFF();
	Q3_OFF();
	Q4_OFF();
	Q5_OFF();
	Q1_OFF();
	
  while(1) 
  {
		HAL_GPIO_TogglePin(GPIOA, GPIO_LEDRED);
		HAL_Delay(300);
  }
}


#ifdef USE_FULL_ASSERT

/**
   * @brief Reports the name of the source file and the source line number
   * where the assert_param error has occurred.
   * @param file: pointer to the source file name
   * @param line: assert_param error line source number
   * @retval None
   */
void assert_failed(uint8_t* file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
    ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */

}

#endif

/**
  * @}
  */ 

/**
  * @}
*/ 

/****************************END OF FILE****/
