/****************Includes***************/
#include "main.h"
#include "LichtController.h"


/****************Defines****************/

/***************Variables***************/

const uint16_t blinkSlow = 500;
const uint16_t blinkMedium = 250;
const uint16_t blinkFast = 100;


/**********Function Prototypes**********/
void OnOffHandler(Output_t* Output);
void DimmHandler(Output_t* Output);
void BlinkHandler(Output_t* Output);
void FixHandler(Output_t* Output);
void OverrideHandler(Output_t* Output);

/***************Functions***************/
void NormalOperation(void)
{
	if(operationState != StateSetOutputs && Inputs[CONFIGURATION_KILL_INPUT].Value >= 4095/2)
	{
		for(uint8_t i = 0; i < 6; i++)
		{
			if(!Outputs[i].ignoreKillswitch)
			{
				Outputs[i].Override = OutputOROff;
			}
		}
	}
	else if(operationState != StateSetOutputs)
	{
		for(uint8_t i = 0; i < 6; i++)
		{
			Outputs[i].Override = OutputORNone;
		}
	}
		
	//StateNormal
	{
		if(ms_cntr % blinkMedium == 0)
		{
			switch(operationState)
			{
				case StateNormal:
					 //HAL_GPIO_TogglePin(UserLED_BL_GPIO_Port, UserLED_BL_Pin);
					break;
				case StateSetInputs:
					switch(InputSetParam)
					{
						case InputSetNone:
							break;
						case InputSetMode:
							if(ms_cntr < blinkMedium)
							{
								HAL_GPIO_WritePin(UserLED_RD_GPIO_Port, UserLED_RD_Pin, GPIO_PIN_RESET);
								HAL_GPIO_WritePin(UserLED_GN_GPIO_Port, UserLED_GN_Pin, GPIO_PIN_SET);
							}
							else
							{
								HAL_GPIO_WritePin(UserLED_RD_GPIO_Port, UserLED_RD_Pin, GPIO_PIN_SET);
								HAL_GPIO_WritePin(UserLED_GN_GPIO_Port, UserLED_GN_Pin, GPIO_PIN_RESET);
							}
							break;
						case InputSetRange:
							HAL_GPIO_TogglePin(InputToSet->ledPort, InputToSet->ledPin);
							if(ms_cntr < blinkMedium)
							{
								HAL_GPIO_WritePin(UserLED_GN_GPIO_Port, UserLED_GN_Pin, GPIO_PIN_RESET);
								HAL_GPIO_WritePin(UserLED_BL_GPIO_Port, UserLED_BL_Pin, GPIO_PIN_SET);
							}
							else
							{
								HAL_GPIO_WritePin(UserLED_GN_GPIO_Port, UserLED_GN_Pin, GPIO_PIN_SET);
								HAL_GPIO_WritePin(UserLED_BL_GPIO_Port, UserLED_BL_Pin, GPIO_PIN_RESET);
							}
							break;
					}
					break;
				case StateSetOutputs:
					if(OutputSetParam == OutputSetBasicInput)
					{
						if(OutputToSet->ignoreKillswitch == true)
							HAL_GPIO_TogglePin(Inputs[3].ledPort, Inputs[3].ledPin);
					}
					break;
			}
		}
			
		if(ms_cntr % blinkFast == 0)
		{
			if(operationState == StateSetInputs)
			{
				switch(InputSetParam)
				{
					case InputSetNone:
						HAL_GPIO_TogglePin(InputToSet->ledPort, InputToSet->ledPin);
						break;
					case InputSetMode:
						if(ms_cntr <= InputToSet->Mode * blinkFast * 2)
							HAL_GPIO_TogglePin(InputToSet->ledPort, InputToSet->ledPin);
						else
							HAL_GPIO_WritePin(InputToSet->ledPort, InputToSet->ledPin, GPIO_PIN_RESET);
						break;
					case InputSetRange:
						//HAL_GPIO_TogglePin(InputToSet->ledPort, InputToSet->ledPin);
						break;
				}
			}
			else if(operationState == StateSetOutputs)
			{
				if(OutputSetParam != 0 && OutputSetParam <= 3)
				{
					if(ms_cntr <= ((OutputSetParam - 1) * blinkFast * 2))
					{
						HAL_GPIO_WritePin(UserLED_RD_GPIO_Port, UserLED_RD_Pin, GPIO_PIN_SET);
						HAL_GPIO_TogglePin(UserLED_BL_GPIO_Port, UserLED_BL_Pin);
					}
					else
					{
						HAL_GPIO_WritePin(UserLED_RD_GPIO_Port, UserLED_RD_Pin, GPIO_PIN_RESET);
						HAL_GPIO_WritePin(UserLED_BL_GPIO_Port, UserLED_BL_Pin, GPIO_PIN_SET);
					}
				}
				else if(OutputSetParam != 0)
				{
					if(ms_cntr <= ((OutputSetParam - 4) * blinkFast * 2))
					{
						HAL_GPIO_WritePin(UserLED_RD_GPIO_Port, UserLED_RD_Pin, GPIO_PIN_SET);
						HAL_GPIO_TogglePin(UserLED_GN_GPIO_Port, UserLED_GN_Pin);
					}
					else
					{
						HAL_GPIO_WritePin(UserLED_RD_GPIO_Port, UserLED_RD_Pin, GPIO_PIN_RESET);
						HAL_GPIO_WritePin(UserLED_GN_GPIO_Port, UserLED_GN_Pin, GPIO_PIN_SET);
					}
				}
				else
				{
					HAL_GPIO_WritePin(UserLED_RD_GPIO_Port, UserLED_RD_Pin, GPIO_PIN_RESET);
					HAL_GPIO_WritePin(UserLED_GN_GPIO_Port, UserLED_GN_Pin, GPIO_PIN_SET);
					HAL_GPIO_WritePin(UserLED_BL_GPIO_Port, UserLED_BL_Pin, GPIO_PIN_SET);
				}
				if(OutputSetParam == OutputSetBasicInput)
				{
					HAL_GPIO_TogglePin(Inputs[OutputToSet->assignedInput].ledPort, Inputs[OutputToSet->assignedInput].ledPin);
					if(OutputToSet->ignoreKillswitch == false)
						HAL_GPIO_TogglePin(Inputs[3].ledPort, Inputs[3].ledPin);
				}
			}	
			for(uint8_t currentOutputNumber = 0; currentOutputNumber < 6; currentOutputNumber++)
			{
				currentOutput = &Outputs[currentOutputNumber];
				if(currentOutput->Override == OutputORBlinkFast)
				{
					uint16_t currCompare = __HAL_TIM_GET_COMPARE(currentOutput->timer, currentOutput->channel);
					__HAL_TIM_SET_COMPARE(currentOutput->timer, currentOutput->channel, ~currCompare); 
				}
			}
		}
		
	}
	
	for(uint8_t currentOutputNumber = 0; currentOutputNumber < 6; currentOutputNumber++)
	{
		currentOutput = &Outputs[currentOutputNumber];
		currentOutput->cntr = (currentOutput->cntr + 1) % currentOutput->time;
		if(currentOutput->Override == OutputORNone)
		{
			switch(currentOutput->Mode)
			{
				case OutputOnOff:
					OnOffHandler(currentOutput);
					break;
				case OutputDimm:
					DimmHandler(currentOutput);
					break;
				case OutputBlink:
					if((Inputs[currentOutput->assignedInput].Value > currentOutput->lowSwitchingValue && Inputs[currentOutput->assignedInput].Value < currentOutput->highSwitchingValue) ^ currentOutput->Invert)
						BlinkHandler(currentOutput);
					else
						__HAL_TIM_SET_COMPARE(currentOutput->timer, currentOutput->channel, currentOutput->minIntensity);
					break;
				case OutputFix:
					FixHandler(currentOutput);
					break;
			}
		}
		else
		{
			OverrideHandler(currentOutput);
		}
	}
}

void OnOffHandler(Output_t* Output)
{
	bool state;
	if(Inputs[Output->assignedInput].Value > Output->lowSwitchingValue && Inputs[Output->assignedInput].Value < Output->highSwitchingValue && !Output->Invert)
		state = false;
	else
		state = true;
	
	if(state != Output->previousState)
	{
		Output->stateChanged = true;
		Output->previousState = state;
		Output->cntr = 1;
	}
	
	if(Output->stateChanged)
	{
		if(Output->time <= 10)
		{
			if(state == false)
					__HAL_TIM_SET_COMPARE(Output->timer, Output->channel, Output->minIntensity);
			else
					__HAL_TIM_SET_COMPARE(Output->timer, Output->channel, Output->maxIntensity);
		}
		else
		{
			if(Output->cntr % Output->time == 0)
			{
				Output->stateChanged = false;
				return;
			}
			uint16_t value;
			if(state == false)
				value = Output->maxIntensity - ((double)(Output->maxIntensity - Output->minIntensity)/(double)Output->time * (double)Output->cntr);
			else
				value = Output->minIntensity + ((double)(Output->maxIntensity - Output->minIntensity)/(double)Output->time * (double)Output->cntr);
			
			__HAL_TIM_SET_COMPARE(Output->timer, Output->channel, value);
		}
	}	
}

void DimmHandler(Output_t* Output)
{
	uint16_t value = (double)Inputs[Output->assignedInput].Value / (double)INPUT_SCALED_RANGE * (double)(Output->maxIntensity - Output->minIntensity);
	if(Output->Invert)
		value = (Output->maxIntensity - Output->minIntensity) - value;
	value += Output->minIntensity;
	__HAL_TIM_SET_COMPARE(Output->timer, Output->channel, value);
}

void BlinkHandler(Output_t* Output)
{
	static bool state = false;
	switch(Output->SubMode)
	{
		case BlinkStandard:
			if(Output->cntr >= Output->time / 2)
				__HAL_TIM_SetCompare(Output->timer, Output->channel, Output->maxIntensity);
			else
				__HAL_TIM_SetCompare(Output->timer, Output->channel, Output->minIntensity);
			break;
		case BlinkOnce:
			if(Output->cntr == 0)
				__HAL_TIM_SetCompare(Output->timer, Output->channel, Output->maxIntensity);
			else if(Output->cntr == 100)
				__HAL_TIM_SetCompare(Output->timer, Output->channel, Output->minIntensity);
			break;
		case BlinkAntiColl:
			if(Output->cntr == 0)
				__HAL_TIM_SetCompare(Output->timer, Output->channel, Output->maxIntensity);
			else if(Output->cntr == 100)
				__HAL_TIM_SetCompare(Output->timer, Output->channel, Output->minIntensity);
			else if(Output->cntr == 200)
				__HAL_TIM_SetCompare(Output->timer, Output->channel, Output->maxIntensity);
			else if(Output->cntr == 300)
				__HAL_TIM_SetCompare(Output->timer, Output->channel, Output->minIntensity);
			break;
		case BlinkBeacon:
			if(Output->cntr % Output->time == 0)
				state = !state;
			
			uint16_t value;
			if(state == false)
				value = Output->maxIntensity - ((double)(Output->maxIntensity - Output->minIntensity)/(double)Output->time * (double)Output->cntr);
			else
				value = Output->minIntensity + ((double)(Output->maxIntensity - Output->minIntensity)/(double)Output->time * (double)Output->cntr);
			
			__HAL_TIM_SET_COMPARE(Output->timer, Output->channel, value);
			break;
	}
}

void FixHandler(Output_t* Output)
{
	if(!Output->Invert)
		__HAL_TIM_SET_COMPARE(Output->timer, Output->channel, Output->maxIntensity);
	else
		__HAL_TIM_SET_COMPARE(Output->timer, Output->channel, Output->minIntensity);
}

void OverrideHandler(Output_t* Output)
{
	if(Output->Override == OutputOROff)
		__HAL_TIM_SetCompare(Output->timer, Output->channel, 0);
	else if(Output->Override == OutputOROn)
		__HAL_TIM_SetCompare(Output->timer, Output->channel, OUTPUT_RANGE);
	else if(Output->Override == OutputORMin)
		__HAL_TIM_SetCompare(Output->timer, Output->channel, Output->minIntensity);
	else if(Output->Override == OutputORMax)
		__HAL_TIM_SetCompare(Output->timer, Output->channel, Output->maxIntensity);
}
