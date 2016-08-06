
#define __DRIVER_STATUSMACHINE_GLOBALS

#include "Config.h"
#include "OSinclude.h"
#include "Driver_DBUS.h"
#include "Driver_vision.h"
#include "Driver_Control.h"
#include "Driver_mpu9250.h"
#include "Driver_FricMotor.h"
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
    ControlMode = ControlMode_KM;
    FricStatus = FricStatus_Stop;
    KMSubschema = KMSubschema_Normal;
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
    
    //帧率过低进保护
    if(DBUSFrameRate < 3)
    {
        ControlMode = ControlMode_Protect;
        return;
    }
    
    //基本模式
    if(DBUS_ReceiveData.switch_left == 1)
    {
        ControlMode = ControlMode_RC;
    }
    else if(DBUS_ReceiveData.switch_left == 2)
    {
        ControlMode = ControlMode_KM;
    }
    else
    {
        ControlMode = ControlMode_Protect;
    } 
    
    //控制子模式
    if(ControlMode == ControlMode_RC)
    {
/*******************************************  ↓  摩擦轮  ↓  *******************************************/
        if((DBUS_ReceiveData.switch_right == 3) || (DBUS_ReceiveData.switch_right == 2))
        {
            FricStatus = FricStatus_Working;
        }
        else
        {
            FricStatus = FricStatus_Stop;
        }
/*******************************************  ↑  摩擦轮  ↑  *******************************************/
        KMSubschema = KMSubschema_Normal;
    }
    //键鼠模式
    else if(ControlMode == ControlMode_KM)
    {
/*******************************************  ↓  摩擦轮  ↓  *******************************************/
        if((DBUS_ReceiveData.switch_right == 3) || (DBUS_ReceiveData.switch_right == 2))
        {
            if(DBUS_ReceiveData.keyBoard.key_code & KEY_Z)
            {
                //暴击模式
                FricStatus = FricStatus_Crazy;
            }
            else
            {
                //正常工作
                FricStatus = FricStatus_Working;
            }
        }
        else
        {
            FricStatus = FricStatus_Stop;
        }
/*******************************************  ↑  摩擦轮  ↑  *******************************************/
/*******************************************  ↓   模式   ↓  *******************************************/
        //无论什么时候按下CTRL键回归手动Normal模式
        if(DBUS_CheckPush(KEY_SHIFT))
        {
            KMSubschema = KMSubschema_Normal;
        }
        
        //只允许在Normal模式下进行模式切换
        if(KMSubschema == KMSubschema_Normal)
        {
            //补给站模式
            if(DBUS_CheckPush(KEY_CTRL))
            {
                KMSubschema = KMSubschema_Supply;
            }
            //半自动模式
//            else if(DBUS_ReceiveData.mouse.press_right)
//            {
//                KMSubschema = KMSubschema_Halfauto;
//            }
            //摇摆模式
            else if(DBUS_CheckPush(KEY_F))
            {
                KMSubschema = KMSubschema_Swing;
            }
            //大符模式
            else if(DBUS_CheckPush(KEY_X))
            {
                KMSubschema = KMSubschema_Bigsample;
            }
            //全自动模式
            else if(DBUS_CheckPush(KEY_C))
            {
                KMSubschema = KMSubschema_Fullauto;
            }
            //圈圈模式
            else if(DBUS_CheckPush(KEY_G))
            {
                KMSubschema = KMSubschema_Circle;
            }
        }
        //用于半自动回归
//        else if((KMSubschema == KMSubschema_Halfauto) && (!DBUS_ReceiveData.mouse.press_right))
//        {
//            KMSubschema = KMSubschema_Normal;
//        }
/*******************************************  ↑   模式   ↑  *******************************************/
    }
    else
    {
        FricStatus = FricStatus_Stop;
        KMSubschema = KMSubschema_Normal;
    }
}















