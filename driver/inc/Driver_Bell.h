#ifndef __DRIVER_BELL_H
#define __DRIVER_BELL_H


#include "stm32f4xx.h"


#define MAXAWarning     65535           //警告最长序列


//警告类型
typedef enum
{
    None,               //无警告
    MPUOffset,          //mpu9250离线
    ArtOffset,          //炮摩擦轮离线
    ArtAlign,           //炮摩擦轮需校准
    CloudOffset,        //云台电机离线
    PokeStuck,          //拨弹电机卡住
}Warning_TypeDef;


void Bell_InitConfig(void);
void Bell_Bark(uint8_t bark);
void Bell_BarkWarning(uint8_t Serial, uint16_t Count);



#endif
