
#define __MOTOR_GLOBALS

#include "Task_CANSend.h"
#include "Driver_Motor.h"
#include "Driver_Control.h"

#include "Handler.h"


/*******************
Yaw：
	左极限：4000
	右极限：2100
	中心线：3100
    电流负：顺时针转动（右转）
    电流正：逆时针转动（左转）
    角速度：Z轴，左负右正
Pitch：
	上极限：7190
	下极限：7890
	水平位：7600
    电流正：抬头
    电流负：低头
    角速度：X轴，抬头负，低头正
*******************/



/**
  * @brief  电机初始化
  * @param  void
  * @retval void
  */
void Motor_InitConfig(void)
{
    PitchMotorAngle = PitchCenter;
    YawMotorAngle   = YawCenter;
    
    TIM8->CCR1 = 1000;
    TIM8->CCR2 = 1000;
    
    //拨弹电机方向，应保持不变
    GPIO_ResetBits(GPIOA, GPIO_Pin_5);
    //拨弹电机编码器位置初始化
    TIM3->CNT = ENCODERCenter;
    //拨弹电机速度初始化
    PokeSpeed = 0;
    //舵机角度初始化
    SteeringAngle(0);
}
    
    
    
/**
  * @brief  云台电机电流控制
  * @param  pitch轴电流，yaw轴电流
  * @retval void
  */
void CloudMotorCurrent(int16_t Pitch, int16_t Yaw)
{
    static CanSend_Type   SendData;
    
    SendData.CANx = 2;
    
    SendData.SendCanTxMsg.DLC   =   8;
    SendData.SendCanTxMsg.IDE   =   CAN_ID_STD;
    SendData.SendCanTxMsg.RTR   =   CAN_RTR_Data;
    SendData.SendCanTxMsg.StdId =   COULDCONTROLID;
    
    SendData.SendCanTxMsg.Data[0] = Yaw >> 8;
    SendData.SendCanTxMsg.Data[1] = Yaw;
    SendData.SendCanTxMsg.Data[2] = Pitch >> 8;
    SendData.SendCanTxMsg.Data[3] = Pitch;
    SendData.SendCanTxMsg.Data[4] = 0;
    SendData.SendCanTxMsg.Data[5] = 0;
    SendData.SendCanTxMsg.Data[6] = 0;
    SendData.SendCanTxMsg.Data[7] = 0;
    
    xQueueSend(Queue_CANSend, &SendData, 10);
}


/**
  * @brief  拨弹电机电流控制
  * @param  电流大小（±399）
  * @retval void
  */
void PokeMotorCurrent(int16_t Current)
{
    int16_t S;
    
    S = Current > 399 ? 399 : Current;
    S = Current < -399 ? -399 : Current;
    
    if(Current >= 0)
    {
        GPIO_ResetBits(GPIOA, GPIO_Pin_5);
        TIM5->CCR2 = S;
    }
    else
    {
        GPIO_SetBits(GPIOA, GPIO_Pin_5);
        TIM5->CCR2 = -S;
    }
}


/**
  * @brief  枪摩擦轮起停
  * @param  0 停止        1 启动
  * @retval void
  */
void GunFric_Control(uint8_t Control)
{
    if(Control)
    {
        TIM8->CCR1 = 1000 + GUNWORKSPEED;
        TIM8->CCR2 = 1000 + GUNWORKSPEED;
    }
    else
    {
        TIM8->CCR1 = 1000;
        TIM8->CCR2 = 1000;
    }
}


/**
  * @brief  拨弹电机起停
  * @param  0 停止        1 打开
  * @retval void
  */
void Poke_Control(uint8_t status)
{
    if(status)
    {
        //摩擦轮正常旋转才可发射
        if(TIM8->CCR1 > 50)
        {
            PokeSpeedTarget = POKEWORKSPEED;
        }
    }
    else
    {
        PokeSpeedTarget = 0;
    }
}



/**
  * @brief  舵机角度控制
  * @param  角度
  * @retval void
  */
void SteeringAngle(uint16_t angle)
{
    
}


