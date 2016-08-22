
#ifndef __DRIVER_CLOUDMOTOR_H
#define __DRIVER_CLOUDMOTOR_H

#include "Config.h"
#include "stm32f4xx.h"


//电机CAN ID
#define YAWMOTORCANID		0x205
#define PITCHMOTORCANID		0x206

#define COULDCONTROLID	    0x1FF


//角度限幅
#if INFANTRY == 1       //萨摩

    #define PitchEncoderUPLimit             5500            //云台机械上限编码器值
    #define PitchEncoderDOWNLimit           4550            //云台机械下限编码器值
    #define PitchEncoderCenter              4820            //云台Pitch水平编码器值
    #define YawEncoderLEFTLimit             5600            //云台机械左极限编码器值
    #define YawEncoderRIGHTLimit            3800            //云台机械右极限编码器值
    #define YawEncoderCenter                4650            //云台Yaw中编码器值

    #define PitchABSUPLimit                 60.0F           //操作绝对角度上极限
    #define PitchABSDOWNLimit               -40.0F          //操作绝对角度下极限
    
    #define PitchEncoderPassZeroBoundary    1               //Pitch电机编码器分界值（距此值判断过零
    #define YawEncoderPassZeroBoundary      1               //Pitch电机编码器分界值（距此值判断过零

#elif INFANTRY == 2     //阿拉斯加

    #define PitchEncoderUPLimit             3500
    #define PitchEncoderDOWNLimit           2470
    #define PitchEncoderCenter              2808
    #define YawEncoderLEFTLimit             5150
    #define YawEncoderRIGHTLimit            3150
    #define YawEncoderCenter                4122
    
    #define PitchABSUPLimit                 60.0F 
    #define PitchABSDOWNLimit               -40.0F
    
    #define PitchEncoderPassZeroBoundary    1
    #define YawEncoderPassZeroBoundary      1

#elif INFANTRY == 3     //哈士奇

    #define PitchEncoderUPLimit             5600
    #define PitchEncoderDOWNLimit           4740
    #define PitchEncoderCenter              5038
    #define YawEncoderLEFTLimit             4600
    #define YawEncoderRIGHTLimit            2220
    #define YawEncoderCenter                3380
    
    #define PitchABSUPLimit                 60.0F 
    #define PitchABSDOWNLimit               -40.0F
    
    #define PitchEncoderPassZeroBoundary    1
    #define YawEncoderPassZeroBoundary      1

#elif INFANTRY == 4     //边牧

    #define PitchEncoderUPLimit             5500
    #define PitchEncoderDOWNLimit           4480
    #define PitchEncoderCenter              4862
    #define YawEncoderLEFTLimit             4570
    #define YawEncoderRIGHTLimit            2450
    #define YawEncoderCenter                3380
                
    #define PitchABSUPLimit                 60.0F 
    #define PitchABSDOWNLimit               -40.0F
    
    #define PitchEncoderPassZeroBoundary    1
    #define YawEncoderPassZeroBoundary      1

#elif INFANTRY == 5     //狗蛋

    #define PitchEncoderUPLimit             5830
    #define PitchEncoderDOWNLimit           4910
    #define PitchEncoderCenter              5066
    #define YawEncoderLEFTLimit             10400
    #define YawEncoderRIGHTLimit            8200
    #define YawEncoderCenter                9265
                
    #define PitchABSUPLimit                 60.0F 
    #define PitchABSDOWNLimit               -40.0F
    
    #define PitchEncoderPassZeroBoundary    1
    #define YawEncoderPassZeroBoundary      5000

#elif INFANTRY == 6     //小英雄

    #define PitchEncoderUPLimit             4460
    #define PitchEncoderDOWNLimit           3490
    #define PitchEncoderCenter              4012
    #define YawEncoderLEFTLimit             7100
    #define YawEncoderRIGHTLimit            5000
    #define YawEncoderCenter                6180
                
    #define PitchABSUPLimit                 60.0F 
    #define PitchABSDOWNLimit               -40.0F
    
    #define PitchEncoderPassZeroBoundary    1
    #define YawEncoderPassZeroBoundary      1000

#elif INFANTRY == 7     //基地

    #define PitchEncoderUPLimit             7850
    #define PitchEncoderDOWNLimit           7030
    #define PitchEncoderCenter              7360
    #define YawEncoderLEFTLimit             4340
    #define YawEncoderRIGHTLimit            2370
    #define YawEncoderCenter                3335
                
    #define PitchABSUPLimit                 60.0F 
    #define PitchABSDOWNLimit               -40.0F
    
    #define PitchEncoderPassZeroBoundary    2000
    #define YawEncoderPassZeroBoundary      1000

#endif




#ifdef  __CLOUDMOTOR_GLOBALS
#define __CLOUDMOTOR_EXT
#else   
#define __CLOUDMOTOR_EXT extern
#endif


//角度类型枚举
typedef enum
{
    AngleMode_OPP,          //相对角度
    AngleMode_ABS           //绝对角度
}AngleMode_Enum;


//云台电机参数结构体
typedef struct
{
    uint16_t RealEncoderAngle;                  //电机角度编码器值  
    float RealABSAngle;                         //实际角度（绝对值）
    float TargetABSAngle;                       //目标角度（绝对值）
    uint16_t FrameCounter;                      //帧率计数器
    uint16_t FrameRate;                         //帧率
}ClourMotorParam_Struct;

//云台参数结构体
typedef struct
{
    ClourMotorParam_Struct Pitch;               //Pitch电机参数
    ClourMotorParam_Struct Yaw;                 //Yaw电机参数
    uint8_t Lock;                               //0 锁定，云台电机停止       1 正常工作
}CloudParam_Struct;


//云台参数
__CLOUDMOTOR_EXT CloudParam_Struct CloudParam;


void CloudMotor_InitConfig(void);
void CloudMotorCurrent(int16_t Pitch, int16_t Yaw);
void Cloud_Adjust(uint8_t mode);
void Cloud_AutoAdjust(float FeedSpeed, uint8_t mode);
void Cloud_YawAngleSet(float Target, AngleMode_Enum mode);
void Cloud_PitchAngleSet(float Target);


#endif
