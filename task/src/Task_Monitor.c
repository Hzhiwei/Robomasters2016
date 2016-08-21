
#include "Handler.h"

#include "Task_Monitor.h"

#include "bsp_oled.h"
#include "Driver_Bell.h"
#include "Driver_DBUS.h"
#include "Driver_Judge.h"
#include "Driver_vision.h"
#include "Driver_Chassis.h"
#include "Driver_mpu9250.h"
#include "Driver_CloudMotor.h"
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
    static uint8_t WarningCounter = 0;
    uint8_t WarningLocation = 0;
    //系统状态，每一位对应一个错误，0正常，1错误
/****************************************************************
    15  |   14  |   13  |   12  |   11  |   10  |   9   |   8   |
        |       |       |       |       |裁判系统|左前底盘|右前底盘|
-----------------------------------------------------------------
    7   |   6   |   5   |   4   |   3   |   2   |   1   |   0   |
左后底盘|右后底盘|底盘Gyr|    -  | vision|  Yaw  | Pitch |  DBUS |
****************************************************************/
    uint16_t SysErrorStatus = 0;
    
    for(;;)
    {
/**************************  ↓   数据帧率统计   ↓  **************************/
        //DBUS帧率统计
        DBUSFrameRate = DBUSFrameCounter;	
        DBUSFrameCounter = 0;
        
        //云台电机帧率统计
        CloudParam.Pitch.FrameRate = CloudParam.Pitch.FrameCounter;
        CloudParam.Pitch.FrameCounter = 0;
        CloudParam.Yaw.FrameRate = CloudParam.Yaw.FrameCounter;
        CloudParam.Yaw.FrameCounter = 0;
        
        //底盘陀螺仪数据帧统计
        SuperGyoFrameRate = SuperGyoFrameCounter;
        SuperGyoFrameCounter = 0;
        
        //视觉帧率统计
        PCFrameRate = PCFrameCounter;
        PCFrameCounter = 0;
        
        //底盘电机帧率统计
        ChassisFrameRate[0] = ChassisFrameCounter[0];
        ChassisFrameCounter[0] = 0;
        ChassisFrameRate[1] = ChassisFrameCounter[1];
        ChassisFrameCounter[1] = 0;
        ChassisFrameRate[2] = ChassisFrameCounter[2];
        ChassisFrameCounter[2] = 0;
        ChassisFrameRate[3] = ChassisFrameCounter[3];
        ChassisFrameCounter[3] = 0;
        
        //裁判系统帧率统计
        JudgeFrameRate = JudgeFrameCounter;
        JudgeFrameCounter = 0;
        
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
        if(CloudParam.Pitch.FrameRate < 30)
        {
            SysErrorStatus |= 0x0002;
        }
        else
        {
            SysErrorStatus &= 0xFFFD;
        }
        //Yaw云台帧率过低
        if(CloudParam.Yaw.FrameRate < 30)
        {
            SysErrorStatus |= 0x0004;
        }
        else
        {
            SysErrorStatus &= 0xFFFB;
        }
        //视觉帧率过低
        if(PCFrameRate < 2)
        {
            SysErrorStatus |= 0x0008;
        }
        else
        {
            SysErrorStatus &= 0xFFF7;
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
        //左前底盘电机帧率过低
        if(ChassisFrameRate[0] < 30)
        {
            SysErrorStatus |= 0x0040;
        }
        else
        {
            SysErrorStatus &= 0xFFBF;
        }
        //右前底盘电机帧率过低
        if(ChassisFrameRate[1] < 30)
        {
            SysErrorStatus |= 0x0080;
        }
        else
        {
            SysErrorStatus &= 0xFF7F;
        }
        //左后底盘电机帧率过低
        if(ChassisFrameRate[2] < 30)
        {
            SysErrorStatus |= 0x0100;
        }
        else
        {
            SysErrorStatus &= 0xFEFF;
        }
        //右后底盘电机帧率过低
        if(ChassisFrameRate[3] < 30)
        {
            SysErrorStatus |= 0x0200;
        }
        else
        {
            SysErrorStatus &= 0xFDFF;
        }
        //裁判系统帧率过低
        if(JudgeFrameRate < 3)
        {
            SysErrorStatus |= 0x0400;
            ChassisMaxSumCurrent = 2380;
        }
        else
        {
            SysErrorStatus &= 0xFBFF;
        }
        
/**************************  ↑   数据帧率统计   ↑  **************************/
/**************************************************************************************************/
/**************************  ↓   警告   ↓  **************************/
        if(WarningCounter == 0)
        {
            WarningLocation = 0;
            GUI_WidgetText_SetText(Oled_Handler, "");
            
#if INFANTRY == 7
            
            //yaw云台电机
            if(SysErrorStatus & 0x0004)
            {
#if USEESP8266orOLEDorOLED == 0
                GUI_WidgetText_AddText(Oled_Handler, "Yaw   ");
#endif
                WarningLocation++;
            }
            //pitch云台电机
            if(SysErrorStatus & 0x0002)
            {
#if USEESP8266orOLEDorOLED == 0
                GUI_WidgetText_AddText(Oled_Handler, "Pitch ");
#endif
                WarningLocation++;
            }
            //主机串口
            if(SysErrorStatus & 0x0008)
            {
#if USEESP8266orOLEDorOLED == 0
                GUI_WidgetText_AddText(Oled_Handler, "PC    ");
#endif
                WarningLocation++;
            }
            //底盘陀螺仪
            if(SysErrorStatus & 0x0020)
            {
#if USEESP8266orOLEDorOLED == 0
                GUI_WidgetText_AddText(Oled_Handler, "GROY  ");
#endif
                WarningLocation++;
            }
            //裁判系统
            if(SysErrorStatus & 0x0400)
            {
#if USEESP8266orOLEDorOLED == 0
                GUI_WidgetText_AddText(Oled_Handler, "Judge ");
#endif
                WarningLocation++;
            }
            //无警告
            if(WarningLocation == 0)
            {
#if USEESP8266orOLEDorOLED == 0
                GUI_WidgetText_AddText(Oled_Handler, "OK    ");
#endif
                WarningLocation++;
            }
            
#else
            
            //DBUS
            if(SysErrorStatus & 0x0001)
            {
#if USEESP8266orOLEDorOLED == 0
                GUI_WidgetText_AddText(Oled_Handler, "DBUS  ");
#endif
                WarningLocation++;
            }
            //pitch云台电机
            if(SysErrorStatus & 0x0002)
            {
#if USEESP8266orOLEDorOLED == 0
                GUI_WidgetText_AddText(Oled_Handler, "Pitch ");
#endif
                WarningLocation++;
            }
            //yaw云台电机
            if(SysErrorStatus & 0x0004)
            {
#if USEESP8266orOLEDorOLED == 0
                GUI_WidgetText_AddText(Oled_Handler, "Yaw   ");
#endif
                WarningLocation++;
            }
            //主机串口
            if(SysErrorStatus & 0x0008)
            {
#if USEESP8266orOLEDorOLED == 0
                GUI_WidgetText_AddText(Oled_Handler, "PC    ");
#endif
                WarningLocation++;
            }
            //底盘陀螺仪
            if(SysErrorStatus & 0x0020)
            {
#if USEESP8266orOLEDorOLED == 0
                GUI_WidgetText_AddText(Oled_Handler, "Groy  ");
#endif
                WarningLocation++;
            }
            //左前底盘电机
            if(SysErrorStatus & 0x0040)
            {
#if USEESP8266orOLEDorOLED == 0
                GUI_WidgetText_AddText(Oled_Handler, "LF    ");
#endif
                WarningLocation++;
            }
            //右前底盘电机
            if(SysErrorStatus & 0x0080)
            {
#if USEESP8266orOLEDorOLED == 0
                GUI_WidgetText_AddText(Oled_Handler, "RF    ");
#endif
                WarningLocation++;
            }
            //左后底盘电机
            if(SysErrorStatus & 0x0100)
            {
#if USEESP8266orOLEDorOLED == 0
                GUI_WidgetText_AddText(Oled_Handler, "LB    ");
#endif
                WarningLocation++;
            }
            //右后底盘电机
            if(SysErrorStatus & 0x0200)
            {
#if USEESP8266orOLEDorOLED == 0
                GUI_WidgetText_AddText(Oled_Handler, "RB    ");
#endif
                WarningLocation++;
            }
            //裁判系统
            if(SysErrorStatus & 0x0400)
            {
#if USEESP8266orOLEDorOLED == 0
                GUI_WidgetText_AddText(Oled_Handler, "Judge ");
#endif
                WarningLocation++;
            }
            //无警告
            if(WarningLocation == 0)
            {
#if USEESP8266orOLEDorOLED == 0
                GUI_WidgetText_AddText(Oled_Handler, "OK    ");
#endif
                WarningLocation++;
            }
            
#endif
            
        GUI_WidgetText_Show(Oled_Handler, IS, IS);
            
        }
        else
        {
            if(WarningCounter >= 2)
            {
                WarningCounter = 0;
            }
            else
            {
                WarningCounter++;
            }
        }
/**************************  ↑   警告标志置位   ↑  **************************/

        
        vTaskDelay(200);
    }
}
