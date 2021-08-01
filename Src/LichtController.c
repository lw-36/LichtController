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

Output_t Outputs[6];
Output_t* Output1 = &Outputs[0];
Output_t* Output2 = &Outputs[1];
Output_t* Output3 = &Outputs[2];
Output_t* Output4 = &Outputs[3];
Output_t* Output5 = &Outputs[4];
Output_t* Output6 = &Outputs[5];

OperationStates_t operationState = StateNormal;

Button_t ButtonSet;
Button_t ButtonMode;

Input_t* currentInput;
Output_t* currentOutput;

uint16_t ms_cntr = 0;
uint16_t cntr_10ms = 10;

/***************Functions***************/
void ConfigInputs(void)
{
	//Config PWM-Inputs
	{		
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

