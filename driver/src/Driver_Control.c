
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
#if INFANTRY == 1       //萨摩
    
#elif INFANTRY == 2     //阿拉斯加
    
    PitchOPID.P = 9;
    PitchOPID.I = 0.5;
    PitchOPID.D = 0;
    PitchOPID.CurrentError = 0;
    PitchOPID.LastError = 0;
    PitchOPID.LastTick = 0;
    PitchOPID.IMax = 10;
    PitchOPID.PIDMax = 400;
    
    PitchIPID.P = 50;
    PitchIPID.I = 5;
    PitchIPID.D = 0;
    PitchIPID.CurrentError = 0;
    PitchIPID.LastError = 0;
    PitchIPID.LastTick = 0;
    PitchIPID.IMax = 300;
    PitchIPID.PIDMax = 5000;
    
    YawOPID.P = 16;
    YawOPID.I = 0.1;
    YawOPID.D = 0;
    YawOPID.CurrentError = 0;
    YawOPID.LastError = 0;
    YawOPID.LastTick = 0;
    YawOPID.IMax = 5;
    YawOPID.PIDMax = 300;
    
    YawIPID.P = 80;
    YawIPID.I = 5;
    YawIPID.D = 0;
    YawIPID.CurrentError = 0;
    YawIPID.LastError = 0;
    YawIPID.LastTick = 0;
    YawIPID.IMax = 100;
    YawIPID.PIDMax = 5000;
    
    ChassisOPID.P = 1.4;
    ChassisOPID.I = 0;
    ChassisOPID.D = 0;
    ChassisOPID.CurrentError = 0;
    ChassisOPID.LastError = 0;
    ChassisOPID.LastTick = 0;
    ChassisOPID.IMax = 0;
    ChassisOPID.PIDMax = 300;
    
    ChassisIPID.P = 12;
    ChassisIPID.I = 0;
    ChassisIPID.D = 0;
    ChassisIPID.CurrentError = 0;
    ChassisIPID.LastError = 0;
    ChassisIPID.LastTick = 0;
    ChassisIPID.IMax = 0;
    ChassisIPID.PIDMax = 950;
    
    PokeOPID.CurrentError = 0;
    PokeOPID.LastError = 0;
    PokeOPID.P = 0.07;
    PokeOPID.I = 0;
    PokeOPID.D = 0;
    PokeOPID.IMax = 0;
    PokeOPID.PIDMax = 130;
    PokeOPID.LastTick = 0;
    
    PokeIPID.CurrentError = 0;
    PokeIPID.LastError = 0;
    PokeIPID.P = 3.8;
    PokeIPID.I = 0.2;
    PokeIPID.D = 0;
    PokeIPID.IMax = 200;
    PokeIPID.PIDMax = 90;
    PokeIPID.LastTick = 0;
    
#elif INFANTRY == 3     //哈士奇
    
#elif INFANTRY == 4     //边牧（没名字，先这么叫吧）
    
#elif INFANTRY == 5     //狗蛋
    
#elif INFANTRY == 6     //英雄
    
#endif
}


/**
  * @note   modified
  * @brief  Yaw轴PID
  * @param  void
  * @retval 
  */
int16_t Control_YawPID(void)
{
	portTickType CurrentTick = xTaskGetTickCount(); 
/***************************************	外环	******************************************/
	YawOPID.CurrentError = CloudParam.Yaw.TargetABSAngle - CloudParam.Yaw.RealABSAngle;
	
	YawOPID.Pout = YawOPID.P * YawOPID.CurrentError;
	
	YawOPID.Iout += YawOPID.I * YawOPID.CurrentError;
	YawOPID.Iout = YawOPID.Iout > YawOPID.IMax ? YawOPID.IMax : YawOPID.Iout;
	YawOPID.Iout = YawOPID.Iout < -YawOPID.IMax ? -YawOPID.IMax : YawOPID.Iout;
	
	YawOPID.Dout = -YawOPID.D * (SuperGyoParam.Omega - Position.Real.OZ);
	
	YawOPID.PIDout = (YawOPID.Pout + YawOPID.Iout + YawOPID.Dout);
	
	YawOPID.PIDout = YawOPID.PIDout > YawOPID.PIDMax ? YawOPID.PIDMax : YawOPID.PIDout;
	YawOPID.PIDout = YawOPID.PIDout < -YawOPID.PIDMax ? -YawOPID.PIDMax : YawOPID.PIDout;
	
	YawOPID.LastError = YawOPID.CurrentError;
	
/***************************************	内环	******************************************/
	YawIPID.CurrentError = YawOPID.PIDout - (Position.Real.OZ - SuperGyoParam.Omega);	
//	YawIPID.CurrentError = -DBUS_ReceiveData.ch3 - (Position.Real.OZ - SuperGyoParam.Omega);
	
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
  * @note   modified
  * @brief  Pitch轴PID
  * @param  void
  * @retval void
  */
int16_t Control_PitchPID(void)
{
	portTickType CurrentTick = xTaskGetTickCount(); 
/***************************************	外环	******************************************/

	PitchOPID.CurrentError = CloudParam.Pitch.TargetABSAngle - CloudParam.Pitch.RealABSAngle;
	
	PitchOPID.Pout = PitchOPID.P * PitchOPID.CurrentError;
	
	PitchOPID.Iout += PitchOPID.I * PitchOPID.CurrentError;
	PitchOPID.Iout = PitchOPID.Iout > PitchOPID.IMax ? PitchOPID.IMax : PitchOPID.Iout;
	PitchOPID.Iout = PitchOPID.Iout < -PitchOPID.IMax ? -PitchOPID.IMax : PitchOPID.Iout;
	
	PitchOPID.Dout = -PitchOPID.D * Position.Real.OX;
	
	PitchOPID.PIDout = PitchOPID.Pout + PitchOPID.Iout + PitchOPID.Dout;
	
	PitchOPID.PIDout = PitchOPID.PIDout > PitchOPID.PIDMax ? PitchOPID.PIDMax : PitchOPID.PIDout;
	PitchOPID.PIDout = PitchOPID.PIDout < -PitchOPID.PIDMax ? -PitchOPID.PIDMax : PitchOPID.PIDout;
	
	PitchOPID.LastError = PitchOPID.CurrentError;
	
/***************************************	内环	******************************************/
	PitchIPID.CurrentError = PitchOPID.PIDout - Position.Real.OX;	
//	PitchIPID.CurrentError = DBUS_ReceiveData.ch4 - Position.Real.OX;
	
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
    
    PokeOPID.Pout = PokeOPID.P * PokeOPID.CurrentError;
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
    
    PokeIPID.Dout = PokeIPID.D * (PokeIPID.CurrentError - PokeIPID.LastError);
    
    PokeIPID.PIDout = -(PokeIPID.Pout + PokeIPID.Iout + PokeIPID.Dout);
    
    PokeIPID.PIDout = PokeIPID.PIDout > PokeIPID.PIDMax ? PokeIPID.PIDMax : PokeIPID.PIDout;
    PokeIPID.PIDout = PokeIPID.PIDout < -PokeIPID.PIDMax ? -PokeIPID.PIDMax : PokeIPID.PIDout;
    
    PokeIPID.LastError = PokeIPID.CurrentError;
    
    return PokeIPID.PIDout;
}


/** 
  * @note   modified
  * @brief  底盘角度PID
  * @param  0 正常PID         1 自动跟踪PID
  * @retval void
  */
void Control_ChassisPID(void)
{
    /************************************       外环PID       ************************************/
    ChassisOPID.CurrentError = ChassisParam.TargetABSAngle - SuperGyoParam.Angle;
    
    ChassisOPID.Pout = ChassisOPID.P * ChassisOPID.CurrentError;
    
    ChassisOPID.Iout = ChassisOPID.I * ChassisOPID.CurrentError;
    ChassisOPID.Iout = ChassisOPID.Iout > ChassisOPID.IMax ? ChassisOPID.IMax : ChassisOPID.Iout;
    ChassisOPID.Iout = ChassisOPID.Iout < -ChassisOPID.IMax ? -ChassisOPID.IMax : ChassisOPID.Iout;
    
    ChassisOPID.Dout = ChassisOPID.D * (ChassisOPID.CurrentError - ChassisOPID.LastError);
    
    ChassisOPID.PIDout = ChassisOPID.Pout + ChassisOPID.Iout + ChassisOPID.Dout;
    ChassisOPID.PIDout = ChassisOPID.PIDout > ChassisOPID.PIDMax ? ChassisOPID.PIDMax : ChassisOPID.PIDout;
    ChassisOPID.PIDout = ChassisOPID.PIDout < -ChassisOPID.PIDMax ? -ChassisOPID.PIDMax : ChassisOPID.PIDout;
    
    /************************************       内环PID       ************************************/
    ChassisIPID.CurrentError = ChassisOPID.PIDout - SuperGyoParam.Omega ;
    
    ChassisIPID.Pout = ChassisIPID.P * ChassisIPID.CurrentError;
    
    ChassisIPID.Iout = ChassisIPID.I * ChassisIPID.CurrentError;
    ChassisIPID.Iout = ChassisIPID.Iout > ChassisIPID.IMax ? ChassisIPID.IMax : ChassisIPID.Iout;
    ChassisIPID.Iout = ChassisIPID.Iout < -ChassisIPID.IMax ? -ChassisIPID.IMax : ChassisIPID.Iout;
    
    ChassisIPID.Dout = ChassisIPID.D * (ChassisIPID.CurrentError - ChassisIPID.LastError);
    
    ChassisIPID.PIDout = -(ChassisIPID.Pout + ChassisIPID.Iout + ChassisIPID.Dout);
    ChassisIPID.PIDout = ChassisIPID.PIDout > ChassisIPID.PIDMax ? ChassisIPID.PIDMax : ChassisIPID.PIDout;
    ChassisIPID.PIDout = ChassisIPID.PIDout < -ChassisIPID.PIDMax ? -ChassisIPID.PIDMax : ChassisIPID.PIDout;
    
    ChassisParam.TargetOmega = ChassisIPID.PIDout;
}







