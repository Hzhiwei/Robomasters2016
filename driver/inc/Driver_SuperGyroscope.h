
#ifndef __DRIVER_SUPERGYROSCOPE_H
#define __DRIVER_SUPERGYROSCOPE_H


#include "stm32f4xx.h"
#include "OSinclude.h"


#define SUPERGYROSCOPECANIC     0x11


#ifdef  __DRIVER_SUPERGYROSCOPE_GLOBALS
#define __DRIVER_SUPERGYROSCOPE_EXT
#else
#define __DRIVER_SUPERGYROSCOPE_EXT extern
#endif


//底盘陀螺仪数据结构体
typedef struct
{
    float Angle;
    float Omega;
    float Offset;
}SuperGyoParam_Struct;


//底盘陀螺仪数据
__DRIVER_SUPERGYROSCOPE_EXT SuperGyoParam_Struct SuperGyoParam;
//底盘陀螺仪数据帧计数
__DRIVER_SUPERGYROSCOPE_EXT float SuperGyoFrameCounter;
//底盘陀螺仪数据帧帧率
__DRIVER_SUPERGYROSCOPE_EXT float SuperGyoFrameRate;


void SuperGyo_InitConfig(void);


#endif
