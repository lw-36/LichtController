#ifndef __TYPES_H
	#define __TYPES_H
/****************Includes****************/
#include <stdint.h>
#include <stdbool.h>
#include "main.h"

/****************Typedefs****************/
//Hardware Types

typedef enum
{
	OutputOnOff,
	OutputDimm,
	OutputBlink,
	OutputFix
}OutputModes_t;	

typedef enum
{
	InputNone,
	InputStandard,
	InputKillswitch,
	InputModeswitch,
	InputModesAmount
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

typedef struct
{
	GPIO_PinState ButtonFlag;
	bool ButtonChanged;
	bool ButtonPressedLong;
	uint16_t ButtonPressedCounter;
	GPIO_TypeDef* ButtonPort;
	uint16_t ButtonPin;
}Button_t;

//Software Types
typedef enum
{
	StateNormal,
	StateSetInputs,
	StateSetOutputs
}OperationStates_t;

typedef enum
{
	InputSetNone,
	InputSetMode,
	InputSetRange
}InputSet_t;

#endif
