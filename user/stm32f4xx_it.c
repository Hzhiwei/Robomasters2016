/**
  ******************************************************************************
  * @file    Project/STM32F4xx_StdPeriph_Templates/stm32f4xx_it.c 
  * @author  MCD Application Team
  * @version V1.4.0
  * @date    04-August-2014
  * @brief   Main Interrupt Service Routines.
  *          This file provides template for all exceptions handler and 
  *          peripherals interrupt service routine.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2014 STMicroelectronics</center></h2>
  *
  * Licensed under MCD-ST Liberty SW License Agreement V2, (the "License");
  * You may not use this file except in compliance with the License.
  * You may obtain a copy of the License at:
  *
  *        http://www.st.com/software_license_agreement_liberty_v2
  *
  * Unless required by applicable law or agreed to in writing, software 
  * distributed under the License is distributed on an "AS IS" BASIS, 
  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  * See the License for the specific language governing permissions and
  * limitations under the License.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_it.h"
#include "main.h"

/** @addtogroup Template_Project
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/******************************************************************************/
/*            Cortex-M4 Processor Exceptions Handlers                         */
/******************************************************************************/

/**
  * @brief  This function handles NMI exception.
  * @param  None
  * @retval None
  */
void NMI_Handler(void)
{
}

/**
  * @brief  This function handles Hard Fault exception.
  * @param  None
  * @retval None
  */
void HardFault_Handler(void)
{
  /* Go to infinite loop when Hard Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Memory Manage exception.
  * @param  None
  * @retval None
  */
void MemManage_Handler(void)
{
  /* Go to infinite loop when Memory Manage exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Bus Fault exception.
  * @param  None
  * @retval None
  */
void BusFault_Handler(void)
{
  /* Go to infinite loop when Bus Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Usage Fault exception.
  * @param  None
  * @retval None
  */
void UsageFault_Handler(void)
{
  /* Go to infinite loop when Usage Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Debug Monitor exception.
  * @param  None
  * @retval None
  */
void DebugMon_Handler(void)
{
}


/******************************************************************************/
/*                 STM32F4xx Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32f4xx.s).                                               */
/******************************************************************************/

/**
  * @brief  This function handles PPP interrupt request.
  * @param  None
  * @retval None
  */
/*void PPP_IRQHandler(void)
{
}*/

/**
  * @}
  */ 


/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

#include "Config.h"
#include "OSinclude.h"
#include "Driver_DBUS.h"
#include "Driver_Judge.h"
#include "Driver_vision.h"
#include "Driver_Chassis.h"
#include "Driver_ESP8266.h"
#include "Driver_ESP8266.h"
#include "Driver_FricMotor.h"
#include "Driver_CloudMotor.h"
#include "Driver_StatusMachine.h"
#include "Driver_SuperGyroscope.h"

#include "CommonDataStructure.h"

#include "String.h"


//CAN接收数据存储缓存
CanRxMsg CanRxData;

//用于清空串口标志位的临时变量
uint8_t UARTtemp;


/**
  * @brief  CAN数据接收中断服务函数
  * @param  void
  * @retval void
  */
#if CANPORT == 1
void CAN1_RX0_IRQHandler(void)
#else
void CAN2_RX0_IRQHandler(void)
#endif
{
    static uint8_t FristGyroData = 0;       //第一次收到陀螺仪数据标志位
    FormatTrans dataTrans;                  //格式转换变量
    
    int16_t YawPassZeroBuffer;
    int16_t PitchPassZeroBuffer;
    
    //读取CAN数据
#if CANPORT == 1
    CAN_Receive(CAN1, 0, &CanRxData);
#else
    CAN_Receive(CAN2, 0, &CanRxData);
#endif
    
    switch(CanRxData.StdId)
    {
        //YAW 电机
        case    YAWMOTORCANID  :
        {
            CloudParam.Yaw.FrameCounter++;
     
//电机编码器过零处理
#if YAWMOTORENCODERPLUSEDIR == 1
            YawPassZeroBuffer = ((int16_t)CanRxData.Data[0] << 8) | CanRxData.Data[1];
#else
            YawPassZeroBuffer = 8191 - ((int16_t)CanRxData.Data[0] << 8) | CanRxData.Data[1];
#endif
            
            if(YawPassZeroBuffer < YawEncoderPassZeroBoundary)
            {
                CloudParam.Yaw.RealEncoderAngle = YawPassZeroBuffer + 8191;
            }
            else
            {
                CloudParam.Yaw.RealEncoderAngle = YawPassZeroBuffer;
            }
            break;
        }
        //PITCH 电机
        case    PITCHMOTORCANID  :
        {
            CloudParam.Pitch.FrameCounter++;

//电机编码器过零处理
#if PITCHMOTORENCODERPLUSEDIR == 1
            PitchPassZeroBuffer = ((int16_t)CanRxData.Data[0] << 8) | CanRxData.Data[1];
#else
            PitchPassZeroBuffer = 8191 - (((int16_t)CanRxData.Data[0] << 8) | CanRxData.Data[1]);
#endif
            
            if(PitchPassZeroBuffer < PitchEncoderPassZeroBoundary)
            {
                CloudParam.Pitch.RealEncoderAngle = PitchPassZeroBuffer + 8191;
            }
            else
            {
                CloudParam.Pitch.RealEncoderAngle = PitchPassZeroBuffer;
            }
            break;
        }
        //底盘陀螺仪
        case    SUPERGYROSCOPECANIC :
        {
            SuperGyoFrameCounter++;
            
            dataTrans.U[0] = CanRxData.Data[0];
            dataTrans.U[1] = CanRxData.Data[1];
            dataTrans.U[2] = CanRxData.Data[2];
            dataTrans.U[3] = CanRxData.Data[3];
            
            if(FristGyroData)
            {
                SuperGyoParam.Angle = dataTrans.F - SuperGyoParam.Offset;
            }
            else
            {
                SuperGyoParam.Angle = 0;
                SuperGyoParam.Offset = dataTrans.F;
                FristGyroData = 1;
            }
            
            dataTrans.U[0] = CanRxData.Data[4];
            dataTrans.U[1] = CanRxData.Data[5];
            dataTrans.U[2] = CanRxData.Data[6];
            dataTrans.U[3] = CanRxData.Data[7];
            SuperGyoParam.Omega = dataTrans.F;
            
            break;
        }
        
//电机种类为3510
#if MOTORTYPE == 1
        //左前轮
        case    LFCHASSISCANID  :
        {
            ChassisFrameCounter[0]++;
            break;
        }
        //右前轮
        case    RFCHASSISCANID  :
        {
            ChassisFrameCounter[1]++;
            break;
        }
        //左后轮
        case    LBCHASSISCANID  :
        {
            ChassisFrameCounter[2]++;
            break;
        }
        //右后轮
        case    RBCHASSISCANID  :
        {
            ChassisFrameCounter[3]++;
            break;
        }
#else       //35电机
        //左前轮
        case    LFCHASSISCANID  :
        {
            ChassisFrameCounter[0]++;
            
            ChassisParam.LF.NeedCurrent = ((uint16_t)CanRxData.Data[1] << 8) | CanRxData.Data[0];
            ChassisParam.LF.RealSpeed = ((int16_t)CanRxData.Data[3] << 8) | CanRxData.Data[2];
            ChassisParam.LF.RealCurrent = ((uint16_t)CanRxData.Data[5] << 8) | CanRxData.Data[4];
            
            break;
        }
        //右前轮
        case    RFCHASSISCANID  :
        {
            ChassisFrameCounter[1]++;
            
            ChassisParam.RF.NeedCurrent = ((uint16_t)CanRxData.Data[1] << 8) | CanRxData.Data[0];
            ChassisParam.RF.RealSpeed = ((int16_t)CanRxData.Data[3] << 8) | CanRxData.Data[2];
            ChassisParam.RF.RealCurrent = ((uint16_t)CanRxData.Data[5] << 8) | CanRxData.Data[4];
            
            break;
        }
        //左后轮
        case    LBCHASSISCANID  :
        {
            ChassisFrameCounter[2]++;
            
            ChassisParam.LB.NeedCurrent = ((uint16_t)CanRxData.Data[1] << 8) | CanRxData.Data[0];
            ChassisParam.LB.RealSpeed = ((int16_t)CanRxData.Data[3] << 8) | CanRxData.Data[2];
            ChassisParam.LB.RealCurrent = ((uint16_t)CanRxData.Data[5] << 8) | CanRxData.Data[4];
            
            break;
        }
        //右后轮
        case    RBCHASSISCANID  :
        {
            ChassisFrameCounter[3]++;
            
            ChassisParam.RB.NeedCurrent = ((uint16_t)CanRxData.Data[1] << 8) | CanRxData.Data[0];
            ChassisParam.RB.RealSpeed = ((int16_t)CanRxData.Data[3] << 8) | CanRxData.Data[2];
            ChassisParam.RB.RealCurrent = ((uint16_t)CanRxData.Data[5] << 8) | CanRxData.Data[4];
            
            break;
        }
#endif
     
//小英雄 摩擦轮使用3510电机        
#if FRICTYPE == 1
        //左炮摩擦轮
        case    ARTILLERYFRICCANIDLEFT :
        {
            ArtilleryFricRealSpeed[0] = ((int16_t)CanRxData.Data[2] << 8) | CanRxData.Data[3];
            break;
        }
        //右炮摩擦轮
        case    ARTILLERYFRICCANIDRIGHT :
        {
            ArtilleryFricRealSpeed[1] = ((int16_t)CanRxData.Data[2] << 8) | CanRxData.Data[3];
            break;
        }
#endif
    }
    
#if CANPORT == 1
	CAN_ITConfig(CAN1, CAN_IT_FMP0, ENABLE);
#else
	CAN_ITConfig(CAN2, CAN_IT_FMP0, ENABLE);
#endif
}


/**
  * @brief  不使用的CAN端口，也要写中断服务函数，否则会死机
  * @param  void
  * @retval void
  */
#if CANPORT == 1
void CAN2_RX0_IRQHandler(void)
{
	CAN_ITConfig(CAN2, CAN_IT_FMP0, ENABLE);
}
#else
void CAN1_RX0_IRQHandler(void)
{
	CAN_ITConfig(CAN1, CAN_IT_FMP0, ENABLE);
}
#endif


/**
  * @brief  DBUS空闲中断(UART5)
  * @param  void
  * @retval void
  */
void UART5_IRQHandler(void)
{
    UARTtemp = UART5->DR;
    UARTtemp = UART5->SR;
    
    DMA_Cmd(DMA1_Stream0, DISABLE);
    
    //数据量正确
    if(DMA1_Stream0->NDTR == DBUSBackLength)
    {
        DBUSFrameCounter++;         //帧数增加
        DBUS_DataDecoding();        //解码
    }
    
    //重启DMA
    DMA_ClearFlag(DMA1_Stream0, DMA_FLAG_TCIF0 | DMA_FLAG_HTIF0);
    while(DMA_GetCmdStatus(DMA1_Stream0) != DISABLE);
    DMA_SetCurrDataCounter(DMA1_Stream0, DBUSLength + DBUSBackLength);
    DMA_Cmd(DMA1_Stream0, ENABLE);
}


/**
  * @brief  裁判系统空闲中断(UART4)
  * @param  void
  * @retval void
  * @note   DJI很毒,以50HZ发送Info包，当有实时数据包产生时，在最近的一帧Info前无间隔加上实时数据包
  */
void UART4_IRQHandler(void)
{
    FormatTrans FT;
    
#if USEESP8266orOLED == 1
    uint16_t JudgeDataLenght, index;
#endif
    
    UARTtemp = UART4->DR;
    UARTtemp = UART4->SR;
    
    DMA_Cmd(DMA1_Stream2, DISABLE);
    
    //比赛进程信息，只读电压电流
    if((DMA1_Stream2->NDTR == JudgeBufferLength - JudgeFrameLength_1) && 
        (Verify_CRC16_Check_Sum(JudgeDataBuffer, 38 + 8) == 1) &&
        (JudgeDataBuffer[4] == 1))
    {
        JudgeFrameCounter++;        //帧数增加
        
        //读取电压
        FT.U[3] = JudgeDataBuffer[15];
        FT.U[2] = JudgeDataBuffer[14];
        FT.U[1] = JudgeDataBuffer[13];
        FT.U[0] = JudgeDataBuffer[12];
        InfantryJudge.RealVoltage = FT.F;
        
        //读取电压
        FT.U[3] = JudgeDataBuffer[19];
        FT.U[2] = JudgeDataBuffer[18];
        FT.U[1] = JudgeDataBuffer[17];
        FT.U[0] = JudgeDataBuffer[16];
        InfantryJudge.RealCurrent = FT.F;
        
        //剩余血量
        InfantryJudge.LastBlood = ((int16_t)JudgeDataBuffer[11] << 8) | JudgeDataBuffer[10];
        
        //功率限制
        ChassisMaxSumCurrent = CHASSISMAXPOWERRATE * CHASSISMAXPOWER * 1000 / InfantryJudge.RealVoltage;
    }
    //被攻击信息
    else if((DMA1_Stream2->NDTR == JudgeBufferLength - JudgeFrameLength_1 - JudgeFrameLength_2) && 
        (Verify_CRC16_Check_Sum(JudgeDataBuffer, 3 + 8) == 1) &&
        (JudgeDataBuffer[4] == 2))
    {
        JudgeFrameCounter++;        //帧数增加
        
        //扣血类型
        if(!(JudgeDataBuffer[6] >> 4))
        {
            //受伤装甲板ID
            InfantryJudge.LastHartID = JudgeDataBuffer[6] & 0x0F;
            //受伤时间
            InfantryJudge.LastHartTick = xTaskGetTickCountFromISR();
        }
    }
    //射击信息
    else if((DMA1_Stream2->NDTR == JudgeBufferLength - JudgeFrameLength_1 - JudgeFrameLength_3) && 
        (Verify_CRC16_Check_Sum(JudgeDataBuffer, 16 + 8) == 1) &&
        (JudgeDataBuffer[4] == 3))
    {
        JudgeFrameCounter++;        //帧数增加
        
#if INFANTRY == 7
        InfantryJudge.ShootNum++;
        InfantryJudge.ShootFail = 0;
#endif
        
        //子弹射速
        FT.U[3] = JudgeDataBuffer[9];
        FT.U[2] = JudgeDataBuffer[8];
        FT.U[1] = JudgeDataBuffer[7];
        FT.U[0] = JudgeDataBuffer[6];
        InfantryJudge.LastShotSpeed = FT.F;
        
        //子弹出膛时间
        InfantryJudge.LastShotTick = xTaskGetTickCountFromISR();
    }
   
//使用ESP8266发送上位机数据
#if USEESP8266orOLED == 1
    if(ESP8266APPortLinkFlag)
    {
        JudgeDataLenght = JudgeBufferLength - DMA1_Stream2->NDTR;
        
        for(index = 0; index < JudgeDataLenght; index++)
        {
            ESP8266TXBuffer[index] = JudgeDataBuffer[index];
        }
        
        ESP8266_SendPack(JudgeDataLenght);
    }
#endif
    
    //重启DMA
    DMA_ClearFlag(DMA1_Stream2, DMA_FLAG_TCIF2 | DMA_FLAG_HTIF2);
    while(DMA_GetCmdStatus(DMA1_Stream2) != DISABLE);
    DMA_SetCurrDataCounter(DMA1_Stream2, JudgeBufferLength);
    DMA_Cmd(DMA1_Stream2, ENABLE);
}


/**
  * @brief  PC视觉数据 串口中断(USART1)
  * @param  void
  * @retval void
  */
void USART1_IRQHandler(void)
{
    uint8_t i, Sum = 0;
    FormatTrans Buffer;
    
    UARTtemp = USART1->SR;          //与USART1->DR读取配合清除ORD位
    
    PCDataBufferPoint = (PCDataBufferPoint + 1) % PCDATALENGTH;
    PCDataBuffer[PCDataBufferPoint] = USART1->DR;
    
    //找帧头
    if ((PCDataBuffer[(PCDataBufferPoint + 1) % PCDATALENGTH] == 0x44) &&
        (PCDataBuffer[(PCDataBufferPoint + 2) % PCDATALENGTH] == 0xBB) &&
        (PCDataBuffer[PCDataBufferPoint] == 0xFF))
    {
        for (i = 4; i <= 23; i++)       //求校检和 
        {
            Sum += PCDataBuffer[(PCDataBufferPoint + i) % PCDATALENGTH];
        }

        if (PCDataBuffer[(PCDataBufferPoint + 24) % PCDATALENGTH] == (Sum & 0xFF))       //校检通过
        {
            PCFrameCounter++;
            
#if INFANTRY == 7
            if(PCDataBuffer[(PCDataBufferPoint + 3) % PCDATALENGTH] == 0)       //ID=0
            {
                Buffer.U[3] = PCDataBuffer[(PCDataBufferPoint + 16) % PCDATALENGTH];
                Buffer.U[2] = PCDataBuffer[(PCDataBufferPoint + 17) % PCDATALENGTH];
                Buffer.U[1] = PCDataBuffer[(PCDataBufferPoint + 18) % PCDATALENGTH];
                Buffer.U[0] = PCDataBuffer[(PCDataBufferPoint + 19) % PCDATALENGTH];
                PCVisionTargetColor = Buffer.F;
            }
            else
            {
                VisionUpdataFlag = 1;
                
                EnemyDataBufferPoint = (EnemyDataBufferPoint + 1) % ENEMYDATABUFFERLENGHT;
                
                //解码
                Buffer.U[3] = PCDataBuffer[(PCDataBufferPoint + 4) % PCDATALENGTH];
                Buffer.U[2] = PCDataBuffer[(PCDataBufferPoint + 5) % PCDATALENGTH];
                Buffer.U[1] = PCDataBuffer[(PCDataBufferPoint + 6) % PCDATALENGTH];
                Buffer.U[0] = PCDataBuffer[(PCDataBufferPoint + 7) % PCDATALENGTH];
                EnemyDataBuffer[EnemyDataBufferPoint].X = Buffer.F;

                Buffer.U[3] = PCDataBuffer[(PCDataBufferPoint + 8) % PCDATALENGTH];
                Buffer.U[2] = PCDataBuffer[(PCDataBufferPoint + 9) % PCDATALENGTH];
                Buffer.U[1] = PCDataBuffer[(PCDataBufferPoint + 10) % PCDATALENGTH];
                Buffer.U[0] = PCDataBuffer[(PCDataBufferPoint + 11) % PCDATALENGTH];
                EnemyDataBuffer[EnemyDataBufferPoint].Y = Buffer.F;

                Buffer.U[3] = PCDataBuffer[(PCDataBufferPoint + 12) % PCDATALENGTH];
                Buffer.U[2] = PCDataBuffer[(PCDataBufferPoint + 13) % PCDATALENGTH];
                Buffer.U[1] = PCDataBuffer[(PCDataBufferPoint + 14) % PCDATALENGTH];
                Buffer.U[0] = PCDataBuffer[(PCDataBufferPoint + 15) % PCDATALENGTH];
                EnemyDataBuffer[EnemyDataBufferPoint].Z = Buffer.F;

                Buffer.U[3] = PCDataBuffer[(PCDataBufferPoint + 16) % PCDATALENGTH];
                Buffer.U[2] = PCDataBuffer[(PCDataBufferPoint + 17) % PCDATALENGTH];
                Buffer.U[1] = PCDataBuffer[(PCDataBufferPoint + 18) % PCDATALENGTH];
                Buffer.U[0] = PCDataBuffer[(PCDataBufferPoint + 19) % PCDATALENGTH];
                EnemyDataBuffer[EnemyDataBufferPoint].TimeStamp = Buffer.F;
                PCVisionTargetColor = Buffer.F;

                Buffer.U[3] = PCDataBuffer[(PCDataBufferPoint + 20) % PCDATALENGTH];
                Buffer.U[2] = PCDataBuffer[(PCDataBufferPoint + 21) % PCDATALENGTH];
                Buffer.U[1] = PCDataBuffer[(PCDataBufferPoint + 22) % PCDATALENGTH];
                Buffer.U[0] = PCDataBuffer[(PCDataBufferPoint + 23) % PCDATALENGTH];
                EnemyDataBuffer[EnemyDataBufferPoint].Time = Buffer.I;

                EnemyDataBuffer[EnemyDataBufferPoint].ID = PCDataBuffer[(PCDataBufferPoint + 3) % PCDATALENGTH];
                
                EnemyDataBuffer[EnemyDataBufferPoint].Tick = xTaskGetTickCountFromISR();
            }
#else
            if(PCDataBuffer[(PCDataBufferPoint + 3) % PCDATALENGTH] != 0)       //ID!=0
            {
                VisionUpdataFlag = 1;
                
                EnemyDataBufferPoint = (EnemyDataBufferPoint + 1) % ENEMYDATABUFFERLENGHT;
                
                //解码
                Buffer.U[3] = PCDataBuffer[(PCDataBufferPoint + 4) % PCDATALENGTH];
                Buffer.U[2] = PCDataBuffer[(PCDataBufferPoint + 5) % PCDATALENGTH];
                Buffer.U[1] = PCDataBuffer[(PCDataBufferPoint + 6) % PCDATALENGTH];
                Buffer.U[0] = PCDataBuffer[(PCDataBufferPoint + 7) % PCDATALENGTH];
                EnemyDataBuffer[EnemyDataBufferPoint].X = Buffer.F;

                Buffer.U[3] = PCDataBuffer[(PCDataBufferPoint + 8) % PCDATALENGTH];
                Buffer.U[2] = PCDataBuffer[(PCDataBufferPoint + 9) % PCDATALENGTH];
                Buffer.U[1] = PCDataBuffer[(PCDataBufferPoint + 10) % PCDATALENGTH];
                Buffer.U[0] = PCDataBuffer[(PCDataBufferPoint + 11) % PCDATALENGTH];
                EnemyDataBuffer[EnemyDataBufferPoint].Y = Buffer.F;

                Buffer.U[3] = PCDataBuffer[(PCDataBufferPoint + 12) % PCDATALENGTH];
                Buffer.U[2] = PCDataBuffer[(PCDataBufferPoint + 13) % PCDATALENGTH];
                Buffer.U[1] = PCDataBuffer[(PCDataBufferPoint + 14) % PCDATALENGTH];
                Buffer.U[0] = PCDataBuffer[(PCDataBufferPoint + 15) % PCDATALENGTH];
                EnemyDataBuffer[EnemyDataBufferPoint].Z = Buffer.F;

                Buffer.U[3] = PCDataBuffer[(PCDataBufferPoint + 16) % PCDATALENGTH];
                Buffer.U[2] = PCDataBuffer[(PCDataBufferPoint + 17) % PCDATALENGTH];
                Buffer.U[1] = PCDataBuffer[(PCDataBufferPoint + 18) % PCDATALENGTH];
                Buffer.U[0] = PCDataBuffer[(PCDataBufferPoint + 19) % PCDATALENGTH];
                EnemyDataBuffer[EnemyDataBufferPoint].TimeStamp = Buffer.F;

                Buffer.U[3] = PCDataBuffer[(PCDataBufferPoint + 20) % PCDATALENGTH];
                Buffer.U[2] = PCDataBuffer[(PCDataBufferPoint + 21) % PCDATALENGTH];
                Buffer.U[1] = PCDataBuffer[(PCDataBufferPoint + 22) % PCDATALENGTH];
                Buffer.U[0] = PCDataBuffer[(PCDataBufferPoint + 23) % PCDATALENGTH];
                EnemyDataBuffer[EnemyDataBufferPoint].Time = Buffer.I;

                EnemyDataBuffer[EnemyDataBufferPoint].ID = PCDataBuffer[(PCDataBufferPoint + 3) % PCDATALENGTH];
                
                EnemyDataBuffer[EnemyDataBufferPoint].Tick = xTaskGetTickCountFromISR();
            }
#endif
        }
    }
}


/**
  * @brief  (ESP8266)USART3中断服务函数
  * @param  void 
  * @retval void
  */
uint16_t ks = 0;
void USART3_IRQHandler(void)
{
    UARTtemp = USART3->DR;
    UARTtemp = USART3->SR;
    
    DMA_Cmd(DMA1_Stream1, DISABLE);
    
    LatestRespond = 1;
    
    //重启DMA
    DMA_ClearFlag(DMA1_Stream1, DMA_FLAG_TCIF1 | DMA_FLAG_HTIF1);
    while(DMA_GetCmdStatus(DMA1_Stream1) != DISABLE);
    DMA_SetCurrDataCounter(DMA1_Stream1, ESP8266RXBufferLenght);
    DMA_Cmd(DMA1_Stream1, ENABLE);
    
}










