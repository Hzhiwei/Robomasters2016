#ifndef __DRIVER_JUDGE_H
#define __DRIVER_JUDGE_H


#include "stm32f4xx.h"


#define JudgeFrameHeader        0xA5        //帧头 


#ifdef  __DRIVER_GLOBALS
#define __DRIVER_EXT
#else
#define __DRIVER_EXT extern
#endif


//小符状态枚举
typedef enum
{
    BUFF_TYPE_NONE, //无效
    BUFF_TYPE_ARMOR = 0x01, //防御符
    BUFF_TYPE_SUPPLY = 0x04, //加血符
    BUFF_TYPE_BULLFTS= 0x08, //加弹符
}LBuffType_Enum;


//位置状态结构体
typedef __packed struct
{
    uint8_t flag; //0 无效， 1 有效
    uint32_t x;
    uint32_t y;
    uint32_t z;
    uint32_t compass;
}GpsData_Struct;


//比赛进程信息结构体
typedef __packed struct
{
    uint32_t remainTime;
    uint16_t remainLifeValue;
    float realChassisOutV;
    float realChassisOutA;
    uint8_t runeStatus[4];
    uint8_t bigRune0Status;
    uint8_t bigRune1status;
    uint8_t conveyorBelts0:2;
    uint8_t conveyorBelts1:2;
    uint8_t parkingApron0:1;
    uint8_t parkingApron1:1;
    uint8_t parkingApron2:1;
    uint8_t parkingApron3:1;
    GpsData_Struct gpsData;
}GameInfo_Struct;


//实时血量变化信息结构体
typedef __packed struct
{
    uint8_t weakId:4;
    uint8_t way:4;
    uint16_t value;
}RealBloodChangedData_Struct;


//实时射击信息结构体
typedef __packed struct
{
    float realBulletShootSpeed;
    float realBulletShootFreq;
    float realGolfShootSpeed;
    float realGolfShootFreq;
}RealShootData_Struct;


//实时电压
__DRIVER_EXT float JudgeRealVoltage;
//实时电流
__DRIVER_EXT float JudgeRealCurrent;
//帧率计数器
__DRIVER_EXT float JudgeFrameCounter;
//帧率
__DRIVER_EXT float JudgeFrameRate;



#endif
