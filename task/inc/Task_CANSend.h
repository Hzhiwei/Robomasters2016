
#ifndef __TASN_CANSEND_H
#define __TASN_CANSEND_H


#include "stm32f4xx.h"
#include "OSinclude.h"


typedef struct
{
    uint8_t     CANx;               //CAN±àºÅ     1 CAN1      2 CAN2
    CanTxMsg    SendCanTxMsg;       //·¢ËÍÊý¾Ý
}CanSend_Type;


void Task_CANSend(void *Parameters);


#endif
