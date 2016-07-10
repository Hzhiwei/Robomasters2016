
#include "BSP_TIM.h"


/**
  * @brief  TIM³õÊ¼»¯
  * @param  void
  * @retval void
  */
void BSP_TIM_InitConfig(void)
{
    TIM_TimeBaseInitTypeDef     TIM_TimeBaseInitStructure;
    TIM_OCInitTypeDef           TIM_OCInitStructure;
    TIM_ICInitTypeDef           TIM_ICInitStructure;
    
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM8, ENABLE);
    
    //BiBiBi£¨TIM2)
    TIM_TimeBaseInitStructure.TIM_ClockDivision =   0;
    TIM_TimeBaseInitStructure.TIM_CounterMode   =   TIM_CounterMode_Up;
    TIM_TimeBaseInitStructure.TIM_Period        =   300;
    TIM_TimeBaseInitStructure.TIM_Prescaler     =   83;
    TIM_TimeBaseInit(TIM2, &TIM_TimeBaseInitStructure);

    TIM_OCInitStructure.TIM_OCMode     =   TIM_OCMode_PWM1;
    TIM_OCInitStructure.TIM_OutputState=   TIM_OutputState_Enable;
    TIM_OCInitStructure.TIM_Pulse      =   0;
    TIM_OCInitStructure.TIM_OCPolarity =   TIM_OCPolarity_Low;
    TIM_OC1Init(TIM2, &TIM_OCInitStructure);
    
    TIM_OC1PreloadConfig(TIM2, TIM_OCPreload_Enable);
    
    TIM_Cmd(TIM2, ENABLE);
    
    
    //²¦µ¯µç»ú£¨TIM5)
    TIM_TimeBaseInitStructure.TIM_ClockDivision =   0;
    TIM_TimeBaseInitStructure.TIM_CounterMode   =   TIM_CounterMode_Up;
    TIM_TimeBaseInitStructure.TIM_Period        =   399;
    TIM_TimeBaseInitStructure.TIM_Prescaler     =   20;
    TIM_TimeBaseInit(TIM5, &TIM_TimeBaseInitStructure);

    TIM_OCInitStructure.TIM_OCMode     =   TIM_OCMode_PWM1;
    TIM_OCInitStructure.TIM_OutputState=   TIM_OutputState_Enable;
    TIM_OCInitStructure.TIM_Pulse      =   0;
    TIM_OCInitStructure.TIM_OCPolarity =   TIM_OCPolarity_High;
    TIM_OC2Init(TIM5, &TIM_OCInitStructure);
    
    TIM_Cmd(TIM5, ENABLE);
    
    
    //²¦µ¯µç»ú±àÂëÆ÷(TIM3)
    TIM_TimeBaseInitStructure.TIM_ClockDivision =   0;
    TIM_TimeBaseInitStructure.TIM_CounterMode   =   TIM_CounterMode_Up;
    TIM_TimeBaseInitStructure.TIM_Period        =   65535;
    TIM_TimeBaseInitStructure.TIM_Prescaler     =   0;
    TIM_TimeBaseInit(TIM3, &TIM_TimeBaseInitStructure);
    
    TIM_EncoderInterfaceConfig(TIM3, TIM_EncoderMode_TI12, TIM_ICPolarity_Rising, TIM_ICPolarity_Rising);
    
    TIM_ICStructInit(&TIM_ICInitStructure);
    TIM_ICInitStructure.TIM_ICFilter = 0;
    TIM_ICInit(TIM3, &TIM_ICInitStructure);
    
    TIM_ClearFlag(TIM3, TIM_FLAG_Update);
    
    TIM_Cmd(TIM3, ENABLE);
    
    
    //Ç¹Ä¦²ÁÂÖ(CH1,CH2)£¬¶æ»ú(CH4)£¨TIM8)(100Hz)
    TIM_TimeBaseInitStructure.TIM_ClockDivision =   0;
    TIM_TimeBaseInitStructure.TIM_CounterMode   =   TIM_CounterMode_Up;
    TIM_TimeBaseInitStructure.TIM_Period        =   2999;
    TIM_TimeBaseInitStructure.TIM_Prescaler     =   167;
    TIM_TimeBaseInit(TIM8, &TIM_TimeBaseInitStructure);

    TIM_OCInitStructure.TIM_OCMode     =   TIM_OCMode_PWM2;
    TIM_OCInitStructure.TIM_OutputState=   TIM_OutputState_Enable;
    TIM_OCInitStructure.TIM_Pulse      =   1000;
    TIM_OCInitStructure.TIM_OCPolarity =   TIM_OCPolarity_High;
    TIM_OCInitStructure.TIM_OCIdleState=   TIM_OCIdleState_Reset; 
    TIM_OC1Init(TIM8, &TIM_OCInitStructure);
    TIM_OC2Init(TIM8, &TIM_OCInitStructure);
    
    TIM_OCInitStructure.TIM_OCMode     =   TIM_OCMode_PWM2;
    TIM_OCInitStructure.TIM_OutputState=   TIM_OutputState_Enable;
    TIM_OCInitStructure.TIM_Pulse      =   1500;
    TIM_OCInitStructure.TIM_OCPolarity =   TIM_OCPolarity_Low;
    TIM_OCInitStructure.TIM_OCIdleState =  TIM_OCIdleState_Reset; 
    TIM_OC4Init(TIM8, &TIM_OCInitStructure);
    
    TIM_Cmd(TIM8, ENABLE);
    TIM_CtrlPWMOutputs(TIM8, ENABLE);
}


