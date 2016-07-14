
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
#define PitchUPLimit            5600
#define PitchDOWNLimit          4500
#define PitchCenter             4844
#define YawLEFTLimit            5600
#define YawRIGHTLimit           3800
#define YawCenter               4650
#define ABSPITCHUPLIMIT         60.0F          //pitch绝对角度上限幅
#define ABSPITCHDOWNLIMIT       -40.0F         //pitch绝对角度下限幅
#elif INFANTRY == 2     //阿拉斯加
#define PitchUPLimit            3500
#define PitchDOWNLimit          2600
#define PitchCenter             2888
#define YawLEFTLimit            5500
#define YawRIGHTLimit           3000
#define YawCenter               4100
#define ABSPITCHUPLIMIT         60.0F          //pitch绝对角度上限幅
#define ABSPITCHDOWNLIMIT       -40.0F         //pitch绝对角度下限幅
#elif INFANTRY == 3     //哈士奇
#define PitchUPLimit            4400
#define PitchDOWNLimit          3500
#define PitchCenter             3897
#define YawLEFTLimit            4600
#define YawRIGHTLimit           2220
#define YawCenter               3380
#define ABSPITCHUPLIMIT         60.0F          //pitch绝对角度上限幅
#define ABSPITCHDOWNLIMIT       -40.0F         //pitch绝对角度下限幅
#elif INFANTRY == 4     //边牧
#define PitchUPLimit            5400
#define PitchDOWNLimit          4430
#define PitchCenter             4862
#define YawLEFTLimit            4630
#define YawRIGHTLimit           2230
#define YawCenter               3440
#define ABSPITCHUPLIMIT         60.0F          //pitch绝对角度上限幅
#define ABSPITCHDOWNLIMIT       -40.0F         //pitch绝对角度下限幅
#elif INFANTRY == 5     //狗蛋
#define PitchUPLimit            5400
#define PitchDOWNLimit          4600
#define PitchCenter             4862
#define YawLEFTLimit            10400
#define YawRIGHTLimit           8400
#define YawCenter               9270
#define ABSPITCHUPLIMIT         60.0F          //pitch绝对角度上限幅
#define ABSPITCHDOWNLIMIT       -40.0F         //pitch绝对角度下限幅
#elif INFANTRY == 6     //英雄
#define PitchUPLimit            7850
#define PitchDOWNLimit          7060
#define PitchCenter             7377
#define YawLEFTLimit            3980
#define YawRIGHTLimit           2560
#define YawCenter               3327
#define ABSPITCHUPLIMIT         60.0F          //pitch绝对角度上限幅
#define ABSPITCHDOWNLIMIT       -40.0F         //pitch绝对角度下限幅
#endif




#ifdef  __CLOUDMOTOR_GLOBALS
#define __CLOUDMOTOR_EXT
#else   
#define __CLOUDMOTOR_EXT extern
#endif


//角度类型枚举
typedef enum
{
    AngleMode_Encoder,
    AngleMode_ABS
}AngleMode_Enum;


//云台电机参数结构体
typedef struct
{
    uint16_t EncoderTargetAngle;                //目标角度（编码器值）
    float   ABSTargetAngle;                     //目标角度（实际值）
    uint16_t RealEncoderAngle;                  //电机实际角度（编码器值）    
    AngleMode_Enum AngleMode;                   //角度模式，指明目标角度使用绝对角度还是相对角度
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
void Cloud_YawAngleSet(float Target, uint8_t mode);
void Cloud_PitchAngleSet(float Target, uint8_t mode);


#endif
