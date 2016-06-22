
#ifndef __TASK_BELL_H
#define __TASK_BELL_H


#include "stm32f4xx.h"
#include "OSinclude.h"


#ifdef  __TASK_BELL_GLOBALS
#define __TASK_BELL_EXT
#else
#define __TASK_BELL_EXT extern
#endif


//蜂鸣器循环次数
__TASK_BELL_EXT uint16_t BellWarningCount;
//当前蜂鸣器循环编号
__TASK_BELL_EXT uint8_t BellWarningNum;


void Task_BellBark(void *Parameters);


#endif
