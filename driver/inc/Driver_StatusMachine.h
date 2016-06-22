
#ifndef __DRIVER_STATUSMACHINE_H
#define __DRIVER_STATUSMACHINE_H


#include "stm32f4xx.h"


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
}GunStatus_TypeDef;

//云台状态
typedef enum
{
    CloudStatus_Normal,             //正常模式
    CloudStatus_Auto,               //自动模式
}CloudStatus_TypeDef;


//控制模式
typedef enum
{
    ControlStatus_Protect,          //保护模式，全部停止
    ControlStatus_RC,               //遥控器控制（增量式）
    ControlStatus_KM,               //键鼠控制
}ControlStatus_TypeDef;


//状态量
__DRIVER_STATUSMACHINE_EXT  CloudStatus_TypeDef         CloudStatus;
__DRIVER_STATUSMACHINE_EXT  GunStatus_TypeDef           GunStatus;
__DRIVER_STATUSMACHINE_EXT  ControlStatus_TypeDef       ControlStatus;


void StatusMachine_InitConfig(void);
void StatusMachine_Update(void);


#endif
