
#include "Task_CANSend.h"
#include "Handler.h"


/**
  * @brief  CAN∑¢ÀÕ»ŒŒÒ
  * @param  unused
  * @retval void
  */
void Task_CANSend(void *Parameters)
{
    CanSend_Type    CANSendData;
    
    for(;;)
    {
		xQueueReceive(Queue_CANSend, &CANSendData, portMAX_DELAY);
        
        if(CANSendData.CANx == 1)
        {
            CAN_Transmit(CAN1, &(CANSendData.SendCanTxMsg));
        }
        else
        {
            CAN_Transmit(CAN2, &(CANSendData.SendCanTxMsg));
        }
        
    }
}
