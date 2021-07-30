/****************Includes***************/
#include <stdint.h>
#include <stdbool.h>
#include "main.h"

/****************Defines****************/
typedef enum
{
	OnOff,
	Dimm,
	Blink,
	Fix
}OutputModes_t;	

typedef enum
{
	None,
	Standard,
	Killswitch,
	Modeswitch
}InputModes_t;

typedef struct
{
	uint32_t risingEdge;
	uint32_t fallingEdge;
	bool risingEdgeNext;
	TIM_HandleTypeDef* timer;
	uint32_t channel;
}LowLevelInput_t;

typedef struct
{
	OutputModes_t Mode;
	uint8_t assignedInput;
	bool Invert;
	uint16_t switchingValue;
	uint16_t minIntensity;
	uint16_t maxIntensity;
}Output_t;

typedef struct
{
	InputModes_t Mode;
	uint16_t minValue;
	uint16_t maxValue;
	uint16_t unscaledValue;
	int16_t Value;
	LowLevelInput_t LLInput;
}Input_t;

typedef enum
{
	Normal,
	Setinputs,
	SetOutputs
}OperationStates_t;

#define INPUT_SCALED_RANGE	4095			//Max 32767
#define INPUT_TOLERANCE			3
#define OUTPUT_RANGE				65535			//Max 65535

/***************Variables***************/
extern Input_t Input1;
extern Input_t Input2;
extern Input_t Input3;
extern Input_t Input4;

/**********Function Prototypes**********/
void ConfigInputs(void);
void ConfigOutputs(void);

void setInputRange(Input_t* Input);
