
#ifndef __BSP_SPI_H
#define __BSP_SPI_H


#include "stm32f4xx.h"


void BSP_SPI_InitConfig(void);
void BSP_SPI_ReadWriteByte(SPI_TypeDef *SPIx, uint8_t *Tx, uint8_t *Rx);


#endif
