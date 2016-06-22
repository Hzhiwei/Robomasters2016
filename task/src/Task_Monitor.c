
#include "Handler.h"

#include "Task_Monitor.h"

#include "Driver_Bell.h"
#include "Driver_DBUS.h"
#include "Driver_Motor.h"
#include "Driver_vision.h"
#include "Driver_mpu9250.h"
#include "Driver_SuperGyroscope.h"

#include "Config.h"

int16_t speed = 0;

/**
  * @brief  监视器任务
  * @param  unused
  * @retval voif
  */
void Task_Monitor(void *Parameters)
{
    //系统状态，每一位对应一个错误，0正常，1错误
/****************************************************************
    15  |   14  |   13  |   12  |   11  |   10  |   9   |   8   |
        |       |       |       |       |       |       |       |
-----------------------------------------------------------------
    7   |   6   |   5   |   4   |   3   |   2   |   1   |   0   |
        |       |底盘Gyr|右摩擦  |左摩擦 |  Yaw  | Pitch |  DBUS |
****************************************************************/
    uint16_t SysErrorStatus = 0;
    
    for(;;)
    {
/**************************  ↓   数据帧率统计   ↓  **************************/
        //DBUS帧率统计
        DBUSFrameRate = DBUSFrameCounter;	
        DBUSFrameCounter = 0;
        
        //云台电机帧率统计
        PitchMotorFrameRate = PitchMotorFrameCounter;
        PitchMotorFrameCounter = 0;
        YawMotorFrameRate = YawMotorFrameCounter;
        YawMotorFrameCounter = 0;
        
        //底盘陀螺仪数据帧统计
        SuperGyoFrameRate = SuperGyoFrameCounter;
        SuperGyoFrameCounter = 0;
        
        //视觉帧率统计
        PCFrameRate = PCFrameCounter;
        PCFrameCounter = 0;
        
        //DBUS帧率过低
        if(DBUSFrameRate < 6)
        {
            SysErrorStatus |= 0x0001;
        }
        else
        {
            SysErrorStatus &= 0xFFFE;
        }
        //Pitch云台帧率过低
        if(PitchMotorFrameRate < 30)
        {
            SysErrorStatus |= 0x0002;
        }
        else
        {
            SysErrorStatus &= 0xFFFD;
        }
        //Yaw云台帧率过低
        if(YawMotorFrameRate < 30)
        {
            SysErrorStatus |= 0x0004;
        }
        else
        {
            SysErrorStatus &= 0xFFFB;
        }
        //底盘陀螺仪帧率过低
        if(SuperGyoFrameRate < 30)
        {
            SysErrorStatus |= 0x0020;
        }
        else
        {
            SysErrorStatus &= 0xFFDF;
        }
/**************************  ↑   数据帧率统计   ↑  **************************/
/**************************************************************************************************/
/**************************  ↓   警告标志置位   ↓  **************************/
        //DBUS
        if(SysErrorStatus & 0x0001)
        {
            Bell_BarkWarning(2, MAXAWarning);
        }
        //yaw云台电机
        else if(SysErrorStatus & 0x0004)
        {
            Bell_BarkWarning(3, MAXAWarning);
        }
        //pitch云台电机
        else if(SysErrorStatus & 0x0002)
        {
            Bell_BarkWarning(4, MAXAWarning);
        }
        //底盘陀螺仪
        else if(SysErrorStatus & 0x0020)
        {
            Bell_BarkWarning(7, MAXAWarning);
        }
        else
        {
            Bell_BarkWarning(0, MAXAWarning);
        }
            

        
        vTaskDelay(200);
    }
}
