
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


typedef union u8Tofloat
{
    float floatdata;
    uint8_t uint8_tdata[4];
}u8Todouble;


//底盘陀螺仪角度
__DRIVER_SUPERGYROSCOPE_EXT float SuperGyoAngle;
//底盘陀螺仪上次角度
__DRIVER_SUPERGYROSCOPE_EXT float LastSuperGyoAngle;
//底盘陀螺仪角速度
__DRIVER_SUPERGYROSCOPE_EXT float SuperGyoOmega;
//底盘陀螺仪角速度转为编码器单位
__DRIVER_SUPERGYROSCOPE_EXT float SuperGyoMotorEncoderOmega;
//底盘陀螺仪数据帧计数
__DRIVER_SUPERGYROSCOPE_EXT float SuperGyoFrameCounter;
//底盘陀螺仪数据帧帧率
__DRIVER_SUPERGYROSCOPE_EXT float SuperGyoFrameRate;


void SuperGyo_InitConfig(void);


#endif
