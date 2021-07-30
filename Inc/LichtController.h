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
	TIM_HandleTypeDef* timer;
	uint32_t channel;
}Output_t;

typedef struct
{
	InputModes_t Mode;
	uint16_t minValue;
	uint16_t maxValue;
	uint16_t unscaledValue;
	int16_t Value;
	GPIO_TypeDef* ledPort;
	uint16_t ledPin;
	LowLevelInput_t LLInput;
}Input_t;

typedef enum
{
	StateNormal,
	StateSetInputs,
	StateSetOutputs
}OperationStates_t;

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

extern GPIO_PinState ButtonSetFlag; 
extern GPIO_PinState ButtonModeFlag;

extern bool ButtonSetChanged;
extern bool ButtonModeChanged;

extern bool ButtonSetPressedLong;
extern bool ButtonModePressedLong;

extern Input_t* currentInput;
/**********Function Prototypes**********/
void ConfigInputs(void);
void ConfigOutputs(void);

void setInputRange(Input_t* Input);
