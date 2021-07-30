/****************Includes***************/
#include "main.h"
#include "LichtController.h"
#include <stdint.h>
#include <stdbool.h>

/****************Defines****************/

/***************Variables***************/
Input_t Input1;
Input_t Input2;
Input_t Input3;
Input_t Input4;

OperationStates_t operationState = Normal;

/***************Functions***************/
void ConfigInputs(void)
{
	extern TIM_HandleTypeDef htim1;
	Input1.Mode = Standard;
	Input1.maxValue = 0;
	Input1.minValue = 65535;
	Input1.Value = 0;
	Input1.LLInput.timer = &htim1;
	Input1.LLInput.channel = TIM_CHANNEL_1;
	Input1.LLInput.risingEdgeNext = false;
}


void ConfigOutputs(void)
{
	
}


void setInputRange(Input_t* Input)
{
	while(HAL_GPIO_ReadPin(UserButton1_GPIO_Port, UserButton1_Pin) == GPIO_PIN_SET)
		HAL_Delay(1);
	HAL_Delay(50);
	do
	{
		if(Input->unscaledValue < Input->minValue + INPUT_TOLERANCE)
			Input->minValue = Input->unscaledValue + INPUT_TOLERANCE;
		if(Input->unscaledValue > Input->maxValue - INPUT_TOLERANCE)
			Input->maxValue = Input->unscaledValue - INPUT_TOLERANCE;
		HAL_Delay(1);
	}while(HAL_GPIO_ReadPin(UserButton1_GPIO_Port, UserButton1_Pin) == GPIO_PIN_RESET);
	return;
}
