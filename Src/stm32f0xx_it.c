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
#include <stdbool.h>
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
	if(TIM_FLAG_CC1)
	{
		CurrentInput = Input1;
	}
	
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
		
		CurrentInput->Value = (double)(CurrentInput->unscaledValue - CurrentInput->minValue) / (double)((CurrentInput->maxValue) - (CurrentInput->minValue)) * (double)(INPUT_SCALED_RANGE);
			if(CurrentInput->Value < 0)
				CurrentInput->Value = 0;
			else if(CurrentInput->Value >= INPUT_SCALED_RANGE)
				CurrentInput->Value = INPUT_SCALED_RANGE;
	}
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
	const uint16_t blinkSlow = 500;
	const uint16_t blinkFast = 125;
	static uint16_t ms_cntr = 0;
	static uint8_t debounceTimer = 50;
	static bool debouncing = false;
	static uint16_t ButtonSetPressedCounter = 0;
	static uint16_t ButtonModePressedCounter = 0;
	
	ms_cntr = (ms_cntr + 1) % 1000;
	
	if(ms_cntr % blinkSlow == 0)
	{
		switch(operationState)
		{
			case StateNormal:
				HAL_GPIO_TogglePin(UserLED_RD_GPIO_Port, UserLED_RD_Pin);
				break;
			case StateSetInputs:
				HAL_GPIO_TogglePin(UserLED_BL_GPIO_Port, UserLED_BL_Pin);
				break;
			case StateSetOutputs:
				HAL_GPIO_TogglePin(UserLED_GN_GPIO_Port, UserLED_GN_Pin);
				break;
		}
	}
		
	if(ms_cntr % blinkFast == 0)
	{
		if(operationState == StateSetInputs)
		{
			HAL_GPIO_TogglePin(currentInput->ledPort, currentInput->ledPin);
		}
	}
	
	//Check Buttons
	
	if(HAL_GPIO_ReadPin(ButtonSet_GPIO_Port, ButtonSet_Pin) != ButtonSetFlag || HAL_GPIO_ReadPin(ButtonMode_GPIO_Port, ButtonMode_Pin) != ButtonModeFlag)
			debouncing = true;
	if(debouncing)
		{
			debounceTimer--;
			if(debounceTimer == 0)
			{
				debounceTimer = 50;
				debouncing = false;
				if(HAL_GPIO_ReadPin(ButtonSet_GPIO_Port, ButtonSet_Pin) != ButtonSetFlag && ButtonSetFlag)
					ButtonSetChanged = true;
				if(HAL_GPIO_ReadPin(ButtonMode_GPIO_Port, ButtonMode_Pin) != ButtonModeFlag && ButtonModeFlag)
					ButtonModeChanged = true;
				
				ButtonSetFlag = HAL_GPIO_ReadPin(ButtonSet_GPIO_Port, ButtonSet_Pin);
				ButtonModeFlag = HAL_GPIO_ReadPin(ButtonMode_GPIO_Port, ButtonMode_Pin);
			}
		}
		
		if(ButtonSetFlag)
		{
			ButtonSetPressedCounter++;
			if(ButtonSetPressedCounter >= BUTTON_PRESSED_LONG_DURATION)
				ButtonSetPressedLong = true;
		}
		else
			ButtonSetPressedCounter = 0;
		
		if(ButtonModeFlag)
		{
			ButtonModePressedCounter++;
			if(ButtonModePressedCounter >= BUTTON_PRESSED_LONG_DURATION)
				ButtonModePressedLong = true;
		}
		else
			ButtonModePressedCounter = 0;

		
//	HAL_GPIO_TogglePin(UserLED_BL_GPIO_Port, UserLED_BL_Pin);
//	HAL_GPIO_TogglePin(UserLED_RD_GPIO_Port, UserLED_RD_Pin);
//	HAL_GPIO_TogglePin(UserLED_GN_GPIO_Port, UserLED_GN_Pin);
//	
//	HAL_GPIO_TogglePin(IN1_LED_GPIO_Port, IN1_LED_Pin);
//	HAL_GPIO_TogglePin(IN2_LED_GPIO_Port, IN2_LED_Pin);
//	HAL_GPIO_TogglePin(IN3_LED_GPIO_Port, IN3_LED_Pin);
//	HAL_GPIO_TogglePin(IN4_LED_GPIO_Port, IN4_LED_Pin);
	
  /* USER CODE END TIM6_IRQn 0 */
  HAL_TIM_IRQHandler(&htim6);
  /* USER CODE BEGIN TIM6_IRQn 1 */

  /* USER CODE END TIM6_IRQn 1 */
}

/* USER CODE BEGIN 1 */

/* USER CODE END 1 */
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
