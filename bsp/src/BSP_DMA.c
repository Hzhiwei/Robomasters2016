
#include "BSP_DMA.h"
#include "Driver_DBUS.h"


/**
  * @brief  DMA³õÊ¼»¯
  * @param  void
  * @retval void
  */
void BSP_DMA_InitConfig(void)
{
    DMA_InitTypeDef     DMA_InitStructure;
    
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA1, ENABLE);
    
    DMA_InitStructure.DMA_Channel           =   DMA_Channel_4;
    DMA_InitStructure.DMA_PeripheralBaseAddr=   (uint32_t)(&UART5->DR);
    DMA_InitStructure.DMA_Memory0BaseAddr   =   (uint32_t)(DBUSBuffer);
    DMA_InitStructure.DMA_DIR               =   DMA_DIR_PeripheralToMemory;
    DMA_InitStructure.DMA_BufferSize        =   DBUSLength + DBUSBackLength;
    DMA_InitStructure.DMA_PeripheralInc     =   DMA_PeripheralInc_Disable;
    DMA_InitStructure.DMA_MemoryInc         =   DMA_MemoryInc_Enable;
    DMA_InitStructure.DMA_MemoryDataSize    =   DMA_MemoryDataSize_Byte;
    DMA_InitStructure.DMA_PeripheralDataSize=   DMA_PeripheralDataSize_Byte;
    DMA_InitStructure.DMA_Mode              =   DMA_Mode_Normal;
    DMA_InitStructure.DMA_Priority          =   DMA_Priority_Medium;
    DMA_InitStructure.DMA_FIFOMode          =   DMA_FIFOMode_Disable;
    DMA_InitStructure.DMA_FIFOThreshold     =   DMA_FIFOThreshold_Full;
    DMA_InitStructure.DMA_MemoryBurst       =   DMA_MemoryBurst_Single;
    DMA_InitStructure.DMA_PeripheralBurst   =   DMA_PeripheralBurst_Single;
    DMA_Init(DMA1_Stream0, &DMA_InitStructure);
    DMA_Cmd(DMA1_Stream0, ENABLE);
}



