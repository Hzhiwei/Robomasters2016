
#ifndef __DRIVER_STATUSMACHINE_H
#define __DRIVER_STATUSMACHINE_H


#include "stm32f4xx.h"


#define VisiolModeChangeDataSendNum     10


#ifdef  __DRIVER_STATUSMACHINE_GLOBALS
#define __DRIVER_STATUSMACHINE_EXT
#else
#define __DRIVER_STATUSMACHINE_EXT extern
#endif

//枪状态
typedef enum
{
    GunStatus_Stop,                 //枪停止
    GunStatus_Motor,                //仅开启摩擦轮
    GunStatus_Shot,                 //发射
}GunStatus_Enum;

//自动模式开关
typedef enum
{
    AutoMode_ON,                    //自动模式开
    AutoMode_OFF,                   //自动模式关
}AutoMode_Enum;


//控制模式
typedef enum
{
    ControlMode_Protect,          //保护模式，全部停止
    ControlMode_RC,               //遥控器控制（增量式）
    ControlMode_KM,               //键鼠控制
    ControlMode_AUTO,             //自主控制
}ControlMode_Enum;


//状态量
__DRIVER_STATUSMACHINE_EXT  AutoMode_Enum           AutoMode;
__DRIVER_STATUSMACHINE_EXT  GunStatus_Enum          GunStatus;
__DRIVER_STATUSMACHINE_EXT  ControlMode_Enum        ControlMode;


void StatusMachine_InitConfig(void);
void StatusMachine_Update(void);


#endif
