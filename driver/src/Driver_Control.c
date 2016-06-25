
#define __DRIVER_CONTROL_GLOBALS

#include "Config.h"
#include "Driver_DBUS.h"
#include "Driver_Chassis.h"
#include "Driver_Control.h"
#include "Driver_mpu9250.h"
#include "Driver_PokeMotor.h"
#include "Driver_CloudMotor.h"
#include "Driver_SuperGyroscope.h"


/**
  * @brief  云台电机控制初始化
  * @param  void
  * @retval void
  */
void CloudPID_InitConfig(void)
{
//    EncoderTargetPitch = PitchCenter;
//    EncoderTargetYaw = YawCenter;
//    PokeSpeedTarget = 0;
//    ABSTargetPitch = 0;
//    ABSTargetYaw = 0;
    
#if MOTORTYPE == 1      //萨摩
	YawOPID.LastError = 0;
	YawOPID.P = 7;
	YawOPID.I = 0;
	YawOPID.D = 0;
	YawOPID.IMax = 0;
	YawOPID.PIDMax = 5000;
	YawOPID.LastTick = 0;
	
	YawIPID.LastError = 0;
	YawIPID.P = 4;
	YawIPID.I = 0.2;
	YawIPID.D = 0;
	YawIPID.IMax = 100;
	YawIPID.PIDMax = 5000;
	YawIPID.LastTick = 0;
	
	
	PitchOPID.LastError = 0;
	PitchOPID.P = 9;
	PitchOPID.I = 0.02;
	PitchOPID.D = 0;
	PitchOPID.IMax = 500;
	PitchOPID.PIDMax = 8000;
	PitchOPID.LastTick = 0;
	
	PitchIPID.LastError = 0;
	PitchIPID.P = 2;
	PitchIPID.I = 0;
	PitchIPID.D = 0;
	PitchIPID.IMax = 0;
	PitchIPID.PIDMax = 5000;
	PitchIPID.LastTick = 0;
#elif MOTORTYPE == 2        //阿拉斯加
	YawOPID.LastError = 0;
	YawOPID.P = 17;
	YawOPID.I = 0;
	YawOPID.D = 0;
	YawOPID.IMax = 0;
	YawOPID.PIDMax = 5000;
	YawOPID.LastTick = 0;
	
	YawIPID.LastError = 0;
	YawIPID.P = 4;
	YawIPID.I = 0.2;
	YawIPID.D = 0;
	YawIPID.IMax = 100;
	YawIPID.PIDMax = 5000;
	YawIPID.LastTick = 0;
	
	
	PitchOPID.LastError = 0;
	PitchOPID.P = 15;
	PitchOPID.I = 0.02;
	PitchOPID.D = 0;
	PitchOPID.IMax = 500;
	PitchOPID.PIDMax = 8000;
	PitchOPID.LastTick = 0;
	
	PitchIPID.LastError = 0;
	PitchIPID.P = 1.8;
	PitchIPID.I = 0;
	PitchIPID.D = 0;
	PitchIPID.IMax = 0;
	PitchIPID.PIDMax = 5000;
	PitchIPID.LastTick = 0;
#elif MOTORTYPE == 2        //哈士奇
	YawOPID.LastError = 0;
	YawOPID.P = 5;
	YawOPID.I = 0;
	YawOPID.D = 0;
	YawOPID.IMax = 0;
	YawOPID.PIDMax = 5000;
	YawOPID.LastTick = 0;
	
	YawIPID.LastError = 0;
	YawIPID.P = 2;
	YawIPID.I = 0.2;
	YawIPID.D = 0;
	YawIPID.IMax = 100;
	YawIPID.PIDMax = 5000;
	YawIPID.LastTick = 0;
	
	
	PitchOPID.LastError = 0;
	PitchOPID.P = 5;
	PitchOPID.I = 0.02;
	PitchOPID.D = 0;
	PitchOPID.IMax = 500;
	PitchOPID.PIDMax = 8000;
	PitchOPID.LastTick = 0;
	
	PitchIPID.LastError = 0;
	PitchIPID.P = 1;
	PitchIPID.I = 0;
	PitchIPID.D = 0;
	PitchIPID.IMax = 0;
	PitchIPID.PIDMax = 5000;
	PitchIPID.LastTick = 0;
#endif
    
    PokeOPID.CurrentError = 0;
    PokeOPID.LastError = 0;
    PokeOPID.P = 0.2;
    PokeOPID.I = 0;
    PokeOPID.D = 0;
    PokeOPID.IMax = 0;
    PokeOPID.PIDMax = 130;
    PokeOPID.LastTick = 0;
    
    PokeIPID.CurrentError = 0;
    PokeIPID.LastError = 0;
    PokeIPID.P = 0.8;
    PokeIPID.I = 0.15;
    PokeIPID.D = 0.3;
    PokeIPID.IMax = 100;
    PokeIPID.PIDMax = 120;
    PokeIPID.LastTick = 0;
    
    ChassisOPID.CurrentError = 0;
    ChassisOPID.LastError = 0;
    ChassisOPID.P = 11;
    ChassisOPID.I = 0;
    ChassisOPID.D = 0;
    ChassisOPID.IMax = 0;
    ChassisOPID.PIDMax = 500;
    ChassisOPID.LastTick = 0;
    
    ChassisIPID.CurrentError = 0;
    ChassisIPID.LastError = 0;
    ChassisIPID.P = 3;
    ChassisIPID.I = 0;
    ChassisIPID.D = 0;
    ChassisIPID.IMax = 0;
    ChassisIPID.PIDMax = 600;
    ChassisIPID.LastTick = 0;
    
}


/**
  * @brief  Yaw轴PID
  * @param  void
  * @retval 
  */
int16_t Control_YawPID(void)
{
	portTickType CurrentTick = xTaskGetTickCount(); 
/***************************************	外环	******************************************/
	YawOPID.CurrentError = CloudParam.Yaw.EncoderTargetAngle - CloudParam.Yaw.RealEncoderAngle;
	
	YawOPID.Pout = YawOPID.P * YawOPID.CurrentError;
	
	YawOPID.Iout += YawOPID.I * YawOPID.CurrentError;
	YawOPID.Iout = YawOPID.Iout > YawOPID.IMax ? YawOPID.IMax : YawOPID.Iout;
	YawOPID.Iout = YawOPID.Iout < -YawOPID.IMax ? -YawOPID.IMax : YawOPID.Iout;
	
	YawOPID.Dout = -YawOPID.D * (SuperGyoMotorEncoderOmega - Position.MotorEncoderOmega.Z);
	
	YawOPID.PIDout = (YawOPID.Pout + YawOPID.Iout + YawOPID.Dout);
	
	YawOPID.PIDout = YawOPID.PIDout > YawOPID.PIDMax ? YawOPID.PIDMax : YawOPID.PIDout;
	YawOPID.PIDout = YawOPID.PIDout < -YawOPID.PIDMax ? -YawOPID.PIDMax : YawOPID.PIDout;
	
	YawOPID.LastError = YawOPID.CurrentError;
	
/***************************************	内环	******************************************/
	YawIPID.CurrentError = YawOPID.PIDout - (Position.MotorEncoderOmega.Z - SuperGyoMotorEncoderOmega);	
//	YawIPID.CurrentError =  - (Position.MotorEncoderOmega.Z - SuperGyoMotorEncoderOmega);
	
	YawIPID.Pout = YawIPID.P * YawIPID.CurrentError;
	
	YawIPID.Iout += YawIPID.I * YawIPID.CurrentError;
	YawIPID.Iout = YawIPID.Iout > YawIPID.IMax ? YawIPID.IMax : YawIPID.Iout;
	YawIPID.Iout = YawIPID.Iout < -YawIPID.IMax ? -YawIPID.IMax : YawIPID.Iout;
	
	if(YawIPID.LastTick != CurrentTick)
	{
		YawIPID.Dout = YawIPID.D * (YawIPID.CurrentError - YawIPID.LastError) * 5 / (CurrentTick - YawIPID.LastTick);
	}
	else
	{
		YawIPID.Dout = YawIPID.D * (YawIPID.CurrentError - YawIPID.LastError);
	}
	
	YawIPID.PIDout = (YawIPID.Pout + YawIPID.Iout + YawIPID.Dout);
	
	YawIPID.PIDout = YawIPID.PIDout > YawIPID.PIDMax ? YawIPID.PIDMax : YawIPID.PIDout;
	YawIPID.PIDout = YawIPID.PIDout < -YawIPID.PIDMax ? -YawIPID.PIDMax : YawIPID.PIDout;
	
	YawIPID.LastError = YawIPID.CurrentError;
	YawIPID.LastTick = CurrentTick;
	
	return (short)YawIPID.PIDout;
}


/**
  * @brief  Pitch轴PID
  * @param  void
  * @retval void
  */
int16_t Control_PitchPID(void)
{
	portTickType CurrentTick = xTaskGetTickCount(); 
/***************************************	外环	******************************************/

	PitchOPID.CurrentError = CloudParam.Pitch.EncoderTargetAngle - CloudParam.Pitch.RealEncoderAngle;
	
	PitchOPID.Pout = PitchOPID.P * PitchOPID.CurrentError;
	
	PitchOPID.Iout += PitchOPID.I * PitchOPID.CurrentError;
	PitchOPID.Iout = PitchOPID.Iout > PitchOPID.IMax ? PitchOPID.IMax : PitchOPID.Iout;
	PitchOPID.Iout = PitchOPID.Iout < -PitchOPID.IMax ? -PitchOPID.IMax : PitchOPID.Iout;
	
	PitchOPID.Dout = -PitchOPID.D * Position.MotorEncoderOmega.X;
	
	PitchOPID.PIDout = PitchOPID.Pout + PitchOPID.Iout + PitchOPID.Dout;
	
	PitchOPID.PIDout = PitchOPID.PIDout > PitchOPID.PIDMax ? PitchOPID.PIDMax : PitchOPID.PIDout;
	PitchOPID.PIDout = PitchOPID.PIDout < -PitchOPID.PIDMax ? -PitchOPID.PIDMax : PitchOPID.PIDout;
	
	PitchOPID.LastError = PitchOPID.CurrentError;
	
/***************************************	内环	******************************************/
	PitchIPID.CurrentError = PitchOPID.PIDout - Position.MotorEncoderOmega.X;	
//	PitchIPID.CurrentError =  - Position.MotorEncoderOmega.X;
	
	PitchIPID.Pout = PitchIPID.P * PitchIPID.CurrentError;
	
	PitchIPID.Iout += PitchIPID.I * PitchIPID.CurrentError;
	PitchIPID.Iout = PitchIPID.Iout > PitchIPID.IMax ? PitchIPID.IMax : PitchIPID.Iout;
	PitchIPID.Iout = PitchIPID.Iout < -PitchIPID.IMax ? -PitchIPID.IMax : PitchIPID.Iout;
	
	if(YawIPID.LastTick != CurrentTick)
	{
        PitchIPID.Dout = PitchIPID.D * (PitchIPID.CurrentError - PitchIPID.LastError) * 5 / (CurrentTick - PitchIPID.LastTick);
    }
    else
    {
        PitchIPID.Dout = PitchIPID.D * (PitchIPID.CurrentError - PitchIPID.LastError) / (CurrentTick - PitchIPID.LastTick);
    }
	
	PitchIPID.PIDout = (PitchIPID.Pout + PitchIPID.Iout + PitchIPID.Dout);
	
	PitchIPID.PIDout = PitchIPID.PIDout > PitchIPID.PIDMax ? PitchIPID.PIDMax : PitchIPID.PIDout;
	PitchIPID.PIDout = PitchIPID.PIDout < -PitchIPID.PIDMax ? -PitchIPID.PIDMax : PitchIPID.PIDout;
	
	PitchIPID.LastError = PitchIPID.CurrentError;
	PitchIPID.LastTick = CurrentTick;
	
	return (short)PitchIPID.PIDout;
}


/**
  * @brief  拨弹电机PID
  * @param  void
  * @retval 
  */
int16_t Control_PokeIPID(void)
{
    /*****************************  外环  *****************************/
    PokeOPID.CurrentError = PokeMotorParam.TargetLocation - PokeMotorParam.RealLocation;
    
    //误差小30线
    if((PokeOPID.CurrentError < 30) && (PokeOPID.CurrentError > -30))
    {
        PokeMotorParam.Status = PokeChassisParam_Working;
    }
    
    //卡弹反转时高速调节
    if(PokeMotorParam.Status == PokeChassisParam_Stuck)
    {
        PokeOPID.Pout = 3.0F * PokeOPID.P * PokeOPID.CurrentError;
    }
    else
    {
        PokeOPID.Pout = PokeOPID.P * PokeOPID.CurrentError;
    }
    
    PokeOPID.Dout = PokeOPID.D * (PokeOPID.CurrentError - PokeOPID.LastError);
    
    PokeOPID.PIDout = PokeOPID.Pout + PokeOPID.Dout;
    PokeOPID.PIDout = PokeOPID.PIDout > PokeOPID.PIDMax ? PokeOPID.PIDMax : PokeOPID.PIDout;
    PokeOPID.PIDout = PokeOPID.PIDout < -PokeOPID.PIDMax ? -PokeOPID.PIDMax : PokeOPID.PIDout;
    
    PokeOPID.LastError = PokeOPID.CurrentError;
    
    /*****************************  内环  *****************************/
    PokeIPID.CurrentError = PokeOPID.PIDout - PokeMotorParam.RealSpeed;
//    PokeIPID.CurrentError = DBUS_ReceiveData.ch1 - PokeMotorParam.RealSpeed;
    
    PokeIPID.Pout = PokeIPID.P * PokeIPID.CurrentError;
    
    PokeIPID.Iout += PokeIPID.CurrentError * PokeIPID.I;
    PokeIPID.Iout = PokeIPID.Iout > PokeIPID.IMax ? PokeIPID.IMax : PokeIPID.Iout;
    PokeIPID.Iout = PokeIPID.Iout < -PokeIPID.IMax ? -PokeIPID.IMax : PokeIPID.Iout;
    
    //根据积分判断是否卡弹，当积分达到80%上限时认为卡弹,并反转指定角度
    if(((PokeIPID.Iout > 0.9 * PokeIPID.IMax) || (PokeIPID.Iout < -0.9 * PokeIPID.IMax)) && (PokeMotorParam.Status == PokeChassisParam_Working))
    {
        
        PokeMotorParam.Status = PokeChassisParam_Stuck;
        PokeMotorParam.TargetLocation = PokeMotorParam.RealLocation + POKESTRUCKDEALLINES;
    }
    
    PokeIPID.Dout = PokeIPID.D * (PokeIPID.CurrentError - PokeIPID.LastError);
    
    PokeIPID.PIDout = -(PokeIPID.Pout + PokeIPID.Iout + PokeIPID.Dout);
    
    PokeIPID.PIDout = PokeIPID.PIDout > PokeIPID.PIDMax ? PokeIPID.PIDMax : PokeIPID.PIDout;
    PokeIPID.PIDout = PokeIPID.PIDout < -PokeIPID.PIDMax ? -PokeIPID.PIDMax : PokeIPID.PIDout;
    
    PokeIPID.LastError = PokeIPID.CurrentError;
    
    return PokeIPID.PIDout;
}


/**
  * @brief  底盘角速度PID
  * @param  void
  * @retval void
  */
void Control_ChassisPID(void)
{
    /************************************       外环PID       ************************************/
    ChassisOPID.CurrentError = CloudParam.Yaw.ABSTargetAngle - SuperGyoAngle;
    
    ChassisOPID.Pout = ChassisOPID.P * ChassisOPID.CurrentError;
    
    ChassisOPID.PIDout = ChassisOPID.Pout;
    
    ChassisOPID.PIDout = ChassisOPID.PIDout > ChassisOPID.PIDMax ? ChassisOPID.PIDMax : ChassisOPID.PIDout;
    ChassisOPID.PIDout = ChassisOPID.PIDout < -ChassisOPID.PIDMax ? -ChassisOPID.PIDMax : ChassisOPID.PIDout;
    
    /************************************       内环PID       ************************************/
    ChassisIPID.CurrentError = ChassisOPID.PIDout - SuperGyoOmega;
    
    ChassisIPID.Pout = ChassisIPID.P * ChassisIPID.CurrentError;
    
    ChassisIPID.PIDout = -ChassisIPID.Pout;
    
    ChassisIPID.PIDout = ChassisIPID.PIDout > ChassisIPID.PIDMax ? ChassisIPID.PIDMax : ChassisIPID.PIDout;
    ChassisIPID.PIDout = ChassisIPID.PIDout < -ChassisIPID.PIDMax ? -ChassisIPID.PIDMax : ChassisIPID.PIDout;
    
    ChassisParam.Omega = ChassisIPID.PIDout;
}






