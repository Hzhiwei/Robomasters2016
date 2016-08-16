#ifndef __DRIVER_JUDGE_H
#define __DRIVER_JUDGE_H


#include "Config.h"
#include "stm32f4xx.h"
#include "OSinclude.h"

#define JudgeBufferLength       150
#define JudgeFrameLength_1      46
#define JudgeFrameLength_2      11
#define JudgeFrameLength_3      24

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


//裁判系统结构体
typedef struct
{
    float RealVoltage;                  //实时电压
    float RealCurrent;                  //实时电流
    int16_t LastBlood;                  //剩余血量
    uint8_t LastHartID;                 //上次收到伤害的装甲板ID号
    portTickType LastHartTick;          //上次受伤害时间 
    float LastShotSpeed;                //上次射击速度
    portTickType LastShotTick;          //上次射击时间
#if INFANTRY == 7
    uint16_t ShootNum;                  //已发射子弹数
    uint8_t BulletUseUp;                //1 基地子弹射完          0 基地子弹未射完
    uint16_t ShootFail;                 //发射失败时间 
#endif
}InfantryJudge_Struct;


//裁判系统数据缓存
__DRIVER_EXT uint8_t JudgeDataBuffer[JudgeBufferLength];
//实时电压
__DRIVER_EXT InfantryJudge_Struct InfantryJudge;
//帧率计数器
__DRIVER_EXT float JudgeFrameCounter;
//帧率
__DRIVER_EXT float JudgeFrameRate;


unsigned char Get_CRC8_Check_Sum(unsigned char *pchMessage,unsigned int dwLength,unsigned char ucCRC8);
unsigned int Verify_CRC8_Check_Sum(unsigned char *pchMessage, unsigned int dwLength);
void Append_CRC8_Check_Sum(unsigned char *pchMessage, unsigned int dwLength);
uint16_t Get_CRC16_Check_Sum(uint8_t *pchMessage,uint32_t dwLength,uint16_t wCRC);
uint32_t Verify_CRC16_Check_Sum(uint8_t *pchMessage, uint32_t dwLength);
void Append_CRC16_Check_Sum(uint8_t * pchMessage,uint32_t dwLength);
    
void Judge_InitConfig(void);



#endif
