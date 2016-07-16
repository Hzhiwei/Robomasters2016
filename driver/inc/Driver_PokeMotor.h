
#ifndef __DRIVER_POKEMOTOR_H
#define __DRIVER_POKEMOTOR_H

#include "Config.h"
#include "stm32f4xx.h"
#include "OSinclude.h"


#define POKEENCODERLINESPERCIRCLE       5050        //拨弹电机编码器每周线数
#define POKELINESPERSTEP                841         //拨弹电机拨弹步进线数

//拨弹电机编码器中点位置
#define POKEENCODERCenter               0x7FFF


#ifdef  __DRIVER_POKEMOTOR_GLOBALS
#define __DRIVER_POKEMOTOR_EXT
#else
#define __DRIVER_POKEMOTOR_EXT extern
#endif


//拨弹电机状态枚举
typedef enum
{
    PokeMotorParam_Working,                        //正常
    PokeMotorParam_StuckDealing,                   //正在反转处理卡弹
    PokeMotorParam_Stuck                           //卡弹
}PokeChassisParam_Enum;  
    
    
//拨弹电机参数    
typedef struct  
{   
    int16_t RealSpeed;              //拨弹电机速度
    long RealLocation;              //拨弹电机实际位置
    long TargetLocation;            //拨弹电机目标位置
    PokeChassisParam_Enum Status;    //状态
    portTickType LastShotTick;      //上次发射时间
}PokeMotorParam_Struct;


//拨弹电机参数
__DRIVER_POKEMOTOR_EXT PokeMotorParam_Struct PokeMotorParam;


void PokeMotor_InitConfig(void);
void PokeMotorCurrent(int16_t Current);
void PokeMotor_Step(void);
void PokeMotor_Adjust(void);




#endif
