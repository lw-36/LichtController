#ifndef __SETOUTPUTS_H
	#define __SETOUTPUTS_H
/****************Includes***************/
#include <stdint.h>
#include <stdbool.h>
#include "main.h"
#include "Types.h"
#include "LichtController.h"

/****************Defines****************/


/***************Variables***************/
extern OutputSet_t OutputSetParam;
extern Output_t* OutputToSet;

/**********Function Prototypes**********/
void SetOutputsFunc(void);
void SaveOutputConfig(void);

#endif