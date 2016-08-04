#ifndef __DRIVER_FRICMOTOR_H
#define __DRIVER_FRICMOTOR_H


#include "Config.h"
#include "stm32f4xx.h"


#ifdef  __DRIVER_FRICMOTOR_GLOBALS
#define __DRIVER_FRICMOTOR_EXT
#else
#define __DRIVER_FRICMOTOR_EXT extern
#endif


#define ARTILLERYFRICCANIDLEFT                  0x201               //左摩擦轮CAN ID
#define ARTILLERYFRICCANIDRIGHT                 0x202               //左摩擦轮CAN ID
#define ARTILLERYFRICCONTROLCANID               0x200               //摩擦轮控制ID


#define ARTILLERYFRICMAXSPEED                  8600                //炮摩擦轮最大速度
#define ARTILLERYFRICMAXCURREN                 30000               //摩擦轮最大电流
#define ARTILLERYFRICMINCURRENOFTMAXSPEED      1200                //摩擦轮最大速度时，需要的最小电流
#define ARTILLERYACCBELOWSPEED                 7000                //当摩擦轮速度低于此速度时给最大电流加速


//小英雄枪炮一体增加炮摩擦轮
#if INFANTRY == 6
__DRIVER_FRICMOTOR_EXT int16_t ArtilleryFricRealSpeed[2];          //摩擦轮实际速度  0左    1右
__DRIVER_FRICMOTOR_EXT int16_t ArtilleryFricTargetSpeed;           //摩擦轮目标速度  0停止  else 最大速度
#endif


void FricMotor_InitConfig(void);

#if FRICTYPE == 1
void FricArtillerySpeed_Adjust(int16_t TargetSpeed);
void FricArtilleryMotorCurrent(int16_t LeftArtilleryCurrent, int16_t RightArtilleryCurrent);
#else
void FricGunControl(uint8_t Control);
#endif



#endif
