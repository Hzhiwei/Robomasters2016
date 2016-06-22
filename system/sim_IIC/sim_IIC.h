
#ifndef _SIM_IIC_H
#define _SIM_IIC_H

#include "stm32f4xx.h"
//#include "sys.h"


#define IIC_SCL_L		GPIO_ResetBits(GPIOB, GPIO_Pin_13)
#define IIC_SCL_H		GPIO_SetBits(GPIOB, GPIO_Pin_13)
#define IIC_SDA_L		GPIO_ResetBits(GPIOB, GPIO_Pin_15)
#define IIC_SDA_H		GPIO_SetBits(GPIOB, GPIO_Pin_15)
#define	IIC_SDA_Read	GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_15)

void IIC_Init(void);
uint8_t IIC_SingleSend(uint8_t addr, uint8_t reg, uint8_t data);
uint8_t IIC_SingleRead(uint8_t addr, uint8_t reg, uint8_t *data);
uint8_t IIC_SendBuffer(uint8_t addr, uint8_t reg, uint8_t len, uint8_t *data);
uint8_t IIC_ReadBuffer(uint8_t addr, uint8_t reg, uint8_t len, uint8_t *data);


void IIC_Delay(u32 time);
uint8_t IIC_Start(void);
void IIC_Stop(void);
void IIC_ACK(void);
void IIC_NACK(void);
uint8_t IIC_WaitACK(void);
void IIC_SendByte(uint8_t byte);
uint8_t IIC_ReadByte(void);



#endif
