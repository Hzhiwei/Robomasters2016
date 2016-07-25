#ifndef __DRIVER_FRICMOTOR_H
#define __DRIVER_FRICMOTOR_H


#include "stm32f4xx.h"


#ifdef  __DRIVER_FRICMOTOR_GLOBALS
#define __DRIVER_FRICMOTOR_EXT
#else
#define __DRIVER_FRICMOTOR_EXT extern
#endif

typedef enum
{
    FricStatus_Stop,
    FricStatus_Working,
    FricStatus_Crazy,
}FricStatus_Enum;


__DRIVER_FRICMOTOR_EXT FricStatus_Enum FricStatus;


void FricMotor_InitConfig(void);
void GunFric_Control(uint8_t Control);


#endif
