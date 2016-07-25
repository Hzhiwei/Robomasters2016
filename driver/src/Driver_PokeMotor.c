
#define __DRIVER_POKEMOTOR_GLOBALS

#include "OSinclude.h"
#include "Driver_Control.h"
#include "Driver_PokeMotor.h"

//拨弹电机编码器5050/圈

/**
  * @brief  拨弹电机初始化
  * @param  void
  * @retval void
  */
void PokeMotor_InitConfig(void)
{
    PokeMotorParam.RealLocation = 0;
    PokeMotorParam.RealSpeed = 0;
    PokeMotorParam.Status = PokeMotorParam_Suit;
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
    
    if((PokeMotorParam.Status == PokeMotorParam_Suit) && 
        (CurrentTick - PokeMotorParam.LastShotTick >= POKESTEPMINTIMECRACK))
    {
        PokeMotorParam.TargetLocation = PokeMotorParam.RealLocation - POKELINESPERSTEP;
        PokeMotorParam.LastShotTick = CurrentTick;
    }
}


/**
  * @brief  拨弹电机控制函数
  * @param  1 PID控制     0失控
  * @retval void
  * @note   此函数要求周期运行
  */
void PokeMotor_Adjust(uint8_t mode)
{
    int16_t PokeCurrent;
    static portTickType LastStruckCheckTick = 0;
    static long LastCheckLocation = 0;
    static portTickType LastStallDealTick = 0;
    portTickType CurrentTick = xTaskGetTickCount();
    
    if(mode)
    {
        //拨弹电机速度位置计算
        PokeMotorParam.RealSpeed = POKEENCODERCenter - TIM3->CNT;
        TIM3->CNT = POKEENCODERCenter;
        
        //误差小于25线认为达到目标位置,否则处于调节状态
        if((PokeMotorParam.RealLocation > PokeMotorParam.TargetLocation ? (PokeMotorParam.RealLocation - PokeMotorParam.TargetLocation) : (PokeMotorParam.TargetLocation > PokeMotorParam.RealLocation)) < 25)
        {
            PokeMotorParam.Status = PokeMotorParam_Suit;
        }
        else
        {
            PokeMotorParam.Status = PokeMotorParam_Adjusting;
        }
        
        //每20ms检测卡弹
        if(CurrentTick - LastStruckCheckTick >= 200)
        {
            //只有调节状态才有可能卡弹，进行卡弹检测
            if(PokeMotorParam_Adjusting == PokeMotorParam.Status)
            {
                //20ms内运动小于50线认为卡弹
                if((PokeMotorParam.RealLocation > LastCheckLocation ? (PokeMotorParam.RealLocation - LastCheckLocation) : (LastCheckLocation - PokeMotorParam.RealLocation)) < 50)
                {
                    //卡弹反转处理时间间隔为100ms，防止多次反转累加
                    if(CurrentTick - LastStallDealTick > 100)
                    {
                        PokeMotorParam.TargetLocation = PokeMotorParam.RealLocation + PokeMotorParam.TargetLocation > PokeMotorParam.RealLocation ? POKESTRUCKDEALLINES : -POKESTRUCKDEALLINES;
                    }
                    
                    LastStallDealTick = CurrentTick;
                }
            }
            
            LastCheckLocation = PokeMotorParam.RealLocation;
            LastStruckCheckTick = CurrentTick;
        }
        
        
        PokeMotorParam.RealLocation += PokeMotorParam.RealSpeed;
        
        PokeCurrent = Control_PokeIPID();
        
        PokeMotorCurrent(PokeCurrent);
    }
    else
    {
        PokeMotorCurrent(0);
    }
}




