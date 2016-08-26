
#define __DRIVER_VISION_GLOBALS

#include "Config.h"
#include "Driver_vision.h"
#include "Driver_mpu9250.h"
#include "Driver_CloudMotor.h"
#include <math.h>


//攻击模式指令
const uint8_t AttackModeOrder[PCDATALENGTH] = {0x00, 0x00, 0x09, 0x00, 0x00,
                                               0x00, 0x00, 0x00, 0x00, 0x00,
                                               0x00, 0x00, 0x00, 0x00, 0x00,
                                               0x00, 0x00, 0x00, 0x00, 0x00,
                                               0x00, 0x00, 0x00, 0x00, 0x00,
                                               0x00};

//大符模式指令
const uint8_t BigSymbolModeOrder[PCDATALENGTH] = {0x00, 0x00, 0x28, 0x00, 0x00,
                                                  0x00, 0x00, 0x00, 0x00, 0x00,
                                                  0x00, 0x00, 0x00, 0x00, 0x00,
                                                  0x00, 0x00, 0x00, 0x00, 0x00,
                                                  0x00, 0x00, 0x00, 0x00, 0x00,
                                                  0x00};

//红目标指令
const uint8_t RedEnemyOrder[PCDATALENGTH] = {0x00, 0x00, 0xFA, 0x00, 0x00,
                                                  0x00, 0x00, 0x00, 0x00, 0x00,
                                                  0x00, 0x00, 0x00, 0x00, 0x00,
                                                  0x00, 0x00, 0x00, 0x00, 0x00,
                                                  0x00, 0x00, 0x00, 0x00, 0x00,
                                                  0x00};

//蓝目标指令
const uint8_t BlueEnemyOrder[PCDATALENGTH] = {0x00, 0x00, 0xFB, 0x00, 0x00,
                                                  0x00, 0x00, 0x00, 0x00, 0x00,
                                                  0x00, 0x00, 0x00, 0x00, 0x00,
                                                  0x00, 0x00, 0x00, 0x00, 0x00,
                                                  0x00, 0x00, 0x00, 0x00, 0x00,
                                                  0x00};

//关机模式指令
const uint8_t ShutdownOrder[PCDATALENGTH] = {0x00, 0x00, 0xFF, 0x00, 0x00,
                                             0x00, 0x00, 0x00, 0x00, 0x00,
                                             0x00, 0x00, 0x00, 0x00, 0x00,
                                             0x00, 0x00, 0x00, 0x00, 0x00,
                                             0x00, 0x00, 0x00, 0x00, 0x00,
                                             0x00};



/*
X正 右
Y正 下
Z正 前
*/


/**
  * @brief  视觉初始化
  * @param  void
  * @retval void
  */
void Vision_InitConfig(void)
{
    PCDataBufferPoint = 0;
    EnemyDataBufferPoint = 0;
    
#if INFANTRY == 7
    PCVisionTargetColor = -1;
#endif
    
    EnemyDataBuffer[EnemyDataBufferPoint].ID = 1;
    EnemyDataBuffer[EnemyDataBufferPoint].Time = 0;
    EnemyDataBuffer[EnemyDataBufferPoint].X = 0;
    EnemyDataBuffer[EnemyDataBufferPoint].Y = 0;
    EnemyDataBuffer[EnemyDataBufferPoint].Z = 1;
}


/**
  * @brief  将直角坐标系转换为电机编码器角度
  * @param  X
  * @param  Y
  * @param  Z
  * @param  0 不考虑重力加速度      1 考虑重力加速度
  * @retval 角度(编码器，中间为0）
  */
//    AngleF_Struct ReturnData;
//    float distance;
//    float X0, Y0, Z0;
//    float L;
//    float AngleA;
AngleF_Struct RecToPolar(float X, float Y, float Z, float RealPitch, uint16_t PitchEncoder, uint8_t mode)
{
    
//    L = sqrt(X * X + Z * Z);
//    AngleA = (CloudParam.Pitch.RealABSAngle - (CloudParam.Pitch.RealEncoderAngle - PitchEncoderCenter) * 0.04395F) * 0.0087266F;
//    X0 = X;
//    Y0 = L * sin(AngleA);
//    Z0 = Z;
//    distance = L * cos(AngleA);
//    
//    if((X0 == 0) && (Y0 == 0) && (Z0 == 0))
//    {
//        ReturnData.H = 0;
//        ReturnData.V = 0;
//        return ReturnData;
//    }
//    
//    ReturnData.H =  - atan(X0 / Z0) * 57.2958F;
//    
//    if(mode == 0)
//    {
//    //不考虑重力加速度
//        ReturnData.V = -atan(Y0 / distance) * 57.2958F;
//    }
//    else
//    {
//    //考虑重力加速度
//        ReturnData.V = (atan(((ACCofGravity * distance * distance) / (GUNSpeed * GUNSpeed) - Y0) / sqrt(Y0 * Y0 + distance * distance)) - atan(Y0 / distance)) / 2 * 57.2958F;
//    }
//    
//    return ReturnData;









    AngleF_Struct ReturnData;
    float Distance = sqrt(X * X + Z * Z);
    float distance;;
    float X0, Y0, Z0;
    
    X0 = X;
    Y0 = Y;
    Z0 = Z;
    
    if((X0 == 0) && (Y0 == 0) && (Z0 == 0))
    {
        ReturnData.H = 0;
        ReturnData.V = 0;
        return ReturnData;
    }
    
    ReturnData.H =  - atan(X / Z0) * 57.2958F;
    
    distance = sqrt(X0 * X0 + Z0 * Z0);
    if(mode == 0)
    {
    //不考虑重力加速度
        ReturnData.V = -atan(Y0 / distance) * 57.2958F;
    }
    else
    {
    //考虑重力加速度
        ReturnData.V = (atan(((ACCofGravity * distance * distance) / (GUNSpeed * GUNSpeed) - Y0) / sqrt(Y0 * Y0 + distance * distance)) - atan(Y0 / distance)) / 2 * 57.2958F;
    }
    
    return ReturnData;
}


/**
  * @brief  路径拟合核心函数，根据输入进行一次拟合
  * @param  预判样本时间长度
  * @param  预判时间
  * @param  预判位置
  * @retval 0 成功        1 失败
  */
uint8_t ForcastCore(uint16_t SampleTime, uint16_t ForcastTime, Point_Struct *ForcastPoint)
{
    int RelativeTime;                       //相对时间，防止绝对时间超范围
    uint16_t index = 0, Currentindex;       
    uint16_t SampleNum = 0;
    

    float A = 0;
    float B = 0;
    float C = 0;
    float D = 0;
    float E = 0;
    
    float Fx = 0;
    float Gx = 0;
    float Hx = 0;
    float Ix = 0;
    float Jx = 0;
    float Kx = 0;
    float Lx = 0;
    float Mx = 0;
    float Nx = 0;
    
    float Fz = 0;
    float Gz = 0;
    float Hz = 0;
    float Iz = 0;
    float Jz = 0;
    float Kz = 0;
    float Lz = 0;
    float Mz = 0;
    float Nz = 0;
    
    float Pax, Pbx, Pcx;
    float Paz, Pbz, Pcz;
    
    //寻找起点
    for(SampleNum = 0; SampleNum < ENEMYDATABUFFERLENGHT; SampleNum++)
    {
        if(EnemyDataBuffer[(EnemyDataBufferPoint + ENEMYDATABUFFERLENGHT - SampleNum - 1) % ENEMYDATABUFFERLENGHT].Time + SampleTime < EnemyDataBuffer[EnemyDataBufferPoint].Time)
        {
            break;
        }
    }
    
    //拟合数据量过少
    if(SampleNum < 5)
    {
        return 1;
    }
    
    E =  -(1 + SampleNum);
    
    //数据拟合
    for(index = 0; index <= SampleNum; index++)
    {
        Currentindex = (EnemyDataBufferPoint + ENEMYDATABUFFERLENGHT - index) % ENEMYDATABUFFERLENGHT;
        
        RelativeTime = EnemyDataBuffer[Currentindex].Time - EnemyDataBuffer[(EnemyDataBufferPoint + ENEMYDATABUFFERLENGHT - SampleNum) % ENEMYDATABUFFERLENGHT].Time;        
        
        A = A - RelativeTime * RelativeTime * RelativeTime * RelativeTime;
        B = B - RelativeTime * RelativeTime * RelativeTime;
        C = C - RelativeTime * RelativeTime;
        D = D - RelativeTime;
        
        Fx = Fx + RelativeTime * RelativeTime * EnemyDataBuffer[Currentindex].X;
        Gx = Gx + RelativeTime * EnemyDataBuffer[Currentindex].X;
        Hx = Hx + EnemyDataBuffer[Currentindex].X;
        
        Fz = Fz + RelativeTime * RelativeTime * EnemyDataBuffer[Currentindex].Z;
        Gz = Gz + RelativeTime * EnemyDataBuffer[Currentindex].Z;
        Hz = Hz + EnemyDataBuffer[Currentindex].Z;
    }
    

    Ix = D * Fx - C * Gx;
    Jx = A * D - B * C;
    Kx = B * D - C * C;
    Lx = E * Fx - Hx * C;
    Mx = A * E - C * C;
    Nx = B * E - C * D;
    
    Iz = D * Fz - C * Gz;
    Jz = A * D - B * C;
    Kz = B * D - C * C;
    Lz = E * Fz - Hz * C;
    Mz = A * E - C * C;
    Nz = B * E - C * D;
    
    //数据非法
    if((!(Mx * Kx - Jx * Nx)) ||
        (!Kx) ||
        (!C) ||
        (!(Mz * Kz - Jz * Nz)) ||
        (!Kz))
    {
        return 1;
    }

    Pax = (Ix * Nx - Lx * Kx) / (Mx * Kx - Jx * Nx);
    Pbx = -(Ix + Pax * Jx)  / Kx;
    Pcx = - (Fx + Pax * A + Pbx * B) / C;
    
    Paz = (Iz * Nz - Lz * Kz) / (Mz * Kz - Jz * Nz);
    Pbz = -(Iz + Paz * Jz) / Kz;
    Pcz = - (Fz + Paz * A + Pbz * B) / C;
    
    ForcastTime += EnemyDataBuffer[EnemyDataBufferPoint].Time - EnemyDataBuffer[(EnemyDataBufferPoint + ENEMYDATABUFFERLENGHT - SampleNum) % ENEMYDATABUFFERLENGHT].Time;
    
    ForcastPoint->X = (ForcastTime * ForcastTime * Pax + Pbx * ForcastTime + Pcx);
    ForcastPoint->Y = EnemyDataBuffer[Currentindex].Y;
    ForcastPoint->Z = (Paz * ForcastTime * ForcastTime + Pbz * ForcastTime + Pcz);
    
    return 0;
}


/**
  * @brief  进行一次拟合，输出目标角度（相对云台电机）
  * @param  拟合样本时间(ms)
  * @param  拟合时间长度(ms)
  * @param  时间模式    0 自定义时间（即ForcastTime）， 1 自动设定预判时间（根据距离及子弹速度确定）
  * @retval 0 二次拟合成功        1 因样本数据不够，拟合失败直接使用当前位置作为目标位置
  */
uint8_t ForcastOnce(uint16_t SampleTime, uint16_t ForcastTime, AngleF_Struct *ForcastAngle, uint8_t TimeMode)
{
    Point_Struct ForcastPoint;
    float distance;
    
    //自动调节拟合时间长度
    if(TimeMode)
    {
        distance = sqrt(EnemyDataBuffer[EnemyDataBufferPoint].X * EnemyDataBuffer[EnemyDataBufferPoint].X + EnemyDataBuffer[EnemyDataBufferPoint].Y * EnemyDataBuffer[EnemyDataBufferPoint].Y);
        ForcastTime = distance * 1000 / GUNSpeed;
    }
    
    if(ForcastCore(SampleTime, ForcastTime, &ForcastPoint) == 0)        //二次拟合成功
    {
        //目标点转换为目标角度
        *ForcastAngle = RecToPolar(ForcastPoint.X, 
                                    ForcastPoint.Y, 
                                    ForcastPoint.Z,
                                    Position.Euler.Pitch,
                                    CloudParam.Pitch.RealEncoderAngle,
                                    1);
        
        return 0;
    }
    //二次拟合失败
    else
    {
        //拟合失败直接使用当前位置作为目标位置
        *ForcastAngle = RecToPolar(EnemyDataBuffer[EnemyDataBufferPoint].X, 
                                    EnemyDataBuffer[EnemyDataBufferPoint].Y, 
                                    EnemyDataBuffer[EnemyDataBufferPoint].Z,
                                    Position.Euler.Pitch,
                                    CloudParam.Pitch.RealEncoderAngle,
                                    1);
        
        return 1;
    }
}
    
    
/**
  * @brief  给PC发指令
  * @param  指令
  * @retval void
  */
void SendPCOrder(PCOrder_Enum order)
{
//    if(DMA2_Stream7->NDTR == 0)
//    {
        if(PCOrder_Attack == order)
        {
            DMA_Cmd(DMA2_Stream7, DISABLE);                     //关闭 DMA 传输
            if (DMA_GetCmdStatus(DMA2_Stream7) != DISABLE)      //确保 DMA 可以被设置
            {
                return;
            }                
            DMA_ClearFlag(DMA2_Stream7, DMA_FLAG_TCIF7 | DMA_FLAG_HTIF7);       //清空标志位
            DMA2_Stream7->M0AR = (uint32_t)AttackModeOrder;     //设置数据
            DMA_SetCurrDataCounter(DMA2_Stream7, PCDATALENGTH); //数据传输量
            DMA_Cmd(DMA2_Stream7, ENABLE);                      //开启 DMA 传输
        }
        else if(PCOrder_BigSample == order)
        {
            DMA_Cmd(DMA2_Stream7, DISABLE);                     //关闭 DMA 传输
            if (DMA_GetCmdStatus(DMA2_Stream7) != DISABLE)      //确保 DMA 可以被设置
            {
                return;
            }            
            DMA_ClearFlag(DMA2_Stream7, DMA_FLAG_TCIF7 | DMA_FLAG_HTIF7);       //清空标志位
            DMA2_Stream7->M0AR = (uint32_t)BigSymbolModeOrder;  //设置数据
            DMA_SetCurrDataCounter(DMA2_Stream7, PCDATALENGTH); //数据传输量
            DMA_Cmd(DMA2_Stream7, ENABLE);                      //开启 DMA 传输
        }
        else if(PCOrder_Shutdown == order)
        {
            DMA_Cmd(DMA2_Stream7, DISABLE);                     //关闭 DMA 传输
            if (DMA_GetCmdStatus(DMA2_Stream7) != DISABLE)      //确保 DMA 可以被设置
            {
                return;
            }            
            DMA_ClearFlag(DMA2_Stream7, DMA_FLAG_TCIF7 | DMA_FLAG_HTIF7);       //清空标志位
            DMA2_Stream7->M0AR = (uint32_t)ShutdownOrder;       //设置数据
            DMA_SetCurrDataCounter(DMA2_Stream7, PCDATALENGTH); //数据传输量
            DMA_Cmd(DMA2_Stream7, ENABLE);                      //开启 DMA 传输
        }
//    }
}


/**
  * @brief  发送目标颜色
  * @param  'R' 红色      'B' 蓝色      其他 红色
  * @retval void
  */
void SendEnemyColor(char Co)
{
    if('B' == Co)
    {
        DMA_Cmd(DMA2_Stream7, DISABLE);                     //关闭 DMA 传输
        if (DMA_GetCmdStatus(DMA2_Stream7) != DISABLE)      //确保 DMA 可以被设置
        {
            return;
        }            
        DMA_ClearFlag(DMA2_Stream7, DMA_FLAG_TCIF7 | DMA_FLAG_HTIF7);       //清空标志位
        DMA2_Stream7->M0AR = (uint32_t)BlueEnemyOrder;      //设置数据
        DMA_SetCurrDataCounter(DMA2_Stream7, PCDATALENGTH); //数据传输量
        DMA_Cmd(DMA2_Stream7, ENABLE);                      //开启 DMA 传输
    }
    else
    {
        DMA_Cmd(DMA2_Stream7, DISABLE);                     //关闭 DMA 传输
        if (DMA_GetCmdStatus(DMA2_Stream7) != DISABLE)      //确保 DMA 可以被设置
        {
            return;
        }            
        DMA_ClearFlag(DMA2_Stream7, DMA_FLAG_TCIF7 | DMA_FLAG_HTIF7);       //清空标志位
        DMA2_Stream7->M0AR = (uint32_t)RedEnemyOrder;       //设置数据
        DMA_SetCurrDataCounter(DMA2_Stream7, PCDATALENGTH); //数据传输量
        DMA_Cmd(DMA2_Stream7, ENABLE);                      //开启 DMA 传输
    }
}






