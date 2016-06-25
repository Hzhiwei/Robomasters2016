
#define __DRIVER_STATUSMACHINE_GLOBALS

#include "Config.h"
#include "OSinclude.h"
#include "Driver_DBUS.h"
#include "Driver_StatusMachine.h"


/*
wasd前后左右控制
qe高速自旋
鼠标左键发射（未启动摩擦轮则启动摩擦轮），右键自动射击
ctrl + shift关闭摩擦轮
*/

/**
  * @brief  状态机初始化
  * @param  void
  * @retval void
  */
void StatusMachine_InitConfig(void)
{
    AutoMode = AutoMode_OFF;
    GunStatus = GunStatus_Stop;
    ControlMode = ControlMode_Protect;
}


/**
  * @brief  状态机状态更新
  * @param  void
  * @retval void
  * @note   遥控器左拨码开关1遥控器控制：右1正常，3开摩擦轮，2发射
            左拨码开关3：保护模式，电机全部关闭
            左拨码开关2：键盘控制，右拨码开关3打开摩擦轮，鼠标右键自动射击
            QE自旋
  */
void StatusMachine_Update(void)
{
    //帧率过低停机
    if(DBUSFrameRate < 3)
    {
        ControlMode = ControlMode_Protect;
        
        return;
    }
    
    //遥控器控制
    if(DBUS_ReceiveData.switch_left == 1)
    {
        ControlMode = ControlMode_RC;
        
        //摩擦轮
        if(DBUS_ReceiveData.switch_right == 3)
        {
            GunStatus = GunStatus_Motor;
        }
        else if(DBUS_ReceiveData.switch_right == 2)
        {
            GunStatus = GunStatus_Shot;
        }
        else
        {
            GunStatus = GunStatus_Stop;
        }
        
        //自动射击关闭
        AutoMode = AutoMode_OFF;
    }
    //键鼠控制
    else if(DBUS_ReceiveData.switch_left == 2)
    {
        ControlMode = ControlMode_KM;
    }
    //保护模式
    else
    {
        ControlMode = ControlMode_Protect;
    }
}















