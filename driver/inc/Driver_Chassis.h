
#ifndef __DRIVER_CHASSIS_H
#define __DRIVER_CHASSIS_H


#include "stm32f4xx.h"


#define LFCHASSISCONTROLCANID           0x75
#define Max_WheelSpeed                  900
#define ChassisLimitCurrent             3000            //底盘电流限制极限


//底盘电机CANID
#define LFCHASSISCANID                  0x41
#define RFCHASSISCANID                  0x42
#define LBCHASSISCANID                  0x43
#define RBCHASSISCANID                  0x44
//底盘电机速度控制
#define CHASSISSPEEDSETCANID            0x46
//底盘电机电流控制
#define CHASSISCURRENTSETCANID          0x40


#ifdef  __DRIVER_CHASSIS_GLOBALS
#define __DRIVER_CHASSIS_EXT
#else
#define __DRIVER_CHASSIS_EXT extern
#endif


//底盘电机参数结构体
typedef struct
{
    //电机目标速度
    struct
    {
        int16_t LF;
        int16_t LB;
        int16_t RF;
        int16_t RB;
    }TargetSpeed;
    //电机实际速度
    struct
    {
        int16_t LF;
        int16_t LB;
        int16_t RF;
        int16_t RB;
    }RealSpeed;
    //限制电流
    struct
    {
        uint16_t LF;
        uint16_t LB;
        uint16_t RF;
        uint16_t RB;
    }LimitCurrent;
    //实际电流
    struct
    {
        uint16_t LF;
        uint16_t LB;
        uint16_t RF;
        uint16_t RB;
    }RealCurrent;
}MotorParam_Struct;


__DRIVER_CHASSIS_EXT float ChassisMaxSumCurrent;        //底盘最大总电流限制
__DRIVER_CHASSIS_EXT MotorParam_Struct MotorStatus;        //电机状态


void Chassis_SendAngle(void);
static void MecanumCalculate(float Vx, float Vy, float Omega, int16_t *Speed);


#endif
