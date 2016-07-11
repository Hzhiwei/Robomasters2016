
#include "Task_Control.h"

#include "Config.h"

#include "mpu9250dmp.h"
#include "Driver_DBUS.h"
#include "Driver_vision.h"
#include "Driver_Chassis.h"
#include "Driver_mpu9250.h"
#include "Driver_Control.h"
#include "Driver_Steering.h"
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
    
    AngleI_Struct AutoTargetAngle;      //自动射击目标角度
    int16_t ForcastTargetEncoderOmega;
    
void Task_Control(void *Parameters)
{
    int16_t XSpeed, YSpeed;
    portTickType ControlLastTick = 0;
    float MouseSpinIntBuffer = 0;
    uint16_t dmpresetCounter = 0;
    AngleI_Struct ForcastAngle;     //预判结果
    int16_t PitchCurrent = 0, YawCurrent = 0;
    
    for(;;)
    {
        ControlLastTick = xTaskGetTickCount();
        
/************************  ↓  姿态更新  ↓ ************************/
        MPU9250_Update();
        
        //DMP每20s重启，否则会傻逼
        if(dmpresetCounter >= 5000)
        {
            dmpresetCounter = 0;
            mpu_set_dmp_state(0);
            mpu_set_dmp_state(1);
        }
        else
        {
            dmpresetCounter++;
        }
/************************  ↑  姿态更新  ↑ ************************/
/**************************************************************************************************/
/*********************  ↓  状态机状态更新  ↓ *********************/
        StatusMachine_Update();
/*********************  ↑  状态机状态更新  ↑ *********************/
/**************************************************************************************************/
/*********************  ↓  根据状态机控制  ↓ *********************/
////        //遥控器控制
        if(ControlMode == ControlMode_RC)
        {
//      手动
            CloudParam.Yaw.ABSTargetAngle -= DBUS_ReceiveData.ch3 / 600.0F;
            Cloud_YawAngleSet(CloudParam.Yaw.ABSTargetAngle, 0);
            
            CloudParam.Pitch.ABSTargetAngle += DBUS_ReceiveData.ch4 / 250.0F;
            CloudParam.Pitch.ABSTargetAngle = CloudParam.Pitch.ABSTargetAngle > ABSPITCHUPLIMIT ? ABSPITCHUPLIMIT : CloudParam.Pitch.ABSTargetAngle;
            CloudParam.Pitch.ABSTargetAngle = CloudParam.Pitch.ABSTargetAngle < ABSPITCHDOWNLIMIT ? ABSPITCHDOWNLIMIT : CloudParam.Pitch.ABSTargetAngle;
            CloudParam.Pitch.AngleMode = AngleMode_ABS;
            
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


////      大符模式
//                    CloudParam.Pitch.AngleMode = AngleMode_Encoder;         //编码器模式
//                    CloudParam.Yaw.AngleMode = AngleMode_Encoder;         //编码器模式
//                    
//                    ForcastAngle = RecToPolar(EnemyDataBuffer[EnemyDataBufferPoint].X, 
//                                                EnemyDataBuffer[EnemyDataBufferPoint].Y, 
//                                                EnemyDataBuffer[EnemyDataBufferPoint].Z, 
//                                                Position.Euler.Pitch,
//                                                CloudParam.Pitch.RealEncoderAngle,
//                                                1);
//                    
//                    CloudParam.Pitch.EncoderTargetAngle = ForcastAngle.V + PitchCenter;
//                    CloudParam.Yaw.EncoderTargetAngle = ForcastAngle.H + YawCenter;
//                    
//                    Cloud_Adjust(1);
            
            
///****************************************   遥控器控制，此处注释，临时改为自动射击调试模式*/
//            //Pitch轴不使用预判，直接使用最新帧数据的pitch数据作为目标数据
//            //Yaw轴使用预判
//            
//            //Pitch轴直接使用最新数据
//            CloudParam.Pitch.AngleMode = AngleMode_Encoder;         //编码器模式
//            ForcastAngle = RecToPolar(EnemyDataBuffer[EnemyDataBufferPoint].X, EnemyDataBuffer[EnemyDataBufferPoint].Y, EnemyDataBuffer[EnemyDataBufferPoint].Z, 1);
//            CloudParam.Pitch.EncoderTargetAngle = ForcastAngle.V + PitchCenter;
//            PitchCurrent = Control_PitchPID();
//            
//            //Yaw轴速度跟随
//            //计算目标角速度大小（编码器单位）
//            if(VisionUpdataFlag)
//            {
//                ForcastOnce(400, 200, &ForcastAngle, 0);        //预判 
//                ForcastTarget.Target.H = ForcastAngle.H;
//                ForcastTarget.TargetTick = ControlLastTick + 150;
//            }
//            
//            if(ForcastTarget.TargetTick > ControlLastTick)
//            {
//                if(ForcastTarget.TargetTick - ControlLastTick > 30)
//                {
//                    ForcastTargetEncoderOmega = ((float)ForcastAngle.H + YawCenter - CloudParam.Yaw.RealEncoderAngle) * 1000 / ((int)ForcastTarget.TargetTick - ControlLastTick);
//                    if((ForcastTargetEncoderOmega < 500) && (ForcastTargetEncoderOmega > -500))
//                    {
//                        ForcastTargetEncoderOmega /= 3.5F;
//                    }
//                }
//            }
//            else
//            {
//                ForcastTargetEncoderOmega = 0;
//            }
//            
//            YawCurrent = VControl_YawPID(ForcastTargetEncoderOmega);
//            
//            CloudMotorCurrent(PitchCurrent, YawCurrent);
//            
//            if(DBUS_ReceiveData.switch_right == 3)
//            {
//                GunFric_Control(1);
//            }
//            else if(DBUS_ReceiveData.switch_right == 2)
//            {
//                GunFric_Control(1);
//                PokeMotor_Step();
//            }
//            else
//            {
//                GunFric_Control(0);
//            }
            
/******************     直接跟随的搓逼模式，战五的渣渣    **********************/
//            if(EnemyDataBuffer[EnemyDataBufferPoint].ID)
//            {
//                AutoTargetAngle = RecToPolar(EnemyDataBuffer[EnemyDataBufferPoint].X, 
//                                            EnemyDataBuffer[EnemyDataBufferPoint].Y,
//                                            EnemyDataBuffer[EnemyDataBufferPoint].Z,
//                                            Position.Euler.Pitch,
//                                            CloudParam.Pitch.RealEncoderAngle,
//                                            0);
//                
//                Cloud_YawAngleSet(AutoTargetAngle.H, 1);
//                Cloud_PitchAngleSet(AutoTargetAngle.V, 1);
//            
//                Cloud_Adjust(1);
//                    
//                if(DBUS_ReceiveData.switch_right == 3)
//                {
//                    GunFric_Control(1);
//                }
//                else if(DBUS_ReceiveData.switch_right == 2)
//                {
//                    GunFric_Control(1);
//                    PokeMotor_Step();
//                }
//                else
//                {
//                    GunFric_Control(0);
//                }
//            }
        }
        else if(ControlMode == ControlMode_KM)
        {
            //速度倍率设置
            if((DBUS_ReceiveData.keyBoard.key_code & KEY_V) & (!(LASTDBUS_ReceiveData.keyBoard.key_code & KEY_V)))
            {
                if(ChassisParam.SpeedLevel == ChassisSpeedLevel_Hight)
                {
                    ChassisParam.SpeedLevel = ChassisSpeedLevel_Low;
                }
                else if(ChassisParam.SpeedLevel == ChassisSpeedLevel_Low)
                {
                    ChassisParam.SpeedLevel = ChassisSpeedLevel_Hight;
                }
            }
            
            //前后
            if((DBUS_ReceiveData.keyBoard.key_code & KEY_W) && (DBUS_ReceiveData.keyBoard.key_code & KEY_CTRL))
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
                XSpeed = ChassisParam.SpeedLevel == ChassisSpeedLevel_Hight ? MAXWORKINGSPEED : MAXWORKINGSPEED * LOWSPEEDOVERRIDE;
            }
            else if(DBUS_ReceiveData.keyBoard.key_code & KEY_S)
            {
                XSpeed = -(ChassisParam.SpeedLevel == ChassisSpeedLevel_Hight ? MAXWORKINGSPEED : MAXWORKINGSPEED * LOWSPEEDOVERRIDE);
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
            
            //补给站模式
            if(DBUS_ReceiveData.keyBoard.key_code & KEY_CTRL)
            {
                Steering_Control(1);//舱门控制
                
                CloudParam.Pitch.AngleMode = AngleMode_Encoder;         //编码器模式
                CloudParam.Pitch.EncoderTargetAngle = DEPOTABSPITCH + PitchCenter;
                
                MouseSpinIntBuffer = DBUS_ReceiveData.mouse.x / (MOUSESPINPARAM * 4);
                MouseSpinIntBuffer = MouseSpinIntBuffer > MOUSEINTLIMIT ? MOUSEINTLIMIT : MouseSpinIntBuffer;
                MouseSpinIntBuffer = MouseSpinIntBuffer < -MOUSEINTLIMIT ? -MOUSEINTLIMIT : MouseSpinIntBuffer;
                CloudParam.Yaw.ABSTargetAngle -= MouseSpinIntBuffer;
                CloudParam.Yaw.AngleMode = AngleMode_ABS;
                
                Cloud_Adjust(1);
            }
            else
            {
                Steering_Control(0);
                //自动射击
                if(DBUS_ReceiveData.mouse.press_right)
                {
                    
                    //旋转控制
                    if(DBUS_ReceiveData.keyBoard.key_code & KEY_E)
                    {
                        CloudParam.Yaw.ABSTargetAngle -= QESPINPARAM;
                    }
                    else if(DBUS_ReceiveData.keyBoard.key_code & KEY_Q)
                    {
                        CloudParam.Yaw.ABSTargetAngle += QESPINPARAM;
                    }
                    else
                    {
                        MouseSpinIntBuffer = DBUS_ReceiveData.mouse.x / MOUSESPINPARAM;
                        MouseSpinIntBuffer = MouseSpinIntBuffer > MOUSEINTLIMIT ? MOUSEINTLIMIT : MouseSpinIntBuffer;
                        MouseSpinIntBuffer = MouseSpinIntBuffer < -MOUSEINTLIMIT ? -MOUSEINTLIMIT : MouseSpinIntBuffer;
                        
                        CloudParam.Yaw.ABSTargetAngle -= MouseSpinIntBuffer;
                    } 
                    
//                    CloudParam.Pitch.ABSTargetAngle -= DBUS_ReceiveData.mouse.y / MOUSEPITCHPARAM;
//                    CloudParam.Pitch.ABSTargetAngle = CloudParam.Pitch.ABSTargetAngle > ABSPITCHUPLIMIT ? ABSPITCHUPLIMIT : CloudParam.Pitch.ABSTargetAngle;
//                    CloudParam.Pitch.ABSTargetAngle = CloudParam.Pitch.ABSTargetAngle < ABSPITCHDOWNLIMIT ? ABSPITCHDOWNLIMIT : CloudParam.Pitch.ABSTargetAngle;
                
                    
                    //Pitch轴不使用预判，直接使用最新帧数据的pitch数据作为目标数据
                    //Yaw轴使用预判
                    
                    //Pitch轴直接使用最新数据
                    CloudParam.Pitch.AngleMode = AngleMode_Encoder;         //编码器模式
                    
                    ForcastAngle = RecToPolar(EnemyDataBuffer[EnemyDataBufferPoint].X, 
                                                EnemyDataBuffer[EnemyDataBufferPoint].Y, 
                                                EnemyDataBuffer[EnemyDataBufferPoint].Z, 
                                                Position.Euler.Pitch,
                                                CloudParam.Pitch.RealEncoderAngle,
                                                1);
                    
                    CloudParam.Pitch.EncoderTargetAngle = ForcastAngle.V + PitchCenter;
                    PitchCurrent = Control_PitchPID();
                    
                    //Yaw轴速度跟随
                    //计算目标角速度大小（编码器单位）
                    if(VisionUpdataFlag)
                    {
                        ForcastOnce(400, 150, &ForcastAngle, 0);        //预判 
                        ForcastTarget.Target.H = ForcastAngle.H;
                        ForcastTarget.TargetTick = ControlLastTick + 150;
                    }
                    
                    if(ForcastTarget.TargetTick > ControlLastTick)
                    {
                        if(ForcastTarget.TargetTick - ControlLastTick > 30)
                        {
                            ForcastTargetEncoderOmega = ((float)ForcastAngle.H + YawCenter - CloudParam.Yaw.RealEncoderAngle) * 1000 / ((int)ForcastTarget.TargetTick - ControlLastTick);
                            if((ForcastTargetEncoderOmega < 500) && (ForcastTargetEncoderOmega > -500))
                            {
                                ForcastTargetEncoderOmega /= 3.5F;
                            }
                        }
                    }
                    else
                    {
                        ForcastTargetEncoderOmega = 0;
                    }
                    
                    YawCurrent = VControl_YawPID(ForcastTargetEncoderOmega);
                    
                    CloudMotorCurrent(PitchCurrent, YawCurrent);
                }
                //大符模式
                else if(DBUS_ReceiveData.keyBoard.key_code & KEY_X)
                {
                    CloudParam.Pitch.AngleMode = AngleMode_Encoder;         //编码器模式
                    CloudParam.Yaw.AngleMode = AngleMode_Encoder;         //编码器模式
                    
                    ForcastAngle = RecToPolar(EnemyDataBuffer[EnemyDataBufferPoint].X, 
                                                EnemyDataBuffer[EnemyDataBufferPoint].Y, 
                                                EnemyDataBuffer[EnemyDataBufferPoint].Z, 
                                                Position.Euler.Pitch,
                                                CloudParam.Pitch.RealEncoderAngle,
                                                1);
                    
                    CloudParam.Pitch.EncoderTargetAngle = ForcastAngle.V + PitchCenter;
                    CloudParam.Yaw.EncoderTargetAngle = ForcastAngle.H + YawCenter;
                    
                    Cloud_Adjust(1);
                }
                else    //手动射击
                {
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
                        MouseSpinIntBuffer = DBUS_ReceiveData.mouse.x / MOUSESPINPARAM;
                        MouseSpinIntBuffer = MouseSpinIntBuffer > MOUSEINTLIMIT ? MOUSEINTLIMIT : MouseSpinIntBuffer;
                        MouseSpinIntBuffer = MouseSpinIntBuffer < -MOUSEINTLIMIT ? -MOUSEINTLIMIT : MouseSpinIntBuffer;
                        
                        CloudParam.Yaw.ABSTargetAngle -= MouseSpinIntBuffer;
                        CloudParam.Yaw.AngleMode = AngleMode_ABS;
                    } 
                    
                    CloudParam.Pitch.ABSTargetAngle -= DBUS_ReceiveData.mouse.y / MOUSEPITCHPARAM;
                    CloudParam.Pitch.ABSTargetAngle = CloudParam.Pitch.ABSTargetAngle > ABSPITCHUPLIMIT ? ABSPITCHUPLIMIT : CloudParam.Pitch.ABSTargetAngle;
                    CloudParam.Pitch.ABSTargetAngle = CloudParam.Pitch.ABSTargetAngle < ABSPITCHDOWNLIMIT ? ABSPITCHDOWNLIMIT : CloudParam.Pitch.ABSTargetAngle;
                    CloudParam.Pitch.AngleMode = AngleMode_ABS;
                
                    Cloud_Adjust(1);
                }
            }
            
            //发射
            if(DBUS_ReceiveData.switch_right == 3)
            {
                if(DBUS_ReceiveData.keyBoard.key_code & KEY_Z)
                {
                    GunFric_Control(2);
                }
                else
                {
                    GunFric_Control(1);
                }
                if(DBUS_ReceiveData.mouse.press_left)
                {
                    PokeMotor_Step();
                }
            }
            else
            {
                GunFric_Control(0);
            }
        }
        //全自动模式
        else if(ControlMode == ControlMode_AUTO)
        {       
            //旋转控制
            
            //Pitch轴不使用预判，直接使用最新帧数据的pitch数据作为目标数据
            //Yaw轴使用预判
            
            //Pitch轴直接使用最新数据
            CloudParam.Pitch.AngleMode = AngleMode_Encoder;         //编码器模式
            
            ForcastAngle = RecToPolar(EnemyDataBuffer[EnemyDataBufferPoint].X, 
                                        EnemyDataBuffer[EnemyDataBufferPoint].Y, 
                                        EnemyDataBuffer[EnemyDataBufferPoint].Z, 
                                        Position.Euler.Pitch,
                                        CloudParam.Pitch.RealEncoderAngle,
                                        1);
            
            CloudParam.Pitch.EncoderTargetAngle = ForcastAngle.V + PitchCenter;
            PitchCurrent = Control_PitchPID();
            
            //Yaw轴速度跟随
            //计算目标角速度大小（编码器单位）
            if(VisionUpdataFlag)
            {
                ForcastOnce(400, 150, &ForcastAngle, 0);        //预判 
                ForcastTarget.Target.H = ForcastAngle.H;
                ForcastTarget.TargetTick = ControlLastTick + 150;
            }
            
            if(ForcastTarget.TargetTick > ControlLastTick)
            {
                if(ForcastTarget.TargetTick - ControlLastTick > 30)
                {
                    ForcastTargetEncoderOmega = ((float)ForcastAngle.H + YawCenter - CloudParam.Yaw.RealEncoderAngle) * 1000 / ((int)ForcastTarget.TargetTick - ControlLastTick);
                    if((ForcastTargetEncoderOmega < 500) && (ForcastTargetEncoderOmega > -500))
                    {
                        ForcastTargetEncoderOmega /= 3.5F;
                    }
                }
            }
            else
            {
                ForcastTargetEncoderOmega = 0;
            }
            
            YawCurrent = VControl_YawPID(ForcastTargetEncoderOmega);
            
            CloudMotorCurrent(PitchCurrent, YawCurrent);
        }
        else
        {
            Cloud_Adjust(0);
            Chassis_MotorDebug();       //电机调试模式，速度0，电流700
            GunFric_Control(0);
        }
        
/*********************  ↑  根据状态机控制  ↑ *********************/
/**************************************************************************************************/
/**********************  ↓  拨弹电机控制   ↓ ***********************/
        PokeMotor_Adjust();
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



