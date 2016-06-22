
#define __DRIVER_CONTROL_GLOBALS

#include "Driver_Control.h"
#include "Driver_Motor.h"
#include "Driver_mpu9250.h"
#include "Driver_SuperGyroscope.h"


/**
  * @brief  云台电机控制初始化
  * @param  void
  * @retval void
  */
void CloudPID_InitConfig(void)
{
    EncoderTargetPitch = PitchCenter;
    EncoderTargetYaw = YawCenter;
    PokeSpeedTarget = 0;
    ABSTargetPitch = 0;
    ABSTargetYaw = 0;
    
	
	//速度置零
	GnuSpeedTarget = 0;
	
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
    
    PokePID.CurrentError = 0;
    PokePID.LastError = 0;
    PokePID.P = 1;
    PokePID.I = 0.3;
    PokePID.D = 0;
    PokePID.IMax = 60;
    PokePID.PIDMax = 100;
    PokePID.LastTick = 0;
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
	YawOPID.CurrentError = EncoderTargetYaw - YawMotorAngle;
	
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

	PitchOPID.CurrentError = EncoderTargetPitch - PitchMotorAngle;
	
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
  * @brief  拨弹电机速度PID
  * @param  void
  * @retval 
  */
int16_t Control_PokePID(void)
{
    PokePID.CurrentError = PokeSpeedTarget - PokeSpeed;
    
    PokePID.Pout = PokePID.P * PokePID.CurrentError;
    
    PokePID.Iout += PokePID.CurrentError * PokePID.I;
    PokePID.Iout = PokePID.Iout > PokePID.IMax ? PokePID.IMax : PokePID.Iout;
    PokePID.Iout = PokePID.Iout < -PokePID.IMax ? -PokePID.IMax : PokePID.Iout;
    
    PokePID.Dout = PokePID.D * (PokePID.CurrentError - PokePID.LastError);
    
    PokePID.PIDout = PokePID.Pout + PokePID.Iout + PokePID.Dout;
    
    PokePID.PIDout = PokePID.PIDout > PokePID.PIDMax ? PokePID.PIDMax : PokePID.PIDout;
    PokePID.PIDout = PokePID.PIDout < -PokePID.PIDMax ? -PokePID.PIDMax : PokePID.PIDout;
    
    PokePID.LastError = PokePID.CurrentError;
    
    return PokePID.PIDout;
}


/**
  * @brief  设置pitch目标位置
  * @param  目标位置
  * @param  角度模式
  * @retval void
  */
void Control_SetTargetPitch(float TargetPitch, CoordinateMode_Enum mode)
{
    if(mode == Encoder)
    {
        TargetPitch = TargetPitch > PitchUPLimit ? PitchUPLimit : TargetPitch;
        TargetPitch = TargetPitch < PitchDOWNLimit ? PitchDOWNLimit : TargetPitch;
        
        EncoderTargetPitch = (int16_t)TargetPitch;
    }
    else
    {
        ABSTargetPitch = TargetPitch;
    }
}


/**
  * @brief  设置yaw目标位置
  * @param  目标位置
  * @param  目标位置模式
  * @retval void
  */
void Control_SetTargetYaw(float TargetYaw, CoordinateMode_Enum mode)
{
    if(mode == Encoder)
    {
        TargetYaw = TargetYaw > YawLEFTLimit ? YawLEFTLimit : TargetYaw;
        TargetYaw = TargetYaw < YawRIGHTLimit ? YawRIGHTLimit : TargetYaw;
        
        EncoderTargetYaw = (int16_t)TargetYaw;
    }
    else
    {
        ABSTargetYaw = TargetYaw;
    }
}




