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
	switch(OutputSetParam)
	{
		// Basic Settings
		case OutputSetBasicNone:
			if(ButtonMode.ButtonChanged)
			{
				ButtonMode.ButtonChanged = false;
				OutputToSet->Override = OutputORNone;
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
				OutputToSet->Mode = (OutputToSet->Mode + 1) % 4;
			}
			else if(ButtonSet.ButtonChanged)
			{
				ButtonSet.ButtonChanged = false;
				if(OutputToSet->Mode == OutputBlink || OutputToSet->Mode == OutputOnOff)
					OutputSetParam = OutputSetBasicSubMode;
				else
					OutputSetParam = OutputSetBasicSubMode;
			}
			break;
		case OutputSetBasicSubMode:
			if(ButtonMode.ButtonChanged)
			{
				ButtonMode.ButtonChanged = false;
				if(OutputToSet->Mode == OutputBlink)
				{
					OutputToSet->SubMode = (OutputToSet->SubMode + 1) % 4;
				}
				else if(OutputToSet->Mode == OutputOnOff)
				{
					OutputToSet->Fade = !OutputToSet->Fade;
				}
			}
			else if(ButtonSet.ButtonChanged)
			{
				ButtonSet.ButtonChanged = false;
				OutputSetParam = OutputSetBasicInput;
			}
			break;
		case OutputSetBasicInput:
			if(ButtonMode.ButtonChanged)
			{
				ButtonMode.ButtonChanged = false;
				OutputToSet->assignedInput = (OutputToSet->assignedInput + 1) % 4;
			}
			else if(ButtonSet.ButtonChanged)
			{
				ButtonSet.ButtonChanged = false;
				OutputSetParam = OutputSetBasicNone;
			}
			break;
			
		//Advanced Settings	
		case OutputSetAdvIntensity:
			if(ButtonMode.ButtonPressed)
			{
				SetOutputIntensityRange(OutputToSet, &Inputs[CONFIGURATION_KILL_INPUT]);
			}
			else if(ButtonSet.ButtonChanged)
			{
				ButtonSet.ButtonChanged = false;
				ButtonMode.ButtonChanged = false;
				OutputSetParam = OutputSetAdvTransition;
			}
			break;
		case OutputSetAdvTransition:
			if(ButtonMode.ButtonChanged)
			{
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
				SetOutputInputRange(OutputToSet, &Inputs[OutputToSet->assignedInput]);
			}
			else if(ButtonSet.ButtonChanged)
			{
				ButtonSet.ButtonChanged = false;
				ButtonMode.ButtonChanged = false;
				OutputSetParam = OutputSetAdvPolarity;
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

void SetOutputIntensityRange(Output_t* Output, Input_t* Input)
{
	uint16_t currentIntensity = ((double)Input->Value / (double)(INPUT_SCALED_RANGE)) * (double)OUTPUT_RANGE;
	if(currentIntensity > Output->maxIntensity)
	{
		Output->maxIntensity = currentIntensity;
	}
	else if(currentIntensity < Output->minIntensity)
		Output->minIntensity = currentIntensity;
}

void SetOutputInputRange(Output_t* Output, Input_t* Input)
{
	int16_t currentValue = Input->Value;
	if(currentValue > Output->highSwitchingValue)
	{
		Output->highSwitchingValue = currentValue;
	}
	else if(currentValue < Output->lowSwitchingValue)
	{
		Output->lowSwitchingValue = currentValue;
	}
}
