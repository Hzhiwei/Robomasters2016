
#define __MPU9250_GLOBALS

#include "Config.h"
#include "Driver_mpu9250.h"
#include "OSinclude.h"
#include "mpu9250dmp.h"


/**
  * @brief  mpu9250初始化
  * @param  void
  * @retval void
  */
void MPU9250_InitConfig(void)
{
#if INFANTRY == 1       //萨摩
    
    Omega_Offset.X = 0;
    Omega_Offset.Y = 0;
    Omega_Offset.Z = 0;
    
    Euler_Offset.Pitch = 0.7;
    Euler_Offset.Roll = 0;
    Euler_Offset.Yaw = 0;
    
#elif INFANTRY == 2     //阿拉斯加
    
    Omega_Offset.X = -0.67;
    Omega_Offset.Y = -0.48;
    Omega_Offset.Z = 0.48;
    
    Euler_Offset.Pitch = 0;
    Euler_Offset.Roll = 0;
    Euler_Offset.Yaw = 0;
    
#elif INFANTRY == 3     //哈士奇
    
    Omega_Offset.X = 0;
    Omega_Offset.Y = 0;
    Omega_Offset.Z = 0;
    
    Euler_Offset.Pitch = 0;
    Euler_Offset.Roll = 0;
    Euler_Offset.Yaw = 0;
    
#elif INFANTRY == 4     //边牧（没名字，先这么叫吧）
    
    Omega_Offset.X = 0;
    Omega_Offset.Y = 0;
    Omega_Offset.Z = 0;
    
    Euler_Offset.Pitch = 0;
    Euler_Offset.Roll = 0;
    Euler_Offset.Yaw = 0;
    
#elif INFANTRY == 5     //狗蛋
    
    Omega_Offset.X = 0;
    Omega_Offset.Y = 0;
    Omega_Offset.Z = 0;
    
    Euler_Offset.Pitch = 0;
    Euler_Offset.Roll = 0;
    Euler_Offset.Yaw = 0;
    
#elif INFANTRY == 6     //英雄
    
    Omega_Offset.X = 0;
    Omega_Offset.Y = 0;
    Omega_Offset.Z = 0;
    
    Euler_Offset.Pitch = 0;
    Euler_Offset.Roll = 0;
    Euler_Offset.Yaw = 0;
    
#elif INFANTRY == 7     //基地
    
    Omega_Offset.X = 0;
    Omega_Offset.Y = 0;
    Omega_Offset.Z = 0;
    
    Euler_Offset.Pitch = -4;
    Euler_Offset.Roll = 0;
    Euler_Offset.Yaw = 0;
    
#endif
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
    float GX, GY, GZ;
    
    result = MPU9250DMP_GetEuler(&Pitch, &Roll, &Yaw);
    if(result)return 1;
    result = MPU9250_GetGyro(&OX, &OY, &OZ);
    if(result)return 1;
    result = MPU9250_GetAccel(&GX, &GY, &GZ);
    if(result)return 1;
    
    Position.Euler.Pitch = Pitch - Euler_Offset.Pitch;
    Position.Euler.Roll = Roll - Euler_Offset.Roll;
    Position.Euler.Yaw = Yaw - Euler_Offset.Yaw;
    
    Position.Real.OX = -OX - Omega_Offset.X;
    Position.Real.OY = -OY - Omega_Offset.Y;
    Position.Real.OZ = -OZ - Omega_Offset.Z;
    
    Position.Real.GX = GX;
    Position.Real.GY = GY;
    Position.Real.GZ = GZ;
    
    return 0;
}

