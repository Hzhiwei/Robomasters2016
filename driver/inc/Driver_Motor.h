
#ifndef __DRIVER_MOTOR_H
#define __DRIVER_MOTOR_H


#include "stm32f4xx.h"


//电机CAN ID
#define YAWCANID		0x205
#define PITCHCANID		0x206

#define COULDCONTROLID	0x1FF

//编码器中点位置
#define ENCODERCenter   0x7FFF


//角度限幅
#define PitchUPLimit            5800
#define PitchDOWNLimit          4500
#define PitchCenter             4844
#define YawLEFTLimit            6730
#define YawRIGHTLimit           2630
#define YawCenter               4650

//枪摩擦轮速度
#define GUNWORKSPEED            260         //使枪子弹不超速的最大枪摩擦轮速度
#define POKEWORKSPEED           -100        //使枪子弹不超频最大速度(符号控制方向）


#ifdef  __MOTOR_GLOBALS
#define __MOTOR_EXT
#else   
#define __MOTOR_EXT extern
#endif


//云台电机实际角度
__MOTOR_EXT int16_t PitchMotorAngle, YawMotorAngle;
//拨弹电机实际速度
__MOTOR_EXT int16_t PokeSpeed;
//云台电机数据帧数计数器
__MOTOR_EXT uint16_t PitchMotorFrameCounter, YawMotorFrameCounter;
//云台电机数据帧率
__MOTOR_EXT uint16_t PitchMotorFrameRate, YawMotorFrameRate;


void Motor_InitConfig(void);
void CloudMotorCurrent(int16_t Pitch, int16_t Yaw);
void PokeMotorCurrent(int16_t Current);
void GunFric_Control(uint8_t Control);
void Poke_Control(uint8_t status);
void SteeringAngle(uint16_t angle);



#endif
