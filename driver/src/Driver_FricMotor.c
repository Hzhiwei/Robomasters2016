
#define __DRIVER_FRICMOTOR_GLOBALS

#include "Config.h"
#include "Handler.h"
#include "Task_CANSend.h"
#include "Driver_Control.h"
#include "Driver_FricMotor.h"


/**
  * @brief  摩擦轮初始化
  * @param  void
  * @retval void
  */
void FricMotor_InitConfig(void)
{ 
#if FRICTYPE == 1
    ArtilleryFricRealSpeed[0] = 0;
    ArtilleryFricRealSpeed[1] = 0;
    ArtilleryFricTargetSpeed = 0;
#else
    //速度初始化为0
    TIM8->CCR1 = 1000;
    TIM8->CCR2 = 1000;
#endif
}


#if FRICTYPE == 1
/**
  * @brief  炮摩擦轮速度调节
  * @param  目标速度
  * @retval void
  */
void FricArtillerySpeed_Adjust(int16_t Target)
{
    int16_t Current[2];
    
    ArtilleryFricTargetSpeed = Target;
    
    Control_FricPID(Current);
    
    FricArtilleryMotorCurrent(Current[0], Current[1]);
}



/**
  * @brief  炮摩擦轮电机电流控制
  * @param  电流
  * @retval void
  */
void FricArtilleryMotorCurrent(int16_t LeftArtilleryCurrent, int16_t RightArtilleryCurrent)
{
    CanSend_Type   SendData;

#if CANPORT == 1
    SendData.CANx = 1;
#else
    SendData.CANx = 2;
#endif
    
    SendData.SendCanTxMsg.DLC   =   8;
    SendData.SendCanTxMsg.IDE   =   CAN_ID_STD;
    SendData.SendCanTxMsg.RTR   =   CAN_RTR_Data;
    SendData.SendCanTxMsg.StdId =   ARTILLERYFRICCONTROLCANID;
    
    SendData.SendCanTxMsg.Data[0] = LeftArtilleryCurrent >> 8;
    SendData.SendCanTxMsg.Data[1] = LeftArtilleryCurrent;
    SendData.SendCanTxMsg.Data[2] = RightArtilleryCurrent >> 8;
    SendData.SendCanTxMsg.Data[3] = RightArtilleryCurrent;
    SendData.SendCanTxMsg.Data[4] = 0;
    SendData.SendCanTxMsg.Data[5] = 0;
    SendData.SendCanTxMsg.Data[6] = 0;
    SendData.SendCanTxMsg.Data[7] = 0;
    
    xQueueSend(Queue_CANSend, &SendData, 10);
}


#else


/**
  * @brief  枪摩擦轮起停
  * @param  0 停止        1 启动        2 狂野模式
  * @retval void
  */
void FricGunControl(uint8_t Control)
{
    if(Control == 1)
    {
        TIM8->CCR1 = 1000 + FRICMOTORWORKINGSPEED;
        TIM8->CCR2 = 1000 + FRICMOTORWORKINGSPEED;
    }
    else if(Control == 2)
    {
        TIM8->CCR1 = 1000 + 1000;
        TIM8->CCR2 = 1000 + 1000;
    }
    else
    {
        TIM8->CCR1 = 1000;
        TIM8->CCR2 = 1000;
    }
}

#endif











