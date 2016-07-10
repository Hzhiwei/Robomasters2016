
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


#if INFANTRYTYPE == 1 || INFANTRYTYPE == 2
//AB车模式
typedef enum
{
    ABInfantry_Master,      //主车，正常控制
    ABInfantry_Slave        //从车，自动射击
}ABInfantryMode_Enum;
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
//预判速度PID
__DRIVER_CONTROL_EXT PID_Type ForcastYawSpeedPID;

#if INFANTRYTYPE == 1 || INFANTRYTYPE == 2
//AB车模式
__DRIVER_CONTROL_EXT ABInfantryMode_Enum ABInfantryMode;
#endif


void CloudPID_InitConfig(void);
int16_t Control_YawPID(void);
int16_t Control_PitchPID(void);
int16_t Control_PokeIPID(void);
void Control_ChassisPID(void);
int16_t VControl_YawPID(float TargetOmega);


#endif
