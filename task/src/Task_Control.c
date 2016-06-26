
#include "Task_Control.h"

#include "Config.h"

#include "Driver_DBUS.h"
#include "Driver_vision.h"
#include "Driver_Chassis.h"
#include "Driver_mpu9250.h"
#include "Driver_Control.h"
#include "Driver_PokeMotor.h"
#include "Driver_FricMotor.h"
#include "Driver_CloudMotor.h"
#include "Driver_StatusMachine.h"
#include "Driver_SuperGyroscope.h"

/**
  * @brief  控制任务
  * @param  ubused
  * @retval void
  */
    float PitchSum = 0, YawSum = 0;
    
void Task_Control(void *Parameters)
{
    int16_t XSpeed, YSpeed;
    int16_t Counter = 0;
    portTickType ControlLastTick = 0;
    float IntBuffer;
    
    for(;;)
    {
        ControlLastTick = xTaskGetTickCount();
        
/************************  ↓  姿态更新  ↓ ************************/
        MPU9250_Update();
/************************  ↑  姿态更新  ↑ ************************/
/**************************************************************************************************/
/*********************  ↓  状态机状态更新  ↓ *********************/
        StatusMachine_Update();
/*********************  ↑  状态机状态更新  ↑ *********************/
/**************************************************************************************************/
/*********************  ↓  根据状态机控制  ↓ *********************/
        //遥控器控制
        if(ControlMode == ControlMode_RC)
        {
            CloudParam.Yaw.ABSTargetAngle -= DBUS_ReceiveData.ch3 / 600.0F;
            
            CloudParam.Pitch.EncoderTargetAngle += DBUS_ReceiveData.ch4 / 50.0F;
            CloudParam.Pitch.EncoderTargetAngle = CloudParam.Pitch.EncoderTargetAngle > PitchUPLimit ? PitchUPLimit : CloudParam.Pitch.EncoderTargetAngle;
            CloudParam.Pitch.EncoderTargetAngle = CloudParam.Pitch.EncoderTargetAngle < PitchDOWNLimit ? PitchDOWNLimit : CloudParam.Pitch.EncoderTargetAngle;
            
            Cloud_YawAngleSet(CloudParam.Yaw.ABSTargetAngle, 0);
//            Cloud_PitchAngleSet(CloudParam.Pitch.EncoderTargetAngle);
            
            Cloud_Adjust(1);
            
            //摩擦轮
            if(GunStatus == GunStatus_Motor)
            {
                GunFric_Control(1);
            }
            else if(GunStatus == GunStatus_Shot)
            {
                GunFric_Control(1);
                PokeMotor_Step();
            }
            else
            {
                GunFric_Control(0);
            }
            
            Chassis_SpeedSet(DBUS_ReceiveData.ch2 * 900 / 660, DBUS_ReceiveData.ch1 * 900 / 660);
            Chassis_Control(1);
        }
        else if(ControlMode == ControlMode_KM)
        {
            //前后
            if((DBUS_ReceiveData.keyBoard.key_code & KEY_W) && (DBUS_ReceiveData.keyBoard.key_code & KEY_SHIFT))
            {
                //潜行模式（进补给站）
                XSpeed = SNEAKSPEED;
            }
            else if((DBUS_ReceiveData.keyBoard.key_code & KEY_W) && (DBUS_ReceiveData.keyBoard.key_code & KEY_S))
            {
                XSpeed = 0;
            }
            else if(DBUS_ReceiveData.keyBoard.key_code & KEY_W)
            {
                XSpeed = MAXWORKINGSPEED;
            }
            else if(DBUS_ReceiveData.keyBoard.key_code & KEY_S)
            {
                XSpeed = -MAXWORKINGSPEED;
            }
            else
            {
                XSpeed = 0;
            }
            
            //左右平移
            if((DBUS_ReceiveData.keyBoard.key_code & KEY_A) && (DBUS_ReceiveData.keyBoard.key_code & KEY_D))
            {
                //同时按下无效
                YSpeed = 0;
            }
            else if(DBUS_ReceiveData.keyBoard.key_code & KEY_A)
            {
                YSpeed = -MAXSHIFTSPEED;
            }
            else if(DBUS_ReceiveData.keyBoard.key_code & KEY_D)
            {
                YSpeed = MAXSHIFTSPEED;
            }
            else
            {
                YSpeed = 0;
            }
            Chassis_SpeedSet(XSpeed, YSpeed);
            Chassis_Control(1);
            
            //旋转控制
            if(DBUS_ReceiveData.keyBoard.key_code & KEY_E)
            {
                CloudParam.Yaw.ABSTargetAngle -= QESPINPARAM;
                Cloud_YawAngleSet(CloudParam.Yaw.ABSTargetAngle, 0);
            }
            else if(DBUS_ReceiveData.keyBoard.key_code & KEY_Q)
            {
                CloudParam.Yaw.ABSTargetAngle += QESPINPARAM;
                Cloud_YawAngleSet(CloudParam.Yaw.ABSTargetAngle, 0);
            }
            else
            {
                IntBuffer = DBUS_ReceiveData.mouse.x / MOUSESPINPARAM;
                IntBuffer = IntBuffer > MOUSEINTLIMIT ? MOUSEINTLIMIT : IntBuffer;
                IntBuffer = IntBuffer < -MOUSEINTLIMIT ? -MOUSEINTLIMIT : IntBuffer;
                
                CloudParam.Yaw.ABSTargetAngle -= IntBuffer;
                Cloud_YawAngleSet(CloudParam.Yaw.ABSTargetAngle, 0);
            } 
            CloudParam.Pitch.EncoderTargetAngle -= DBUS_ReceiveData.mouse.y;
            CloudParam.Pitch.EncoderTargetAngle = CloudParam.Pitch.EncoderTargetAngle > PitchUPLimit ? PitchUPLimit : CloudParam.Pitch.EncoderTargetAngle;
            CloudParam.Pitch.EncoderTargetAngle = CloudParam.Pitch.EncoderTargetAngle < PitchDOWNLimit ? PitchDOWNLimit : CloudParam.Pitch.EncoderTargetAngle;
        
            //发射
            if(DBUS_ReceiveData.switch_right == 3)
            {
                GunFric_Control(1);
                if(DBUS_ReceiveData.mouse.press_left)
                {
                    PokeMotor_Step();
                }
            }
            else
            {
                GunFric_Control(0);
            }
            
            Cloud_Adjust(1);
        }
        else
        {
            Cloud_Adjust(0);
            Chassis_Control(0);
            GunFric_Control(0);
        }
        
/*********************  ↑  根据状态机控制  ↑ *********************/
/**************************************************************************************************/
/**********************  ↓  拨弹电机控制   ↓ ***********************/
        //将拨弹电机控制频率降低
        if(Counter >= 5)
        {
            Counter = 0;
        }
        else
        {
            Counter++;
        }
        
        if(Counter == 0)
        {
            PokeMotor_Adjust();
        }
/*********************  ↑  拨弹电机PID  ↑  ***********************/
/**************************************************************************************************/
/*********************  ↓  云台PID      ↓  **********************/
/*********************  ↑  云台PID      ↑  **********************/
/**************************************************************************************************/
/*********************  ↓   电机控制    ↓  **********************/
/*********************  ↑   电机控制    ↑  **********************/
/**************************************************************************************************/
        
		vTaskDelayUntil(&ControlLastTick, 5 / portTICK_RATE_MS);
    }
}



