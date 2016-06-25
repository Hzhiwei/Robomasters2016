
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


//云台PID数据
__DRIVER_CONTROL_EXT PID_Type PitchOPID, YawOPID, PitchIPID, YawIPID;
//拨弹电机速度PID
__DRIVER_CONTROL_EXT PID_Type PokeIPID, PokeOPID;     
//底盘角度PID
__DRIVER_CONTROL_EXT PID_Type ChassisIPID, ChassisOPID;     


void CloudPID_InitConfig(void);
int16_t Control_YawPID(void);
int16_t Control_PitchPID(void);
int16_t Control_PokeIPID(void);
void Control_ChassisPID(void);


#endif
