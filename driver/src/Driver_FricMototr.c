
#include "Config.h"
#include "Driver_FricMotor.h"


/**
  * @brief  摩擦轮初始化
  * @param  void
  * @retval void
  */
void FricMotor_InitConfig(void)
{
    //速度初始化为0
    TIM8->CCR1 = 1000;
    TIM8->CCR2 = 1000;
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
        TIM8->CCR1 = 1000 + FRICMOTORWORKINGSPEED;
        TIM8->CCR2 = 1000 + FRICMOTORWORKINGSPEED;
    }
    else
    {
        TIM8->CCR1 = 1000;
        TIM8->CCR2 = 1000;
    }
}








