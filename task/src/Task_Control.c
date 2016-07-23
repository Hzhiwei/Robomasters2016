
#include "Task_Control.h"

#include "Config.h"

#include "mpu9250dmp.h"
#include "Driver_DBUS.h"
#include "Driver_vision.h"
#include "Driver_Chassis.h"
#include "Driver_mpu9250.h"
#include "Driver_Control.h"
#include "Driver_Steering.h"
#include "Driver_PokeMotor.h"
#include "Driver_FricMotor.h"
#include "Driver_CloudMotor.h"
#include "Driver_StatusMachine.h"
#include "Driver_SuperGyroscope.h"


/**
  * @brief  控制任务（周期5ms）
  * @param  ubused
  * @retval void
  */
    int16_t aaa = 0, bbb = 0;
    
void Task_Control(void *Parameters)
{
    portTickType CurrentControlTick = 0;                //当前系统时间
    uint32_t dmpresetCounter;                            //dmp重启计数器
    
    for(;;)
    {
        CurrentControlTick = xTaskGetTickCount();
        
/************************  ↓  姿态更新  ↓ ************************/
        MPU9250_Update();
        
        //DMP每5s重启，否则会傻逼
        if(dmpresetCounter >= 5000)
        {
            dmpresetCounter = 0;
            mpu_set_dmp_state(0);
            mpu_set_dmp_state(1);
        }
        else
        {
            dmpresetCounter++;
        }
/************************  ↑  姿态更新  ↑ ************************/
/**************************************************************************************************/
/*********************  ↓  状态机状态更新  ↓ *********************/
        StatusMachine_Update();
/*********************  ↑  状态机状态更新  ↑ *********************/
/**************************************************************************************************/
/*********************  ↓  根据状态机控制  ↓ *********************/
        
        Cloud_YawAngleSet(-DBUS_ReceiveData.ch3 / 11.0F, AngleMode_ABS);
		Cloud_PitchAngleSet(DBUS_ReceiveData.ch4 / 6.0F);
        
        if(ControlMode != ControlMode_Protect)
        {
            Cloud_Adjust(1);
        }
        else
        {
            Cloud_Adjust(0);
        }
        
        
//    CloudMotorCurrent(aaa, bbb);
        
        
/*********************  ↑  根据状态机控制  ↑ *********************/
/**************************************************************************************************/
/**********************  ↓  拨弹电机控制   ↓ ***********************/
        PokeMotor_Adjust();
/*********************  ↑  拨弹电机PID  ↑  ***********************/
/**************************************************************************************************/
/*********************  ↓  云台PID      ↓  **********************/
/*********************  ↑  云台PID      ↑  **********************/
/**************************************************************************************************/
/*********************  ↓   电机控制    ↓  **********************/
/*********************  ↑   电机控制    ↑  **********************/
/**************************************************************************************************/
        
		vTaskDelayUntil(&CurrentControlTick, 5 / portTICK_RATE_MS);
    }
}



