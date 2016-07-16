
#define __DRIVER_STATUSMACHINE_GLOBALS

#include "Config.h"
#include "OSinclude.h"
#include "Driver_DBUS.h"
#include "Driver_vision.h"
#include "Driver_Control.h"
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
    static uint8_t RateCounter = 0;
    static uint8_t BigSampleCounter = 0;
    static uint8_t AttackCounter = 0;
    static portTickType LastPCShutdownSignalTick = 0;
    portTickType CurrentTick = xTaskGetTickCount();
    
    
    //帧率过低停机
    if(DBUSFrameRate < 3)
    {
        ControlMode = ControlMode_Protect;
        
        SneakMode = 0;
        
        return;
    }
    
    //遥控器控制
    if(DBUS_ReceiveData.switch_left == 1)
    {
        FristToKM = 1;
        
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
        
        SneakMode = 0;
    }
    //键鼠控制
    else if(DBUS_ReceiveData.switch_left == 2)
    {
        if(FristToKM == 1)
        {
            ControlMode = ControlMode_KM;
            FristToKM = 0;
        }
        
        //潜行控制
        if(DBUS_ReceiveData.keyBoard.key_code & KEY_CTRL)
        {
            SneakMode = 1;
        }
        else if(DBUS_ReceiveData.keyBoard.key_code & KEY_SHIFT)
        {
            SneakMode = 0;
        }
        
        if(DBUS_ReceiveData.keyBoard.key_code & KEY_C)
        {
            ControlMode = ControlMode_KM;
        }
        else if(DBUS_ReceiveData.keyBoard.key_code & KEY_F)
        {
            ControlMode = ControlMode_AUTO;
        }
        
        //降低发送频率减小串口负担
        if(RateCounter == 4)
        {
            //敌方目标红色
            if((DBUS_ReceiveData.ch1 > 600) && 
                (DBUS_ReceiveData.ch2 > 600) &&
                (DBUS_ReceiveData.ch3 < -600) &&
                (DBUS_ReceiveData.ch4 > 600))
            {
                SendEnemyColor('R');
            }
            //敌方目标蓝色
            else if((DBUS_ReceiveData.ch1 < -600) && 
                (DBUS_ReceiveData.ch2 > 600) &&
                (DBUS_ReceiveData.ch3 > 600) &&
                (DBUS_ReceiveData.ch4 > 600))
            {
                SendEnemyColor('B');
            }
            //大符模式
            else if(DBUS_ReceiveData.keyBoard.key_code & KEY_X)
            {
                AttackCounter = 0;
                
                if(BigSampleCounter < VisiolModeChangeDataSendNum)
                {
                    VisionType = VisionType_BigSample;
                    SendPCOrder(PCOrder_BigSample);
                    BigSampleCounter++;
                }
            }
            //自动射击模式（主机，单主控并不是）
            else
            {
                BigSampleCounter = 0;
                
                if(AttackCounter < VisiolModeChangeDataSendNum)
                {
                    VisionType = VisionType_Attack;
                    SendPCOrder(PCOrder_Attack);
                    AttackCounter++;
                }
            }
            RateCounter = 0;
        }
        else
        {
            RateCounter++;
        }
        
//#if INFANTRYTYPE == 1 
//        if(DBUS_ReceiveData.keyBoard.key_code & KEY_C)
//        {
//            ABInfantryMode = ABInfantry_Master;
//        }
//        else if(DBUS_ReceiveData.keyBoard.key_code & KEY_F)
//        {
//            ABInfantryMode = ABInfantry_Slave;
//        }
//#elif INFANTRYTYPE == 2
//        if(DBUS_ReceiveData.keyBoard.key_code & KEY_F)
//        {
//            ABInfantryMode = ABInfantry_Master;
//        }
//        else if(DBUS_ReceiveData.keyBoard.key_code & KEY_C)
//        {
//            ABInfantryMode = ABInfantry_Slave;
//        }
//#endif
    }
    //保护模式
    else
    {
        FristToKM = 1;
        SneakMode = 0;
        
        ControlMode = ControlMode_Protect;
        
        //关机
        if((DBUS_ReceiveData.ch1 > 600) && 
            (DBUS_ReceiveData.ch2 < -600) &&
            (DBUS_ReceiveData.ch3 < -600) &&
            (DBUS_ReceiveData.ch4 > 600))
        {
            if(LastPCShutdownSignalTick + 5000 < CurrentTick)
            {
                SendPCOrder(PCOrder_Shutdown);
            }
        }
        else
        {
            LastPCShutdownSignalTick = CurrentTick;
        }
    }
}















