
#include "BSP_SPI.h"


/**
  * @brief  SPI初始化
  * @param  void
  * @retval void
  */
void BSP_SPI_InitConfig(void)
{
    SPI_InitTypeDef      SPI_InitStructure;
    
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2, ENABLE);
    
    //SPI2  mpu9250
    SPI_InitStructure.SPI_Mode              =   SPI_Mode_Master;
    SPI_InitStructure.SPI_BaudRatePrescaler =   4;
    SPI_InitStructure.SPI_CPHA              =   SPI_CPHA_2Edge;
    SPI_InitStructure.SPI_CPOL              =   SPI_CPOL_High;
    SPI_InitStructure.SPI_CRCPolynomial     =   10;
    SPI_InitStructure.SPI_DataSize          =   SPI_DataSize_8b;
    SPI_InitStructure.SPI_Direction         =   SPI_Direction_2Lines_FullDuplex;
    SPI_InitStructure.SPI_FirstBit          =   SPI_FirstBit_MSB;
    SPI_InitStructure.SPI_NSS               =   SPI_NSS_Soft;
    SPI_Init(SPI2, &SPI_InitStructure);
    
    SPI_Cmd(SPI2, ENABLE);
}


/**
  * @brief  SPI读写一个数据
  * @param  void
  * @retval void
  */
void BSP_SPI_ReadWriteByte(SPI_TypeDef *SPIx, uint8_t *Tx, uint8_t *Rx)
{
    while(SPI_I2S_GetFlagStatus(SPIx, SPI_I2S_FLAG_TXE) == RESET);
    SPI_I2S_SendData(SPIx, *Tx);
    while(SPI_I2S_GetFlagStatus(SPIx, SPI_I2S_FLAG_RXNE) == RESET);
    *Rx = SPI_I2S_ReceiveData(SPIx);
}

