/****************Includes***************/
#include "main.h"
#include "LichtController.h"
#include <stdint.h>
#include <stdbool.h>

/****************Defines****************/

/***************Variables***************/
uint8_t InputToSet = 0;

/***************Functions***************/
void SetInputsFunc(void)
{
	if(ButtonModeChanged)
	{
		ButtonModeChanged = false;
		InputToSet = (InputToSet + 1) % 4;
		if(currentInput->Mode == None)
			HAL_GPIO_WritePin(currentInput->ledPort, currentInput->ledPin, GPIO_PIN_RESET);
		else
			HAL_GPIO_WritePin(currentInput->ledPort, currentInput->ledPin, GPIO_PIN_SET);
	
		currentInput =  &Inputs[InputToSet];
	}
	return;
}
