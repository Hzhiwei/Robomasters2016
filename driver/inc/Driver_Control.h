
#ifndef __DRIVER_CONTROL_H
#define __DRIVER_CONTROL_H


#include "stm32f4xx.h"
#include "OSinclude.h"


#ifdef  __DRIVER_CONTROL_GLOBALS
#define __DRIVER_CONTROL_EXT
#else
#define __DRIVER_CONTROL_EXT extern
#endif


#define Motor3510Speed      4000        //不超速3510最大速度


typedef enum
{
    Encoder,
    ABS
}CoordinateMode_Enum;


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



//云台Pitch目标角度(编码器线为单位）
__DRIVER_CONTROL_EXT  uint16_t    EncoderTargetPitch, EncoderTargetYaw;
//云台Pitch，yaw目标角度（实际值）
__DRIVER_CONTROL_EXT float ABSTargetPitch, ABSTargetYaw;  
//拨弹电机目标速度
__DRIVER_CONTROL_EXT  int16_t    PokeSpeedTarget;
//云台PID数据
__DRIVER_CONTROL_EXT PID_Type PitchOPID, YawOPID, PitchIPID, YawIPID;
//拨弹电机速度PID
__DRIVER_CONTROL_EXT PID_Type PokePID;
//枪电机目标速度电流
__DRIVER_CONTROL_EXT uint16_t GnuSpeedTarget;       


void CloudPID_InitConfig(void);
int16_t Control_YawPID(void);
int16_t Control_PitchPID(void);
int16_t Control_PokePID(void);
void Control_SetTargetPitch(float TargetPitch, CoordinateMode_Enum mode);
void Control_SetTargetYaw(float TargetYaw, CoordinateMode_Enum mode);
void Control_SetTargetPitch(float TargetPitch, CoordinateMode_Enum mode);
void Control_SetTargetYaw(float TargetYaw, CoordinateMode_Enum mode);


#endif
