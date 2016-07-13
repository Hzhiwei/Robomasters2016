
#ifndef __DRIVER_CHASSIS_H
#define __DRIVER_CHASSIS_H


#include "stm32f4xx.h"


#define LFCHASSISCONTROLCANID           0x75
#define MaxWheelSpeed                  900
#define ChassisLimitCurrent             2750            //底盘电流限制极限
#define MOUSEINTLIMIT           0.9F        //鼠标旋转速度上限，当鼠标旋转时底盘速度跟不上则减小此值


//底盘电机CANID
#define LFCHASSISCANID                  0x41
#define RFCHASSISCANID                  0x42
#define LBCHASSISCANID                  0x43
#define RBCHASSISCANID                  0x44
//底盘电机速度控制
#define CHASSISSPEEDSETCANID            0x46
//底盘电机电流控制
#define CHASSISCURRENTSETCANID          0x40


#if INFANTRY == 1 || INFANTRY == 2 || INFANTRY == 3 || INFANTRY == 4 || INFANTRY == 5
#define CHASSISMAXPOWER                 80.0F       //底盘最大功率
#elif INFANTRY == 6
#define CHASSISMAXPOWER                 160.0F      //底盘最大功率
#endif


#ifdef  __DRIVER_CHASSIS_GLOBALS
#define __DRIVER_CHASSIS_EXT
#else
#define __DRIVER_CHASSIS_EXT extern
#endif


//速度枚举
typedef enum
{
    ChassisSpeedLevel_Hight,
    ChassisSpeedLevel_Low
}ChassisSpeedLevel_Enum;


//单电机参数
typedef struct
{
    int16_t TargetSpeed;             //电机目标速度
    int16_t RealSpeed;               //电机实际速度
    uint16_t LimitCurrent;           //限制电流
    uint16_t RealCurrent;            //实际电流
    uint16_t NeedCurrent;            //需求电流
}OneMotorParam_Struct;


//底盘电机参数结构体
typedef struct
{
    OneMotorParam_Struct LF;
    OneMotorParam_Struct RF;
    OneMotorParam_Struct LB;
    OneMotorParam_Struct RB;
    ChassisSpeedLevel_Enum SpeedLevel;
    float VX;
    float VY;
    float Omega;
}ChassisParam_Struct;


//底盘最大总电流限制
__DRIVER_CHASSIS_EXT float ChassisMaxSumCurrent;   
//电机状态
__DRIVER_CHASSIS_EXT ChassisParam_Struct ChassisParam;    
//底盘电机帧计数器
__DRIVER_CHASSIS_EXT uint16_t ChassisFrameCounter[4];   
//底盘电机帧计数器
__DRIVER_CHASSIS_EXT uint16_t ChassisFrameRate[4];  



void Chassis_InitConfig(void);
void Chassis_Adjust(void);
void Chassis_SendMotorParam(uint8_t mode);
static void MecanumCalculate(float Vx, float Vy, float Omega, int16_t *Speed);
void Chassis_OmegaSet(float Target);
void Chassis_SpeedSet(float XSpeed, float YSpeed);
void Chassis_Control(uint8_t mode, uint8_t PIDChoie);
void Chassis_MotorDebug(void);


#endif
