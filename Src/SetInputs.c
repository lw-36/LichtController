/****************Includes***************/
#include "main.h"
#include "LichtController.h"
#include <stdint.h>
#include <stdbool.h>

/****************Defines****************/


/**********Function Prototypes**********/
void setInputRange(Input_t* Input);

/***************Variables***************/
uint8_t InputToSetNumber = 0;
Input_t* InputToSet;
InputSet_t InputSetParam = InputSetNone;

/***************Functions***************/
void SetInputsFunc(void)
{
	switch(InputSetParam)
	{
		case InputSetNone:
			if(ButtonMode.ButtonChanged)
			{
				ButtonMode.ButtonChanged = false;
				InputToSetNumber = (InputToSetNumber + 1) % 4;
				if(InputToSet->Mode == InputNone)
					HAL_GPIO_WritePin(InputToSet->ledPort, InputToSet->ledPin, GPIO_PIN_RESET);
				else
					HAL_GPIO_WritePin(InputToSet->ledPort, InputToSet->ledPin, GPIO_PIN_SET);
				
				InputToSet = &Inputs[InputToSetNumber];
			}
			else if(ButtonSet.ButtonChanged)
			{
				ButtonSet.ButtonChanged = false;
				InputSetParam = InputSetMode;
			}
			break;
		case InputSetMode:
			if(ButtonMode.ButtonChanged)
			{
				ButtonMode.ButtonChanged = false;
				InputToSet->Mode = (InputToSet->Mode + 1) % InputModesAmount;
			}
			else if(ButtonSet.ButtonChanged)
			{
				ButtonSet.ButtonChanged = false;
				HAL_GPIO_WritePin(UserLED_RD_GPIO_Port, UserLED_RD_Pin, GPIO_PIN_SET);
				HAL_GPIO_WritePin(UserLED_BL_GPIO_Port, UserLED_BL_Pin, GPIO_PIN_RESET);
				InputSetParam = InputSetRange;
			}
			break;
		case InputSetRange:	
			setInputRange(InputToSet);
			if(ButtonSet.ButtonChanged)
			{
				ButtonSet.ButtonChanged = false;
				ButtonMode.ButtonChanged = false; //empty buffer
				HAL_GPIO_WritePin(UserLED_GN_GPIO_Port, UserLED_GN_Pin, GPIO_PIN_SET);
				HAL_GPIO_WritePin(UserLED_BL_GPIO_Port, UserLED_BL_Pin, GPIO_PIN_RESET);
				InputSetParam = InputSetNone;
			}
			break;
	}
	return;
}


void setInputRange(Input_t* Input)
{
	if(Input->unscaledValue < Input->minValue + INPUT_TOLERANCE)
		Input->minValue = Input->unscaledValue + INPUT_TOLERANCE;
	if(Input->unscaledValue > Input->maxValue - INPUT_TOLERANCE)
		Input->maxValue = Input->unscaledValue - INPUT_TOLERANCE;
	return;
}

void SaveInputConfig(void)
{
	volatile uint16_t size = sizeof(Inputs);
	volatile uint32_t* rdAddress = (uint32_t*)Inputs;
	volatile uint32_t wrAddress = 0x0800F000;
	HAL_FLASH_Unlock();
	FLASH_EraseInitTypeDef test;
	test.TypeErase = FLASH_TYPEERASE_PAGES;
	test.PageAddress = 0x0800F000;
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
