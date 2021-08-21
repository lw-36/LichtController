#ifndef __TYPES_H
	#define __TYPES_H
/****************Includes****************/
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include "main.h"

/****************Typedefs****************/
//Hardware Types
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
	uint16_t timeoutCntr;
	uint16_t minValueUnscaled;
	uint16_t maxValueUnscaled;
	uint16_t unscaledValue;
	int16_t Value;
	GPIO_TypeDef* ledPort;
	uint16_t ledPin;
	LowLevelInput_t LLInput;
}Input_t;

typedef enum
{
	OutputOnOff,
	OutputDimm,
	OutputBlink,
	OutputFix,
	OutputDisabled
}OutputModes_t;

typedef enum
{
	BlinkStandard,
	BlinkOnce,
	BlinkAntiColl,
	BlinkBeacon
}BlinkPattern_t;

typedef enum
{
	OutputORNone,
	OutputOROn,
	OutputOROff,
	OutputORBlinkFast,
	OutputORBlinkMedium,
	OutputORBlinkSlow,
	OutputORMin,
	OutputORMax
}OutputOverride_t;

typedef struct
{
	OutputModes_t Mode;
	BlinkPattern_t SubMode;
	uint8_t assignedInput;
	uint8_t dimmInput;
	bool Invert;
	int16_t lowSwitchingValue;
	int16_t highSwitchingValue;
	uint16_t minIntensity;
	uint16_t maxIntensity;
	TIM_HandleTypeDef* timer;
	uint32_t channel;
	OutputOverride_t Override;
	uint32_t time;
	uint32_t cntr;
	bool stateChanged;
	bool previousState;
	bool ignoreKillswitch;
	uint16_t startValue;
	uint16_t totalDuration;
}Output_t;

typedef struct
{
	GPIO_PinState ButtonFlag;
	bool ButtonChanged;
	bool ButtonPressed;
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
	OutputSetBasicNone,				// 0
	OutputSetBasicMode,
	OutputSetBasicSubMode,
	OutputSetBasicInput,
	OutputSetAdvIntensity,		// 4
	OutputSetAdvTransition,
	OutputSetAdvInputRange,
	OutputSetAdvPolarity
}OutputSet_t;


#endif
