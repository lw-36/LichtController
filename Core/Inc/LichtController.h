#ifndef __LICHTCONTROLLER_H
	#define __LICHTCONTROLLER_H
/****************Includes***************/
#include <stdint.h>
#include <stdbool.h>
#include "main.h"
#include "Types.h"
#include "SetInputs.h"
#include "SetOutputs.h"

/****************Defines****************/


#define INPUT_SCALED_RANGE	4095			//Max 32767
#define INPUT_TOLERANCE			3
#define OUTPUT_RANGE				65535			//Max 65535
#define CONFIGURATION_KILL_INPUT			3			//3 = Input4

#define BUTTON_PRESSED_LONG_DURATION	2000 	//in ms

/***************Variables***************/
extern Input_t Inputs[4];
extern Input_t* Input1;
extern Input_t* Input2;
extern Input_t* Input3;
extern Input_t* Input4;

extern Output_t Outputs[6];
extern Output_t* Output1;
extern Output_t* Output2;
extern Output_t* Output3;
extern Output_t* Output4;
extern Output_t* Output5;
extern Output_t* Output6;
	
extern Button_t ButtonSet;
extern Button_t ButtonMode;
extern OperationStates_t operationState;

extern Input_t* currentInput;
extern Output_t* currentOutput;

extern uint16_t ms_cntr;
extern uint16_t ms_cntr_old;
extern uint16_t s_cntr;
extern uint16_t cntr_10ms;
/**********Function Prototypes**********/
void ConfigInputs(void);
void ConfigOutputs(void);

void ButtonHandler(void);

#endif
