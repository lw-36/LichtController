/****************Includes***************/
#include "main.h"
#include "LichtController.h"
#include <stdint.h>
#include <stdbool.h>

/****************Defines****************/


/**********Function Prototypes**********/

/***************Variables***************/
uint8_t OutputToSetNumber = 0;
Output_t* OutputToSet;
OutputSet_t OutputSetParam = OutputSetBasicNone;

/***************Functions***************/
void SetOutputsFunc(void)
{
	static bool delValues = false;
	static bool setTopValue = false;
	switch(OutputSetParam)
	{
		// Basic Settings
		case OutputSetBasicNone:
			if(ButtonMode.ButtonChanged)
			{
				ButtonMode.ButtonChanged = false;
				OutputToSet->Override = OutputORNone;
				__HAL_TIM_SET_COMPARE(OutputToSet->timer, OutputToSet->channel, 0);
				OutputToSetNumber = (OutputToSetNumber + 1) % 6;
				OutputToSet = &Outputs[OutputToSetNumber];
				OutputToSet->Override = OutputORBlinkFast;
			}
			else if(ButtonMode.ButtonPressedLong)
			{
				ButtonMode.ButtonPressedLong = false;
				OutputToSet->Override = OutputORNone;
				OutputSetParam = OutputSetAdvIntensity;
			}
			else if(ButtonSet.ButtonChanged)
			{
				ButtonSet.ButtonChanged = false;
				OutputToSet->Override = OutputORNone;
				OutputSetParam = OutputSetBasicMode;
			}
			break;
		case OutputSetBasicMode:
			if(ButtonMode.ButtonChanged)
			{
				ButtonMode.ButtonChanged = false;
				OutputToSet->Mode = (OutputToSet->Mode + 1) % 5;
			}
			else if(ButtonSet.ButtonChanged)
			{
				ButtonSet.ButtonChanged = false;
				if(OutputToSet->Mode == OutputBlink || OutputToSet->Mode == OutputOnOff)
				{
					OutputSetParam = OutputSetBasicSubMode;
					OutputToSet->time = 0;
				}
				else
				{
					OutputSetParam = OutputSetBasicInput;
					OutputToSet->time = 2000;
				}
			}
			break;
		case OutputSetBasicSubMode:
			if(OutputToSet->Mode == OutputBlink)
			{
				if(ButtonMode.ButtonChanged)
				{
					ButtonMode.ButtonChanged = false;
					OutputToSet->SubMode = (OutputToSet->SubMode + 1) % 4;
				}
			}
			else if(OutputToSet->Mode == OutputOnOff)
			{
				if(ButtonMode.ButtonPressedLong)
				{
					ButtonMode.ButtonPressedLong = false;
					if(OutputToSet->dimmInput != 0)
					{
						OutputToSet->dimmInput = 0;
						OutputToSet->minIntensity = 0;
					}
					else
						OutputToSet->dimmInput = 3;
				}
				else if(ButtonMode.ButtonChanged)
				{
					ButtonMode.ButtonChanged = false;
						OutputToSet->dimmInput = (OutputToSet->dimmInput + 1)% 3;
				}
			}
			if(ButtonSet.ButtonChanged)
			{
				ButtonSet.ButtonChanged = false;
				OutputSetParam = OutputSetBasicInput;
			}
			break;
		case OutputSetBasicInput:
			if(ButtonMode.ButtonChanged)
			{
				ButtonMode.ButtonChanged = false;
				HAL_GPIO_WritePin(Inputs[OutputToSet->assignedInput].ledPort, Inputs[OutputToSet->assignedInput].ledPin, GPIO_PIN_RESET);
				OutputToSet->assignedInput = (OutputToSet->assignedInput + 1) % 3;
			}
			else if(ButtonSet.ButtonChanged)
			{
				ButtonSet.ButtonChanged = false;
				HAL_GPIO_WritePin(Inputs[OutputToSet->assignedInput].ledPort, Inputs[OutputToSet->assignedInput].ledPin, GPIO_PIN_SET);
				if(OutputToSet->Mode == OutputDimm)
				{
					OutputToSet->lowSwitchingValue = 0;
					OutputToSet->highSwitchingValue = 4195;
				}
				else if(OutputToSet->Mode != OutputFix)
				{
					OutputToSet->lowSwitchingValue = 2048;
					OutputToSet->highSwitchingValue = 4195;
				}
				OutputSetParam = OutputSetBasicNone;
			}
			if(ButtonMode.ButtonPressedLong)
			{
				ButtonMode.ButtonPressedLong = false;
				OutputToSet->ignoreKillswitch = !OutputToSet->ignoreKillswitch;
			}
			break;
			
		//Advanced Settings	
		case OutputSetAdvIntensity:
			if(ButtonMode.ButtonPressedLong && !ButtonMode.ButtonChanged)
			{
				OutputToSet->Override = OutputORMax;
			}
			else if(ButtonMode.ButtonChanged && ButtonMode.ButtonPressedLong)
			{
				ButtonMode.ButtonPressedLong = false;
				ButtonMode.ButtonChanged = false;
				delValues = true;
				setTopValue = true;
			}
			else if(delValues && ButtonMode.ButtonChanged)
			{
				ButtonMode.ButtonChanged = false;
				if(setTopValue)
				{
					setTopValue = false;
					OutputToSet->Override = OutputORMin;
				}
				else
				{
					OutputToSet->Override = OutputORNone;
					delValues = false;
					ButtonSet.ButtonChanged = false;
				}
			}
			if(delValues)
				SetOutputIntensityRange(OutputToSet, &Inputs[CONFIGURATION_KILL_INPUT], setTopValue);
			
			if(ButtonSet.ButtonChanged && !delValues)
			{
				ButtonSet.ButtonChanged = false;
				ButtonMode.ButtonChanged = false;
				OutputSetParam = OutputSetAdvTransition;
			}
			break;
		case OutputSetAdvTransition:
			if(ButtonMode.ButtonPressed)
			{
				OutputToSet->time = (double)Inputs[CONFIGURATION_KILL_INPUT].Value / (double)INPUT_SCALED_RANGE * (double)8000;
				//OutputToSet->Fade = !OutputToSet->Fade;
				ButtonMode.ButtonChanged = false;
			}
			else if(ButtonSet.ButtonChanged)
			{
				ButtonSet.ButtonChanged = false;
				OutputSetParam = OutputSetAdvInputRange;
			}
			break;
		case OutputSetAdvInputRange:
			if(ButtonMode.ButtonPressed)
			{
				if(delValues == false)
				{
					delValues = true;
					OutputToSet->lowSwitchingValue = 4195 - 8;
					OutputToSet->highSwitchingValue = 8;
				}
				SetOutputInputRange(OutputToSet, &Inputs[OutputToSet->assignedInput]);
			}
			else
			{
				delValues = false;
				if(ButtonSet.ButtonChanged)
				{
					ButtonSet.ButtonChanged = false;
					ButtonMode.ButtonChanged = false;
					OutputSetParam = OutputSetAdvPolarity;
				}
			}
			break;
		case OutputSetAdvPolarity:
			if(ButtonMode.ButtonChanged)
			{
				ButtonMode.ButtonChanged = false;
				OutputToSet->Invert = !OutputToSet->Invert;
			}
			else if(ButtonSet.ButtonChanged)
			{
				ButtonSet.ButtonChanged = false;
				OutputSetParam = OutputSetBasicNone;
			}
			break;
	}
}

void SetOutputIntensityRange(Output_t* Output, Input_t* Input, bool SetHighValue)
{
	uint16_t currentIntensity = 0;
	if(SetHighValue)
		currentIntensity = Output->minIntensity + ((double)Input->Value / (double)(INPUT_SCALED_RANGE)) * (double)(OUTPUT_RANGE - Output->minIntensity);
	else
		currentIntensity = ((double)Input->Value / (double)(INPUT_SCALED_RANGE)) * (double)Output->maxIntensity;
	if(SetHighValue)
		Output->maxIntensity = currentIntensity;
	else
		Output->minIntensity = currentIntensity;
		
	/*if(currentIntensity > Output->maxIntensity)
	{
		Output->maxIntensity = currentIntensity;
	}
	else if(currentIntensity < Output->minIntensity)
		Output->minIntensity = currentIntensity;*/
}

void SetOutputInputRange(Output_t* Output, Input_t* Input)
{
	int16_t currentValue = Input->Value;
	if(currentValue + 8 > Output->highSwitchingValue)
	{
		Output->highSwitchingValue = currentValue + 8;
	}
	else if(currentValue - 8 < Output->lowSwitchingValue)
	{
		Output->lowSwitchingValue = currentValue - 8;
	}
}

void SaveOutputConfig(void)
{
	volatile uint16_t size = sizeof(Outputs);
	volatile uint32_t* rdAddress = (uint32_t*)Outputs;
	volatile uint32_t wrAddress = 0x0800F400;
	HAL_FLASH_Unlock();
	FLASH_EraseInitTypeDef test;
	test.TypeErase = FLASH_TYPEERASE_PAGES;
	test.PageAddress = 0x0800F400;
	test.NbPages = 1;
	uint32_t out;
	HAL_FLASHEx_Erase(&test, &out);
	
	uint16_t i = 0;
	while(i < size)
	{
		HAL_StatusTypeDef retVal = HAL_FLASH_Program(TYPEPROGRAM_WORD, wrAddress, *rdAddress);
		wrAddress += 4;
		rdAddress += 1;
		i += 4;
	}
	
	HAL_FLASH_Lock();
}
