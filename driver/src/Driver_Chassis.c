
#define __DRIVER_CHASSIS_GLOBALS

#include "Config.h"
#include "Handler.h"
#include "OSinclude.h"
#include "Task_CANSend.h"
#include "Driver_Chassis.h"
#include "Driver_Control.h"
#include "Driver_CloudMotor.h"
#include "Driver_SuperGyroscope.h"


volatile float Last_Spd[4];
//麦轮解算矩阵
const signed char MecanumCalculateMAT[4][3] = { 
	{1, 1, 1},
	{1,-1,-1},
	{1,-1, 1}, 
	{1, 1,-1}
};


/**
  * @brief  底盘初始化
  * @param  void 
  * @retval void
  */
void Chassis_InitConfig(void)
{
    for(int i=0;i<=3;i++)
    {
        Last_Spd[i]=0;
    }
    
    ChassisMaxSumCurrent = 2380.0F;
    
    ChassisParam.VX = 0;
    ChassisParam.VY = 0;
    ChassisParam.Omega = 0;
    ChassisParam.SpeedLevel = ChassisSpeedLevel_Hight;
    
    for(int i = 0; i < 4; i++)
    {
        ChassisFrameCounter[i] = 0;
        ChassisFrameRate[i] = 0;
    }
}


/**
  * @brief  角速度设置
  * @param  目标角速度
  * @retval void
  */
void Chassis_OmegaSet(float Target)
{
    ChassisParam.Omega = Target;
}


/**
  * @brief  XY方向速度设置
  * @param  X速度
  * @param  Y速度
  * @retval void
  */
void Chassis_SpeedSet(float XSpeed, float YSpeed)
{
    XSpeed = XSpeed > MaxWheelSpeed ? MaxWheelSpeed : XSpeed;
    XSpeed = XSpeed < -MaxWheelSpeed ? -MaxWheelSpeed : XSpeed;
    
    YSpeed = YSpeed > MaxWheelSpeed ? MaxWheelSpeed : YSpeed;
    YSpeed = YSpeed < -MaxWheelSpeed ? -MaxWheelSpeed : YSpeed;
    
    ChassisParam.VX = XSpeed;
    ChassisParam.VY = YSpeed;
}



/**
  * @brief  底盘调节
  * @param  void  
  * @retval void
  */
void Chassis_Adjust(void)
{
    int16_t WheelSpeed[4];
    int16_t PowerSum;
    
    int16_t ABSSpeed[4];
    
    //麦轮解算
    MecanumCalculate(ChassisParam.VX, ChassisParam.VY, ChassisParam.Omega, WheelSpeed);
    
    ChassisParam.LF.TargetSpeed = WheelSpeed[0];
    ChassisParam.RF.TargetSpeed = WheelSpeed[1];
    ChassisParam.LB.TargetSpeed = WheelSpeed[2];
    ChassisParam.RB.TargetSpeed = WheelSpeed[3];
//    
//    ABSSpeed[0] = (ChassisParam.LF.TargetSpeed > 0 ? ChassisParam.LF.TargetSpeed : -ChassisParam.LF.TargetSpeed);
//    ABSSpeed[1] = (ChassisParam.RF.TargetSpeed > 0 ? ChassisParam.RF.TargetSpeed : -ChassisParam.RF.TargetSpeed);
//    ABSSpeed[2] = (ChassisParam.LB.TargetSpeed > 0 ? ChassisParam.LB.TargetSpeed : -ChassisParam.LB.TargetSpeed);
//    ABSSpeed[3] = (ChassisParam.RB.TargetSpeed > 0 ? ChassisParam.RB.TargetSpeed : -ChassisParam.RB.TargetSpeed);
//    
//    //功率分配
//    PowerSum = ABSSpeed[0] + ABSSpeed[1] + ABSSpeed[2] + ABSSpeed[3];
//    
//    if(PowerSum > 0)
//    {
//        ChassisParam.LF.LimitCurrent = ChassisMaxSumCurrent * ABSSpeed[0] / PowerSum;
//        ChassisParam.RF.LimitCurrent = ChassisMaxSumCurrent * ABSSpeed[1] / PowerSum;
//        ChassisParam.LB.LimitCurrent = ChassisMaxSumCurrent * ABSSpeed[2] / PowerSum;
//        ChassisParam.RB.LimitCurrent = ChassisMaxSumCurrent * ABSSpeed[3] / PowerSum;
//    }
//    else
//    {
//        ChassisParam.LF.LimitCurrent = ChassisMaxSumCurrent / 4;
//        ChassisParam.RF.LimitCurrent = ChassisMaxSumCurrent / 4;
//        ChassisParam.LB.LimitCurrent = ChassisMaxSumCurrent / 4;
//        ChassisParam.RB.LimitCurrent = ChassisMaxSumCurrent / 4;
//    }

    ABSSpeed[0] = (ChassisParam.LF.NeedCurrent > 0 ? ChassisParam.LF.NeedCurrent : -ChassisParam.LF.NeedCurrent);
    ABSSpeed[1] = (ChassisParam.RF.NeedCurrent > 0 ? ChassisParam.RF.NeedCurrent : -ChassisParam.RF.NeedCurrent);
    ABSSpeed[2] = (ChassisParam.LB.NeedCurrent > 0 ? ChassisParam.LB.NeedCurrent : -ChassisParam.LB.NeedCurrent);
    ABSSpeed[3] = (ChassisParam.RB.NeedCurrent > 0 ? ChassisParam.RB.NeedCurrent : -ChassisParam.RB.NeedCurrent);
    
    //功率分配
    PowerSum = ABSSpeed[0] + ABSSpeed[1] + ABSSpeed[2] + ABSSpeed[3];
    
    if(PowerSum > 0)
    {
        ChassisParam.LF.LimitCurrent = ChassisMaxSumCurrent * ABSSpeed[0] / PowerSum;
        ChassisParam.RF.LimitCurrent = ChassisMaxSumCurrent * ABSSpeed[1] / PowerSum;
        ChassisParam.LB.LimitCurrent = ChassisMaxSumCurrent * ABSSpeed[2] / PowerSum;
        ChassisParam.RB.LimitCurrent = ChassisMaxSumCurrent * ABSSpeed[3] / PowerSum;
    }
    else
    {
        ChassisParam.LF.LimitCurrent = ChassisMaxSumCurrent / 4;
        ChassisParam.RF.LimitCurrent = ChassisMaxSumCurrent / 4;
        ChassisParam.LB.LimitCurrent = ChassisMaxSumCurrent / 4;
        ChassisParam.RB.LimitCurrent = ChassisMaxSumCurrent / 4;
    }
}


/**
  * @brief  发送速度与限制电流
  * @param  模式  1 发送数据结构中的目标速度与限制电流     0 限制速度与限制电流为0（用于紧急停机）
  * @retval void
  */
void Chassis_SendMotorParam(uint8_t mode)
{
    static  CanSend_Type   SendData;
    
    #if CANPORT == 1
    SendData.CANx = 1;
    #else
    SendData.CANx = 2;
    #endif
    
    SendData.SendCanTxMsg.DLC   =   8;
    SendData.SendCanTxMsg.IDE   =   CAN_ID_STD;
    SendData.SendCanTxMsg.RTR   =   CAN_RTR_Data;
    
    if(mode)
    {
        SendData.SendCanTxMsg.StdId =   CHASSISSPEEDSETCANID;
        SendData.SendCanTxMsg.Data[1] = ChassisParam.LF.TargetSpeed >> 8;
        SendData.SendCanTxMsg.Data[0] = ChassisParam.LF.TargetSpeed;
        SendData.SendCanTxMsg.Data[3] = ChassisParam.RF.TargetSpeed >> 8;
        SendData.SendCanTxMsg.Data[2] = ChassisParam.RF.TargetSpeed;
        SendData.SendCanTxMsg.Data[5] = ChassisParam.LB.TargetSpeed >> 8;
        SendData.SendCanTxMsg.Data[4] = ChassisParam.LB.TargetSpeed;
        SendData.SendCanTxMsg.Data[7] = ChassisParam.RB.TargetSpeed >> 8;
        SendData.SendCanTxMsg.Data[6] = ChassisParam.RB.TargetSpeed;
        xQueueSend(Queue_CANSend, &SendData, 10);
        
        SendData.SendCanTxMsg.StdId =   CHASSISCURRENTSETCANID;
        SendData.SendCanTxMsg.Data[1] = ChassisParam.LF.LimitCurrent >> 8;
        SendData.SendCanTxMsg.Data[0] = ChassisParam.LF.LimitCurrent;
        SendData.SendCanTxMsg.Data[3] = ChassisParam.RF.LimitCurrent >> 8;
        SendData.SendCanTxMsg.Data[2] = ChassisParam.RF.LimitCurrent;
        SendData.SendCanTxMsg.Data[5] = ChassisParam.LB.LimitCurrent >> 8;
        SendData.SendCanTxMsg.Data[4] = ChassisParam.LB.LimitCurrent;
        SendData.SendCanTxMsg.Data[7] = ChassisParam.RB.LimitCurrent >> 8;
        SendData.SendCanTxMsg.Data[6] = ChassisParam.RB.LimitCurrent;
        xQueueSend(Queue_CANSend, &SendData, 10);
    }
    else
    {
        SendData.SendCanTxMsg.StdId =   CHASSISSPEEDSETCANID;
        SendData.SendCanTxMsg.Data[0] = 0;
        SendData.SendCanTxMsg.Data[1] = 0;
        SendData.SendCanTxMsg.Data[2] = 0;
        SendData.SendCanTxMsg.Data[3] = 0;
        SendData.SendCanTxMsg.Data[4] = 0;
        SendData.SendCanTxMsg.Data[5] = 0;
        SendData.SendCanTxMsg.Data[6] = 0;
        SendData.SendCanTxMsg.Data[7] = 0;
        xQueueSend(Queue_CANSend, &SendData, 10);
        
        SendData.SendCanTxMsg.StdId =   CHASSISCURRENTSETCANID;
        SendData.SendCanTxMsg.Data[0] = 0;
        SendData.SendCanTxMsg.Data[1] = 0;
        SendData.SendCanTxMsg.Data[2] = 0;
        SendData.SendCanTxMsg.Data[3] = 0;
        SendData.SendCanTxMsg.Data[4] = 0;
        SendData.SendCanTxMsg.Data[5] = 0;
        SendData.SendCanTxMsg.Data[6] = 0;
        SendData.SendCanTxMsg.Data[7] = 0;
        xQueueSend(Queue_CANSend, &SendData, 10);
    }
}



/**
  * @brief  电机调试模式，电流700，速度0
  * @param  void
  * @retval void
  */
void Chassis_MotorDebug(void)
{
    static  CanSend_Type   SendData;
    
    #if CANPORT == 1
    SendData.CANx = 1;
    #else
    SendData.CANx = 2;
    #endif
    
    SendData.SendCanTxMsg.DLC   =   8;
    SendData.SendCanTxMsg.IDE   =   CAN_ID_STD;
    SendData.SendCanTxMsg.RTR   =   CAN_RTR_Data;
    
    SendData.SendCanTxMsg.StdId =   CHASSISSPEEDSETCANID;
    SendData.SendCanTxMsg.Data[1] = 0;
    SendData.SendCanTxMsg.Data[0] = 0;
    SendData.SendCanTxMsg.Data[3] = 0;
    SendData.SendCanTxMsg.Data[2] = 0;
    SendData.SendCanTxMsg.Data[5] = 0;
    SendData.SendCanTxMsg.Data[4] = 0;
    SendData.SendCanTxMsg.Data[7] = 0;
    SendData.SendCanTxMsg.Data[6] = 0;
    xQueueSend(Queue_CANSend, &SendData, 10);
    
    SendData.SendCanTxMsg.StdId =   CHASSISCURRENTSETCANID;
    SendData.SendCanTxMsg.Data[1] = 0x02;
    SendData.SendCanTxMsg.Data[0] = 0xBC;
    SendData.SendCanTxMsg.Data[3] = 0x02;
    SendData.SendCanTxMsg.Data[2] = 0xBC;
    SendData.SendCanTxMsg.Data[5] = 0x02;
    SendData.SendCanTxMsg.Data[4] = 0xBC;
    SendData.SendCanTxMsg.Data[7] = 0x02;
    SendData.SendCanTxMsg.Data[6] = 0xBC;
    xQueueSend(Queue_CANSend, &SendData, 10);
}


/**
  * @brief  底盘总控制
  * @param  0 停机        1 正常
  * @retval void
  */
void Chassis_Control(uint8_t mode, uint8_t PIDChoie)
{
    Control_ChassisPID(PIDChoie);
    Chassis_Adjust();
    Chassis_SendMotorParam(mode);
}


/**
  * @brief  麦轮解算
  * @param  x速度（前）
  * @param  y速度（右）
  * @param  角速度（顺时针为正）
  * @retval void
  */
static void MecanumCalculate(float Vx, float Vy, float Omega, int16_t *Speed)
{
	unsigned char ii = 0, jj = 0;
	float temp_mat[3] = {0,0,0};
	float max_spd = 0,temp_speed = 0;
	float temp_ration = 0;	
	float temp_spd[4] = {0,0,0,0};
	temp_mat[0] = Vx;
	temp_mat[1] = Vy;
	temp_mat[2] = Omega;//spinning 使自旋的
	for(ii = 0;ii<4;ii++)
	{
		for(jj = 0;jj<3;jj++)
		{
			temp_spd[ii] += temp_mat[jj] * MecanumCalculateMAT[ii][jj];
		}
	}
	//speed 限幅
	for(ii=0; ii<4; ii++)  
	{
		temp_speed = temp_spd[ii] > 0 ? temp_spd[ii] : -temp_spd[ii];
		max_spd = (max_spd > temp_speed) ? max_spd : temp_speed;
	}
	//如果超过麦伦最大速度，等比例缩小4个速度
	if(max_spd > MaxWheelSpeed)
	{
		temp_ration = MaxWheelSpeed / max_spd;
		for(ii=0; ii<4; ii++)
		{
			temp_spd[ii] = temp_spd[ii]*temp_ration; 
		}	
	}
	
    //设置实际速度
	Speed[0] = (short)temp_spd[0];
	Speed[1] = (short)temp_spd[1];
	Speed[2] = (short)temp_spd[2];
    Speed[3] = (short)temp_spd[3];
}
