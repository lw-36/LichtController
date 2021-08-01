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
OutputSet_t OutputSetParam = OutputSetNone;

/***************Functions***************/
void SetOutputsFunc(void)
{
	switch(OutputSetParam)
	{
		case OutputSetNone:
			if(ButtonSet.ButtonChanged)
			{
				ButtonSet.ButtonChanged = false;
				OutputSetParam = OutputSetMode;
			}
			break;
		case OutputSetMode:
			if(ButtonMode.ButtonChanged)
			{
				ButtonMode.ButtonChanged = false;
				OutputToSetNumber = (OutputToSetNumber + 1) % 6;
				OutputToSet = &Outputs[OutputToSetNumber];
			}
			if(ButtonSet.ButtonChanged)
			{
				ButtonSet.ButtonChanged = false;
				OutputSetParam = OutputSetSubMode;
			}
			break;
		case OutputSetSubMode:
			if(ButtonSet.ButtonChanged)
			{
				ButtonSet.ButtonChanged = false;
				OutputSetParam = OutputSetTransition;
			}
			break;
		case OutputSetTransition:
			if(ButtonSet.ButtonChanged)
			{
				ButtonSet.ButtonChanged = false;
				OutputSetParam = OutputSetIntensity;
			}
			break;
		case OutputSetIntensity:
			if(ButtonSet.ButtonChanged)
			{
				ButtonSet.ButtonChanged = false;
				OutputSetParam = OutputSetInput;
			}
			break;
		case OutputSetInput:
			if(ButtonSet.ButtonChanged)
			{
				ButtonSet.ButtonChanged = false;
				OutputSetParam = OutputSetInputRange;
			}
			break;
		case OutputSetInputRange:
			if(ButtonSet.ButtonChanged)
			{
				ButtonSet.ButtonChanged = false;
				OutputSetParam = OutputSetNone;
			}
			break;
	}
}
