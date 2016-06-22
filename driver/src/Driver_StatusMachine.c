
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
    CloudStatus = CloudStatus_Normal;
    GunStatus = GunStatus_Stop;
    ControlStatus = ControlStatus_KM;
}


/**
  * @brief  状态机状态更新
  * @param  void
  * @retval void
  * @note   枪炮电机同步控制
  */
void StatusMachine_Update(void)
{
    static uint8_t GunFricWorking = 0;
    
    
#if BEBUG_MODE
    //帧率过低停机
    if(DBUSFrameRate < 3)
    {
        ControlStatus = ControlStatus_Protect;
        
        return;
    }
#endif
    
//    //控制模式选择
//    if(DBUS_ReceiveData.switch_left == 1)
//    {
//        //键鼠模式
//        ControlStatus = ControlStatus_KM;
//    }
//    //遥控器控制
//    else if(DBUS_ReceiveData.switch_left == 2)
//    {
//        //遥控模式
//        ControlStatus = ControlStatus_RC;
//    }
//    else
//    {
//        //保护模式
//         ControlStatus = ControlStatus_Protect;
//    }
//        
//    //键鼠控制
//    if(ControlStatus == ControlStatus_KM)
//    {
//        //枪摩擦轮起停
//        if(DBUS_ReceiveData.mouse.press_left)
//        {
//            GunStatus = GunStatus_Motor;
//        }
//        
//        
//        //枪发射左键枪发射（仅在摩擦轮启动时有效）
//        if(DBUS_ReceiveData.mouse.press_left)
//        {
//            GunStatus = GunStatus_Shot;
//        }
//        else
//        {
//            if(GunFricWorking)
//            {
//                GunStatus = GunStatus_Motor;
//            }
//            else
//            {
//                GunStatus = GunStatus_Stop;
//            }
//        }
//            
//        
//        //云台模式
//        if(DBUS_ReceiveData.mouse.press_right)
//        {
//            CloudStatus = CloudStatus_Auto;
//        }
//        else
//        {
//            CloudStatus = CloudStatus_Normal;
//        }
//    }
//    else        //遥控器模式
//    {
        if(DBUS_ReceiveData.switch_right == 2)
        {
            GunStatus = GunStatus_Shot;
        }
        else if(DBUS_ReceiveData.switch_right == 3)
        {
            GunStatus = GunStatus_Motor;
        }
        else
        {
            GunStatus = GunStatus_Stop;
        }
}















