
#ifndef __DRIVER_MPU9250_H
#define __DRIVER_MPU9250_H


#include "stm32f4xx.h"


#ifdef  __MPU9250_GLOBALS
#define __MPU9250_EXT
#else
#define __MPU9250_EXT   extern
#endif


//陀螺仪姿态数据数据结构
typedef struct
{
    //实际数据
    struct
    {
        //角速度实际数据
        float OX;
        float OY;
        float OZ;
        //加速度实际数据
        float GX;
        float GY;
        float GZ;
        //温度实际数据
        float T;
    }Real;
    
    //转化为编码器的 线/s 的角速度数据
    struct
    {
        int16_t X;
        int16_t Y;
        int16_t Z;
    }MotorEncoderOmega;
    
    //结算实际对地欧拉角
    struct
    {
        float Pitch;
        float Roll;
        float Yaw;
    }Euler;
}MPU9250_TypeDef;


typedef struct
{
    float X;
    float Y;
    float Z;
}Offset_TypeDef;


//姿态数据
__MPU9250_EXT MPU9250_TypeDef Position;
//偏差数据数据
__MPU9250_EXT Offset_TypeDef Omega_Offset;


void MPU9250_InitConfig(void);
uint8_t MPU9250_Update(void);


#endif
