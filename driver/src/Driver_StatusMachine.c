
#define __DRIVER_STATUSMACHINE_GLOBALS

#include "Config.h"
#include "OSinclude.h"
#include "Driver_DBUS.h"
#include "Driver_vision.h"
#include "Driver_Control.h"
#include "Driver_mpu9250.h"
#include "Driver_CloudMotor.h"
#include "Driver_StatusMachine.h"
#include "Driver_SuperGyroscope.h"


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
    ControlMode = ControlMode_KM;
    SneakMode = 0;
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
    uint8_t FristToKM = 1;
void StatusMachine_Update(void)
{
    portTickType CurrentTick = xTaskGetTickCount();
    
    //Yaw轴实际绝对角度
    CloudParam.Yaw.RealABSAngle = SuperGyoParam.Angle + ((int16_t)CloudParam.Yaw.RealEncoderAngle - YawEncoderCenter) * 0.043945F;
    //Pitch轴实际绝对角度
    CloudParam.Pitch.RealABSAngle = Position.Euler.Pitch;
    
    //帧率过低停机
    if(DBUSFrameRate < 3)
    {
        ControlMode = ControlMode_Protect;
        
        SneakMode = 0;
        
        return;
    }
    
    //保护模式
    if(DBUS_ReceiveData.switch_left == 3)
    {
        ControlMode = ControlMode_RC;
    }
    else
    {
        ControlMode = ControlMode_Protect;
    }
}















