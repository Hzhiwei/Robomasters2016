
#include "BSP_NVIC.h"


/**
  * @brief  NVIC≥ı ºªØ
  * @param  void
  * @retval void
  */
void BSP_NVIC_InitConfig(void)
{
	NVIC_InitTypeDef	NVIC_InitStructure;
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);
	
	//CAN1
	NVIC_InitStructure.NVIC_IRQChannel						=	CAN1_RX0_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelCmd					=	ENABLE;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority	=	9;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority			=	0;
	NVIC_Init(&NVIC_InitStructure);
	CAN_ITConfig(CAN1, CAN_IT_FMP0, ENABLE);
	
	//CAN2
	NVIC_InitStructure.NVIC_IRQChannel						=	CAN2_RX0_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelCmd					=	ENABLE;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority	=	12;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority			=	0;
	NVIC_Init(&NVIC_InitStructure);
	CAN_ITConfig(CAN2, CAN_IT_FMP0, ENABLE);
	
    //USART1
	NVIC_InitStructure.NVIC_IRQChannel						=	USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelCmd					=	ENABLE;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority	=	8;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority			=	0;
	NVIC_Init(&NVIC_InitStructure);
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
	
    //USART3
	NVIC_InitStructure.NVIC_IRQChannel						=	USART3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelCmd					=	ENABLE;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority	=	8;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority			=	0;
	NVIC_Init(&NVIC_InitStructure);
	USART_ITConfig(USART3, USART_IT_IDLE, ENABLE);
	
    //UART4
	NVIC_InitStructure.NVIC_IRQChannel						=	UART4_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelCmd					=	ENABLE;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority	=	10;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority			=	0;
	NVIC_Init(&NVIC_InitStructure);
	USART_ITConfig(UART4, USART_IT_IDLE, ENABLE);
	
    //UART5
	NVIC_InitStructure.NVIC_IRQChannel						=	UART5_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelCmd					=	ENABLE;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority	=	11;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority			=	0;
	NVIC_Init(&NVIC_InitStructure);
	USART_ITConfig(UART5, USART_IT_IDLE, ENABLE);
}

