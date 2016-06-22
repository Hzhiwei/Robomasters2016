
#define __MPU9250_GLOBALS

#include "Driver_mpu9250.h"
#include "BSP_SPI.h"
#include "OSinclude.h"
#include "mpu9250dmp.h"


/**
  * @brief  mpu9250初始化
  * @param  void
  * @retval void
  */
void MPU9250_InitConfig(void)
{
    Omega_Offset.X = -2.2F;
    Omega_Offset.Y = 1.28F;
    Omega_Offset.Z = -1.1F;
}


/**
  * @brief  mpu9250更新欧拉角及角速度
  * @param  void
  * @retval 0成功     1失败
  */
uint8_t MPU9250_Update(void)
{
    uint8_t result;
    
    float Pitch, Roll, Yaw;
    float OX, OY, OZ;
    
    result = MPU9250DMP_GetEuler(&Pitch, &Roll, &Yaw);
    if(result)return 1;
    result = MPU9250_GetGyro(&OX, &OY, &OZ);
    if(result)return 1;
    
    Position.Euler.Pitch = Pitch;
    Position.Euler.Roll = Roll;
    Position.Euler.Yaw = Yaw;
    
    Position.Real.OX = OX - Omega_Offset.X;
    Position.Real.OY = OY - Omega_Offset.Y;
    Position.Real.OZ = OZ - Omega_Offset.Z;
    
    //角速度反向使之与电机统一
    Position.MotorEncoderOmega.X = -Position.Real.OX * 22.7556F;
    Position.MotorEncoderOmega.Y = -Position.Real.OY * 22.7556F;
    Position.MotorEncoderOmega.Z = -Position.Real.OZ * 22.7556F;
    
    return 0;
}

