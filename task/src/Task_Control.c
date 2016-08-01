
#include "Task_Control.h"

#include "Config.h"

#include "mpu9250dmp.h"
#include "Driver_DBUS.h"
#include "Driver_Judge.h"
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

#include <math.h>


static void Control_KMSubschemaNormal(void);
static void Control_KMSubschemaSupply(void);
static void Control_KMSubschemaHalfauto(void);
static void Control_KMSubschemaSwing(void);
static void Control_KMSubschemaBigsample(void);
static void Control_KMSubschemaFullauto(void);
static void Control_KMSubschemaCircle(void);


//状态切换标志位
static uint8_t JumpToRCFlag = 1, JumpToKMFlag = 1, JumpToProtectFlag = 1;
    
    
/**
  * @brief  控制任务（周期5ms）
  * @param  ubused
  * @retval void
  */
void Task_Control(void *Parameters)
{
    portTickType CurrentControlTick = 0;                //当前系统时间
    uint32_t dmpresetCounter;                            //dmp重启计数器
    
    for(;;)
    {
        CurrentControlTick = xTaskGetTickCount();
        
/************************  ↓  姿态更新  ↓ ************************/
        MPU9250_Update();
        
        //DMP每5s重启，否则会傻逼
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
        
        //Yaw轴实际绝对角度
        CloudParam.Yaw.RealABSAngle = SuperGyoParam.Angle + ((int16_t)CloudParam.Yaw.RealEncoderAngle - YawEncoderCenter) * 0.043945F;
        //Pitch轴实际绝对角度
        CloudParam.Pitch.RealABSAngle = Position.Euler.Pitch;
        
/************************  ↑  姿态更新  ↑ ************************/
/**************************************************************************************************/
/*********************  ↓  状态机状态更新  ↓ *********************/
        StatusMachine_Update();
/*********************  ↑  状态机状态更新  ↑ *********************/
/**************************************************************************************************/
/*********************  ↓  按键按下跳变检测  ↓ *********************/
        DBUS_ButtonCheckJump(CurrentControlTick);
/*********************  ↑  状态机状态更新  ↑ *********************/
/**************************************************************************************************/
/*********************  ↓  摩擦轮  ↓ *********************/
	
        //摩擦轮控制
        if(FricStatus_Working == FricStatus)
        {
            GunFric_Control(1);
#if INFANTRY == 6
            FricArtillerySpeed_Adjust(ARTILLERYFRICSPEED);
#endif
        }
        else if(FricStatus_Crazy == FricStatus)
        {
            GunFric_Control(2);
#if INFANTRY == 6
            FricArtillerySpeed_Adjust(ARTILLERYFRICSPEED);
#endif
        }
        else
        {
            GunFric_Control(0);
#if INFANTRY == 6
            FricArtillerySpeed_Adjust(0);
#endif
        }
/*********************  ↑  状态机状态更新  ↑ *********************/
/**************************************************************************************************/
/*********************  ↓  根据状态机控制  ↓ *********************/
        
        //根据模式控制
        if(ControlMode_RC == ControlMode)
        {
			//由其他模式转入遥控模式
			if(JumpToRCFlag)
			{
                TIM3->CNT = POKEENCODERCenter;              //拨弹电机定时器归位，防止改变模式时转动
				CloudParam.Yaw.TargetABSAngle = SuperGyoParam.Angle;
				PokeMotorParam.TargetLocation = PokeMotorParam.RealLocation;
			}
			
			JumpToRCFlag = 0;
			JumpToKMFlag = 1;
			JumpToProtectFlag = 1;
			
            Control_RCMode();
        }
        else if(ControlMode_KM == ControlMode)
        {
			//由其他模式转入键鼠模式
			if(JumpToKMFlag)
			{
                TIM3->CNT = POKEENCODERCenter;              //拨弹电机定时器归位，防止改变模式时转动
				CloudParam.Yaw.TargetABSAngle = SuperGyoParam.Angle;
				PokeMotorParam.TargetLocation = PokeMotorParam.RealLocation;
			}
			
			JumpToRCFlag = 1;
			JumpToKMFlag = 0;
			JumpToProtectFlag = 1; 

//Debug模式下，此处用于debug，普通模式下用于键鼠控制
#if DEBUGECONTROLRC == 1
            Control_KMSubschemaHalfauto();
#else		
            Control_KMMode();
#endif
        }
        else
        {
			//由其他模式转入保护模式
			if(JumpToProtectFlag)
			{
				CloudParam.Yaw.TargetABSAngle = CloudParam.Yaw.RealABSAngle;
				PokeMotorParam.TargetLocation = PokeMotorParam.RealLocation;
			}
			
			JumpToRCFlag = 1;
			JumpToKMFlag = 1;
			JumpToProtectFlag = 0;
			
            Control_ProtectMode();
        }
        
/*********************  ↑  根据状态机控制  ↑ *********************/
/**************************************************************************************************/
/*********************  ↓  拨弹电机控制   ↓ ***********************/
/*********************  ↑  拨弹电机PID  ↑  ***********************/
/**************************************************************************************************/
/*********************  ↓  云台PID      ↓  **********************/
/*********************  ↑  云台PID      ↑  **********************/
/**************************************************************************************************/
/*********************  ↓   电机控制    ↓  **********************/
/*********************  ↑   电机控制    ↑  **********************/
/**************************************************************************************************/
        
		vTaskDelayUntil(&CurrentControlTick, 5 / portTICK_RATE_MS);
    }
}


/**
  * @breif  遥控控制函数
  * @param  void
  * @retval void
  */
static void Control_RCMode(void)
{
    //云台控制
    Cloud_YawAngleSet(CloudParam.Yaw.TargetABSAngle - DBUS_ReceiveData.ch3 / 500.0F, AngleMode_ABS);
    Cloud_PitchAngleSet(CloudParam.Pitch.TargetABSAngle + DBUS_ReceiveData.ch4 / 120.0F);
    Cloud_Adjust(1);
    
    //底盘控制
    Chassis_TargetDirectionSet(CloudParam.Yaw.TargetABSAngle);
    Chassis_SpeedSet(DBUS_ReceiveData.ch2, DBUS_ReceiveData.ch1);
    Chassis_Adjust(1);
	
    //舵机舱门控制
	Steering_Control(2);
    
#if INFANTRY == 6
    if(DBUS_ReceiveData.switch_right == 2)
    {
        Poke_CylinderAdjust(1);
    }
    else
    {
        Poke_CylinderAdjust(0);
    }
#else
    if(DBUS_ReceiveData.switch_right == 2)
    {
        PokeMotor_Step();
    }
    PokeMotor_Adjust(1);
#endif
    
}


/**
  * @breif  键鼠控制函数
  * @param  void
  * @retval void
  */
static void Control_KMMode(void)
{  
    if(KMSubschema_Normal == KMSubschema)
    {
        Control_KMSubschemaNormal();
    }
    else if(KMSubschema_Supply == KMSubschema)
    {
        Control_KMSubschemaSupply();
    }
    else if(KMSubschema_Halfauto == KMSubschema)
    {
        Control_KMSubschemaHalfauto();
    }
    else if(KMSubschema_Swing == KMSubschema)
    {
        Control_KMSubschemaSwing();
    }
    else if(KMSubschema_Bigsample == KMSubschema)
    {
        Control_KMSubschemaBigsample();
    }
    else if(KMSubschema_Fullauto == KMSubschema)
    {
        Control_KMSubschemaFullauto();
    }
    else if(KMSubschema_Circle == KMSubschema)
    {
        Control_KMSubschemaCircle();
    }
}


/**
  * @breif  保护模式控制函数
  * @param  void
  * @retval void
  */
static void Control_ProtectMode(void)
{
    Cloud_Adjust(0);
    Chassis_Adjust(0);
	Steering_Control(2);
    PokeMotor_Adjust(0);
#if INFANTRY == 6
    FricArtilleryMotorCurrent(0, 0);
#endif
}


/**
  * @brief  键鼠模式子模式——Normal
  * @param  void
  * @retval void
  */
static void Control_KMSubschemaNormal(void)
{
    int8_t Xspeed = 0, Yspeed = 0;
    float TargetYaw;
    
    //云台控制
    if(DBUS_CheckPush(KEY_Q))
    {
        TargetYaw = CloudParam.Yaw.TargetABSAngle + 1.0F;
    }
    else if(DBUS_CheckPush(KEY_E))
    {
        TargetYaw = CloudParam.Yaw.TargetABSAngle - 1.0F;
    }
    else
    {
        TargetYaw = CloudParam.Yaw.TargetABSAngle - DBUS_ReceiveData.mouse.x / 20.0F;
    }
    Cloud_YawAngleSet(TargetYaw, AngleMode_ABS);
    Cloud_PitchAngleSet(CloudParam.Pitch.TargetABSAngle - DBUS_ReceiveData.mouse.y / 20.0F);
    Cloud_Adjust(1);
    
    //底盘控制
    if(DBUS_CheckPush(KEY_W))
    {
        Xspeed = 1;
    }
    else if(DBUS_CheckPush(KEY_S))
    {
        Xspeed = -1;
    }
    else
    {
        Xspeed = 0;
    }
    
    if(DBUS_CheckPush(KEY_D))
    {
        Yspeed = 1;
    }
    else if(DBUS_CheckPush(KEY_A))
    {
        Yspeed = -1;
    }
    else
    {
        Yspeed = 0;
    }
    
    Chassis_TargetDirectionSet(CloudParam.Yaw.TargetABSAngle);
    Chassis_SpeedSet(MAXWORKINGSPEED * Xspeed, MAXWORKINGSPEED * Yspeed);
    Chassis_Adjust(1);
	
    //舵机舱门控制
	Steering_Control(0);
    
#if INFANTRY == 6
    if(DBUS_ReceiveData.mouse.press_left)
    {
        Poke_CylinderAdjust(1);
    }
    else
    {
        Poke_CylinderAdjust(0);
    }
#else
    if(DBUS_ReceiveData.mouse.press_lef)
    {
        PokeMotor_Step();
    }
    PokeMotor_Adjust(1);
#endif
}


/**
  * @brief  键鼠模式子模式——Supply
  * @param  void
  * @retval void
  */
static void Control_KMSubschemaSupply(void)
{
    int8_t Xspeed = 0, Yspeed = 0;
    float TargetYaw;
    
    //云台控制
    if(DBUS_CheckPush(KEY_Q))
    {
        TargetYaw = CloudParam.Yaw.TargetABSAngle + 0.2F;
    }
    else if(DBUS_CheckPush(KEY_E))
    {
        TargetYaw = CloudParam.Yaw.TargetABSAngle - 0.2F;
    }
    else
    {
        TargetYaw = CloudParam.Yaw.TargetABSAngle - DBUS_ReceiveData.mouse.x / 100.0F;
    }
    Cloud_YawAngleSet(TargetYaw, AngleMode_ABS);
    Cloud_PitchAngleSet(DEPOTABSPITCH);
    Cloud_Adjust(1);
    
    //底盘控制
    if(DBUS_CheckPush(KEY_W))
    {
        Xspeed = 1;
    }
    else if(DBUS_CheckPush(KEY_S))
    {
        Xspeed = -1;
    }
    else
    {
        Xspeed = 0;
    }
    
    if(DBUS_CheckPush(KEY_D))
    {
        Yspeed = 1;
    }
    else if(DBUS_CheckPush(KEY_A))
    {
        Yspeed = -1;
    }
    else
    {
        Yspeed = 0;
    }
    
    Chassis_TargetDirectionSet(CloudParam.Yaw.TargetABSAngle);
    Chassis_SpeedSet(SNEAKSPEED * Xspeed, SNEAKSPEED * Yspeed);
    Chassis_Adjust(1);
	
    //舵机舱门控制
	Steering_Control(1);
}


/**
  * @brief  键鼠模式子模式——Halfauto
  * @param  void
  * @retval void
  */
#define LastSpeedLength     6
#define ForcastCloud        0


    AngleF_Struct CurrentAngle;
    AngleF_Struct LastAngle[LastSpeedLength];
    double FeendS = 0;
    float FeedParam = 60;
static void Control_KMSubschemaHalfauto(void)
{
#if ForcastCloud == 1
    
    int8_t index;
    
#if INFANTRY == 6
    if(DBUS_ReceiveData.switch_right == 2)
    {
        Poke_CylinderAdjust(1);
    }
    else
    {
        Poke_CylinderAdjust(0);
    }
#else
    if(DBUS_ReceiveData.switch_right == 2)
    {
        PokeMotor_Step();
    }
    PokeMotor_Adjust(1);
#endif
    
    //底盘控制
    Chassis_Adjust(0);
    
    //预判结果
    ForcastOnce(300, 150, &CurrentAngle, 0);
    
    //云台角度设定
    Cloud_YawAngleSet(SuperGyoParam.Angle + CurrentAngle.H, AngleMode_ABS);
    Cloud_PitchAngleSet(CurrentAngle.V);
    
    //速度补偿计算
    FeendS = (CurrentAngle.H - LastAngle[LastSpeedLength - 1].H);
    
    //云台调节
    Cloud_AutoAdjust(FeendS * FeedParam, 1);
    
    //历史值保存
    for (index = LastSpeedLength - 1; index > 0; index--)
    {
        LastAngle[index] = LastAngle[index - 1];
    }
    LastAngle[0] = CurrentAngle;
    
#else
    
    int8_t index;
    
#if INFANTRY == 6
    if(DBUS_ReceiveData.switch_right == 2)
    {
        Poke_CylinderAdjust(1);
    }
    else
    {
        Poke_CylinderAdjust(0);
    }
#else
    if(DBUS_ReceiveData.switch_right == 2)
    {
        PokeMotor_Step();
    }
    PokeMotor_Adjust(1);
#endif
    
    //底盘控制
    Chassis_Adjust(0);
    
    //角度转换
    CurrentAngle = RecToPolar(EnemyDataBuffer[EnemyDataBufferPoint].X, EnemyDataBuffer[EnemyDataBufferPoint].Y, EnemyDataBuffer[EnemyDataBufferPoint].Z, 0, PitchEncoderCenter, 1);
    
    //云台角度设定
    Cloud_YawAngleSet(SuperGyoParam.Angle + CurrentAngle.H, AngleMode_ABS);
    Cloud_PitchAngleSet(CurrentAngle.V);
    
    //速度补偿计算
    FeendS = (CurrentAngle.H - LastAngle[LastSpeedLength - 1].H);
    
    //云台调节
    Cloud_AutoAdjust(FeendS * FeedParam, 1);
    
    //历史值保存
    for (index = LastSpeedLength - 1; index > 0; index--)
    {
        LastAngle[index] = LastAngle[index - 1];
    }
    LastAngle[0] = CurrentAngle;
#endif
}


/**
  * @brief  键鼠模式子模式——Swing
  * @param  void
  * @retval void
  */
static void Control_KMSubschemaSwing(void)
{
    static uint8_t SwingDir = 0;            //摇摆方向  1左   0右
    int8_t Xspeed = 0, Yspeed = 0;
    float TargetYaw;
    float SwingLimitLeft, SwingLimitRight;
    float TargetRealAngle;
    
    //云台控制
    if(DBUS_CheckPush(KEY_Q))
    {
        TargetYaw = CloudParam.Yaw.TargetABSAngle + 1.0F;
    }
    else if(DBUS_CheckPush(KEY_E))
    {
        TargetYaw = CloudParam.Yaw.TargetABSAngle - 1.0F;
    }
    else
    {
        TargetYaw = CloudParam.Yaw.TargetABSAngle - DBUS_ReceiveData.mouse.x / 20.0F;
    }
    Cloud_YawAngleSet(TargetYaw, AngleMode_ABS);
    Cloud_PitchAngleSet(CloudParam.Pitch.TargetABSAngle - DBUS_ReceiveData.mouse.y / 20.0F);
    Cloud_Adjust(1);
    
    //底盘目标角度设置，用于摇摆
    SwingLimitLeft = CloudParam.Yaw.TargetABSAngle + SWINGMODELIMIT;
    SwingLimitRight = CloudParam.Yaw.TargetABSAngle - SWINGMODELIMIT;
    if(SwingDir)
    {
        if(SwingLimitLeft > SuperGyoParam.Angle)
        {
            Chassis_TargetDirectionSet(ChassisParam.TargetABSAngle + SWINGMODEOMEGA / 200.0F);
        }
        else
        {
            Chassis_TargetDirectionSet(ChassisParam.TargetABSAngle - SWINGMODEOMEGA / 200.0F);
            SwingDir = 0;
        }
    }
    else
    {
        if(SwingLimitRight < SuperGyoParam.Angle)
        {
            Chassis_TargetDirectionSet(ChassisParam.TargetABSAngle - SWINGMODEOMEGA / 200.0F);
        }
        else
        {
            Chassis_TargetDirectionSet(ChassisParam.TargetABSAngle + SWINGMODEOMEGA / 200.0F);
            SwingDir = 1;
        }
    }
     
    //底盘XY速度设置，用于向云台方向前进
    if(DBUS_CheckPush(KEY_W))
    {
        Xspeed = 1;
    }
    else if(DBUS_CheckPush(KEY_S))
    {
        Xspeed = -1;
    }
    else
    {
        Xspeed = 0;
    }
    
    if(DBUS_CheckPush(KEY_D))
    {
        Yspeed = -1;
    }
    else if(DBUS_CheckPush(KEY_A))
    {
        Yspeed = 1;
    }
    else
    {
        Yspeed = 0;
    }
    
    TargetRealAngle = (CloudParam.Yaw.RealABSAngle - SuperGyoParam.Angle) * 0.0174533F;
    Chassis_SpeedSet(SNEAKSPEED * Xspeed * cos(TargetRealAngle) + SNEAKSPEED * Yspeed * sin(TargetRealAngle), 
                    -SNEAKSPEED * Xspeed * sin(TargetRealAngle) - SNEAKSPEED * Yspeed * cos(TargetRealAngle));
    
    Chassis_Adjust(1);
}

  
/**
  * @brief  键鼠模式子模式——Bigsample
  * @param  void
  * @retval void
  */
static void Control_KMSubschemaBigsample(void)
{
    
}


/**
  * @brief  键鼠模式子模式——Fullauto
  * @param  void
  * @retval void
  */
static void Control_KMSubschemaFullauto(void)
{
    
}


/**
  * @brief  键鼠模式子模式——Circle
  * @param  void
  * @retval void
  */
static void Control_KMSubschemaCircle(void)
{
    static int8_t CircleDir = 1;
    
    if(DBUS_CheckPush(KEY_Q | KEY_A))
    {
        CircleDir = 1;
    }
    else if(DBUS_CheckPush(KEY_E | KEY_D))
    {
        CircleDir = -1;
    }
    
    if(xTaskGetTickCount() - InfantryJudge.LastHartTick < CIRCLEMODEDELAY)
    {
        Cloud_YawAngleSet(CloudParam.Yaw.TargetABSAngle + CircleDir * CIRCLEMODEOMEGA / 200.0F, AngleMode_ABS);
    }
    
    Cloud_Adjust(1);
    
    //底盘控制
    Chassis_TargetDirectionSet(CloudParam.Yaw.TargetABSAngle);
    Chassis_SpeedSet(0, 0);
    Chassis_Adjust(1);
}
























