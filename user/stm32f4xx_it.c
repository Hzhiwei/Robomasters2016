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
#include "Driver_DBUS.h"
#include "Driver_Judge.h"
#include "Driver_vision.h"
#include "Driver_Chassis.h"
#include "Driver_CloudMotor.h"
#include "Driver_SuperGyroscope.h"



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
    static uint8_t FristGyroData = 0;
    u8Todouble dataTrans;
    
    uint16_t YawPassZeroBuffer;
    uint16_t PitchPassZeroBuffer;
    
#if CANPORT == 1
    CAN_Receive(CAN1, 0, &CanRxData);
#else
    CAN_Receive(CAN2, 0, &CanRxData);
#endif
    
    switch(CanRxData.StdId)
    {
        case    YAWMOTORCANID  :
        {
            CloudParam.Yaw.FrameCounter++;
            
            YawPassZeroBuffer = ((int16_t)CanRxData.Data[0] << 8) | CanRxData.Data[1];
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
        case    PITCHMOTORCANID  :
        {
            CloudParam.Pitch.FrameCounter++;
            
            PitchPassZeroBuffer = ((int16_t)CanRxData.Data[0] << 8) | CanRxData.Data[1];
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
        case    SUPERGYROSCOPECANIC :
        {
            SuperGyoFrameCounter++;
            
            dataTrans.uint8_tdata[0] = CanRxData.Data[0];
            dataTrans.uint8_tdata[1] = CanRxData.Data[1];
            dataTrans.uint8_tdata[2] = CanRxData.Data[2];
            dataTrans.uint8_tdata[3] = CanRxData.Data[3];
            
            if(FristGyroData)
            {
                SuperGyoParam.Angle = dataTrans.floatdata - SuperGyoParam.Offset;
            }
            else
            {
                SuperGyoParam.Angle = 0;
                SuperGyoParam.Offset = dataTrans.floatdata;
                FristGyroData = 1;
            }
            
            dataTrans.uint8_tdata[0] = CanRxData.Data[4];
            dataTrans.uint8_tdata[1] = CanRxData.Data[5];
            dataTrans.uint8_tdata[2] = CanRxData.Data[6];
            dataTrans.uint8_tdata[3] = CanRxData.Data[7];
            SuperGyoParam.Omega = dataTrans.floatdata;
            
            break;
        }
        case    LFCHASSISCANID  :
        {
            ChassisFrameCounter[0]++;
            
            ChassisParam.LF.NeedCurrent = ((uint16_t)CanRxData.Data[1] << 8) | CanRxData.Data[0];
            ChassisParam.LF.RealSpeed = ((int16_t)CanRxData.Data[3] << 8) | CanRxData.Data[2];
            ChassisParam.LF.RealCurrent = ((uint16_t)CanRxData.Data[5] << 8) | CanRxData.Data[4];
            
            break;
        }
        case    RFCHASSISCANID  :
        {
            ChassisFrameCounter[1]++;
            
            ChassisParam.RF.NeedCurrent = ((uint16_t)CanRxData.Data[1] << 8) | CanRxData.Data[0];
            ChassisParam.RF.RealSpeed = ((int16_t)CanRxData.Data[3] << 8) | CanRxData.Data[2];
            ChassisParam.RF.RealCurrent = ((uint16_t)CanRxData.Data[5] << 8) | CanRxData.Data[4];
            
            break;
        }
        case    LBCHASSISCANID  :
        {
            ChassisFrameCounter[2]++;
            
            ChassisParam.LB.NeedCurrent = ((uint16_t)CanRxData.Data[1] << 8) | CanRxData.Data[0];
            ChassisParam.LB.RealSpeed = ((int16_t)CanRxData.Data[3] << 8) | CanRxData.Data[2];
            ChassisParam.LB.RealCurrent = ((uint16_t)CanRxData.Data[5] << 8) | CanRxData.Data[4];
            
            break;
        }
        case    RBCHASSISCANID  :
        {
            ChassisFrameCounter[3]++;
            
            ChassisParam.RB.NeedCurrent = ((uint16_t)CanRxData.Data[1] << 8) | CanRxData.Data[0];
            ChassisParam.RB.RealSpeed = ((int16_t)CanRxData.Data[3] << 8) | CanRxData.Data[2];
            ChassisParam.RB.RealCurrent = ((uint16_t)CanRxData.Data[5] << 8) | CanRxData.Data[4];
            
            break;
        }
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


//DBUS空闲中断
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


//裁判系统空闲中断
void UART4_IRQHandler(void)
{
    FormatTrans FT;
    
    UARTtemp = UART4->DR;
    UARTtemp = UART4->SR;
    
    DMA_Cmd(DMA1_Stream2, DISABLE);
    
    //比赛进程信息，只读电压电流
    //傻狗说：
    //很奇怪，大疆pdf给的比赛信息有效数据长度是38，而传过来的是37，而sizeof得到的也是38，反正这里有问题，最后决定用大疆发过来的的长度（37）为准
    //我选择相信
    if((DMA1_Stream2->NDTR == JudgeBufferLength - JudgeFrameLength_1) && 
        (Verify_CRC16_Check_Sum(JudgeDataBuffer, 38 + 8) == 1) &&
        (JudgeDataBuffer[4] == 1))
    {
        JudgeFrameCounter++;        //帧数增加
        
        //读取电压
        FT.u8[3] = JudgeDataBuffer[15];
        FT.u8[2] = JudgeDataBuffer[14];
        FT.u8[1] = JudgeDataBuffer[13];
        FT.u8[0] = JudgeDataBuffer[12];
        JudgeRealVoltage = FT.F;
        
        //读取电压
        FT.u8[3] = JudgeDataBuffer[19];
        FT.u8[2] = JudgeDataBuffer[18];
        FT.u8[1] = JudgeDataBuffer[17];
        FT.u8[0] = JudgeDataBuffer[16];
        JudgeRealCurrent = FT.F;
        
        //功率限制
        ChassisMaxSumCurrent = CHASSISMAXPOWERRATE * CHASSISMAXPOWER * 1000 / JudgeRealVoltage;
    }
    
    //重启DMA
    DMA_ClearFlag(DMA1_Stream2, DMA_FLAG_TCIF2 | DMA_FLAG_HTIF2);
    while(DMA_GetCmdStatus(DMA1_Stream2) != DISABLE);
    DMA_SetCurrDataCounter(DMA1_Stream2, JudgeBufferLength);
    DMA_Cmd(DMA1_Stream2, ENABLE);
}


//PC视觉数据 串口中断
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
            if(PCDataBuffer[(PCDataBufferPoint + 3) % PCDATALENGTH] != 0)       //ID!=0
            {
                VisionUpdataFlag = 1;
                PCFrameCounter++;
                
                EnemyDataBufferPoint = (EnemyDataBufferPoint + 1) % ENEMYDATABUFFERLENGHT;
                
                //解码
                Buffer.u8[3] = PCDataBuffer[(PCDataBufferPoint + 4) % PCDATALENGTH];
                Buffer.u8[2] = PCDataBuffer[(PCDataBufferPoint + 5) % PCDATALENGTH];
                Buffer.u8[1] = PCDataBuffer[(PCDataBufferPoint + 6) % PCDATALENGTH];
                Buffer.u8[0] = PCDataBuffer[(PCDataBufferPoint + 7) % PCDATALENGTH];
                EnemyDataBuffer[EnemyDataBufferPoint].X = Buffer.F;

                Buffer.u8[3] = PCDataBuffer[(PCDataBufferPoint + 8) % PCDATALENGTH];
                Buffer.u8[2] = PCDataBuffer[(PCDataBufferPoint + 9) % PCDATALENGTH];
                Buffer.u8[1] = PCDataBuffer[(PCDataBufferPoint + 10) % PCDATALENGTH];
                Buffer.u8[0] = PCDataBuffer[(PCDataBufferPoint + 11) % PCDATALENGTH];
                EnemyDataBuffer[EnemyDataBufferPoint].Y = Buffer.F;

                Buffer.u8[3] = PCDataBuffer[(PCDataBufferPoint + 12) % PCDATALENGTH];
                Buffer.u8[2] = PCDataBuffer[(PCDataBufferPoint + 13) % PCDATALENGTH];
                Buffer.u8[1] = PCDataBuffer[(PCDataBufferPoint + 14) % PCDATALENGTH];
                Buffer.u8[0] = PCDataBuffer[(PCDataBufferPoint + 15) % PCDATALENGTH];
                EnemyDataBuffer[EnemyDataBufferPoint].Z = Buffer.F;

                Buffer.u8[3] = PCDataBuffer[(PCDataBufferPoint + 16) % PCDATALENGTH];
                Buffer.u8[2] = PCDataBuffer[(PCDataBufferPoint + 17) % PCDATALENGTH];
                Buffer.u8[1] = PCDataBuffer[(PCDataBufferPoint + 18) % PCDATALENGTH];
                Buffer.u8[0] = PCDataBuffer[(PCDataBufferPoint + 19) % PCDATALENGTH];
                EnemyDataBuffer[EnemyDataBufferPoint].TimeStamp = Buffer.F;

                Buffer.u8[3] = PCDataBuffer[(PCDataBufferPoint + 20) % PCDATALENGTH];
                Buffer.u8[2] = PCDataBuffer[(PCDataBufferPoint + 21) % PCDATALENGTH];
                Buffer.u8[1] = PCDataBuffer[(PCDataBufferPoint + 22) % PCDATALENGTH];
                Buffer.u8[0] = PCDataBuffer[(PCDataBufferPoint + 23) % PCDATALENGTH];
                EnemyDataBuffer[EnemyDataBufferPoint].Time = Buffer.I;

                EnemyDataBuffer[EnemyDataBufferPoint].ID = PCDataBuffer[(PCDataBufferPoint + 3) % PCDATALENGTH];
            }
        }
    }
}













