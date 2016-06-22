
#include "Task_Control.h"

#include "Config.h"

#include "Driver_DBUS.h"
#include "Driver_Motor.h"
#include "Driver_vision.h"
#include "Driver_Chassis.h"
#include "Driver_mpu9250.h"
#include "Driver_Control.h"
#include "Driver_StatusMachine.h"
#include "Driver_SuperGyroscope.h"

/**
  * @brief  控制任务
  * @param  ubused
  * @retval void
  */
float RC_EncoderYawTargetSum = YawCenter;           //遥控器角度积分
float RC_EncoderPitchTargetSum = PitchCenter;       //遥控器角度积分
  
void Task_Control(void *Parameters)
{
    int16_t PokeCurrent = 0;
    int16_t YawMotorCurrent = 0, PitchMotorCurrent = 0;
    AngleU_Struct TargetEncoderAngle;
    
    int16_t Counter = 0;
    portTickType LastTick = 0;
    
    for(;;)
    {
        LastTick = xTaskGetTickCount();
        
/************************  ↓  姿态更新  ↓ ************************/
        MPU9250_Update();
/************************  ↑  姿态更新  ↑ ************************/
/**************************************************************************************************/
/*********************  ↓  状态机状态更新  ↓ *********************/
        StatusMachine_Update();
/*********************  ↑  状态机状态更新  ↑ *********************/
/**************************************************************************************************/
/*********************  ↓  根据状态机控制  ↓ *********************/
        if(ControlStatus == ControlStatus_KM)
        {
            //云台控制 
            if(CloudStatus == CloudStatus_Auto)     //自动模式
            {
            //预判
//            ForcastOnce(350, 400, &ForcastAngle, 1);
            
            
//                //直接射击
//                ForcastAngle = RecToPolar(EnemyDataBuffer[EnemyDataBufferPoint].X, 
//                                            EnemyDataBuffer[EnemyDataBufferPoint].Y, 
//                                            EnemyDataBuffer[EnemyDataBufferPoint].Z,
//                                            1);
//                
//            YawAngleTarget = ForcastAngle.H;
//            PitchAngleTarget = ForcastAngle.V;
            
            
//            YawAngleTarget = 0;
//            PitchAngleTarget = PitchCenter;
            
            //计算射击质量评估决定是否发射
//            if(
            }
            else    //正常模式
            {
//                //Pitch累加
//                ABSTargetPitch += DBUS_ReceiveData.mouse.y / 6.0F;
//                //Pitch限幅
//                ABSTargetPitch = ABSTargetPitch > PitchDOWNLimit ? PitchDOWNLimit : ABSTargetPitch;
//                ABSTargetPitch = ABSTargetPitch < PitchUPLimit ? PitchUPLimit : ABSTargetPitch;
//                
//                //Yaw目标实际值累加
//                ABSTargetYaw -= DBUS_ReceiveData.mouse.x / 40.0F + DBUS_ReceiveData.ch3 / 1500.0F;
//                //Yaw轴编码器目标值计算
//                YawEncoderBuffer = (ABSTargetYaw - SuperGyoAngle) * 22.75556F + YawCenter;
//                //Yaw限幅
//                YawEncoderBuffer = YawEncoderBuffer > YawLEFTLimit ? YawLEFTLimit : YawEncoderBuffer;
//                YawEncoderBuffer = YawEncoderBuffer < YawRIGHTLimit ? YawRIGHTLimit : YawEncoderBuffer;
//                
//                EncoderTargetPitch = PitchSum;
//                EncoderTargetYaw = YawEncoderBuffer;
            }
        }
        //遥控器控制
//        if(DBUS_ReceiveData.switch_left == 1)
//        {
            if(DBUS_ReceiveData.switch_left == 1)
            {
                RC_EncoderPitchTargetSum += DBUS_ReceiveData.ch4 / 20.0F;
                RC_EncoderPitchTargetSum = RC_EncoderPitchTargetSum > PitchUPLimit ? PitchUPLimit : RC_EncoderPitchTargetSum;
                RC_EncoderPitchTargetSum = RC_EncoderPitchTargetSum < PitchDOWNLimit ? PitchDOWNLimit : RC_EncoderPitchTargetSum;
                
                RC_EncoderYawTargetSum -= DBUS_ReceiveData.ch3 / 20.0F;
                RC_EncoderYawTargetSum = RC_EncoderYawTargetSum > YawLEFTLimit ? YawLEFTLimit : RC_EncoderYawTargetSum;
                RC_EncoderYawTargetSum = RC_EncoderYawTargetSum < YawRIGHTLimit ? YawRIGHTLimit : RC_EncoderYawTargetSum;
                
                EncoderTargetPitch = RC_EncoderPitchTargetSum;
                EncoderTargetYaw = RC_EncoderYawTargetSum;
            }
            else if(DBUS_ReceiveData.switch_left == 2)
            {
                TargetEncoderAngle = RecToPolar(EnemyDataBuffer[EnemyDataBufferPoint].X, EnemyDataBuffer[EnemyDataBufferPoint].Y, EnemyDataBuffer[EnemyDataBufferPoint].Z, 1);
                Control_SetTargetPitch(TargetEncoderAngle.V, Encoder);
                Control_SetTargetYaw(TargetEncoderAngle.H, Encoder);
                
            }
//        }
        
            
        //枪控制
        if(GunStatus == GunStatus_Motor)
        {
            GunFric_Control(1);
            Poke_Control(0);
        }
        else if(GunStatus == GunStatus_Shot)
        {
            GunFric_Control(1);
            Poke_Control(1);
        }
        else
        {
            GunFric_Control(0);
            Poke_Control(0);
        }

/*********************  ↑  根据状态机控制  ↑ *********************/
/**************************************************************************************************/
/**********************  ↓  拨弹电机PID  ↓ ***********************/
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
            //编码器速度计算
            PokeSpeed = ENCODERCenter - TIM3->CNT;
            TIM3->CNT = ENCODERCenter;
            
            //拨弹电机PID
            PokeCurrent = Control_PokePID();
        }
/*********************  ↑  拨弹电机PID  ↑  ***********************/
/**************************************************************************************************/
/*********************  ↓  云台PID      ↓  **********************/
        YawMotorCurrent = Control_YawPID();
        PitchMotorCurrent = Control_PitchPID();
/*********************  ↑  云台PID      ↑  **********************/
/**************************************************************************************************/
/*********************  ↓   电机控制    ↓  **********************/
        if(ControlStatus == ControlStatus_Protect)
        {
            //云台电机电流发送
            CloudMotorCurrent(0, 0);
            //拨弹电机速度控制
            PokeMotorCurrent(0);
        }
        else
        {
            //云台电机电流发送
            CloudMotorCurrent(PitchMotorCurrent, YawMotorCurrent);
            //拨弹电机速度控制
            PokeMotorCurrent(PokeCurrent);
        }
/*********************  ↑   电机控制    ↑  **********************/
/**************************************************************************************************/
        
		vTaskDelayUntil(&LastTick, 5 / portTICK_RATE_MS);
    }
}



