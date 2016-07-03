
#include "BSP_CAN.h"


/**
  * @brief  CAN≥ı ºªØ
  * @param  void
  * @retval void
  */
void BSP_CAN_InitConfig(void)
{
    CAN_InitTypeDef     CAN_InitStructure;
    CAN_FilterInitTypeDef   CAN_FilterInitStructure;
    
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN1, ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN2, ENABLE);
    
    //CAN1
    CAN_InitStructure.CAN_ABOM      =   ENABLE;
    CAN_InitStructure.CAN_AWUM      =   DISABLE;
    CAN_InitStructure.CAN_BS1       =   CAN_BS1_9tq;
    CAN_InitStructure.CAN_BS2       =   CAN_BS2_4tq;
    CAN_InitStructure.CAN_Mode      =   CAN_Mode_Normal;
    CAN_InitStructure.CAN_NART      =   DISABLE;
    CAN_InitStructure.CAN_Prescaler =   3;
    CAN_InitStructure.CAN_RFLM      =   DISABLE;
    CAN_InitStructure.CAN_SJW       =   CAN_SJW_1tq;
    CAN_InitStructure.CAN_TTCM      =   DISABLE;
    CAN_InitStructure.CAN_TXFP      =   DISABLE;
    CAN_Init(CAN1, &CAN_InitStructure);
    
    CAN_FilterInitStructure.CAN_FilterActivation    =   ENABLE;
    CAN_FilterInitStructure.CAN_FilterFIFOAssignment=   CAN_Filter_FIFO0;
    CAN_FilterInitStructure.CAN_FilterIdHigh        =   0x0000;
    CAN_FilterInitStructure.CAN_FilterIdLow         =   0x0000;
    CAN_FilterInitStructure.CAN_FilterMaskIdHigh    =   0x0000;
    CAN_FilterInitStructure.CAN_FilterMaskIdLow     =   0x0000;
    CAN_FilterInitStructure.CAN_FilterMode          =   CAN_FilterMode_IdMask;
    CAN_FilterInitStructure.CAN_FilterNumber        =   0;
    CAN_FilterInitStructure.CAN_FilterScale         =   CAN_FilterScale_32bit;
    CAN_FilterInit(&CAN_FilterInitStructure);
    
    //CAN2
    CAN_InitStructure.CAN_ABOM      =   ENABLE;
    CAN_InitStructure.CAN_AWUM      =   DISABLE;
    CAN_InitStructure.CAN_BS1       =   CAN_BS1_9tq;
    CAN_InitStructure.CAN_BS2       =   CAN_BS2_4tq;
    CAN_InitStructure.CAN_Mode      =   CAN_Mode_Normal;
    CAN_InitStructure.CAN_NART      =   DISABLE;
    CAN_InitStructure.CAN_Prescaler =   3;
    CAN_InitStructure.CAN_RFLM      =   DISABLE;
    CAN_InitStructure.CAN_SJW       =   CAN_SJW_1tq;
    CAN_InitStructure.CAN_TTCM      =   DISABLE;
    CAN_InitStructure.CAN_TXFP      =   DISABLE;
    CAN_Init(CAN2, &CAN_InitStructure);
    
    CAN_FilterInitStructure.CAN_FilterActivation    =   ENABLE;
    CAN_FilterInitStructure.CAN_FilterFIFOAssignment=   CAN_Filter_FIFO0;
    CAN_FilterInitStructure.CAN_FilterIdHigh        =   0x0000;
    CAN_FilterInitStructure.CAN_FilterIdLow         =   0x0000;
    CAN_FilterInitStructure.CAN_FilterMaskIdHigh    =   0x0000;
    CAN_FilterInitStructure.CAN_FilterMaskIdLow     =   0x0000;
    CAN_FilterInitStructure.CAN_FilterMode          =   CAN_FilterMode_IdMask;
    CAN_FilterInitStructure.CAN_FilterNumber        =   14;
    CAN_FilterInitStructure.CAN_FilterScale         =   CAN_FilterScale_32bit;
    CAN_FilterInit(&CAN_FilterInitStructure);
}

