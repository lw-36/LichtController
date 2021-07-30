/****************Includes***************/
#include "main.h"
#include "LichtController.h"
#include <stdint.h>
#include <stdbool.h>

/****************Defines****************/

/***************Variables***************/
Input_t Inputs[4];
Input_t* Input1 = &Inputs[0];
Input_t* Input2 = &Inputs[1];
Input_t* Input3 = &Inputs[2];
Input_t* Input4 = &Inputs[3];

OperationStates_t operationState = StateNormal;

GPIO_PinState ButtonSetFlag = GPIO_PIN_RESET;
GPIO_PinState ButtonModeFlag = GPIO_PIN_RESET;

bool ButtonSetChanged = false;
bool ButtonModeChanged = false;

bool ButtonSetPressedLong = false;
bool ButtonModePressedLong = false;

Input_t* currentInput;

/***************Functions***************/
void ConfigInputs(void)
{
	//Config Input1
	extern TIM_HandleTypeDef htim1;
	Input1->Mode = None;
	Input1->maxValue = 0;
	Input1->minValue = 65535;
	Input1->Value = 0;
	Input1->LLInput.timer = &htim1;
	Input1->LLInput.channel = TIM_CHANNEL_1;
	Input1->LLInput.risingEdgeNext = false;
	Input1->ledPort = IN1_LED_GPIO_Port;
	Input1->ledPin = IN1_LED_Pin;
	
	//Config Input2
	extern TIM_HandleTypeDef htim1;
	Input2->Mode = None;
	Input2->maxValue = 0;
	Input2->minValue = 65535;
	Input2->Value = 0;
	Input2->LLInput.timer = &htim1;
	Input2->LLInput.channel = TIM_CHANNEL_2;
	Input2->LLInput.risingEdgeNext = false;
	Input2->ledPort = IN2_LED_GPIO_Port;
	Input2->ledPin = IN2_LED_Pin;
	
	//Config Input3
	extern TIM_HandleTypeDef htim1;
	Input3->Mode = None;
	Input3->maxValue = 0;
	Input3->minValue = 65535;
	Input3->Value = 0;
	Input3->LLInput.timer = &htim1;
	Input3->LLInput.channel = TIM_CHANNEL_3;
	Input3->LLInput.risingEdgeNext = false;
	Input3->ledPort = IN3_LED_GPIO_Port;
	Input3->ledPin = IN3_LED_Pin;
	
	//Config Input4
	extern TIM_HandleTypeDef htim1;
	Input4->Mode = None;
	Input4->maxValue = 0;
	Input4->minValue = 65535;
	Input4->Value = 0;
	Input4->LLInput.timer = &htim1;
	Input4->LLInput.channel = TIM_CHANNEL_4;
	Input4->LLInput.risingEdgeNext = false;
	Input4->ledPort = IN4_LED_GPIO_Port;
	Input4->ledPin = IN4_LED_Pin;
	
	currentInput = Input1;
}


void ConfigOutputs(void)
{
	
}

void setInputRange(Input_t* Input)
{
	do
	{
		if(Input->unscaledValue < Input->minValue + INPUT_TOLERANCE)
			Input->minValue = Input->unscaledValue + INPUT_TOLERANCE;
		if(Input->unscaledValue > Input->maxValue - INPUT_TOLERANCE)
			Input->maxValue = Input->unscaledValue - INPUT_TOLERANCE;
		HAL_Delay(1);
	}while(HAL_GPIO_ReadPin(ButtonSet_GPIO_Port, ButtonSet_Pin) == GPIO_PIN_RESET);
	return;
}
