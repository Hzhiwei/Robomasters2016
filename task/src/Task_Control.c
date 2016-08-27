
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

#define ShootDelay          0

#if INFANTRY == 7
static void Control_BaseFullAuto(portTickType Tick);
#else
static void Control_KMMode(portTickType Tick);
static void Control_KMSubschemaNormal(void);
static void Control_KMSubschemaSupply(void);
static void Control_KMSubschemaHalfauto(portTickType Tick);
static void Control_KMSubschemaSwing(void);
static void Control_KMSubschemaBigsample(uint8_t FristEnterFlag, portTickType CurrentTick);
static void Control_KMSubschemaFullauto(portTickType Tick);
static void Control_KMSubschemaAutoCircle(void);
static void Control_KMSubschemaMannualBigsample(uint8_t FristEnterFlag, portTickType CurrentTick);
static void ControlSub_MoveToSample(uint8_t Location[2], float CheckLocaion[2]);
static void Control_KMSubschemaFullCircle(void);
#endif


static void Control_RCMode(void);
static void Control_ProtectMode(void);



#if INFANTRY == 1
//九宫格校准点相对中心位置
static float MannualBigsampleCheckToCentern[2] = {-2.5, 2};

//九宫格相对中心点角度位置
static float MannualBigsampleLocation[3][3][2] = {{{8.68, 5.04}, {0, 5.04}, {-8.68, 5.04}},
                                                    {{8.68, 0}, {0, 0}, {-8.68, 0}},
                                                    {{8.68, -5.04}, {0, -5.04}, {-8.68, -5.04}}};

//九宫格重力偏移角度
static float MannualBigsampleGravityOffset[3] = {0, 0, 0};
#elif INFANTRY == 2
//九宫格校准点相对中心位置
static float MannualBigsampleCheckToCentern[2] = {-2.5, 2};

//九宫格相对中心点角度位置
static float MannualBigsampleLocation[3][3][2] = {{{8.68, 5.04}, {0, 5.04}, {-8.68, 5.04}},
                                                    {{8.68, 0}, {0, 0}, {-8.68, 0}},
                                                    {{8.68, -5.04}, {0, -5.04}, {-8.68, -5.04}}};

//九宫格重力偏移角度
static float MannualBigsampleGravityOffset[3] = {0, 0, 0};
#elif INFANTRY == 3
//九宫格校准点相对中心位置
static float MannualBigsampleCheckToCentern[2] = {-2.5, 2};

//九宫格相对中心点角度位置
static float MannualBigsampleLocation[3][3][2] = {{{8.68, 5.04}, {0, 5.04}, {-8.68, 5.04}},
                                                    {{8.68, 0}, {0, 0}, {-8.68, 0}},
                                                    {{8.68, -5.04}, {0, -5.04}, {-8.68, -5.04}}};

//九宫格重力偏移角度
static float MannualBigsampleGravityOffset[3] = {0, 0, 0};
#elif INFANTRY == 4
//九宫格校准点相对中心位置
static float MannualBigsampleCheckToCentern[2] = {-2.5, 2};

//九宫格相对中心点角度位置
static float MannualBigsampleLocation[3][3][2] = {{{8.68, 5.04}, {0, 5.04}, {-8.68, 5.04}},
                                                    {{8.68, 0}, {0, 0}, {-8.68, 0}},
                                                    {{8.68, -5.04}, {0, -5.04}, {-8.68, -5.04}}};

//九宫格重力偏移角度
static float MannualBigsampleGravityOffset[3] = {0, 0, 0};
#elif INFANTRY == 5
//九宫格校准点相对中心位置
static float MannualBigsampleCheckToCentern[2] = {-4, 0};

//九宫格相对中心点角度位置
static float MannualBigsampleLocation[3][3][2] = {{{8.68, 5.04}, {0, 5.04}, {-8.68, 5.04}},
                                                    {{8.68, 0}, {0, 0}, {-8.68, 0}},
                                                    {{8.68, -5.04}, {0, -5.04}, {-8.68, -5.04}}};

//九宫格重力偏移角度
static float MannualBigsampleGravityOffset[3] = {0, 0, 0};
#endif
                                                


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
        CloudParam.Yaw.RealABSAngle = SuperGyoParam.Angle + ((int16_t)CloudParam.Yaw.RealEncoderAngle - YawEncoderCenter) * 0.043945F * ENCODERTOABSANGLEOFFSETPARAM;
        //Pitch轴实际绝对角度
        CloudParam.Pitch.RealABSAngle = Position.Euler.Pitch;

//#warning pitch Angle is Encoder Data
//        CloudParam.Pitch.RealABSAngle = (CloudParam.Pitch.RealEncoderAngle - PitchEncoderCenter) * 0.04395F;
        
/************************  ↑  姿态更新  ↑ ************************/
/**************************************************************************************************/
/*********************  ↓  状态机状态更新  ↓ *********************/
        StatusMachine_Update(CurrentControlTick);
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
#if FRICTYPE == 1
            FricArtillerySpeed_Adjust(ARTILLERYFRICSPEED);
#else
            FricGunControl(1);
#endif
        }
        else if(FricStatus_Crazy == FricStatus)
        {
#if FRICTYPE == 1
            FricArtillerySpeed_Adjust(ARTILLERYFRICSPEED);
#else
            FricGunControl(2);
#endif
        }
        else
        {
#if FRICTYPE == 1
            FricArtillerySpeed_Adjust(0);
#else
            FricGunControl(0);
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
#if FRICTYPE == 0
				PokeMotorParam.TargetLocation = PokeMotorParam.RealLocation;
#endif
                YawOPID.Iout = 0;
                YawOPID.LastError = 0;
                YawIPID.Iout = 0;
                YawIPID.LastError = 0;
                PitchOPID.Iout = 0;
                PitchOPID.LastError = 0;
                PitchIPID.Iout = 0;
                PitchIPID.LastError = 0;
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
#if FRICTYPE == 0
				PokeMotorParam.TargetLocation = PokeMotorParam.RealLocation;
#endif
			}
			
			JumpToRCFlag = 1;
			JumpToKMFlag = 0;
			JumpToProtectFlag = 1; 

//Debug模式下，此处用于debug，普通模式下用于键鼠控制
#if DEBUGECONTROLRC == 1
            Control_KMSubschemaHalfauto(CurrentControlTick);
//            Control_KMSubschemaBigsample(0, CurrentControlTick);
#else	
            
#if INFANTRY == 7
            Control_BaseFullAuto(CurrentControlTick);
#else
            Control_KMMode(CurrentControlTick);
#endif
            
#endif
        }
        else
        {
			//由其他模式转入保护模式
			if(JumpToProtectFlag)
			{
				CloudParam.Yaw.TargetABSAngle = CloudParam.Yaw.RealABSAngle;
#if FRICTYPE == 0
				PokeMotorParam.TargetLocation = PokeMotorParam.RealLocation;
#endif
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
    
#if INFANTRY != 7
    //底盘控制
    Chassis_TargetDirectionSet(CloudParam.Yaw.TargetABSAngle);
    Chassis_SpeedSet(DBUS_ReceiveData.ch2, DBUS_ReceiveData.ch1);
    Chassis_Adjust(1, 0);
	
    //舵机舱门控制
	Steering_Control(2);
#endif
    
#if FRICTYPE == 1
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
        Poke_MotorStep();
    }
    Poke_MotorAdjust(1);
#endif
}


/**
  * @breif  保护模式控制函数
  * @param  void
  * @retval void
  */
static void Control_ProtectMode(void)
{
    
#if INFANTRY == 7
    Cloud_Adjust(0);
#else
    Cloud_Adjust(0);
//    Chassis_Adjust(0, 0);
	Steering_Control(2);
#endif
    
#if FRICTYPE == 1
    FricArtilleryMotorCurrent(0, 0);
#else
    Poke_MotorAdjust(0);
#endif
}



#if INFANTRY == 7
/**
  * @brief  基地自动射击函数
  * @param  void
  * @retval void
  */
#define LastParam           7


static void Control_BaseFullAuto(portTickType Tick)
{
    int8_t index;
    uint8_t ShootSpeed;
    static uint8_t FristFindTarget = 1;
    float distance = sqrt(EnemyDataBuffer[EnemyDataBufferPoint].Z * EnemyDataBuffer[EnemyDataBufferPoint].Z + EnemyDataBuffer[EnemyDataBufferPoint].Y * EnemyDataBuffer[EnemyDataBufferPoint].Y);
    static AngleF_Struct CurrentAngle;
    static float FeedParam = 40;
    static double FeendS = 0;
    static AngleF_Struct LastAngle[LastParam * 2 + 1];
    static uint8_t LastDataNum = 0;     //此变量用于解决视觉帧率低于控制帧率导致多次在控制函数中调节底盘时由于视觉帧未更新导致角度错误问题
    
     //长时间未出子弹认为子弹用光
     if(((SHOOTUSEOFFTICK <= InfantryJudge.ShootFail) && (SHOOTUSEOFFNUM <= InfantryJudge.ShootNum)) || (SHOOTUSEOFFTICK * 2 <= InfantryJudge.ShootFail))
     {
         InfantryJudge.BulletUseUp = 1;
     }
     
     //子弹未用光，自动射击
     if(InfantryJudge.BulletUseUp == 0)
     {
        //预判结果
        ForcastOnce(300, 80, &CurrentAngle, 0);
        
        //云台角度设定
        Cloud_YawAngleSet(CurrentAngle.H, AngleMode_OPP);
        Cloud_PitchAngleSet(CurrentAngle.V);
        
        //发射频率控制
        if(distance < 0.4)
        {
            ShootSpeed = 30;
        }
        else if(distance < AUTOSHOTDISTANCE)
        {
            ShootSpeed = -distance * 5.36 + 32.152;
        }
        else
        {
            ShootSpeed = 10;
        }
        
        if((EnemyDataBuffer[EnemyDataBufferPoint].Z < AUTOSHOTDISTANCE) &&              //指定距离内
            (CurrentAngle.H < AUTOSHOTANGLE) && (CurrentAngle.H > -AUTOSHOTANGLE) &&    //指定角度内
            (Tick - EnemyDataBuffer[(EnemyDataBufferPoint + ENEMYDATABUFFERLENGHT - 30) % ENEMYDATABUFFERLENGHT].Tick < 2000))      //指定时间内
        {
            Poke_MotorSpeedAdjust(1, ShootSpeed);
            InfantryJudge.ShootFail++;      //发射失败时间自增
        }
        else
        {
            Poke_MotorSpeedAdjust(0, ShootSpeed);
        }
        
        //底盘旋转判断
        if(Tick - EnemyDataBuffer[(EnemyDataBufferPoint + ENEMYDATABUFFERLENGHT - 1) % ENEMYDATABUFFERLENGHT].Tick < 250)      //指定时间内
        {
            if(FristFindTarget)
            {
                if(LastDataNum != EnemyDataBufferPoint)
                {
                    Chassis_BaseControl(2,  - atan(EnemyDataBuffer[EnemyDataBufferPoint].X / EnemyDataBuffer[EnemyDataBufferPoint].Z) * 57.2958F + SuperGyoParam.Angle + SuperGyoParam.Offset);   
                }
            }
            else
            {
                if((CurrentAngle.H < AUTOSHOTANGLE) && (CurrentAngle.H > -AUTOSHOTANGLE))          //指定角度内
                {
                    Chassis_BaseControl(0, 0);
                }
                else
                {
                    if(LastDataNum != EnemyDataBufferPoint)
                    {
                        Chassis_BaseControl(2,  - atan(EnemyDataBuffer[EnemyDataBufferPoint].X / EnemyDataBuffer[EnemyDataBufferPoint].Z) * 57.2958F + SuperGyoParam.Angle + SuperGyoParam.Offset);
                    }
                }
            }
            FristFindTarget = 0;
        }
        else
        {
            FristFindTarget = 1;
            Chassis_BaseControl(1, 0);
        }
        
        //速度补偿计算
        FeendS = (CurrentAngle.H - LastAngle[LastParam * 2].H);
        
        //云台调节
        Cloud_AutoAdjust(FeendS * FeedParam, 1);
        
        //历史值保存
        for (index = LastParam * 2; index > 0; index--)
        {
            LastAngle[index] = LastAngle[index - 1];
        }
        LastAngle[0] = CurrentAngle;
    }
    else
    {
        Chassis_BaseControl(3, 0);
        Cloud_YawAngleSet(0, AngleMode_OPP);
        Cloud_PitchAngleSet(0);
        Cloud_Adjust(1);
        Poke_MotorSpeedAdjust(0, ShootSpeed);
    }
}


#else


/**
  * @breif  键鼠控制函数
  * @param  void
  * @retval void
  */
static void Control_KMMode(portTickType Tick)
{
    static uint8_t FristEnter[9] = {1, 1, 1, 1, 1, 1, 1, 1, 1};
    static KMSubschema_Enum LastKMSubschema = KMSubschema_Normal;
    
    if((LastKMSubschema == KMSubschema_Supply) && (KMSubschema != KMSubschema_Supply))
    {
        ChassisParam.TargetABSAngle = SuperGyoParam.Angle;
    }
    
    if(KMSubschema_Normal == KMSubschema)
    {
        FristEnter[1] = 1;
        FristEnter[2] = 1;
        FristEnter[3] = 1;
        FristEnter[4] = 1;
        FristEnter[5] = 1;
        FristEnter[6] = 1;
        FristEnter[7] = 1;
        FristEnter[8] = 1;
        
        Control_KMSubschemaNormal();
        
        FristEnter[0] = 0;
    }
    else if(KMSubschema_Supply == KMSubschema)
    {
        FristEnter[0] = 1;
        FristEnter[2] = 1;
        FristEnter[3] = 1;
        FristEnter[4] = 1;
        FristEnter[5] = 1;
        FristEnter[6] = 1;
        FristEnter[7] = 1;
        FristEnter[8] = 1;
        
        Control_KMSubschemaSupply();
        
        FristEnter[1] = 0;
    }
    else if(KMSubschema_Halfauto == KMSubschema)
    {
        FristEnter[0] = 1;
        FristEnter[1] = 1;
        FristEnter[3] = 1;
        FristEnter[4] = 1;
        FristEnter[5] = 1;
        FristEnter[6] = 1;
        FristEnter[7] = 1;
        FristEnter[8] = 1;
        
        Control_KMSubschemaHalfauto(Tick);
        
        FristEnter[2] = 0;
    }
    else if(KMSubschema_Swing == KMSubschema)
    {
        FristEnter[0] = 1;
        FristEnter[1] = 1;
        FristEnter[2] = 1;
        FristEnter[4] = 1;
        FristEnter[5] = 1;
        FristEnter[6] = 1;
        FristEnter[7] = 1;
        FristEnter[8] = 1;
        
        Control_KMSubschemaSwing();
        
        FristEnter[3] = 0;
    }
    else if(KMSubschema_Bigsample == KMSubschema)
    {
        FristEnter[0] = 1;
        FristEnter[1] = 1;
        FristEnter[2] = 1;
        FristEnter[3] = 1;
        FristEnter[5] = 1;
        FristEnter[6] = 1;
        FristEnter[7] = 1;
        FristEnter[8] = 1;
        
        Control_KMSubschemaBigsample(FristEnter[4], Tick);
        
        FristEnter[4] = 0;
    }
    else if(KMSubschema_Fullauto == KMSubschema)
    {
        FristEnter[0] = 1;
        FristEnter[1] = 1;
        FristEnter[2] = 1;
        FristEnter[3] = 1;
        FristEnter[4] = 1;
        FristEnter[6] = 1;
        FristEnter[7] = 1;
        FristEnter[8] = 1;
        
        Control_KMSubschemaFullauto(Tick);
        
        FristEnter[5] = 0;
    }
    else if(KMSubschema_AutoCircle == KMSubschema)
    {
        FristEnter[0] = 1;
        FristEnter[1] = 1;
        FristEnter[2] = 1;
        FristEnter[3] = 1;
        FristEnter[4] = 1;
        FristEnter[5] = 1;
        FristEnter[7] = 1;
        FristEnter[8] = 1;
        
        Control_KMSubschemaAutoCircle();
        
        FristEnter[6] = 0;
    }
    else if(KMSubschema_MannualBigsample == KMSubschema)
    {
        FristEnter[0] = 1;
        FristEnter[1] = 1;
        FristEnter[2] = 1;
        FristEnter[3] = 1;
        FristEnter[4] = 1;
        FristEnter[5] = 1;
        FristEnter[6] = 1;
        FristEnter[8] = 1;
        
        Control_KMSubschemaMannualBigsample(FristEnter[7], Tick);
        
        FristEnter[7] = 0;
    }
    else if(KMSubschema_FullCircle == KMSubschema)
    {
        FristEnter[0] = 1;
        FristEnter[1] = 1;
        FristEnter[2] = 1;
        FristEnter[3] = 1;
        FristEnter[4] = 1;
        FristEnter[5] = 1;
        FristEnter[6] = 1;
        FristEnter[7] = 1;
        
        Control_KMSubschemaFullCircle();
        
        FristEnter[8] = 0;
    }
    
    LastKMSubschema = KMSubschema;
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
        TargetYaw = CloudParam.Yaw.TargetABSAngle + QESPINSPEED;
    }
    else if(DBUS_CheckPush(KEY_E))
    {
        TargetYaw = CloudParam.Yaw.TargetABSAngle - QESPINSPEED;
    }
    else
    {
        TargetYaw = CloudParam.Yaw.TargetABSAngle - MOUSESPINSPEED * DBUS_ReceiveData.mouse.x / 1000.0F;
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
    if(DBUS_ReceiveData.mouse.press_right)
    {
        //低速模式
        Chassis_SpeedSet(MAXWORKINGSPEED * Xspeed * 0.1, MAXWORKINGSPEED * Yspeed * 0.1);
    }
    else
    {
        Chassis_SpeedSet(MAXWORKINGSPEED * Xspeed, MAXWORKINGSPEED * Yspeed);
    }
    Chassis_Adjust(1, 0);
	
    //舵机舱门控制
	Steering_Control(0);
    
    
#if FRICTYPE == 1
    if(DBUS_ReceiveData.switch_right == 3)
    {
        if(DBUS_CheckJumpMouse(0))
        {
            Poke_CylinderAdjust(1);
        }
        else
        {
            Poke_CylinderAdjust(0);
        }
    }
#else  
    if(DBUS_ReceiveData.switch_right == 3)
    { 
        if(DBUS_ReceiveData.mouse.press_left)
        {
            Poke_MotorStep();
        }
        Poke_MotorAdjust(1);
    }
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
//    float TargetYaw;
    
    //云台控制
//    if(DBUS_CheckPush(KEY_Q))
//    {
//        TargetYaw = CloudParam.Yaw.TargetABSAngle + 0.2F;
//    }
//    else if(DBUS_CheckPush(KEY_E))
//    {
//        TargetYaw = CloudParam.Yaw.TargetABSAngle - 0.2F;
//    }
//    else
//    {
//        TargetYaw = CloudParam.Yaw.TargetABSAngle - MOUSESPINSPEED * DBUS_ReceiveData.mouse.x / 2000.0F;
//    }
    Cloud_YawAngleSet(SuperGyoParam.Angle, AngleMode_ABS);
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
    
    //补给站模式不进行角度闭环，手动设置角速度
    ChassisParam.TargetOmega = DBUS_ReceiveData.mouse.x * 2;
    Chassis_SpeedSet(SNEAKSPEED * Xspeed, SNEAKSPEED * Yspeed);
    Chassis_Adjust(1, 0);
	
    //舵机舱门控制
	Steering_Control(1);
}


/**
  * @brief  键鼠模式子模式——Halfauto
  * @param  void
  * @retval void
  */
#define LastParam           7
    static uint8_t FristFindTarget = 1;
static void Control_KMSubschemaHalfauto(portTickType Tick)
{
    int8_t index;
    float distance = sqrt(EnemyDataBuffer[EnemyDataBufferPoint].Z * EnemyDataBuffer[EnemyDataBufferPoint].Z + EnemyDataBuffer[EnemyDataBufferPoint].Y * EnemyDataBuffer[EnemyDataBufferPoint].Y);
    static AngleF_Struct CurrentAngle;
    static uint8_t LastDataNum = 0;     //此变量用于解决视觉帧率低于控制帧率导致多次在控制函数中调节底盘时由于视觉帧未更新导致角度错误问题
    
#if INFANTRY == 1
    static float FeedParam = 15;
#else
    static float FeedParam = 40;
#endif
    
    
    static double FeendS = 0;
    static AngleF_Struct LastAngle[LastParam * 2 + 1];
    
    //底盘旋转判断
    if(Tick - EnemyDataBuffer[(EnemyDataBufferPoint + ENEMYDATABUFFERLENGHT - 5) % ENEMYDATABUFFERLENGHT].Tick < 500)      //指定时间内
    {
        if(FristFindTarget)
        {
            if(LastDataNum != EnemyDataBufferPoint)
            {
                Chassis_TargetDirectionSet(- atan(EnemyDataBuffer[EnemyDataBufferPoint].X / EnemyDataBuffer[EnemyDataBufferPoint].Z) * 57.2958F + SuperGyoParam.Angle);   
            }
        }
        else
        {
            if((CurrentAngle.H < AUTOSHOTANGLE) && (CurrentAngle.H > -AUTOSHOTANGLE))          //指定角度内
            {
                ;
            }
            else
            {
                if(LastDataNum != EnemyDataBufferPoint)
                {
                    Chassis_TargetDirectionSet(- atan(EnemyDataBuffer[EnemyDataBufferPoint].X / EnemyDataBuffer[EnemyDataBufferPoint].Z) * 57.2958F + SuperGyoParam.Angle);
                }
            }
        }
        FristFindTarget = 0;
    }
    else
    {
        FristFindTarget = 1;
    }
    Chassis_SpeedSet(0, 0);
    Chassis_Adjust(1, 0);
    
    //预判
    ForcastOnce(300, 80, &CurrentAngle, 0);
    
    //云台角度设定
    Cloud_YawAngleSet(CurrentAngle.H, AngleMode_OPP);
    Cloud_PitchAngleSet(CurrentAngle.V);
    
    //发射判断
    if(DBUS_ReceiveData.switch_right == 3)
    {
        if(DBUS_ReceiveData.mouse.press_left)
        {
            Poke_MotorStep();
        }
    }
    Poke_MotorAdjust(1);
    
    //速度补偿计算
    FeendS = (CurrentAngle.H - LastAngle[LastParam * 2].H);
    
    //云台调节
    Cloud_AutoAdjust(FeendS * (FeedParam + 20 * EnemyDataBuffer[EnemyDataBufferPoint].Z / 3), 1);
    
    //历史值保存
    for (index = LastParam * 2; index > 0; index--)
    {
        LastAngle[index] = LastAngle[index - 1];
    }
    LastAngle[0] = CurrentAngle;
    
    LastDataNum = EnemyDataBufferPoint;
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
        TargetYaw = CloudParam.Yaw.TargetABSAngle + QESPINSPEED;
    }
    else if(DBUS_CheckPush(KEY_E))
    {
        TargetYaw = CloudParam.Yaw.TargetABSAngle - QESPINSPEED;
    }
    else
    {
        TargetYaw = CloudParam.Yaw.TargetABSAngle - MOUSESPINSPEED * DBUS_ReceiveData.mouse.x / 1000.0F;
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
    
    
#if FRICTYPE == 1
    if(DBUS_ReceiveData.switch_right == 3)
    {
        if(DBUS_CheckJumpMouse(0))
        {
            Poke_CylinderAdjust(1);
        }
        else
        {
            Poke_CylinderAdjust(0);
        }
    }
#else  
    if(DBUS_ReceiveData.switch_right == 3)
    { 
        if(DBUS_ReceiveData.mouse.press_left)
        {
            Poke_MotorStep();
        }
        Poke_MotorAdjust(1);
    }
#endif
    
    TargetRealAngle = (CloudParam.Yaw.RealABSAngle - SuperGyoParam.Angle) * 0.0174533F;
    Chassis_SpeedSet(SNEAKSPEED * Xspeed * cos(TargetRealAngle) + SNEAKSPEED * Yspeed * sin(TargetRealAngle), 
                    -SNEAKSPEED * Xspeed * sin(TargetRealAngle) - SNEAKSPEED * Yspeed * cos(TargetRealAngle));
    
    Chassis_Adjust(1, 0);
}

  
/**
  * @brief  键鼠模式子模式——Bigsample
  * @param  void
  * @retval void
  */
static void Control_KMSubschemaBigsample(uint8_t FristEnterFlag, portTickType Tick)
{
    AngleF_Struct CurrentAngle;
    static AngleF_Struct UsefulCurrentAngle;
    static uint16_t LastTimeStamp = 254;
    static uint16_t LLastTimeStamp = 255;
    static uint16_t LLLastTimeStamp = 256;
    static uint16_t LLLLastTimeStamp = 257;
    static portTickType LastBigsampleShotTick = 0;
    
    static float OffsetX = 0, OffsetY = 0;
    
    //鼠标偏移
    OffsetX -= DBUS_ReceiveData.mouse.x * BIGSAMPLEOFFSETXPARAM;
    OffsetY -= DBUS_ReceiveData.mouse.y * BIGSAMPLEOFFSETYPARAM;
    
    //右键归零所有偏移
    if(DBUS_ReceiveData.mouse.press_right)
    {
        OffsetX = 0;
        OffsetY = 0;
    }
    
    //防疯转
    if(FristEnterFlag)
    {
        EnemyDataBuffer[EnemyDataBufferPoint].X = 0,
        EnemyDataBuffer[EnemyDataBufferPoint].Y = 0;
        EnemyDataBuffer[EnemyDataBufferPoint].Z = 1;
    }
    
    //角度转换
    CurrentAngle = RecToPolar(EnemyDataBuffer[EnemyDataBufferPoint].X, EnemyDataBuffer[EnemyDataBufferPoint].Y, EnemyDataBuffer[EnemyDataBufferPoint].Z, 0, PitchEncoderCenter, 1);
    
    //云台角度设定及射击
    if(/*(LLLLastTimeStamp != LLLastTimeStamp) &&
        (LLLastTimeStamp != LLastTimeStamp) &&  */
        (LLastTimeStamp != LastTimeStamp) && 
        (LastTimeStamp == EnemyDataBuffer[EnemyDataBufferPoint].TimeStamp))
    {
        UsefulCurrentAngle = CurrentAngle;
        
        //新目标出现
        if((DBUS_ReceiveData.switch_right == 3) && (DBUS_ReceiveData.mouse.press_left))
        {
            Poke_MotorStep();
        }
    }
    Cloud_YawAngleSet(SuperGyoParam.Angle + UsefulCurrentAngle.H + OffsetX, AngleMode_ABS);
    Cloud_PitchAngleSet(UsefulCurrentAngle.V + OffsetY);
    Cloud_Adjust(1);
    Poke_MotorAdjust(1);
    
    LLLLastTimeStamp = LLLastTimeStamp;
    LLLastTimeStamp = LLastTimeStamp;
    LLastTimeStamp = LastTimeStamp;
    LastTimeStamp = EnemyDataBuffer[EnemyDataBufferPoint].TimeStamp;
}


/**
  * @brief  键鼠模式子模式——Fullauto
  * @param  void
  * @retval void
  */
static void Control_KMSubschemaFullauto(portTickType Tick)
{
    Control_KMSubschemaHalfauto(Tick);
}


/**
  * @brief  键鼠模式子模式——Circle
  * @param  void
  * @retval void
  */
static void Control_KMSubschemaAutoCircle(void)
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
    Chassis_Adjust(1, 0);
}


/**
  * @brief  手动大符模式
  * @param  1 第一次进入此模式      0 非第一次
  * @param  系统时间
  * @retval void
  */
static void Control_KMSubschemaMannualBigsample(uint8_t FristEnterFlag, portTickType CurrentTick)
{
    static float CheckLocaion[2] = {0, 0};
    static uint8_t KEYLocation[2] = {1, 1};
    static int16_t ShootCounter = -1;
    
    //获取校准位置
    if(FristEnterFlag)
    {
        CheckLocaion[0] = SuperGyoParam.Angle;
        CheckLocaion[1] = Position.Euler.Pitch;
    }
    
    //获取键盘位置
    if(DBUS_CheckJumpKey(KEY_Q))
    {
        KEYLocation[0] = 0;
        KEYLocation[1] = 0;
        if((DBUS_ReceiveData.switch_right == 3) && (DBUS_ReceiveData.mouse.press_left))
        {
            ShootCounter = ShootDelay;
        }
    }
    else if(DBUS_CheckJumpKey(KEY_W))
    {
        KEYLocation[0] = 1;
        KEYLocation[1] = 0;
        if((DBUS_ReceiveData.switch_right == 3) && (DBUS_ReceiveData.mouse.press_left))
        {
            ShootCounter = ShootDelay;
        }
    }
    else if(DBUS_CheckJumpKey(KEY_E))
    {
        KEYLocation[0] = 2;
        KEYLocation[1] = 0;
        if((DBUS_ReceiveData.switch_right == 3) && (DBUS_ReceiveData.mouse.press_left))
        {
            ShootCounter = ShootDelay;
        }
    }
    else if(DBUS_CheckJumpKey(KEY_A))
    {
        KEYLocation[0] = 0;
        KEYLocation[1] = 1;
        if((DBUS_ReceiveData.switch_right == 3) && (DBUS_ReceiveData.mouse.press_left))
        {
            ShootCounter = ShootDelay;
        }
    }
    else if(DBUS_CheckJumpKey(KEY_S))
    {
        KEYLocation[0] = 1;
        KEYLocation[1] = 1;
        if((DBUS_ReceiveData.switch_right == 3) && (DBUS_ReceiveData.mouse.press_left))
        {
            ShootCounter = ShootDelay;
        }
    }
    else if(DBUS_CheckJumpKey(KEY_D))
    {
        KEYLocation[0] = 2;
        KEYLocation[1] = 1;
        if((DBUS_ReceiveData.switch_right == 3) && (DBUS_ReceiveData.mouse.press_left))
        {
            ShootCounter = ShootDelay;
        }
    }
    else if(DBUS_CheckJumpKey(KEY_Z))
    {
        KEYLocation[0] = 0;
        KEYLocation[1] = 2;
        if((DBUS_ReceiveData.switch_right == 3) && (DBUS_ReceiveData.mouse.press_left))
        {
            ShootCounter = ShootDelay;
        }
    }
    else if(DBUS_CheckJumpKey(KEY_X))
    {
        KEYLocation[0] = 1;
        KEYLocation[1] = 2;
        if((DBUS_ReceiveData.switch_right == 3) && (DBUS_ReceiveData.mouse.press_left))
        {
            ShootCounter = ShootDelay;
        }
    }
    else if(DBUS_CheckJumpKey(KEY_C))
    {
        KEYLocation[0] = 2;
        KEYLocation[1] = 2;
        if((DBUS_ReceiveData.switch_right == 3) && (DBUS_ReceiveData.mouse.press_left))
        {
            ShootCounter = ShootDelay;
        }
    }
    
    if(ShootCounter > 0)
    {
        --ShootCounter;
    }
    else if(ShootCounter == 0)
    {
        --ShootCounter;
        Poke_MotorStep();
    }
    Poke_MotorAdjust(1);
    
    //指向指定角度
    ControlSub_MoveToSample(KEYLocation, CheckLocaion);
    
    //底盘锁定
    Chassis_Adjust(1, 0);
}


/**
  * @brief  手动大符指向指定符点
  * @param  符点位置
  * @param  校准位置
  * @retval void
  */
static void ControlSub_MoveToSample(uint8_t Location[2], float CheckLocaion[2])
{
    float TargetABSLocaion[2];
    static float OffsetX = 0, OffsetY = 0;
    
    //鼠标偏移
    OffsetX -= DBUS_ReceiveData.mouse.x * BIGSAMPLEOFFSETXPARAM;
    OffsetY -= DBUS_ReceiveData.mouse.y * BIGSAMPLEOFFSETYPARAM;
    
    //右键归零所有偏移
    if(DBUS_ReceiveData.mouse.press_right)
    {
        OffsetX = 0;
        OffsetY = 0;
    }
    
    //获取实际位置
    TargetABSLocaion[0] = CheckLocaion[0] - MannualBigsampleCheckToCentern[0] + MannualBigsampleLocation[Location[1]][Location[0]][0] + OffsetX;
    TargetABSLocaion[1] = CheckLocaion[1] - MannualBigsampleCheckToCentern[1] + MannualBigsampleLocation[Location[1]][Location[0]][1] + MannualBigsampleGravityOffset[Location[1]] +OffsetY;
    
    //云台位置控制
    Cloud_YawAngleSet(TargetABSLocaion[0], AngleMode_ABS);
    Cloud_PitchAngleSet(TargetABSLocaion[1]);
    Cloud_Adjust(1);
}


/**
  * @brief  全速自旋模式
  * @param  void
  * @retval void
  */
static void Control_KMSubschemaFullCircle(void)
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
    
    Cloud_YawAngleSet(CloudParam.Yaw.TargetABSAngle + CircleDir * CIRCLEMODEOMEGA / 200.0F, AngleMode_ABS);
    
    Cloud_Adjust(1);
    
    //底盘控制
    Chassis_TargetDirectionSet(CloudParam.Yaw.TargetABSAngle);
    Chassis_SpeedSet(0, 0);
    Chassis_Adjust(1, 0);
}

#endif









