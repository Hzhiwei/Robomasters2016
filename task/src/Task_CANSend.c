
#include "Task_CANSend.h"
#include "Handler.h"


/**
  * @brief  CAN发送任务
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
            //CAN故障重启
            if(CAN1->ESR)
            {
                CAN1->MCR |= 0x02;
                CAN1->MCR &= 0xFD;
            }
            
            CAN_Transmit(CAN1, &(CANSendData.SendCanTxMsg));
        }
        else
        {
            //CAN故障重启
            if(CAN2->ESR)
            {
                CAN2->MCR |= 0x02;
                CAN2->MCR &= 0xFD;
            }
            CAN_Transmit(CAN2, &(CANSendData.SendCanTxMsg));
        }
        
    }
}
