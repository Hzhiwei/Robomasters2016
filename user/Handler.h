
#ifndef __HANDLER_H
#define __HANDLER_H


#include "stm32f4xx.h"
#include "OSinclude.h"


#ifdef  __HANDLER_GLOBALS
#define __HANDLER_EXT
#else
#define __HANDLER_EXT   extern
#endif


/************************   队列句柄    ************************/
__HANDLER_EXT   xQueueHandle    Queue_CANSend;                  //CAN发送队列句柄
/************************   任务句柄    ************************/
__HANDLER_EXT   TaskHandle_t    TaskHandle_BellOnce;            //单次蜂鸣器任务句柄
__HANDLER_EXT   TaskHandle_t    TaskHandle_BellCycle;           //循环蜂鸣器句柄






#endif
