#ifndef __LICHTCONTROLLER_H
	#define __LICHTCONTROLLER_H
/****************Includes***************/
#include <stdint.h>
#include <stdbool.h>
#include "main.h"
#include "Types.h"
#include "SetInputs.h"

/****************Defines****************/


#define INPUT_SCALED_RANGE	4095			//Max 32767
#define INPUT_TOLERANCE			3
#define OUTPUT_RANGE				65535			//Max 65535

#define BUTTON_PRESSED_LONG_DURATION 2000 //in ms

/***************Variables***************/
extern Input_t* Input1;
extern Input_t* Input2;
extern Input_t* Input3;
extern Input_t* Input4;

extern Input_t Inputs[4];

extern OperationStates_t operationState;

extern Button_t ButtonSet;
extern Button_t ButtonMode;

extern Input_t* currentInput;
/**********Function Prototypes**********/
void ConfigInputs(void);
void ConfigOutputs(void);

void ButtonHandler(void);

#endif
