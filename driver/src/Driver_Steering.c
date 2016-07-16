
#include "Driver_Steering.h"



/**
  * @brief  舵机初始化
  * @param  void
  * @retval void
  */
void Steering_InitConfig(void)
{
    TIM8->CCR4 = 0;      //关闭舱门
}


/**
  * @brief  舵机角度控制
  * @param  0 关闭舱门          1 打开舱门      2失控
  * @retval void 
  */
void Steering_Control(uint8_t status)
{
    if(status == 1)
    {
        TIM8->CCR4 = 800;
    }
    else if(status == 0)
    {
        TIM8->CCR4 = 2200;
    }
    else if(status == 2)
    {
        TIM8->CCR4 = 0;
    }
}








