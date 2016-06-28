
#define __DRIVER_POKEMOTOR_GLOBALS

#include "OSinclude.h"
#include "Driver_Control.h"
#include "Driver_PokeMotor.h"


/**
  * @brief  拨弹电机初始化
  * @param  void
  * @retval void
  */
void PokeMotor_InitConfig(void)
{
    PokeMotorParam.RealLocation = 0;
    PokeMotorParam.RealSpeed = 0;
    PokeMotorParam.Status = PokeMotorParam_Working;
    PokeMotorParam.TargetLocation = 0;
    PokeMotorParam.LastShotTick = 0;
    
    //拨弹电机编码器位置初始化
    TIM3->CNT = POKEENCODERCenter;
    
    //拨弹电机方向初始化为顺时针
    GPIO_SetBits(GPIOA, GPIO_Pin_5);
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
    
    //顺时针
    if(Current >= 0)
    {
        GPIO_SetBits(GPIOA, GPIO_Pin_5);
        TIM5->CCR2 = S;
    }
    else
    {
        GPIO_ResetBits(GPIOA, GPIO_Pin_5);
        TIM5->CCR2 = -S;
    }
//#endif
}


/**
  * @brief  拨弹电机步进一步
  * @param  void
  * @retval void
  * @note   拨弹电机处于非停止状态（working,struck）无效,考虑射频
  */
void PokeMotor_Step(void)
{
    portTickType CurrentTick = xTaskGetTickCount();
    
    if((PokeMotorParam.Status == PokeMotorParam_Working) && 
        (CurrentTick - PokeMotorParam.LastShotTick >= POKESTEPMINTIMECRACK) && 
        (PokeMotorParam.RealLocation - PokeMotorParam.TargetLocation >= -30) &&
        (PokeMotorParam.RealLocation - PokeMotorParam.TargetLocation <= 30))
    {
        PokeMotorParam.TargetLocation = PokeMotorParam.RealLocation - POKELINESPERSTEP;
        PokeMotorParam.LastShotTick = CurrentTick;
    }
}


/**
  * @brief  拨弹电机控制函数
  * @param  void
  * @retval void
  * @note   此函数要求周期运行
  */
void PokeMotor_Adjust(void)
{
    int16_t PokeCurrent;
    
    PokeMotorParam.RealSpeed = POKEENCODERCenter - TIM3->CNT;
    TIM3->CNT = POKEENCODERCenter;
    
    //误差小于30线
    if((PokeOPID.CurrentError < 30) && (PokeOPID.CurrentError > -30))
    {
        PokeMotorParam.Status = PokeMotorParam_Working;
    }
    
    //根据积分判断是否卡弹，当积分达到80%上限时认为卡弹,并反转指定角度
    if(((PokeIPID.Iout > PokeIPID.IMax * 0.85F) || (PokeIPID.Iout < -PokeIPID.IMax * 0.85F)) && (PokeMotorParam.Status == PokeMotorParam_Working))
    {
        PokeMotorParam.Status = PokeMotorParam_Stuck;
        
//        PokeMotorParam.TargetLocation -= PokeMotorParam.RealSpeed > 0 ? POKESTRUCKDEALLINES : -POKESTRUCKDEALLINES;
        PokeMotorParam.TargetLocation = PokeMotorParam.RealLocation + POKESTRUCKDEALLINES;
    }
    
    PokeMotorParam.RealLocation += PokeMotorParam.RealSpeed;
    
    PokeCurrent = Control_PokeIPID();
    
    PokeMotorCurrent(PokeCurrent);
}




