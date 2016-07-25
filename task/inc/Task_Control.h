
#ifndef __TASK_CONTROL_H
#define __TASK_CONTROL_H


#include "stm32f4xx.h"
#include "OSinclude.h"


void Task_Control(void *Parameters);
static void Control_RCMode(void);
static void Control_KMMode(void);
static void Control_ProtectMode(void);


#endif
