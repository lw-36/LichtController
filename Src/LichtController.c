/****************Includes***************/
#include "main.h"
#include "LichtController.h"
#include <stdint.h>
#include <stdbool.h>

/****************Defines****************/


/**********Function Prototypes**********/
void ButtonPressedLongHandler(Button_t* Button);
void LoadInputConfig(void);

/***************Variables***************/
Input_t Inputs[4];
Input_t* Input1 = &Inputs[0];
Input_t* Input2 = &Inputs[1];
Input_t* Input3 = &Inputs[2];
Input_t* Input4 = &Inputs[3];

OperationStates_t operationState = StateNormal;

Button_t ButtonSet;
Button_t ButtonMode;

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
	//Config Inputs
	{
		/*
		//Config Input1
		extern TIM_HandleTypeDef htim1;
		Input1->Mode = InputNone;
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
		Input2->Mode = InputNone;
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
		Input3->Mode = InputNone;
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
		Input4->Mode = InputNone;
		Input4->maxValue = 0;
		Input4->minValue = 65535;
		Input4->Value = 0;
		Input4->LLInput.timer = &htim1;
		Input4->LLInput.channel = TIM_CHANNEL_4;
		Input4->LLInput.risingEdgeNext = false;
		Input4->ledPort = IN4_LED_GPIO_Port;
		Input4->ledPin = IN4_LED_Pin;
		*/
		//Test
		
	LoadInputConfig();
	extern TIM_HandleTypeDef htim1;
	
	for(uint8_t i = 0; i < 4; i++)
	{
		if(Inputs[i].Mode == InputNone)
		{
			Inputs[i].minValue = 65535;
			Inputs[i].maxValue = 0;
		}
		else
			HAL_GPIO_WritePin(Inputs[i].ledPort, Inputs[i].ledPin, GPIO_PIN_SET);
		Inputs[i].Value = 0;
		Inputs[i].LLInput.risingEdgeNext = true;
		Inputs[i].LLInput.timer = &htim1;
		Inputs[i].LLInput.channel = i*4;
	}
	
		
	}
	//Config Buttons
	{
		//Config Set-Button
		ButtonSet.ButtonPort = ButtonSet_GPIO_Port;
		ButtonSet.ButtonPin = ButtonSet_Pin;
		
		//Config Mode-Button
		ButtonMode.ButtonPort = ButtonMode_GPIO_Port;
		ButtonMode.ButtonPin = ButtonMode_Pin;
	}
}


void ConfigOutputs(void)
{
	
}

void ButtonHandler(void)
{
	static uint8_t debounceTimer = 50;
	static bool debouncing = false;
	if(HAL_GPIO_ReadPin(ButtonSet.ButtonPort, ButtonSet.ButtonPin) != ButtonSet.ButtonFlag || HAL_GPIO_ReadPin(ButtonMode.ButtonPort, ButtonMode.ButtonPin) != ButtonMode.ButtonFlag)
		debouncing = true;
	if(debouncing)
	{
		debounceTimer--;
		if(debounceTimer == 0)
		{
			debounceTimer = 50;
			debouncing = false;
			if(HAL_GPIO_ReadPin(ButtonSet.ButtonPort, ButtonSet.ButtonPin) != ButtonSet.ButtonFlag && ButtonSet.ButtonFlag)
				ButtonSet.ButtonChanged = true;
			if(HAL_GPIO_ReadPin(ButtonMode.ButtonPort, ButtonMode.ButtonPin) != ButtonMode.ButtonFlag && ButtonMode.ButtonFlag)
				ButtonMode.ButtonChanged = true;
			
			ButtonSet.ButtonFlag = HAL_GPIO_ReadPin(ButtonSet.ButtonPort, ButtonSet.ButtonPin);
			ButtonMode.ButtonFlag = HAL_GPIO_ReadPin(ButtonMode.ButtonPort, ButtonMode.ButtonPin);
		}
	}
		
	ButtonPressedLongHandler(&ButtonSet);
	ButtonPressedLongHandler(&ButtonMode);

}

void ButtonPressedLongHandler(Button_t* Button)
{
	if(Button->ButtonFlag)
	{
		Button->ButtonPressedCounter++;
		if(Button->ButtonPressedCounter >= BUTTON_PRESSED_LONG_DURATION)
			Button->ButtonPressedLong = true;
	}
	else
		Button->ButtonPressedCounter = 0;
}

void LoadInputConfig(void)
{
	 volatile uint16_t size = sizeof(Inputs)/4;
	 uint32_t* wrAddress = (uint32_t*)Inputs;
	 volatile uint32_t* rdAddress = (uint32_t*)0x0800F000;
	
	uint16_t i = 0;
	while(i<size)
	{
		memcpy(wrAddress + i, (uint32_t*)(0x0800F000 + i*4), 4);
		i++;
	}
}

