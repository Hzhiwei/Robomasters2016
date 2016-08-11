
#ifndef __DRIVER_CONTROL_H
#define __DRIVER_CONTROL_H


#include "stm32f4xx.h"
#include "OSinclude.h"
#include "Config.h"


#ifdef  __DRIVER_CONTROL_GLOBALS
#define __DRIVER_CONTROL_EXT
#else
#define __DRIVER_CONTROL_EXT extern
#endif


//PID数据结构
typedef struct
{
	float P;
	float I;
	float D;
	
	float CurrentError;
	float LastError;
	float Pout;
	float Iout;
	float Dout;
	float PIDout;
	
	float IMax;
	float PIDMax;
	
	portTickType LastTick;
}PID_Type;



//云台PID数据
__DRIVER_CONTROL_EXT PID_Type PitchOPID, YawOPID, PitchIPID, YawIPID;
//拨弹电机速度PID
__DRIVER_CONTROL_EXT PID_Type PokeIPID, PokeOPID;     
//底盘角度PID
__DRIVER_CONTROL_EXT PID_Type ChassisIPID, ChassisOPID; 
//底盘自动角度PID
__DRIVER_CONTROL_EXT PID_Type ChassisAUTOIPID, ChassisAUTOOPID; 
//预判速度PID
__DRIVER_CONTROL_EXT PID_Type ForcastYawSpeedPID;

#if INFANTRY == 6
//摩擦轮PID
__DRIVER_CONTROL_EXT PID_Type ArtFricLeftPID, ArtFricRightPID;
#endif


void ControlPID_InitConfig(void);
int16_t Control_YawPID(void);
int16_t Control_PitchPID(void);
int16_t Control_PokeIPID(void);
int16_t Poke_MotorSpeedPID(int16_t Speed);
void Control_ChassisPID(void);
int16_t Control_FeedForwardYawPID(float FeedSpeed);

#if FRICTYPE == 1
void  Control_FricPID(int16_t *Output);
#endif


#endif
