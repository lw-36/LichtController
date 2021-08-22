/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    stm32f0xx_it.c
  * @brief   Interrupt Service Routines.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2021 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "stm32f0xx_it.h"
/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "LichtController.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN TD */

/* USER CODE END TD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
 
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/* External variables --------------------------------------------------------*/
extern TIM_HandleTypeDef htim1;
extern TIM_HandleTypeDef htim6;
/* USER CODE BEGIN EV */

/* USER CODE END EV */

/******************************************************************************/
/*           Cortex-M0 Processor Interruption and Exception Handlers          */ 
/******************************************************************************/
/**
  * @brief This function handles Non maskable interrupt.
  */
void NMI_Handler(void)
{
  /* USER CODE BEGIN NonMaskableInt_IRQn 0 */

  /* USER CODE END NonMaskableInt_IRQn 0 */
  /* USER CODE BEGIN NonMaskableInt_IRQn 1 */

  /* USER CODE END NonMaskableInt_IRQn 1 */
}

/**
  * @brief This function handles Hard fault interrupt.
  */
void HardFault_Handler(void)
{
  /* USER CODE BEGIN HardFault_IRQn 0 */
	__NOP();
  /* USER CODE END HardFault_IRQn 0 */
  while (1)
  {
    /* USER CODE BEGIN W1_HardFault_IRQn 0 */
    /* USER CODE END W1_HardFault_IRQn 0 */
  }
}

/**
  * @brief This function handles System tick timer.
  */
void SysTick_Handler(void)
{
  /* USER CODE BEGIN SysTick_IRQn 0 */

  /* USER CODE END SysTick_IRQn 0 */
  HAL_IncTick();
  /* USER CODE BEGIN SysTick_IRQn 1 */

  /* USER CODE END SysTick_IRQn 1 */
}

/******************************************************************************/
/* STM32F0xx Peripheral Interrupt Handlers                                    */
/* Add here the Interrupt Handlers for the used peripherals.                  */
/* For the available peripheral interrupt handler names,                      */
/* please refer to the startup file (startup_stm32f0xx.s).                    */
/******************************************************************************/

/**
  * @brief This function handles TIM1 break, update, trigger and commutation interrupts.
  */
void TIM1_BRK_UP_TRG_COM_IRQHandler(void)
{
  /* USER CODE BEGIN TIM1_BRK_UP_TRG_COM_IRQn 0 */

  /* USER CODE END TIM1_BRK_UP_TRG_COM_IRQn 0 */
  HAL_TIM_IRQHandler(&htim1);
  /* USER CODE BEGIN TIM1_BRK_UP_TRG_COM_IRQn 1 */

  /* USER CODE END TIM1_BRK_UP_TRG_COM_IRQn 1 */
}

/**
  * @brief This function handles TIM1 capture compare interrupt.
  */
void TIM1_CC_IRQHandler(void)
{
  /* USER CODE BEGIN TIM1_CC_IRQn 0 */
	Input_t* CurrentInput;
	if(TIM1->SR&TIM_FLAG_CC1)
		CurrentInput = Input1;
	else if(TIM1->SR&TIM_FLAG_CC2)
		CurrentInput = Input2;
	else if(TIM1->SR&TIM_FLAG_CC3)
		CurrentInput = Input3;
	else if(TIM1->SR&TIM_FLAG_CC4)
		CurrentInput = Input4;
	else return;
	
	
	if(CurrentInput->LLInput.risingEdgeNext)
	{
		CurrentInput->LLInput.risingEdge = HAL_TIM_ReadCapturedValue(CurrentInput->LLInput.timer, CurrentInput->LLInput.channel);
		__HAL_TIM_SET_CAPTUREPOLARITY(CurrentInput->LLInput.timer, CurrentInput->LLInput.channel, TIM_INPUTCHANNELPOLARITY_FALLING);
		CurrentInput->LLInput.risingEdgeNext = false;
	}
	else
	{
		CurrentInput->LLInput.fallingEdge = HAL_TIM_ReadCapturedValue(CurrentInput->LLInput.timer, CurrentInput->LLInput.channel);
		__HAL_TIM_SET_CAPTUREPOLARITY(CurrentInput->LLInput.timer, CurrentInput->LLInput.channel, TIM_INPUTCHANNELPOLARITY_RISING);
		CurrentInput->LLInput.risingEdgeNext = true;
		
		if(CurrentInput->LLInput.fallingEdge < CurrentInput->LLInput.risingEdge)
			CurrentInput->LLInput.fallingEdge += 0x10000;
		CurrentInput->unscaledValue = CurrentInput->LLInput.fallingEdge - CurrentInput->LLInput.risingEdge;
		
		CurrentInput->Value = (double)(CurrentInput->unscaledValue - CurrentInput->minValueUnscaled) / (double)((CurrentInput->maxValueUnscaled) - (CurrentInput->minValueUnscaled)) * (double)(INPUT_SCALED_RANGE);
			if(CurrentInput->Value < 0)
				CurrentInput->Value = 0;
			else if(CurrentInput->Value >= INPUT_SCALED_RANGE)
				CurrentInput->Value = INPUT_SCALED_RANGE;
	}
	CurrentInput->timeoutCntr = 0;
  /* USER CODE END TIM1_CC_IRQn 0 */
  HAL_TIM_IRQHandler(&htim1);
  /* USER CODE BEGIN TIM1_CC_IRQn 1 */

  /* USER CODE END TIM1_CC_IRQn 1 */
}

/**
  * @brief This function handles TIM6 global interrupt.
  */
void TIM6_IRQHandler(void)
{
  /* USER CODE BEGIN TIM6_IRQn 0 */
	
	ms_cntr = (ms_cntr + 1) % 1000;
	if(cntr_10ms > 0)
		cntr_10ms--;
	if(ms_cntr % 1000 == 0)
		s_cntr = (s_cntr + 1) % 10;
	
	for(uint8_t i = 0; i < 4; i++)
	{
		if(Inputs[i].timeoutCntr >= 100)
			HAL_GPIO_WritePin(Inputs[i].ledPort, Inputs[i].ledPin, GPIO_PIN_RESET);
		else
		{
			Inputs[i].timeoutCntr++;
			HAL_GPIO_WritePin(Inputs[i].ledPort, Inputs[i].ledPin, GPIO_PIN_SET);
		}
	}
	
	//Check Buttons
	ButtonHandler();	
  /* USER CODE END TIM6_IRQn 0 */
  HAL_TIM_IRQHandler(&htim6);
  /* USER CODE BEGIN TIM6_IRQn 1 */

  /* USER CODE END TIM6_IRQn 1 */
}

/* USER CODE BEGIN 1 */

/* USER CODE END 1 */
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
