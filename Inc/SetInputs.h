#ifndef __SETINPUTS_H
	#define __SETINPUTS_H
/****************Includes***************/
#include <stdint.h>
#include <stdbool.h>
#include "main.h"
#include "Types.h"
#include "LichtController.h"

/****************Defines****************/


/***************Variables***************/
extern Input_t* InputToSet;

/**********Function Prototypes**********/
void SetInputsFunc(void);
void SaveInputConfig(void);

#endif
