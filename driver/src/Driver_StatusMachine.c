
#define __DRIVER_STATUSMACHINE_GLOBALS

#include "Config.h"
#include "Driver_DBUS.h"
#include "Driver_Judge.h"
#include "Driver_vision.h"
#include "Driver_Control.h"
#include "Driver_mpu9250.h"
#include "Driver_FricMotor.h"
#include "Driver_CloudMotor.h"
#include "Driver_StatusMachine.h"
#include "Driver_SuperGyroscope.h"

#include "bsp_oled.h"


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
void StatusMachine_Update(portTickType Tick)
{
    
//基地直接进入自动模式
#if INFANTRY == 7
    
    static char Color = 'R';
    static uint32_t PushCounter = 0;
    static uint8_t RedCounter = 0, BlueCounter = 0, OffCounter = 0;
    static uint16_t ColorSendCounter = 0;
    
#if INFANTRY == 7
    int8_t LastPCVisionTargetColor;
#endif
    
    if(Tick > 8000)
    {
        //按键判断
        if(GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_9))
        {
            PushCounter = 0;
        }
        else
        {
            PushCounter++;
        }
        
        //颜色计数器置位
        if(PushCounter == 1)        //切换颜色
        {
            if(Color == 'R')
            {
                Color = 'B';
                BlueCounter = 10;
                RedCounter = 0;
                OffCounter = 0;
                OLED_Print6x8Str(100, 10, 30, 8, (uint8_t *)"BLUE", INV_OFF, IS);
            }
            else
            {
                Color = 'R';
                BlueCounter = 0;
                RedCounter = 10;
                OffCounter = 0;
                OLED_Print6x8Str(100, 10, 30, 8, (uint8_t *)"RED ", INV_OFF, IS);
            }
        }
        else if(PushCounter == 1000)
        {
            BlueCounter = 0;
            RedCounter = 0;
            OffCounter = 50;
            OLED_Print6x8Str(100, 10, 30, 8, (uint8_t *)"OFF", INV_OFF, IS);
        }
        else        //每2s发送颜色切换指令，保证颜色正确
        {
            if(ColorSendCounter >= 300)
            {
                if(Color == 'R')
                {
                    RedCounter += 2;
                }
                else
                {
                    BlueCounter += 2;
                }
                
                ColorSendCounter = 0;
            }
            else
            {
                ColorSendCounter++;
            }
        }
        
        
        //发送指令
        if(BlueCounter)
        {
            SendEnemyColor('B');
            BlueCounter--;
        }
        else if(RedCounter)
        {
            SendEnemyColor('R');
            RedCounter--;
        }
        else if(OffCounter)
        {
            SendPCOrder(PCOrder_Shutdown);
            OffCounter--;
        }
        
        FricStatus = FricStatus_Working;
        ControlMode = ControlMode_KM;
        
#if INFANTRY == 7
        //主机目标颜色显示
        if(LastPCVisionTargetColor != PCVisionTargetColor)
        {
            LastPCVisionTargetColor = PCVisionTargetColor;
            if(PCVisionTargetColor == -1)
            {
                OLED_Print6x8Str(100, 30, 30, 8, (uint8_t *)" -  ", INV_OFF, IS);
            }
            else if(PCVisionTargetColor == 0)
            {
                OLED_Print6x8Str(100, 30, 30, 8, (uint8_t *)"RED ", INV_OFF, IS);
            }
            else if(PCVisionTargetColor == 1)
            {
                OLED_Print6x8Str(100, 30, 30, 8, (uint8_t *)"BLUE", INV_OFF, IS);
            }
        }
#endif
    }
    else
    {
        ControlMode = ControlMode_Protect;
    }
    
    return;
    
#else
    
    static uint8_t RateCounter = 0;
    static uint8_t BigSampleCounter = 0;
    static uint8_t AttackCounter = 0;
    static portTickType LastPCShutdownSignalTick = 0;
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
        
        //自动修改键鼠子模式
        KMSubschema = KMSubschema_Normal;
    }
    //键鼠模式
    else if(ControlMode == ControlMode_KM)
    {
/*******************************************  ↓  摩擦轮  ↓  *******************************************/
        if((DBUS_ReceiveData.switch_right == 3) || (DBUS_ReceiveData.switch_right == 2))
        {
            //正常工作
            FricStatus = FricStatus_Working;
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
        
        if(KMSubschema == KMSubschema_Halfauto)
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
#if AUTOSHOOTSUPPORT == 1
//            //半自动模式
//            else if(DBUS_ReceiveData.mouse.press_right)
//            {
//                KMSubschema = KMSubschema_Halfauto;
//            }
#endif
#if AUTOBIGSAMPLESUPPORT == 1
            //自动大符模式
            else if(DBUS_CheckPush(KEY_X))
            {
                KMSubschema = KMSubschema_Bigsample;
            }
#endif
            //手动大符模式
            else if(DBUS_CheckPush(KEY_Z))
            {
                KMSubschema = KMSubschema_MannualBigsample;
            }
            //摇摆模式
            else if(DBUS_CheckPush(KEY_F))
            {
                KMSubschema = KMSubschema_Swing;
            }
            //全自动模式
            else if(DBUS_CheckPush(KEY_C))
            {
                KMSubschema = KMSubschema_Fullauto;
            }
            //自主圈圈模式
            else if(DBUS_CheckPush(KEY_G))
            {
                KMSubschema = KMSubschema_AutoCircle;
            }
            //全速圈圈模式
            else if(DBUS_CheckPush(KEY_R))
            {
                KMSubschema = KMSubschema_FullCircle;
            }
            else
            {
                BigSampleCounter = 0;
                AttackCounter = 0;
            }
        }
        
        
        //视觉指令
        //降低发送频率减小串口负担
        if(RateCounter == 4)
        {
            //切换红色目标
            if((DBUS_ReceiveData.ch1 > 600) && 
                (DBUS_ReceiveData.ch2 > 600) &&
                (DBUS_ReceiveData.ch3 < -600) &&
                (DBUS_ReceiveData.ch4 > 600))
            {
                SendEnemyColor('R');
            }
            //切换蓝色目标
            else if((DBUS_ReceiveData.ch1 < -600) && 
                (DBUS_ReceiveData.ch2 > 600) &&
                (DBUS_ReceiveData.ch3 > 600) &&
                (DBUS_ReceiveData.ch4 > 600))
            {
                SendEnemyColor('B');
            }
            //大符模式
            else if(KMSubschema == KMSubschema_Bigsample)
            {
                if(BigSampleCounter < VisiolModeChangeDataSendNum)
                {
                    SendPCOrder(PCOrder_BigSample);
                    BigSampleCounter++;
                }
//                else
//                {
//                    SendPCOrder(PCOrder_BigSample);
//                }
            }
            //自动射击模式
            else if(KMSubschema == KMSubschema_Halfauto)
            {
                if(AttackCounter < VisiolModeChangeDataSendNum)
                {
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
    
    
        
        //用于半自动回归
        if((KMSubschema == KMSubschema_Halfauto) && (!DBUS_ReceiveData.mouse.press_right))
        {
            KMSubschema = KMSubschema_Normal;
        }
/*******************************************  ↑   模式   ↑  *******************************************/
    }
    else
    {
        FricStatus = FricStatus_Stop;
        KMSubschema = KMSubschema_Normal;
        
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
    
#endif
    
}















